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

#include <stdio.h>
#include <string.h>

#include "gamessqd_posix.h"
#include "gamessqd.h"
#include "gamessq_common.h"

IMPLEMENT_APP(GamessQdPosixApp)

bool GamessQdPosixApp::OnInit()
{
	// attempt to detatch from everything
	fclose(stdout);
	fclose(stdin);
	fclose(stderr);
	mServer = new GamessQServer;
	return true;
}

/*!
 * This function handles the communication with the GamessQ client interface,
 * and calls the appropreate functions in GamessQServer to keep the queu going.
 *
 * \par Notes on the implementation of the unix domain socket server:
 * The backend first creates a unix domain socket, and does all the standard
 * socket stuff (create, bind, listen). It then creates an array of pollfd
 * structures containing the base socket (that accepts connections) and all of
 * the connected client sockets. It then uses poll and a while loop to deal
 * with any events on the any of the connected sockets. If poll times out (500
 * ms), it runs GamessQServer::CheckQueue(). If GamessQServer::CheckQueue()
 * returns false, and there are no client connections, the while loop breaks,
 * and the program quits.
 */
int GamessQdPosixApp::OnRun()
{
	mMaxNumFds = 10; // a nice default
	mFds = new struct pollfd[mMaxNumFds];
	fd_set fdset;

	// create the socket
	if ((mSock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		LOG_ERROR("socket");
	}
	wxLogDebug(wxT("Socket Created."));

	// start populating poll list with the main socket
	mFds[0].fd = mSock;
	mFds[0].events = POLLIN;
	mNumFds = 1;

	// set up the name of the socket
	struct sockaddr_un name;
	memset(&name, 0, sizeof(struct sockaddr_un));
	name.sun_family = AF_UNIX;
	strcpy(name.sun_path, SOCKETNAME);
	int len = sizeof(name.sun_family) + 1 + strlen(name.sun_path);

	// unlink the socket, just in case there's still one lying around
	unlink(SOCKETNAME);

	// bind the socket
	if (bind(mSock, (struct sockaddr *)&name, len) < 0) {
		LOG_ERROR("bind");
	}
	wxLogDebug(wxT("Socket Bound."));

	// listen to the socket
	if (listen(mSock, 5) < 0) {
		LOG_ERROR("listen");
	}
	wxLogDebug(wxT("Socket Listening."));
	
	bool running = true;
	// don't go into the loop until we have something. If the client doesn't
	// connect inside of 10 seconds, something's wrong.
	poll(mFds, mNumFds, 10000);
	while (running) {
		// block until activity or 500ms
		int n = poll(mFds, mNumFds, 500);
		switch (n) {
			case -1:	
				perror("poll");
				exit(1);
			case 0:
				// timeout, run CheckQueue()
				running = mServer->CheckQueue() || (mNumFds > 1);
				break;
			default:
				// we detected activity, run ReadCommands
				ReadCommands();

				// if there's been activity on the main socket, handle the new
				// connection
				if (mFds[0].revents & POLLIN) {
					HandleConnection();
				}
				break;
		}
	}
	return 0;
}

/*!
 * This function is called when poll returns activity on the main
 * socket and handles the initial verification exchange with the client,
 * and appends the new connection to mFds (allocating more space if
 * necessary).
 */
void GamessQdPosixApp::HandleConnection()
{
	wxLogDebug(wxT("Client Connected"));

	// accept the connection
	int newsock = accept(mSock, NULL, NULL);
	if (newsock < 0) {
		wxString error = wxT("accept: ");
		error << wxSysErrorMsg();
		wxLogError(error);
	}

	// verify the connection
	char c;
	read(newsock, &c, 1);
	if (c == (char)SOCK_CONNECTED) {
		c = SOCK_VERIFY;
		write(newsock, &c, 1);
	} else {
		close(newsock);
	}

	// if we don't have enough space in mFds, allocate more
	if (mNumFds == mMaxNumFds) {
		mMaxNumFds = mMaxNumFds * 2;
		pollfd *newfds = new pollfd[mMaxNumFds];
		for (int i = 0; i < mNumFds; i ++) {
			newfds[i] = mFds[i];
		}
		delete mFds;
		mFds = newfds;
	}

	// add the new socket to mFds
	mFds[mNumFds].fd = newsock;
	mFds[mNumFds].events = POLLIN;
	mFds[mNumFds].revents = 0;
	mNumFds ++;
}

/*!
 * This function is called when poll returns activity on any of the client
 * connections. It looks through all the client connections to see if any of
 * them are waiting to be read. It then reads the command to determine if it is
 * an execute or a request, and calls the appropriate GamessQServer function.
 * It then sends the result back to the client.
 */
void GamessQdPosixApp::ReadCommands()
{
	// iterate through all the client connections
	for (int i = 1; i < mNumFds; i ++) {
		// if we got a hangup...
		if (mFds[i].revents & POLLHUP) {
			wxLogDebug(wxT("Client Disconnected"));
			// close it and remove it from the list
			close(mFds[i].fd);
			mNumFds --;
			for (int j = i; j < mNumFds; j ++) {
				mFds[j] = mFds[j + 1];
			}
			i --;

		// if it was just regular activity
		} else if (mFds[i].revents & POLLIN) {
			char type;
			int len;
			// read whether it was a command or a request
			read(mFds[i].fd, &type, 1);

			// read how much data to expect
			read(mFds[i].fd, &len, sizeof(len));

			// allocate enough data (plus one for a null)
            char *data = new char[len + 1];
            read(mFds[i].fd, data, len * sizeof(char));
			data[len] = 0; //add a null

			if (type == (char)SOCK_COMMAND) {
				char retVal;
				if (mServer->OnExecute(data)) {
					retVal = SOCK_TRUE;
				} else {
					retVal = SOCK_FALSE;
				}
				// send the result
				write(mFds[i].fd, &retVal, 1);
			} else if (type == (char)SOCK_REQUEST) {
				wxString retVal = mServer->OnRequest(data);
                wxLogMessage(retVal);
				len = retVal.Len();
				// send the size of the result
				write(mFds[i].fd, &len, sizeof(int));
				// send the result
                write(mFds[i].fd, retVal.c_str(), len * sizeof(char));
			}
			delete data;
		}
	}
}

