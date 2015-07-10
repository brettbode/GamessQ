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

#ifndef __GAMESSQ_CLIENT_H__
#define __GAMESSQ_CLIENT_H__

#include <wx/string.h>

//! GamessQ client interface
/*!
 * This class provides a standard interface to the client side of the
 * client-server communication.
 */
class GamessQClient
{
	public:
		//! Destructor
		/*!
		 * \note The reason this destructor is virtual is because further down
		 * the class hierarchy this class is inherited by another class that
		 * also inherits wxClient. In order for the wxWidgets housecleaning
		 * stuff to work, this destructor calls have to be able to go up the
		 * chain.
		 */
		virtual ~GamessQClient() {};

		//! Connect to the backend.
		/*
		 * Tries to connect to the GamessQ backend.
		 *
		 * \return True upon success, false if the connection fails.
		 */
		virtual bool Connect() = 0;

		//! Disconnect from the backend.
		virtual void Disconnect() = 0;

		//! Check connection status
		/*!
		 * Returns true if there is a working connection to the server.
		 *
		 * \return True if there is a working connection to the server.
		 */
		virtual bool IsConnected() = 0;

		//! Request data from the server.
		/*!
		 * Sends a data request to the server, and returns the reply.
		 *
		 * \param data A wxString representing the request
		 *
		 * \returns The result of the querry.
		 */
		virtual wxString Request(const wxString &data) = 0;

		//! Send a command to the server.
		/*!
		 * Send a command to the server.
		 *
		 * \param data A wxString representing the command
		 *
		 * \returns True if the command succeeded, false otherwise.
		 */
		virtual bool Execute(const wxString &data) = 0;
};

#endif /* __GAMESSQ_CLIENT_H__ */
