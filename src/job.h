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

#ifndef __JOB_H__
#define __JOB_H__

#include <wx/list.h>
#include <wx/string.h>

// windows thinks it needs this as a preprocessor symbol (silly windows)
#ifdef STATUS_PENDING
	#undef STATUS_PENDING
#endif

//! Represents a GAMESS job
/*!
 * This class represents a GAMESS job and provides functions for polling and
 * managing the status of the job.
 */
class Job
{
	public:
		/*! Represents the status of a job
		 */
		enum Status {
			//! Indicates no status
			STATUS_NONE = -1,

			//! Indicates there was an error in the job
			STATUS_ERROR = 0,
			
			//! Indicates that the job is pending
			STATUS_PENDING,

			//! Indicates that the job is paused
			STATUS_PAUSED,

			//! Indicates that the job is running
			STATUS_RUNNING,

			//! Indicates that the job is done
			STATUS_DONE,

			//! Indicates that the job has been canceled
			STATUS_CANCELED
		};

		//! New job constructor.
		Job(wxString path, int procs);

		//! New job object constructor.
		Job(int id, wxString name, wxString spoolFileName, Status status,
				int procs);

		//! Sets the spool directory.
		static void SetSpoolDir(wxString newSpoolDir);

		//! Returns the spool directory.
		static wxString GetSpoolDir();

		//! Sets the GAMESS directory.
		static void SetGamessDir(wxString newGamessDir);

		//! Returns the GAMESS directory.
		static wxString GetGamessDir();

		//! Sets the next Job ID.
		static void SetNextId(int id);

		//! Returns the next Job ID.
		static int GetNextId();

		//! Start the job running
		/*! Starts the job running, and sets the status code to
		 * JOB_STATUS_RUNNING.
		 *
		 * \note The default implementation of this function is
		 * empty, subclasses (if they want to be useful) should re-implement
		 * this function.
		 */
		virtual void Start() {}

		//! Cancel the job
		/*! Terminates the job processes(if running) and sets the status code
		 * to STATUS_CANCELED.
		 *
		 * \note The default implementation of this function is empty,
		 * subclasses (if they want to be useful) should re-implement this
		 * function.
		 */
		virtual void Cancel() {}

		//! Pause the job
		/*! Pauses the job processes(if running) and sets the status code to
		 * STATUS_PAUSED. It then sets mPausedStatus to the old status code.
		 *
		 * \note The default implementation of this function is empty,
		 * subclasses (if they want to useful) should re-implement this
		 * function.
		 */
		virtual void Pause() {}

		//! Resume the job
		/*! Resumes the job processes(if it was running before it was paused)
		 * and sets the status code to the status code of the job before it was
		 * paused.
		 *
		 * \note The default implementation of this function is empty,
		 * subclasses (if they want to useful) should re-implement this
		 * function.
		 */
		virtual void Resume() {}

		//! Refreshes the job's status
		/*! Checks to make sure the job is still running, if it is not running,
		 * it checks to see if there was an error, or if GAMESS exited
		 * normally. The job status code is then updated.
		 *
		 * \note The default implementation of this function is empty,
		 * subclasses (if they want to useful) should re-implement this
		 * function.
		 */
		virtual void Refresh() {}

		//! Clears this job
		virtual void Clear();
		
		//! Returns the name of the job
		wxString GetName() const;

		//! Returns the spool filename of the job
		wxString GetSpoolFileName() const;

		//! Returns the number of processors this job is supposed to use
		int GetNumProcessors() const;

		//! Returns the status code for this job
		Status GetStatus() const;

		//! Returns the status string for this job
		wxString GetStatusString() const;

		//! Returns the unique ID of this job
		int GetId() const;

		//! Sets the name of this job
		void SetName(wxString name);

		//! Sets the status code for this job
		void SetStatus(Status status);

	protected:
		//! Spool Directory
		/*! The directory where GamessQ stores all its temporary files such as
		 * input files for running and pending jobs as well as output and log
		 * files for completed jobs.
		 */
		static wxString spoolDir;

		//! GAMESS program directory
		/*! The directory where the GAMESS executables and scripts can be found.
		 */
		static wxString gamessDir;

		//! Next unique ID
		/*! A simple counter that stores the ID of the next created Job object.
		 */
		static int nextId;

		//! Name of the job
		/*! While the default name is the name of the input file, this may not
		 * always be the case.
		 */
		wxString mJobName;

		//! Spool filename
		/*!Stores the full path of the temporary input file stored in the spool
		 * directory.
		 *
		 * \sa Job::spoolDir
		 */
		wxString mSpoolFileName;

		//! Number of processors
		/*!Stores the number of processors this job is supposed to use when
		 * running.
		 */
		int mNumProcessors;

		//! Status code
		/*! Stores the status code for this job.
		 *
		 * \sa Job::status
		 */
		Status mStatus;

		//! Paused Status Code
		/*! When a job is paused, this member variable is used to store the
		 * status code of the job before it was paused so that it can be
		 * resumed correctly. When the job is not paused, this variable should
		 * contain -1.
		 *
		 * \sa Job::mStatus
		 * \sa Job::status
		 */
		Status mPausedStatus;

		//! Unique job ID
		/*! Stores the unique id for this job. This ID is used by the client and
		 * server to identify a specific job.
		 */
		int mId;
	
	private:
		//! String representation of statuses
		/*! Stores the string representation of each of the valid status codes.
		 * \sa GetStatusString()
		 */
		static wxString jobStatusString[];
};

WX_DECLARE_LIST(Job, JobList);

#endif /* __JOB_H__ */

