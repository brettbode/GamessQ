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

#include "gamessq_windows_client.h"

#include "gamessq_common.h"
#include "gamessq.h"

/*
 * GamessQWindowsClient implementation
 */
GamessQWindowsClient::GamessQWindowsClient()
{
	mConnection = NULL;
}

GamessQWindowsClient::~GamessQWindowsClient()
{
	Disconnect();
}

/*! \copydetails GamessQClient::Connect()
 */
bool GamessQWindowsClient::Connect()
{
	mConnection = MakeConnection(wxT(""), WX_SERVICE, WX_TOPIC);
	return mConnection != NULL;
}

/*! \copydetails GamessQClient::Disconnect()
 */
void GamessQWindowsClient::Disconnect()
{
	if (mConnection) {
		mConnection->Disconnect();
		delete mConnection;
		mConnection = NULL;
	}
}

/*! \copydetails GamessQClient::IsConnected()
 */
bool GamessQWindowsClient::IsConnected()
{
	return mConnection != NULL;
}

/*! \copydetails GamessQClient::Request()
 */
wxString GamessQWindowsClient::Request(const wxString &data)
{
	wxString output;
	wxString debug;
	debug << wxT("Request: ") << data << wxT(" : ");
	if (mConnection) {
		output = wxString(mConnection->Request(data));
		debug << output;
	} else {
		wxLogError(wxT("No Connection"));
		return wxT("");
	}
	wxLogDebug(debug);
	return output;
}

/*! \copydetails GamessQClient::Execute()
 */
bool GamessQWindowsClient::Execute(const wxString &data)
{
	wxLogDebug(wxT("Execute: ") + data);
	if (mConnection) {
		bool retVal = mConnection->Execute(data);
		return retVal;
	} else {
		wxLogError(wxT("No Connection"));
	}
	return false;
}

/*!
 * \return A new ClientConn object.
 */
wxConnectionBase *GamessQWindowsClient::OnMakeConnection()
{
	return new ClientConn(this);
}

void GamessQWindowsClient::OnDisconnect()
{
	mConnection = NULL;

	// if the server disconnects, there's nothing we can do
	wxTheApp->ExitMainLoop();
}

/*
 * GamessQClientConnection implementation
 */

/*!
 * Creates a new ClientConn with the given parent GamessQWindowsClient object.
 *
 * \param client	The GamessQWindowsClient object associated with this
 *					connection.
 */
GamessQWindowsClient::ClientConn::ClientConn(
		GamessQWindowsClient *client)
{
	mClient = client;
}

/*!
 * Simply relays the disconnect message up the line and then commits sepaku
 */
bool GamessQWindowsClient::ClientConn::OnDisconnect()
{
	mClient->OnDisconnect();
	delete this;
	return true;
}

