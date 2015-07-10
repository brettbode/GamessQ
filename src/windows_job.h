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

#ifndef __WINDOWS_JOB_H__
#define __WINDOWS_JOB_H__

#include "job.h"

#ifndef WINVER
#define WINVER 0x0500
#define _WIN32_WINNT 0x0500
#endif

#include <wx/string.h>
#include <windows.h>

//! Windows based implementation of Job
/*!
 * Due to the fact that windows and unix have completely different process
 * management models, the actual implementation of Job is almost completely
 * different for windows than it is for posix based systems such as Mac and
 * Linux.
 *
 * The windows implementation of Job uses windows job objects to keep track of
 * the processes and threads associated with this job. A handle to the process
 * object of the main process (usually cshell) spawned by Start() is also kept
 * in a member variable so that the exit code can be retrieved once the job has
 * completed. Because windows lacks any sort of mass process or thread
 * management, I had to write it myself, hence the IterateThreads() function.
 *
 * Much of the inspiration for this code came from a couple of articles written
 * by Alex Fedotov(http://www.alexfedotov.com/). Specifically his articles,
 * "Terminating Windows Processes" and "Enumerating Windows Processes". In
 * order to redirect standard output for the log file, the following article on
 * Microsoft Help and Support was also very helpful: "How to spawn console
 * processes with redirected standard handles"
 * (http://support.microsoft.com/kb/190351).
 * 
 * \note Most of the process management code in this class digs fairly deep
 * into the windows API, don't change it unless you know what you doing. If you
 * should decide to dive into the actual code (a very education exercise), I
 * recommend you keep a copy of Firefox open to http://www.msdn.com.
 */
class WindowsJob: public Job
{
	public:
		/*! \copydoc Job::Job(wxString, int)
		 */
		WindowsJob(wxString path, int procs);

		/*! \copydoc Job::Job(int, wxString, wxString, Status, int)
		 */
		WindowsJob(int id, wxString name, wxString spoolFileName, Status status,
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

		//! \copydoc Job::Clear()
		virtual void Clear();

	private:
		//! Removes extra temp files
		void CleanUpTempFiles();

		//! Thread iterator
		bool IterateThreads(bool (*callback)(HANDLE));

		//! Static helper function
		static bool Cancel(HANDLE handle);

		//! Static helper function
		static bool Pause(HANDLE handle);

		//! Static helper function
		static bool Resume(HANDLE handle);

		//! Windows job object handle
		/*! The handle to the windows job object that is used to keep track of
		 * all the processes and threads associated with this job.
		 */
		HANDLE mJobHandle;

		//! Windows process handle
		/*! The handle to the original process spawned by Start(). The return
		 * code of this process indicates success or failure of the job.
		 */
		HANDLE mProcessHandle;
};

#endif /* __WINDOWS_JOB_H__ */
