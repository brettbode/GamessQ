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

#ifndef __GAMESSQD_H__
#define __GAMESSQD_H__

#include <wx/config.h>
#include <wx/string.h>
#include <wx/ffile.h>
#include <wx/log.h>

#include "job.h"

//! Queue management
/*!
 * This class manages the actual queue, as well as handles all actual messages
 * to and from the client interface. Any changes to the way the queue acts
 * should be made here.
 */
class GamessQServer
{
	public:
		//! Default Constructor
		GamessQServer();

		//! Destructor
		virtual ~GamessQServer();

		//! Handle commands from the client
		bool OnExecute(const wxString &data);

		//! Handles requests from the client
		wxString OnRequest(const wxString &data);

		//! Check and refresh queue status
		bool CheckQueue();

	private:
		//! Reads one line from a file and returns it as a wxString.
		static wxString ReadLine(wxFFile *file);

		//! Loads the queue from it's save file
		void LoadQueue();

		//! Saves the current status of the entire queue to a file
		void SaveQueue();

		//! Finds the given job in the queue
		JobList::compatibility_iterator getJob(wxString n);

		//! The job queue
		JobList mJobQueue;

		//! The logger object associated with the log file
		wxLog *mLogger;

		//! The log file object
		wxFFile *mLogFile;

		//! List of jobs paused when queue was deactivated
		/*!
		 * Because there may have been paused jobs already when the queue was
		 * paused, this list allows us to get back the original state of
		 * the queue when the whole thing is resumed.
		 */
		JobList *mPausedList;

		//! Queue is active?
		bool mActive;
};

#endif /* __GAMESSQD_H__*/
