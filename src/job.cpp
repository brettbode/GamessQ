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

#include "job.h"

#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/stdpaths.h>

#include "gamessq_common.h"

wxString Job::spoolDir = wxT("");
wxString Job::gamessDir = wxT("");
int Job::nextId = 0;

// this array stores the string representation of the status code. This must be
// correspond exactly to the status enum.
wxString Job::jobStatusString[] = {
	wxT("ERROR"),
	wxT("Pending"),
	wxT("Paused"),
	wxT("Running"),
	wxT("Done"),
	wxT("Canceled"),
};

/*!
 * Sets the directory where GamessQ stores all its temporary files. If the
 * given directory is an empty string, it will use a reasonable default.
 *
 * \param	newSpoolDir		The new value for spoolDir
 *
 * \sa Job::spoolDir
 */
void Job::SetSpoolDir(wxString newSpoolDir)
{
	wxString oldDir = spoolDir;

	// set a default if none is available
	if (newSpoolDir.IsEmpty()) {
		// this is the first run of GamessQ
		spoolDir = wxStandardPaths::Get().GetUserDataDir();
	} else {
		spoolDir = newSpoolDir;
	}

	if (! wxDirExists(spoolDir)) {
		if (! wxMkdir(spoolDir, 0751)) {
			return;
		}
	}
	
	if (newSpoolDir != oldDir && (! oldDir.IsEmpty())) {
		wxArrayString files;
		wxDir::GetAllFiles(oldDir, &files, wxT("*.*"), wxDIR_FILES);
		for (int i = 0; i < files.GetCount(); i ++) {
			wxLogDebug(files.Item(i));
			wxFileName file(files.Item(i));
			wxFileName source(oldDir, file.GetFullName());
			wxFileName dest(spoolDir, file.GetFullName());
			wxCopyFile(source.GetFullPath(), dest.GetFullPath());
		}
	}
}

/*! \return The directory where GamessQ stores all its temporary files.
 * 
 * \sa Job::spoolDir
 */
wxString Job::GetSpoolDir()
{
	return spoolDir;
}

/*! Sets the directory where the GAMESS executables can be found.
 *
 * \note The temporary files are not copied to the new directory, this should
 * only be used for initial setup.
 *
 * \param	newGamessDir	The new value for gamessDir
 *
 * \sa Job::gamessDir
 */
void Job::SetGamessDir(wxString newGamessDir)
{
	gamessDir = newGamessDir;
}

/*! \return The directory where the GAMESS executables can be found.
 *
 * \sa Job::gamessDir
 */
wxString Job::GetGamessDir()
{
	return gamessDir;
}

/*! Sets the next Job ID
 *
 * \param	id		The new value for nextID
 *
 * \sa Job::nextId
 */
void Job::SetNextId(int id)
{
	nextId = id;
}

/*! \return The next job ID
 *
 * \sa Job::nextId
 */
int Job::GetNextId()
{
	return nextId;
}

/*! This constructor is used when a job is added to the queue. It automatically
 * copies the given input file into the spool directory and sets up a few other
 * things.
 *
 * \param	path	The path to the provided input file
 * \param	procs	The number of processors this job is to use
 */
Job::Job(wxString path, int procs)
{
	mId = nextId;
	nextId ++;
	wxFileName filename(path);
	filename.MakeAbsolute();

	// job name should be the filename without the extension
	mJobName = filename.GetName();
	wxString spoolname;
	spoolname << mJobName << wxT('_') << mId << wxT('.') << filename.GetExt();
	mSpoolFileName = wxFileName(spoolDir, spoolname).GetFullPath();
	
	// copy the file
	if (! wxCopyFile(path, mSpoolFileName)) {
		mStatus = Job::STATUS_ERROR;
	} else {
		mNumProcessors = procs;
		mStatus = Job::STATUS_PENDING;
	}
	mPausedStatus = STATUS_NONE;
}

/*! This constructor is used to simply make a new job object with the given
 * data. In specific, it is used to when rebuilding the job queue from the save
 * file.
 *
 * \param	id				The unique id of the job
 * \param	name			The name of the job
 * \param	spoolFileName	The spool filename of the job
 * \param	status			The status code of the job
 * \param	procs			The number of processors this job is to use
 *							when running
 */
Job::Job(int id, wxString name, wxString spoolFileName, Status status,
		int procs)
{
	mId = id;
	mJobName = name;
	mSpoolFileName = spoolFileName;
	mStatus = status;
	mNumProcessors = procs;
	mPausedStatus = STATUS_NONE;
}

/*! Stops the job (if it is running) and removes any temporary files. This is
 * not part of the destructor because then all the spool data would be lost
 * when the program closed.
 */
void Job::Clear()
{
	Cancel();
	wxFileName name = wxFileName(mSpoolFileName);
	wxLogDebug(name.GetFullPath());
	wxRemoveFile(name.GetFullPath());
	name.SetExt(wxT("log"));
	wxRemoveFile(name.GetFullPath());
	name.SetExt(wxT("dat"));
	wxRemoveFile(name.GetFullPath());
}

/*! \return The name of the job
 *
 * \sa Job::mJobName
 */
wxString Job::GetName() const
{
	return mJobName;
}

/*! \return The spool filename of the job
 *
 * \sa Job::mSpoolFileName
 */
wxString Job::GetSpoolFileName() const
{
	return mSpoolFileName;
}

/*! \return The number of processors this job is supposed to use when running.
 *
 * \sa mNumProcessors
 */
int Job::GetNumProcessors() const
{
	return mNumProcessors;
}

/*! \return the status code for this job.
 *
 * \sa Job::mStatus
 * \sa Job::status
 */
Job::Status Job::GetStatus() const
{
	return mStatus;
}

/*! Takes the status code for this job and converts it into a string
 * representation of the job's status.
 *
 * \return The string representation of this job's status.
 */
wxString Job::GetStatusString() const
{
	return jobStatusString[mStatus];
}

/*! \return The unique ID of this job.
 *
 * \sa mId
 */
int Job::GetId() const
{
	return mId;
}

/*! Sets the name of this job.
 *
 * \param name the new name of this job
 *
 * \sa Job::mJobName
 */
void Job::SetName(wxString name)
{
	mJobName = name;
}

/*! Sets the status code for this job. This only changes the value
 * of a member variable, it does not affect the actual status of a
 * running job in any way.
 *
 * \param status the new status code for this job.
 * 
 * \sa Job::mStatus
 */
void Job::SetStatus(Status status)
{
	mStatus = status;
}

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(JobList);

