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

#ifndef __JOB_QUEUE_MANAGER_H__
#define __JOB_QUEUE_MANAGER_H__

#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/ipc.h>

#include "job.h"
#include "gamessq_client.h"

//! Queue management interface
/*!
 * This class provides an external interface for getting information from and
 * managing the queue. It abstracts away all the grimy details of server
 * queries and commands, leaving a simple function call based interface.
 */
class JobQueueManager
{
	public:
		//! Generic Constructor
		JobQueueManager();

		//! Destructor
		~JobQueueManager();

		//! Initiation function.
		bool Init();

		//! Adds the given job to the queue
		bool Add(wxString path, int procs = 1);

		//! Cancels the given job
		bool Cancel(int index);

		//! Pauses the given job
		bool Pause(int index);

		//! Resumes the given job
		bool Resume(int index);

		//! Removes the given job
		bool Remove(int index);

		//! Clears all finished jobs from the queue
		bool Clear();

		//! Returns the number of jobs currently in the queue.
		int GetNumJobs();

		//! Get job IDs
		int *GetJobIds();

		//! Returns the name of the given job.
		wxString GetName(int index);

		//! Sets the name of the given job.
		bool SetName(int index, wxString name);

		//! Returns the spool filename of the given job.
		wxString GetFileName(int index);

		//! Returns number of processors used by the given job.
		int GetNumProcessors(int index);

		//! Returns the status code of the given job.
		Job::Status GetStatus(int index);

		//! Saves the job output
		bool Save(int index, wxString filename);

		//! Returns the spool directory.
		wxString GetSpoolDir();

		//! Returns the GAMESS directory.
		wxString GetGamessDir();

		//! Sets the spool directory.
		bool SetSpoolDir(wxString newSpoolDir);

		//! Sets the GAMESS directory.
		bool SetGamessDir(wxString newGamessDir);

		//! Activates or pauses the server.
		bool SetActive(bool active);

		//! Is connected?
		bool IsConnected();

		//! Is active?
		bool IsActive();

	private:
		//! GamessQClient object.
		GamessQClient *mClient;
};

#endif /* __JOB_QUEUE_MANAGER_H__ */
