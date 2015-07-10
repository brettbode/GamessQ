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

#include "gamessq_posix_client.h"

#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

#include "gamessq_common.h"

GamessQPosixClient::GamessQPosixClient()
{
	mFd = -1;
}

GamessQPosixClient::~GamessQPosixClient()
{
	Disconnect();
}

/*! \copydetails GamessQClient::Connect()
 */
bool GamessQPosixClient::Connect()
{
	// create the socket
	if ((mFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		LOG_ERROR("socket");
		mFd = -1;
		return false;
	}

	// create the sockaddr structure
	struct sockaddr_un name;
	memset(&name, 0, sizeof(struct sockaddr_un));
	name.sun_family = AF_UNIX;
	strcpy(name.sun_path, SOCKETNAME);

	int len = sizeof(name.sun_family) + 1 + strlen(name.sun_path);

	// try to connect
	if (connect(mFd, (struct sockaddr *)&name, len) < 0) {
		LOG_ERROR("connect");
		close(mFd);
		mFd = -1;
		return false;
	}

	// verify the connection
	char c = SOCK_CONNECTED;
	write(mFd, &c, 1);
	read(mFd, &c, 1);
	if (c == (char)SOCK_VERIFY) {
		return true;
	} else {
		wxLogError(wxT("Could not verify"));
		close(mFd);
		mFd = -1;
		return false;
	}
}

/*! \copydetails GamessQClient::Disconnect()
 */
void GamessQPosixClient::Disconnect()
{
	close(mFd);
}

/*! \copydetails GamessQClient::IsConnected()
 */
bool GamessQPosixClient::IsConnected()
{
	if (mFd == -1) {
		return false;
	}

	// poll and look for a hangup
	struct pollfd p;
	p.fd = mFd;
	p.events = 0;
	poll(&p, 1, 10);
	return (! (p.revents & POLLHUP));
}

/*! \copydetails GamessQClient::Request()
 */
wxString GamessQPosixClient::Request(const wxString &data)
{
	if (! IsConnected()) {
		return wxT("");
	}

	wxString debug;
	debug << wxT("Request: ") << data << wxT(" : ");

	// transmit that it's a request
	char c = SOCK_REQUEST;
	write(mFd, &c, 1);

	// transmit the length of the request
	int len = data.Len();
	write(mFd, &len, sizeof(int));

	// transmit the request
	write(mFd, data.c_str(), len * sizeof(wxChar));

	// read the length of the reply
	read(mFd, &len, sizeof(int));

	// read the reply
	wxChar *output = new wxChar[len];
	read(mFd, output, len * sizeof(wxChar));
	wxString retVal(output, len);

	debug << retVal;
	wxLogDebug(debug);
	return retVal;
}

/*! \copydetails GamessQClient::Execute()
 */
bool GamessQPosixClient::Execute(const wxString &data)
{
	if (! IsConnected()) {
		return false;
	}
	wxLogDebug(wxT("Execute: ") + data);

	// transmit that it's an command
	char c = SOCK_COMMAND;
	write(mFd, &c, 1);

	// transmit the length of the command
	int len = data.Len();
	write(mFd, &len, sizeof(int));

	// transmit the command
	write(mFd, data.c_str(), len * sizeof(wxChar));

	// read the result of the command
	read(mFd, &c, 1);
	return c == (char)SOCK_TRUE;
}

