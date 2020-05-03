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

#include "posix_job.h"

#include <wx/app.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <signal.h>
#include <sys/wait.h>

#include "gamessq_common.h"

PosixJob::PosixJob(wxString path, int procs):
	Job(path, procs),
	mPid(-1)
{ }

PosixJob::PosixJob(int id, wxString name, wxString spoolFileName, Status status,
		int procs):
	Job(id, name, spoolFileName, status, procs),
	mPid(-1)
{ }

/*! Starts the job running and sets the status code to STATUS_RUNNING.
 *
 * The posix implementation of this function uses the GMS helper script
 * provided with GAMESS to launch GAMESS. It does a simple fork and exec to
 * launch the script, and puts all the newly created processes in a process
 * group based on the original forked process.
 *
 * \sa mPid
 */
void PosixJob::Start()
{
	// try to find gamess
	wxString gamessname;
	if (gamessDir.IsEmpty()) {
		wxPathList SystemPath;
		SystemPath.AddEnvList(wxT("PATH"));
		gamessname = SystemPath.FindAbsoluteValidPath(wxT("gms"));
	} else {
		wxFileName name(gamessDir, wxT("gms"));
		name.MakeAbsolute();
		gamessname = name.GetFullPath();
		if (! wxFileExists(gamessname)) {
			gamessname = wxT("");
		}
	}

#ifdef DEFAULT_GAMESS_PATH
	if (gamessname.IsEmpty()) {
        wxStandardPaths & gStdPaths = wxStandardPaths::Get();
		gamessname = wxFileName(
				gStdPaths.GetExecutablePath()).GetPath() +
				wxT(DEFAULT_GAMESS_PATH) + wxT("/gms");
		if (! wxFileExists(gamessname)) {
			gamessname = wxT("");
		}
	}
#endif

	wxString procs;
	procs << mNumProcessors;
	wxFileName name(mSpoolFileName);
	name.MakeRelativeTo(gamessDir);
	wxString input = name.GetFullPath();
	name.SetExt(wxT("log"));
	wxString logfile = name.GetFullPath();

	if (gamessname.IsEmpty()) {
		wxLogError(wxT("Could not find GAMESS gms script"));
		//Create the planning log file and echo the error into it so the user sees it
		wxFile log(logfile, wxFile::write);
		log.Write(wxT("Could not find GAMESS gms script at : "));
		log.Write(gamessDir+wxT("gms"));
		mStatus = STATUS_ERROR;
		return;
	}
	
	wxString command;
	command << wxT("Exec: ") << gamessname;
	command << wxT(" -n ") << procs;
	command << wxT(" -l ") << logfile;
	command << wxT(" ") << input;
	wxLogMessage(command);

	mPid = fork();
	if (mPid == 0) {
		wxSetWorkingDirectory(gamessDir);
		setpgid(0, 0);
		execl(gamessname.ToAscii(), gamessname.ToAscii(),
				"-n", (const char *)procs.ToAscii(),
				"-l", (const char *)logfile.ToAscii(),
				(const char *)input.ToAscii(), NULL);
		exit(1);
	}
	mStatus = STATUS_RUNNING;
}

/*! Terminates the job processes and sets the status code to STATUS_CANCELED.
 *
 * The posix implementation of this function sends SIGINT to the entire process
 * group associated with this job.
 *
 * \sa mPid
 */
void PosixJob::Cancel()
{
	if (mPid != -1) {
		kill(-mPid, SIGINT);
	}
	mStatus = STATUS_CANCELED;
}

/*! Pauses the job processes(if running) and sets the status code to
 * STATUS_CANCELED. It then sets mPausedStatus to the old status code.
 *
 * The posix implementation of this function sends SIGSTP to the entire process
 * group associated with this job.
 *
 * \sa mPid
 */
void PosixJob::Pause()
{
	if (mStatus == STATUS_RUNNING) {
		if (kill(-mPid, SIGSTOP) == 0) {
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
 * The posix implementation of this function sends SIGCONT to the entire
 * process group associated with this job.
 *
 * \sa mPid
 */
void PosixJob::Resume()
{
	if (mPausedStatus == STATUS_NONE) {
		return;
	}
	if (mPausedStatus == STATUS_RUNNING) {
		if (kill(-mPid, SIGCONT) == 0) {
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
void PosixJob::Refresh()
{
	if (mPid == -1) {
		return;
	}

	int status;
	if (waitpid(mPid, &status, WNOHANG) != mPid) {
		return;
	}

	if (status == 0) {
		mStatus = STATUS_DONE;
	} else if (mStatus != STATUS_CANCELED) {
		mStatus = STATUS_ERROR;
	}
}

