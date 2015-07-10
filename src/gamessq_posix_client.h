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

#ifndef __GAMESSQ_POSIX_CLIENT_H__
#define __GAMESSQ_POSIX_CLIENT_H__

#include "gamessq_client.h"

//! Unix domain sockets implementation of GamessQClient
/*!
 * This class provides a Unix domain sockets based implementation of the client
 * side of the GamessQ client-server communication.
 */
class GamessQPosixClient : public GamessQClient
{
	public:
		//! Generic Constructor
		GamessQPosixClient();

		//! Destructor
		~GamessQPosixClient();

		//! Connect to the backend.
		virtual bool Connect();

		//! Disconnect from the backend.
		virtual void Disconnect();

		//! Check connection status
		virtual bool IsConnected();

		//! Request data from the server.
		virtual wxString Request(const wxString &data);

		//! Send a command to the server.
		virtual bool Execute(const wxString &data);
	
	private:
		//! File descriptor associated with the socket
		int mFd;
};

#endif /* __GAMESSQ_POSIX_CLIENT_H__ */
