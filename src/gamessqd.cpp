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

#include "gamessqd.h"

#include <wx/filename.h>

#include "gamessq_common.h"

/*
 * Includes the appropreate implementation of Job, and defines a macro so that
 * the type of job instantiated is determined at compile time.
 */
#ifdef __WINDOWS__
	#include "windows_job.h"
	#define NEW_JOB		new WindowsJob
#else /* __WINDOWS__ */
	#include "posix_job.h"
	#define NEW_JOB		new PosixJob
#endif /* __WINDOWS__ */

// windows thinks it needs this as a preprocessor symbol (silly windows)
#ifdef STATUS_PENDING
	#undef STATUS_PENDING
#endif

/*
 * GamessQServer member functions
 */
GamessQServer::GamessQServer()
{
	// load the configuration from whatever the current system's default
	// configuratio method happens to be.
	wxConfig config(WX_CONFIG_APPNAME);
	wxString spooldir;
	if (config.Read(wxT("Spool Directory"), &spooldir)) {
		Job::SetSpoolDir(spooldir);
	} else {
		// forces it to create the spool directory if one has not yet been set
		Job::SetSpoolDir(_T(""));
	}
	wxString gamessdir;
	if (config.Read(wxT("GAMESS path"), &gamessdir)) {
		Job::SetGamessDir(gamessdir);
	}

	// open the log file and set up the logger
	wxFileName logfilename = wxFileName(Job::GetSpoolDir(), QUEUE_LOG_FILENAME);
	mLogFile = new wxFFile(logfilename.GetFullPath(), wxT("w"));
	if (! mLogFile->IsOpened()) {
		wxLogError(wxT("Cannot Open Log File"));
	}
	mLogger = new wxLogStderr(mLogFile->fp());
	wxLog::SetActiveTarget(mLogger);
	wxLogMessage(wxT("GamessQ server started."));

	// load the queue
	LoadQueue();
	mActive = true;
}

/*!
 * \note The reason this destructor is virtual is because further down the
 * class hierarchy this class is inhereted by another class that also inherits
 * wxServer. In order for the wxWidgets housecleaning stuff to work, this
 * destructor calls have to be able to go up the chain.
 */
GamessQServer::~GamessQServer()
{
//	Normally, we would delete mLogger here, but for some reason it causes a
//	segmentation fault.
	mLogFile->Close();
	delete mLogFile;
}

/*!
 * This function is called every time the client sends a command to the
 * server. It handles the command and makes any changes to the queue
 * that may be needed.
 *
 * \param data The command from the client
 *
 * \return True if the command succeded, false otherwise.
 */
bool GamessQServer::OnExecute(const wxString &data)
{
	// a little debugging
	wxString debug;
	debug << wxT("Execute: ") << data;
	wxLogDebug(debug);

	// most of these items will be useful later
	wxChar command = *(data.c_str());
	wxString item = data.Mid(1);
	wxJobListNode *node = getJob(item);
	wxJobListNode *node2;
	Job *job;
	int index;

	// the return value
	bool retVal = false;

	// based on the command received, do the appropreate thing
	switch(command) {
		case JOB_COMMAND_ADD:
			long procs;
			index = item.Find(wxT(','));
			item.Mid(0, index).ToLong(&procs, 10);
			item = item.Mid(index + 1);

			job = NEW_JOB(item, procs);
			if (job) {
				mJobQueue.Append(job);
				retVal = true;
			}
			break;
		case JOB_COMMAND_REMOVE:
			if (node) {
				job = node->GetData();
				if (mJobQueue.DeleteNode(node)) {
					job->Clear();
					delete job;
					retVal = true;
				}
			}
			break;
		case JOB_COMMAND_PAUSE:
			if (mActive) {
				if (node) {
					node->GetData()->Pause();
					retVal = true;
				}
			} else {
				retVal = false;
			}
			break;
		case JOB_COMMAND_RESUME:
			if (mActive) {
				if (node) {
					node->GetData()->Resume();
					retVal = true;
				}
			} else {
				retVal = false;
			}
			break;
		case JOB_COMMAND_CANCEL:
			if (node) {
				node->GetData()->Cancel();
				retVal = true;
			}
			break;
		case JOB_COMMAND_SET_NAME:
			index = item.Find(wxT(','));
			node = getJob(item.Mid(0, index));
			item = item.Mid(index + 1);
			if (node) {
				node->GetData()->SetName(item);
				retVal = true;
			}
			break;
		case JOB_COMMAND_SAVE:
			index = item.Find(wxT(','));
			node = getJob(item.Mid(0, index));
			item = item.Mid(index + 1);
			if (node) {
				Job *job = node->GetData();
				wxFileName source(job->GetSpoolFileName());
				wxFileName dest(item);

				// make sure we're really finished
				source.SetExt(wxT("log"));
				if (! wxFileExists(source.GetFullPath())) {
					retVal = false;
					break;
				}

				retVal = true;
				source.SetExt(wxT("inp"));
				dest.SetExt(wxT("inp"));
				retVal = retVal & wxCopyFile(source.GetFullPath(),
											 dest.GetFullPath(), true);

				source.SetExt(wxT("log"));
				dest.SetExt(wxT("log"));
				retVal = retVal & wxCopyFile(source.GetFullPath(),
						dest.GetFullPath(), true);

				source.SetExt(wxT("dat"));
				dest.SetExt(wxT("dat"));
				if (wxFileExists(source.GetFullPath())) {
					retVal = retVal & wxCopyFile(source.GetFullPath(),
												 dest.GetFullPath(), true);
				}
				
				source.SetExt(wxT("irc"));	//Pre 2009 versions of GAMESS used .irc files
				dest.SetExt(wxT("irc"));
				// the .irc might not always get created by gamess
				if (wxFileExists(source.GetFullPath())) {
					retVal = retVal & wxCopyFile(source.GetFullPath(),
							dest.GetFullPath(), true);
				}

				source.SetExt(wxT("trj"));
				dest.SetExt(wxT("trj"));
				// the .trj might not always get created by gamess (2009 and later)
				if (wxFileExists(source.GetFullPath())) {
					retVal = retVal & wxCopyFile(source.GetFullPath(),
												 dest.GetFullPath(), true);
				}
				
				source.SetExt(wxT("efp"));
				dest.SetExt(wxT("efp"));
				// the .efp is only created for makeefp runs
				if (wxFileExists(source.GetFullPath())) {
					retVal = retVal & wxCopyFile(source.GetFullPath(),
												 dest.GetFullPath(), true);
				}
				
				source.SetExt(wxT("gamma"));
				dest.SetExt(wxT("gamma"));
				// the .gamma file is not normally created
				if (wxFileExists(source.GetFullPath())) {
					retVal = retVal & wxCopyFile(source.GetFullPath(),
												 dest.GetFullPath(), true);
				}
				
				source.SetExt(wxT("rst"));
				dest.SetExt(wxT("rst"));
				// the .rst is used by certain types of runs for restarts
				if (wxFileExists(source.GetFullPath())) {
					retVal = retVal & wxCopyFile(source.GetFullPath(),
												 dest.GetFullPath(), true);
				}
			}
			break;
		case COMMAND_SPOOLDIR:
			Job::SetSpoolDir(item);
			mLogFile->Close();
			delete mLogFile;
			mLogFile = new wxFFile(wxFileName(item, QUEUE_LOG_FILENAME)
					.GetFullPath(), wxT("a"));
			if (! mLogFile->IsOpened()) {
				wxLogError(wxT("Cannot Open Log File"));
			} else {
				wxLog *logtmp = new wxLogStderr(mLogFile->fp());
				wxLog::SetActiveTarget(logtmp);
				delete mLogger;
				mLogger = logtmp;
			}
			retVal = true;
			break;
		case COMMAND_GAMESSDIR:
			Job::SetGamessDir(item);
			retVal = true;
			break;
		case COMMAND_PAUSE_ALL:
			retVal = true;
			if (! mActive) {
				break;
			}

			mActive = false;
			mPausedList = new JobList();

			// pause all running jobs, and record them in mPausedList
			node = mJobQueue.GetFirst();
			while (node) {
				Job *job = node->GetData();
				if (job->GetStatus() == Job::STATUS_RUNNING) {
					job->Pause();
					mPausedList->Append(job);
				}
				node = node->GetNext();
			}
			break;
		case COMMAND_RESUME_ALL:
			retVal = true;
			if (mActive) {
				break;
			}
			if (! mPausedList) {
				// this should never actually happen, but just in case...
				retVal = false;
				break;
			}
			mActive = true;
			// resume all the jobs in mPausedList
			node = mPausedList->GetFirst();
			while (node) {
				node->GetData()->Resume();
				node = node->GetNext();
			}
			delete mPausedList;
			mPausedList = NULL;
			break;
		case COMMAND_CLEAR:
			retVal = true;
			node = mJobQueue.GetFirst();
			while (node) {
				Job *job = node->GetData();
				if (job->GetStatus() == Job::STATUS_CANCELED ||
						job->GetStatus() == Job::STATUS_DONE ||
						job->GetStatus() == Job::STATUS_ERROR) {
					node2 = node->GetNext();
					mJobQueue.DeleteNode(node);
					job->Clear();
					delete job;
					retVal = true;
					node = node2;
				} else {
					node = node->GetNext();
				}
			}
	}
	SaveQueue();
	return retVal;
}

/*!
 * This function is called every time the client querries the server
 * for information about a specific job, or the queue in general. It
 * must handle the command and return the required information or an
 * empty string if there was an error.
 *
 * \param data The request from the client
 *
 * \return The result of the querry.
 */
wxString GamessQServer::OnRequest(const wxString &data)
{
	// these will be useful
	wxChar command = *(data.c_str());
	wxString item = data.Mid(1);
	wxJobListNode *node = getJob(item);
	wxString retVal;

	switch (command) {
		case JOB_REQUEST_NUMBER:
			retVal << mJobQueue.GetCount();
			break;
		case JOB_REQUEST_IDS:
			node = mJobQueue.GetFirst();
			retVal << mJobQueue.GetCount();
			while (node) {
				retVal << wxT(",") << node->GetData()->GetId();
				node = node->GetNext();
			}
			break;
		case JOB_REQUEST_NAME:
			if (node) {
				retVal = node->GetData()->GetName();
			}
			break;
		case JOB_REQUEST_STATUS:
			if (node) {
				retVal << node->GetData()->GetStatus();
			}
			break;
		case JOB_REQUEST_PROCS:
			if (node) {
				retVal << node->GetData()->GetNumProcessors();
			}
			break;
		case JOB_REQUEST_FILENAME:
			if (node) {
				retVal = node->GetData()->GetSpoolFileName();
			}
			break;
		case REQUEST_SPOOLDIR:
			retVal = Job::GetSpoolDir();
			break;
		case REQUEST_GAMESSDIR:
			retVal = Job::GetGamessDir();
			break;
		case REQUEST_ACTIVE:
			if (mActive) {
				retVal = wxT("y");
			} else {
				retVal = wxT("n");
			}
			break;
		default:
			wxLogDebug(wxT("Unknown Request"));
	}
	// a little debugging
	wxString debug;
	debug << wxT("Request: ") << data << wxT(" : ") << retVal;
	wxLogDebug(debug);
	return retVal;
}

/*!
 * This function is called whenver the server has nothing better to do.
 * It calls Job::Refresh() on the appropreate jobs and starts another
 * one if necisary.
 *
 * \return True if there are jobs running, paused or pending. False if
 * all the jobs in the queue are done, canceled or had an error.
 */
bool GamessQServer::CheckQueue()
{
	if (! mActive) {
		return true;
	}

	bool retVal = false;
	wxJobListNode *node = mJobQueue.GetFirst();
	while (node) {
		node->GetData()->Refresh();
		if (node->GetData()->GetStatus() == Job::STATUS_PAUSED) {
			retVal = true;
		}
		if (node->GetData()->GetStatus() == Job::STATUS_RUNNING) {
			return true;
		}
		node = node->GetNext();
	}
	node = mJobQueue.GetFirst();

	while (node) {
		if (node->GetData()->GetStatus() == Job::STATUS_PENDING) {
			node->GetData()->Start();
			SaveQueue();
			return true;
		}
		node = node->GetNext();
	}
	SaveQueue();
	return retVal;
}

/*!
 * Searches through the queue looking for the job with the given unique ID and
 * returns the wxJobListNode object
 *
 * \param n the unique ID of the job (as a wxString)
 *
 * \return The wxJobListNode for the given job ID if found, NULL otherwise.
 */
wxJobListNode *GamessQServer::getJob(wxString n)
{
	long index;
	if (n.ToLong(&index, 10)) {
		wxJobListNode *node = mJobQueue.GetFirst();
		while (node) {
			if (node->GetData()->GetId() == index) {
				return node;
			}
			node = node->GetNext();
		}
	}
	return NULL;
}

/*!
 * \param file The file to read.
 *
 * \return The next line in file.
 */
wxString GamessQServer::ReadLine(wxFFile *file)
{
	int num = 64;
	char *buf = NULL;
	int i = 0;
	char c;
	while (true) {
		if (buf) {
			// enlarge the buffer;
			char *newbuf = new char[num * 2];
			for (int i = 0; i < num; i ++) {
				newbuf[i] = buf[i];
			}
			num = num * 2;
			delete buf;
			buf = newbuf;
		} else {
			buf = new char[num];
		}
		while (i < num) {
			file->Read(&c, 1);
			if (c == '\n' || file->Eof()) {
				buf[i] = NULL;
				wxString data = wxString(buf, wxConvUTF8);
				delete buf;
				wxLogDebug(data);
				return data;
			}
			buf[i] = c;
			i++;
		}
	}
}

/*!
 * \sa SaveQueue()
 */
void GamessQServer::LoadQueue()
{
	wxFileName filename = wxFileName(Job::GetSpoolDir(), QUEUE_DAT_FILENAME);
	if (! wxFileExists(filename.GetFullPath())) {
		wxLogMessage(wxT("No saved queue file"));
		return;
	}
	wxFFile *file = new wxFFile(filename.GetFullPath(), wxT("r"));
	if (! file->IsOpened()) {
		wxString error = wxT("Failed to open file: ");
		error << filename.GetFullPath();
		wxLogError(error);
		return;
	}
	int id;
	wxString name;
	int procs;
	wxString spoolFileName;
	Job::Status status;

	long tmp;
	wxString data;

	data = ReadLine(file);
	if (! data.ToLong(&tmp, 10)) {
		wxString debug = wxT("Reading queue file: ");
		debug << data << wxT(" Could not be converted to an integer.");
		wxLogDebug(debug);
	}
	int num = tmp;

	data = ReadLine(file);
	if (! data.ToLong(&tmp, 10)) {
		wxString debug = wxT("Reading queue file: ");
		debug << data << wxT(" Could not be converted to an integer.");
		wxLogDebug(debug);
	}
	Job::SetNextId(tmp);

	// read all the job information
	for (int i = 0; i < num && (! file->Eof()); i ++) {
		// read until you find an empty line
		data = ReadLine(file);
		while (data != wxT("")) {
			data = ReadLine(file);
		}

		// read until the first non-empty line
		while (data == wxT("")) {
			data = ReadLine(file);
		}

		// now we're ready to start reading data
		// get id
		if (! data.ToLong(&tmp, 10)) {
			wxString debug = wxT("Reading queue file: ");
			debug << data << wxT(" Could not be converted to an integer.");
			wxLogDebug(debug);
		}
		id = tmp;

		// get name
		name = ReadLine(file);

		// get procs
		data = ReadLine(file);
		if (! data.ToLong(&tmp, 10)) {
			wxString debug = wxT("Reading queue file: ");
			debug << data << wxT(" Could not be converted to an integer.");
			wxLogDebug(debug);
		}
		procs = tmp;

		// get spoolFileName
		spoolFileName = ReadLine(file);

		// get status
		data = ReadLine(file);
		if (! data.ToLong(&tmp, 10)) {
			wxString debug = wxT("Reading queue file: ");
			debug << data << wxT(" Could not be converted to an integer.");
			wxLogDebug(debug);
		}
		status = (Job::Status)tmp;
		
		// in case something was left running
		if (status == Job::STATUS_RUNNING || status == Job::STATUS_PAUSED) {
			status = Job::STATUS_PENDING;
			wxFileName name(spoolFileName);
			name.SetExt(wxT("dat"));
			if (wxFileExists(name.GetFullPath())) {
				wxRemoveFile(name.GetFullPath());
			}
			name.SetExt(wxT("irc"));
			if (wxFileExists(name.GetFullPath())) {
				wxRemoveFile(name.GetFullPath());
			}
			name.SetExt(wxT("trj"));
			if (wxFileExists(name.GetFullPath())) {
				wxRemoveFile(name.GetFullPath());
			}
			name.SetExt(wxT("log"));
			if (wxFileExists(name.GetFullPath())) {
				wxRemoveFile(name.GetFullPath());
			}
			name.SetExt(wxT("efp"));
			if (wxFileExists(name.GetFullPath())) {
				wxRemoveFile(name.GetFullPath());
			}
			name.SetExt(wxT("gamma"));
			if (wxFileExists(name.GetFullPath())) {
				wxRemoveFile(name.GetFullPath());
			}
			name.SetExt(wxT("rst"));
			if (wxFileExists(name.GetFullPath())) {
				wxRemoveFile(name.GetFullPath());
			}
		}

		// create the job and append it to the queue
		Job *job = NEW_JOB(id, name, spoolFileName, status, procs);
		mJobQueue.Append(job);
	}

	file->Close();
	delete file;
}

/*!
 * The queue is stored in a simple line by line text file. The first line
 * conatins the number of jobs in the queue, and the second line contains the
 * next unique job ID. After the initial information, there is a blank line and
 * then an entry fore each job seperated by blank lines and formatted as
 * follows.
 *
 * \li Unique Job ID
 * \li Job Name
 * \li Number of Processors
 * \li Spool Filename
 * \li Status code
 *
 * \sa LoadQueue()
 * \sa Job
 */
void GamessQServer::SaveQueue()
{
	wxFileName filename = wxFileName(Job::GetSpoolDir(), QUEUE_DAT_FILENAME);
	wxFFile *file = new wxFFile(filename.GetFullPath(), wxT("w"));
	wxString data;

	// write initial information
	int num = mJobQueue.GetCount();
	data << num << wxT('\n');
	if (num > 0) {
		data << Job::GetNextId() << wxT('\n');
	} else {
		data << 0 << wxT('\n');
	}
	file->Write(data);

	// for every job, dump it's info to the file
	wxJobListNode *node = mJobQueue.GetFirst();
	for (; node; node = node->GetNext()) {
		Job *job = node->GetData();
		data = wxT("\n");
		data << job->GetId() << wxT('\n');
		data << job->GetName() << wxT('\n');
		data << job->GetNumProcessors() << wxT('\n');
		data << job->GetSpoolFileName() << wxT('\n');
		data << job->GetStatus() << wxT('\n');
		file->Write(data);
	}
	file->Close();
	delete file;
}

