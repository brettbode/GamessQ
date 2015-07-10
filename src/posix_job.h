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

#ifndef __POSIX_JOB_H__
#define __POSIX_JOB_H__

#include "job.h"


//! Posix based implementation of Job
/*!
 * This implementation of Job is for use in posix based systems such as Mac and
 * Linux. It uses the standard unix fork and exec method of spawning processes,
 * the new process is then put into a process group with the same id as itself.
 * The unix signals system is then used to manage the spawned processes via the
 * newly created process group.
 */
class PosixJob: public Job
{
	public:
		/*! \copydoc Job::Job(wxString, int)
		 */
		PosixJob(wxString path, int procs);

		/*! \copydoc Job::Job(int, wxString, wxString, Status, int)
		 */
		PosixJob(int id, wxString name, wxString spoolFileName, Status status,
				int procs);

		//! Start the job running
		virtual void Start();

		//! Cancel the job
		virtual void Cancel();

		//! Pause the job
		virtual void Pause();

		//! Resume the job
		virtual void Resume();

		//! Refreshes the job's status
		virtual void Refresh();

	private:
		//! Process ID
		/*! Stores the process ID returned the initial call to fork(). This is
		 * also the group ID of all the sub-processes associated with this
		 * GAMESS job.
		 */
		int mPid;
};

#endif /* __POSIX_JOB_H__ */
