/*
 * Copyright (C) 2008 Iowa State University 
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "windows_job.h"

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/utils.h>
#include <wx/stdpaths.h>
#include <tlhelp32.h>

#include "gamessq_common.h"

WindowsJob::WindowsJob(wxString path, int procs):
	Job(path, procs),
	mJobHandle(NULL),
	mProcessHandle(NULL)
{ }

WindowsJob::WindowsJob(int id, wxString name, wxString spoolFileName,
		Status status, int procs):
	Job(id, name, spoolFileName, status, procs),
	mJobHandle(NULL),
	mProcessHandle(NULL)
{ }

/*! 
 *
 * Starts the job running and sets the status code to STATUS_RUNNING.
 * 
 * The windows implementation of this function opens a new process for
 * GAMESS and redirects stdout to a log file handle and redirects
 * stderr to NUL. The process is created in a suspended state, and the
 * added to a new windows job object before it is resumed.
 *
 * \par Notes of explanation concerning the implementation of Start:
 * In order to make cshell happy, you must provide it with valid standard
 * output and standard error handles. If either of the above is missing, cshell
 * will simply quit with an exit status of 1, and nothing will happen. Standard
 * output goes to the log file anyway, so that one is obvious. In order to
 * provide a valid standard error handle, I used the windows NUL file.
 *
 * \par
 * The process is created in a suspended state, added to the job object and
 * then resumed. The reason for this is because when you run a script with
 * cshell, the first thing it will do is fork and start running other processes
 * as described by the shell script. In order to make sure that all of its sub
 * processes are part of the job object, it must be added to the job object
 * before it has a chance to do anything. Since we have no guarantee as to when
 * the sub-process will start getting processor time, it is suspended until we
 * know it has been added to the job object.
 *
 * \sa WindowsJob::mProcessHandle
 * \sa WindowsJob::mJobHandle
 */
void WindowsJob::Start()
{
	// set this here, so we don't accidentally start it twice
	mStatus = STATUS_RUNNING;

	// try to find gamess
	wxString gamessname;
	if (gamessDir.IsEmpty()) {
		wxPathList SystemPath;
		SystemPath.AddEnvList(wxT("PATH"));
		gamessname = SystemPath.FindAbsoluteValidPath(wxT("rungms.bat"));
	} else {
		wxFileName name(gamessDir, wxT("rungms.bat"));
		name.MakeAbsolute();
		gamessname = name.GetFullPath();
		if (! wxFileExists(gamessname)) {
			gamessname = wxT("");
		}
	}

#ifdef DEFAULT_GAMESS_PATH
	if (gamessname.IsEmpty()) {
		wxStandardPathsBase& gStdPaths = wxStandardPaths::Get();
		gamessname = gStdPaths.GetResourcesDir() + wxT(DEFAULT_GAMESS_PATH) +
				wxT("/rungms.bat");
		if (! wxFileExists(gamessname)) {
			gamessname = wxT("");
		}
	}
#endif

	if (gamessname.IsEmpty()) {
		wxLogError(wxT("Could not find GAMESS"));
		mStatus = STATUS_ERROR;
		return;
	}

	// clean up old temp stuff so ddikick doesn't fail
	CleanUpTempFiles();

	wxFileName dest(GetSpoolFileName());
	dest = wxFileName(gamessDir + wxT("\\scratch"), dest.GetName());
	dest.SetExt(wxT("F05"));
	wxString debug = wxT("Copying file from ") + GetSpoolFileName();
	debug << wxT(" to ") << dest.GetFullPath();
	wxLogDebug(debug);
	wxCopyFile(GetSpoolFileName(), dest.GetFullPath(), true);


	// generate the command
	wxFileName name(gamessname);
	name.SetFullName(wxT("rungms.bat"));
//	name.SetFullName(wxT("csh.exe"));
//	name.MakeAbsolute();
	wxString command = name.GetFullPath();
//	name.SetFullName(wxT("runscript.csh"));
//	command << wxT(" -e -f ") << name.GetFullPath();
	name = wxFileName(GetSpoolFileName());
	command << wxT(" ") << name.GetName();
	command << wxT(" 13-64.pgi.linux.blas ");
	command << mNumProcessors;
//	command << wxT(" ") << gamessDir;
//	command << wxT(" ") << wxGetHostName();
	wxLogMessage(wxT("Exec: ") + command);

	// set up the security attributes for the input and output handles
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = true;

	// create a the log file and it's handle
	name.SetExt(wxT("log"));
	name.MakeAbsolute();
	HANDLE logFileHandle = CreateFile(name.GetFullPath(), GENERIC_WRITE,
			FILE_SHARE_READ, &sa, CREATE_ALWAYS, 0, NULL);
	if (logFileHandle == INVALID_HANDLE_VALUE) {
		LOG_ERROR("CreateFile");
	}

	// make a null handle for stderr
	HANDLE nullHandle = CreateFile(wxT("NUL"), GENERIC_WRITE,
			FILE_SHARE_READ, &sa, CREATE_ALWAYS, 0, NULL);
	if (nullHandle == INVALID_HANDLE_VALUE) {
		LOG_ERROR("CreateFile");
	}

	// create a new STARTUPINFO object with the newly created handles
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow=SW_HIDE; // hide the window
	si.dwFlags=STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdOutput = logFileHandle;
	si.hStdInput = NULL;
	si.hStdError = nullHandle;

	// create a new PROCESS_INFORMATION object
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	// create a new job
	mJobHandle = CreateJobObject(NULL, NULL);
	if (! mJobHandle) {
		LOG_ERROR("CreateJobObject");
	}

	// now we create the actual process, it needs to be created in a suspended
	// state so that we can assign it to the job object before it gets a chance
	// to spawn any more processes.
	if (CreateProcess(NULL, (wxChar *)command.wc_str(), NULL, NULL, true,
		CREATE_SUSPENDED, NULL, spoolDir, &si, &pi)) {
		if (! AssignProcessToJobObject(mJobHandle, pi.hProcess)) {
			LOG_ERROR("AssignProcessToJobObject");
		}
		mProcessHandle = pi.hProcess;
		ResumeThread(pi.hThread);
	} else {
		mStatus = STATUS_ERROR;
		LOG_ERROR("CreateProcess");
		mStatus = STATUS_ERROR;
		mProcessHandle = NULL;
		CloseHandle(pi.hProcess);
	}

	// close our side of the IO handles
	CloseHandle(logFileHandle);
	CloseHandle(nullHandle);

	// we don't need this one anymore
	CloseHandle(pi.hThread);
}

/* Terminates the job processes and sets the status code to
 * STATUS_CANCELED.
 *
 * The windows implementation of this job uses IterateThreads() to terminate
 * all the threads associated with this job.
 *
 * \sa IterateThreads
 */
void WindowsJob::Cancel()
{
	if (IterateThreads(Cancel)) {
		CleanUpTempFiles();
		CloseHandle(mJobHandle);
		CloseHandle(mProcessHandle);
		mProcessHandle = NULL;
		mStatus = STATUS_CANCELED;
	}
}

/*! Pauses the job processes(if running) and sets the status code to
 * STATUS_CANCELED. It then sets mPausedStatus to the old status code.
 *
 * The windows implementation of this job uses IterateThreads() to suspend all
 * the threads associated with this job.
 *
 * \sa IterateThreads
 */
void WindowsJob::Pause()
{
	if (mStatus == STATUS_RUNNING) {
		if (IterateThreads(Pause)) {
			mPausedStatus = mStatus;
			mStatus = STATUS_PAUSED;
		}
	} else if (mStatus == STATUS_PENDING) {
		mPausedStatus = mStatus;
		mStatus = STATUS_PAUSED;
	}
}

/*! Resumes the job processes(if it was running before it was paused) and sets
 * the status code to the status code of the job before it was paused.
 *
 * The windows implementation of this job uses IterateThreads() to resume all
 * the threads associated with this job.
 *
 * \sa IterateThreads
 */
void WindowsJob::Resume()
{
	if (mPausedStatus == STATUS_NONE) {
		return;
	}
	if (mPausedStatus == STATUS_RUNNING) {
		if (IterateThreads(Resume)) {
			mStatus = mPausedStatus;
			mPausedStatus = STATUS_NONE;
		}
	} else if (mPausedStatus == STATUS_PENDING) {
		mStatus = mPausedStatus;
		mPausedStatus = STATUS_NONE;
	}
}

/*! Checks to make sure the job is still running, if it is not running, it
 * checks to see if there was an error, or if GAMESS exited normally. The job
 * status code is then updated.
 */
void WindowsJob::Refresh()
{
	// make sure the job is or was recently running first
	if (mProcessHandle) {
		// try to get the exit code of the process
		DWORD exitCode;
		if (! GetExitCodeProcess(mProcessHandle, &exitCode)) {
			LOG_ERROR("GetExitCodeProcess");
		} else {
			if (exitCode != STILL_ACTIVE) {
				// if it's no long running...
				wxString debug = wxT("Exit Code: ");
				debug << exitCode;
				wxLogDebug(debug);
				if (exitCode == 0) {
					// we're ok, the job completed normally
					mStatus = STATUS_DONE;
					
					// move the finished files back to the spool dir.
					wxFileName dest(mSpoolFileName);
					wxFileName source(gamessDir + wxT("\\temp"),
							dest.GetName());
					source.SetExt(wxT("dat"));
					dest.SetExt(wxT("dat"));
					wxCopyFile(source.GetFullPath(), dest.GetFullPath(), true);
					source.SetExt(wxT("irc"));
					dest.SetExt(wxT("irc"));
					if (wxFileExists(source.GetFullPath())) {
						wxCopyFile(source.GetFullPath(), dest.GetFullPath(),
								true);
					}
				} else {
					mStatus = STATUS_ERROR;
				}
				// clean up our temp files, and close the rest of the handles
				CleanUpTempFiles();
				CloseHandle(mJobHandle);
				CloseHandle(mProcessHandle);
				mProcessHandle = NULL;
			}
		}
	}
}

/* The windows implementation of this function simply cleans up a fiew extra
 * temp files required when running GAMESS on windows.
 */
void WindowsJob::Clear()
{
	Job::Clear();
	CleanUpTempFiles();
}

/*! Due to the way GAMESS has to be run on windows, there are several extra
 * temporary files created outside of the spool directory. This function cleans
 * up some of these.
 */
void WindowsJob::CleanUpTempFiles()
{
	wxFileName name(mSpoolFileName);
	wxArrayString delfiles;
	wxDir::GetAllFiles(gamessDir + wxT("\\scratch"), &delfiles,
			name.GetName() + wxT(".*"), wxDIR_FILES);
	wxDir::GetAllFiles(gamessDir + wxT("\\temp"), &delfiles,
			name.GetName() + wxT(".*"), wxDIR_FILES);
	for (int i = 0; i < delfiles.Count(); i ++) {
		wxRemoveFile(delfiles.Item(i));
		wxLogDebug(wxT("Removing: ") + delfiles.Item(i));
	}
}

/*! Iterates over all the threads associated with this job (as determined by
 * mJobHandle), and runs the given callback function on them. The callback
 * function need not close the handle to the thread.
 *
 * \param callback The callback function to run on each thread.
 *
 * \par A few notes on the necessity and implementation of IterateThreads:
 * Before going to far into this function, you must realize that as much as one
 * may hope to the contrary, windows has no concept of mass process management.
 * If you want to do something to multiple processes or threads, windows
 * provides a few mediocre iteration methods, but beyond that, you're on your
 * own to find the processes and/or threads and work with them one at a time.
 * The following function was written for that exact purpose, it searches
 * through the list of all threads present in the entire system, finds the ones
 * whose parent process is part of the job object and runs the given callback
 * function on the thread.
 *
 * \par
 * The reason this is written on the thread level as opposed to the process
 * level (which would have been much easier), is because windows has no concept
 * of suspending a process, it can only suspend threads. Since the ability to
 * suspend and resume jobs was something that I wanted in GamessQ, it has to be
 * done on the tread level. When it comes to terminating processes, simply
 * terminating all their threads works just as well as terminating the process
 * via windows TerminatProcess function.
 *
 * \note
 * As clumsy as this method may seem, and as much as you may think and hope
 * that there MUST be a better way to do it, there really isn't. So far as I
 * have been able to find (and I searched for a while) this is the cleanest,
 * most efficient way to handle windows processes and threads in mass.
 *
 * \sa mJobHandle
 */
bool WindowsJob::IterateThreads(bool (*callback)(HANDLE))
{
	// We'll need this for later
	bool retVal = true;

	// get all the process ids for the job object
	JOBOBJECT_BASIC_PROCESS_ID_LIST *info;
	int num = mNumProcessors + 15; // make sure there's enough space
	int size = sizeof(JOBOBJECT_BASIC_PROCESS_ID_LIST) + num * sizeof(DWORD);
	info = (JOBOBJECT_BASIC_PROCESS_ID_LIST *)malloc(size);
	ZeroMemory(info, size);
	info->NumberOfAssignedProcesses = num;
	if (! QueryInformationJobObject(mJobHandle, JobObjectBasicProcessIdList,
			info, size, NULL)) {
		LOG_ERROR("QueryInformationJobObject");
		return false;
	}
	num = info->NumberOfProcessIdsInList;

	// get a thread snapshot
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 entry;
	ZeroMemory(&entry, sizeof(entry));
	entry.dwSize = sizeof(entry);
	if (! Thread32First(snapshot, &entry)) {
		LOG_ERROR("Thread32First");
		CloseHandle(snapshot);
		return false;
	}

	// iterate through all the threads in the system.
	bool more = true;
	while (more) {
		// search through the job ids
		for (int i = 0; i < num; i ++) {
			// if this thread's parent is in the list, run the callback on it
			if (entry.th32OwnerProcessID == info->ProcessIdList[i]) {
				HANDLE threadHandle = OpenThread(THREAD_TERMINATE |
						THREAD_SUSPEND_RESUME , false, entry.th32ThreadID);
				if (! threadHandle) {
					LOG_ERROR("OpenThread");
				} else {
					retVal = retVal && callback(threadHandle);
				}
				CloseHandle(threadHandle);
				break;
			}
		}

		// get the next thread, and make sure it's valid
		ZeroMemory(&entry, sizeof(entry));
		entry.dwSize = sizeof(entry);
		if (! Thread32Next(snapshot, &entry)) {
			if (GetLastError() == ERROR_NO_MORE_FILES) {
				more = false;
			} else {
				LOG_ERROR("Thread32Next");
				free(info);
				CloseHandle(snapshot);
				return false;
			}
		}
	}

	// clean up
	free(info);
	CloseHandle(snapshot);
	return retVal;
}

/*! This function is to be passed to IterateThreads(). It terminates the
 * thread associated with the given handle.
 *
 * \param handle a handle to the thread to be terminated
 *
 * \sa IterateThreads()
 * \sa Cancel()
 */
bool WindowsJob::Cancel(HANDLE handle)
{
	return TerminateThread(handle, 0) != 0;
}

/*! This function is to be passed to IterateThreads(). It suspends the thread
 * associated with the given handle.
 *
 * \param handle a handle to the thread to be terminated
 *
 * \sa IterateThreads()
 * \sa Pause()
 */
bool WindowsJob::Pause(HANDLE handle)
{
	return SuspendThread(handle) != -1;
}

/*! This function is to be passed to IterateThreads(). It resumes the
 * thread associated with the given handle.
 *
 * \param handle a handle to the thread to be resumed
 *
 * \sa IterateThreads()
 * \sa Resume()
 */
bool WindowsJob::Resume(HANDLE handle)
{
	return ResumeThread(handle) != -1;
}

