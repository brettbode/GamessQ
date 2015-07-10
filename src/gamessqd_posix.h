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

#ifndef __GAMESSQD_POSIX_H__
#define __GAMESSQD_POSIX_H__

#include <wx/app.h>
#include <wx/string.h>
#include <sys/socket.h>
#include <poll.h>
#include <sys/un.h>
#include <unistd.h>

#include "gamessqd.h"
#include "gamessq_common.h"

//! Main backend application for Posix based systems.
/*!
 * This class contains a version of the application that uses Unix domain
 * sockets as the backbone for communication between the client and server.
 */
class GamessQdPosixApp: public wxAppConsole
{
	public:
		/*! Initiation function
		 */
		virtual bool OnInit();

		//! wxWidgets version of main()
		virtual int OnRun();

	private:
		//! Haneles new connections
		void HandleConnection();

		//! Handles commands and requests from the client
		void ReadCommands();

		//! GamessQServer object
		GamessQServer *mServer;

		//! Maxiumum number of file descriptors
		/*!
		 * Stores the maximum number of pollfd structures that can be stored
		 * in mFds before more space needs to be allocated
		 */
		int mMaxNumFds;

		//! poll file descriptor array
		/*!
		 * Stores the pollfd structures and thence the file descriptors for
		 * all the connections
		 */
		pollfd *mFds;

		//! Number of file descriptors
		/*!
		 * Stores the number of pollfd structures stored in mFds
		 */
		int mNumFds;

		//! Main socket
		/*!
		 * The file descriptor for the main socket where connections are
		 * accepted.
		 */
		int mSock;
};

#endif /* __GAMESSQD_POSIX_H__ */

