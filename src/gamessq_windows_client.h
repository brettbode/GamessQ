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

#ifndef __GAMESSQ_WINDOWS_CLIENT_H__
#define __GAMESSQ_WINDOWS_CLIENT_H__

#include "gamessq_client.h"

#include <wx/string.h>
#include <wx/ipc.h>

//! DDE implementation of GamessQClient
/*!
 * This class uses the wxClient class to provide a DDE based implementation of
 * the client side of the GamessQ client-server communication.
 */
class GamessQWindowsClient : public wxClient, public GamessQClient
{
	public:
		//! Generic Constructor
		GamessQWindowsClient();

		//! Destructor
		~GamessQWindowsClient();

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

		//! Called by wxWdigets when it has made the connection.
		wxConnectionBase *OnMakeConnection();

	private:
		//! Helper implementation of wxConnection
		/*!
		 * Helper class, provides a bare-bones extension of wxConnection. The
		 * only real reason this class exists is so that we can detect a server
		 * disconnect.
		 */
		class ClientConn : public wxConnection
		{
			public:
				//! Constructor
				ClientConn(GamessQWindowsClient *client);
		
				//! Called when the server disconnects
				virtual bool OnDisconnect();
		
			private:
				/*! Stores the GamessQWindowsClient object
				 */
				GamessQWindowsClient *mClient;
		};

		//! Called when the server disconnects.
		void OnDisconnect();

		/*! Stores the connection object.
		 */
		wxConnectionBase *mConnection;
};

#endif /* __GAMESSQ_WINDOWS_CLIENT_H__ */
