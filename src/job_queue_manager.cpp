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

#include "job_queue_manager.h"

#include <wx/app.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "gamessq_common.h"

//! wxString tokenizer.
/*!
 * Splits the given string into little strings based on the given delimiter.
 *
 * \param string	The string to split
 * \param delimiter The delimiter used to split the string
 *
 * \return A wxArrayString object containing all the sub-strings in order
 */
wxArrayString wxStringSplit(const wxString string, wxChar delimiter);

/*
 * Includes the appropriate implementation of GamessQClient, and macros out the
 * instantiation, so that it creates an instance of the correct class.
 */
#ifdef __WINDOWS__
	#include "gamessq_windows_client.h"
	#define NEW_CLIENT	new GamessQWindowsClient
#else /* __WINDOWS__ */
	#include "gamessq_posix_client.h"
	#define NEW_CLIENT	new GamessQPosixClient
#endif /* __WINDOWS__ */

/*
 * JobQueueManager implementation
 */

JobQueueManager::JobQueueManager()
{
	mClient = NULL;
}

JobQueueManager::~JobQueueManager()
{
	if (mClient) {
		delete mClient;
	}
}

/*!
 * Tries to connect to the server, if it can't connect, it tries to start the
 * server and then connect.
 *
 * \return	True if it is successfully connected to the server, false
 * otherwise.
 */
bool JobQueueManager::Init()
{
	mClient = NEW_CLIENT();

	// wx will complain in windows if it can't connect
	wxLogNull *noLog;
	noLog = new wxLogNull();
	bool connected = mClient->Connect();
	delete noLog;

	// if we can't connect, start the backend
	if (! connected) {
#ifdef __WXMAC__
		wxFileName filename(wxTheApp->argv[0]);
        filename.RemoveLastDir();
        filename.RemoveLastDir();
        filename.RemoveLastDir();
        filename.SetFullName(GAMESSQD_EXEC_NAME);
        wxLogDebug(filename.GetFullPath());
        if (! filename.FileExists()) {
            // Try /usr/local/bin instead
            filename = wxT("/usr/local/bin/");
            filename.SetFullName(GAMESSQD_EXEC_NAME);
			if (! filename.FileExists()) {
				wxLogError(wxT("Unable to locate backend daemon in /usr/local/bin or where the app is located."));
			}
		}
		filename.MakeAbsolute();
		wxLogDebug(filename.GetFullPath());
		int pid = fork();
		if (pid == 0) {
			execl(filename.GetFullPath().ToAscii(),
					filename.GetFullPath().ToAscii(), NULL);
			exit(1);
		}
#else /* __WXMAC__ */
        wxFileName filename(wxTheApp->argv[0]);
        filename.SetFullName(GAMESSQD_EXEC_NAME);
		wxExecute(filename.GetFullPath());
#endif
		// wx will complain in windows if it can't connect
		noLog = new wxLogNull();
		// wait for the backend to come up
		for (int i = 0; i < 10 && (! connected); i++) {
			connected = mClient->Connect();
			wxMilliSleep(500);
		}
		delete noLog;
	}
	
	if (connected) {
		return true;
	} else {
		delete mClient;
		mClient = NULL;
		return false;
	}
}

/*!
 * \param path	Path to the GAMESS input file.
 * \param procs	Number of processors this job is to consume.
 *
 * \return True upon success, false otherwise.
 *
 * \sa Job::Job(wxString, int)
 */
bool JobQueueManager::Add(wxString path, int procs)
{
	if (! mClient) {
		return false;
	}
	wxString message;
	message << (wxChar)JOB_COMMAND_ADD;
	message << procs << wxT(",");
	message << path;
	return mClient->Execute(message);
}

/*!
 * \param index the unique ID of the job to cancel.
 *
 * \return True upon success, false otherwise.
 *
 * \sa Job::Cancel()
 */
bool JobQueueManager::Cancel(int index)
{
	if (! mClient) {
		return false;
	}
	wxString message;
	message << (wxChar)JOB_COMMAND_CANCEL;
	message << index;
	return mClient->Execute(message);
}

/*!
 * \param index the unique ID of the job to pause.
 *
 * \return True upon success, false otherwise.
 *
 * \sa Job::Pause()
 */
bool JobQueueManager::Pause(int index)
{
	if (! mClient) {
		return false;
	}
	wxString message;
	message << (wxChar)JOB_COMMAND_PAUSE;
	message << index;
	return mClient->Execute(message);
}

/*!
 * \param index the unique ID of the job to resume.
 *
 * \return True upon success, false otherwise.
 *
 * \sa Job::Resume()
 */
bool JobQueueManager::Resume(int index)
{
	if (! mClient) {
		return false;
	}
	wxString message;
	message << (wxChar)JOB_COMMAND_RESUME;
	message << index;
	return mClient->Execute(message);
}

/*!
 * \param index the unique ID of the job to remove.
 *
 * \return True upon success, false otherwise.
 *
 * \sa Job::Clear()
 */
bool JobQueueManager::Remove(int index)
{
	if (! mClient) {
		return false;
	}
	wxString message;
	message << (wxChar)JOB_COMMAND_REMOVE;
	message << index;
	return mClient->Execute(message);
}

/*!
 * \return True upon success, false otherwise.
 *
 * \sa Job::Clear()
 */
bool JobQueueManager::Clear()
{
	if (! mClient) {
		return false;
	}
	wxString message;
	message << (wxChar)COMMAND_CLEAR;
	return mClient->Execute(message);
}

/*!
 * \return the number of jobs currently in the queue.
 */
int JobQueueManager::GetNumJobs()
{
	if (! mClient) {
		return 0;
	}
	long retVal = 0;
	wxString message;
	message << (wxChar)JOB_REQUEST_NUMBER;
	wxString data = mClient->Request(message);
	if (data.IsNumber()) {
		data.ToLong(&retVal, 10);
	}
	return (int)retVal;
}

/*!
 * Returns an array representing the job ids of all the jobs in the queue. The
 * first element in the array is the number of jobs in the array.  The
 * remaining elements are the unique ids of the jobs in order.
 *
 * \return	An integer array representing the job ids of all the jobs in
 *			the queue
 */
int *JobQueueManager::GetJobIds()
{
	if (! mClient) {
		int *ids = new int[1];
		*ids = 0;
		return ids;
	}
	wxString message;
	message << (wxChar)JOB_REQUEST_IDS;
	wxString data = mClient->Request(message);
	if (data == wxT("")) {
		int *ids = new int[1];
		*ids = 0;
		return ids;
	}

	wxArrayString idList = wxStringSplit(data, wxT(','));
	long num = 0;
	idList.Item(0).ToLong(&num, 10);
	long tmp;
	int *ids = new int[(int)num + 1];

	ids[0] = (int)num;

	for (int i = 1; i <= (int)num; i++) {
		if (idList.Item(i).ToLong(&tmp, 10)) {
			ids[i] = (int)tmp;
		} else {
			ids[i] = -1;
		}
	}
	return ids;
}

/*!
 * \param index The unique id of the job
 *
 * \return The name of the given job.
 * 
 * \sa Job::GetName()
 */
wxString JobQueueManager::GetName(int index)
{
	if (! mClient) {
		return wxT("");
	}
	wxString message;
	message << (wxChar)JOB_REQUEST_NAME;
	message << index;
	return mClient->Request(message);
}

/*!
 * \param index The unique id of the job
 * \param name	The new name for this job
 *
 * \return True upon success, false otherwise.
 *
 * \sa Job::SetName()
 */
bool JobQueueManager::SetName(int index, wxString name)
{
	if (! mClient) {
		return false;
	}
	wxString message;
	message << (wxChar)JOB_COMMAND_SET_NAME;
	message << index << wxT(',');
	message << name;
	return mClient->Execute(message);
}

/*!
 * \param index The unique id of the job
 *
 * \return The spool filename of the given job.
 *
 * \sa Job::GetSpoolFileName()
 */
wxString JobQueueManager::GetFileName(int index)
{
	if (! mClient) {
		return wxT("");
	}
	wxString message;
	message << (wxChar)JOB_REQUEST_FILENAME;
	message << index;
	return mClient->Request(message);
}

/*!
 * \param index The unique id of the job
 *
 * \return The number of processors used by the given job.
 *
 * \sa Job::GetNumProcessors()
 */
int JobQueueManager::GetNumProcessors(int index)
{
	if (! mClient) {
		return 0;
	}
	long retVal = 0;
	wxString message;
	message << (wxChar)JOB_REQUEST_PROCS;
	message << index;
	wxString data = mClient->Request(message);
	if (data.IsNumber()) {
		data.ToLong(&retVal, 10);
	}
	return (int)retVal;
}

/*!
 * \param index The unique id of the job
 *
 * \return The status code of the given job.
 *
 * \sa Job::GetStatus();
 */
Job::Status JobQueueManager::GetStatus(int index)
{
	if (! mClient) {
		return Job::STATUS_ERROR;
	}
	long retVal = 0;
	wxString message;
	message << (wxChar)JOB_REQUEST_STATUS;
	message << index;
	wxString data = mClient->Request(message);
	if (data.IsNumber()) {
		data.ToLong(&retVal, 10);
	}
	return (Job::Status)retVal;
}

/*!
 * Copies the job output data from the spool directory to the given location.
 *
 * \note The output data will consist of a .log file, a .dat file and possibly
 * a .irc or .trj file and efp, gamma and/or rst files.
 *
 * \param index		The unique id of the job whose data you want to copy
 * \param filename	The base filename for the copied data
 *
 * \return True upon success, false otherwise.
 */
bool JobQueueManager::Save(int index, wxString filename)
{
	if (! mClient) {
		return false;
	}
	wxString message;
	message << (wxChar)JOB_COMMAND_SAVE;
	message << index << wxT(',');
	message << filename;
	return mClient->Execute(message);
}

/*! \copydetails Job::GetSpoolDir()
 * \sa Job::GetSpoolDir()
 */
wxString JobQueueManager::GetSpoolDir()
{
	if (! mClient) {
		return wxT("");
	}
	wxString message;
	message << (wxChar)REQUEST_SPOOLDIR;
    wxString result = mClient->Request(message);
	return result;
}

/*! \copydetails Job::GetGamessDir()
 * \sa Job::GetGamessDir()
 */
wxString JobQueueManager::GetGamessDir()
{
	if (! mClient) {
		return wxT("");
	}
	wxString message;
	message << (wxChar)REQUEST_GAMESSDIR;
    wxString result = mClient->Request(message);
    return result;
}

/*! \copydetails Job::SetSpoolDir()
 *
 * \note All the files from the old spool directory are copied, and the backend
 * logger is set up in the new directory.
 *
 * \sa Job::SetSpoolDir()
 */
bool JobQueueManager::SetSpoolDir(wxString newSpoolDir)
{
	if (! mClient) {
		return false;
	}
	wxString message;
	message << (wxChar)COMMAND_SPOOLDIR;
	message << newSpoolDir;
	return mClient->Execute(message);
}

/*! \copydetails Job::SetGamessDir()
 * \sa Job::SetGamessDir()
 */
bool JobQueueManager::SetGamessDir(wxString newGamessDir)
{
	if (! mClient) {
		return false;
	}
	wxString message;
	message << (wxChar)COMMAND_GAMESSDIR;
	message << newGamessDir;
	return mClient->Execute(message);
}

/*!
 * If active is true, this function activates the server. If active is false,
 * this function pauses the server.
 *
 * \param active	Activate or pause
 *
 * \return True upon success, false otherwise.
 */
bool JobQueueManager::SetActive(bool active)
{
	if (! mClient) {
		return false;
	}
	wxString message;
	if (active) {
		message << (wxChar)COMMAND_RESUME_ALL;
	} else {
		message << (wxChar)COMMAND_PAUSE_ALL;
	}
	return mClient->Execute(message);
}

/*!
 * \return True if connected to the server, false otherwise.
 */
bool JobQueueManager::IsConnected()
{
	if (! mClient) {
		return false;
	}
	return mClient->IsConnected();
}

/*!
 * \return True if the server is active, false if it is paused.
 */
bool JobQueueManager::IsActive()
{
	if (! mClient) {
		return false;
	}
	wxString message;
	message << (wxChar)REQUEST_ACTIVE;
	return mClient->Request(message) == wxT("y");
}

wxArrayString wxStringSplit(const wxString string, wxChar delimeter) {
	int index = string.Find(delimeter);
	wxArrayString arr;
	wxString data = string;
	while (index != wxNOT_FOUND) {
		arr.Add(data.Mid(0, index));
		data = data.Mid(index + 1);
		index = data.Find(delimeter);
	}
	arr.Add(data);
	return arr;
}

