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

#include "gamessqd_windows.h"

#include <wx/filename.h>
#include <wx/arrstr.h>
#include <wx/msgdlg.h>

#include "gamessqd.h"
#include "log_viewer.h"
#include "resources.h"
#include "gamessq_common.h"

/*
 * GamessQd implementation
 */

IMPLEMENT_APP(GamessQdWindowsApp)

BEGIN_EVENT_TABLE(GamessQdWindowsApp, wxApp)
	EVT_TIMER(ID_TIMER, GamessQdWindowsApp::OnTimer)
	EVT_MENU(ID_MANAGER, GamessQdWindowsApp::OnShowManager)
	EVT_MENU(ID_LOGS, GamessQdWindowsApp::OnViewLogs)
	EVT_MENU(ID_EXIT, GamessQdWindowsApp::OnExitEvent)
	EVT_TASKBAR_LEFT_DCLICK(GamessQdWindowsApp::OnDClick)
END_EVENT_TABLE()

/*!
 * Initiates a bunch of stuff such as the taskbar icon and timer.
 */
bool GamessQdWindowsApp::OnInit()
{
	mServer = new GamessQServer;
	if (! Create(WX_SERVICE)) {
		wxLogFatalError(wxT("Cannot create Connection!"));
		return false;
	}
	wxLogDebug(wxT("Connection created"));
	mTimer = new wxTimer(this, ID_TIMER);
	mTimer->Start(1000);
	mIcon = new GamessQTaskbarIcon();
	mRunningJobs = false;
	mNumConnections = 0;
	return true;
}

/*!
 * Called when the application is about to exit.
 */
int GamessQdWindowsApp::OnExit()
{
	delete mServer;
	delete mTimer;
	mIcon->RemoveIcon();
	delete mIcon;
	return 0;
}

/*!
 * This function is called once every second, it simply calls
 * GamessQServer::CheckQueue()
 */
void GamessQdWindowsApp::OnTimer(wxTimerEvent &event) {
	mRunningJobs = mServer->CheckQueue();
}

/*!
 * Called when someone clicks "Show Manager" in the popup menu
 *
 * This function attempts to launch the client application.
 *
 * \param event The menu selection event
 */
void GamessQdWindowsApp::OnShowManager(wxCommandEvent &event)
{
	wxLogDebug(wxT("Show Manager"));
	wxFileName filename(wxTheApp->argv[0]);
	filename.SetFullName(wxT("gamessq.exe"));
	wxExecute(filename.GetFullPath());
}

/*!
 * Called when someone clicks "View Logs" in the popup menu
 *
 * Creates a new LogViewer frame with the main queue log file and
 * displays it.
 *
 * \param event The menu selection event
 */
void GamessQdWindowsApp::OnViewLogs(wxCommandEvent &event)
{
	wxLogDebug(wxT("View Logs"));
	wxFileName filename(Job::GetSpoolDir(), wxT("server.log"));
	wxArrayString arr;
	arr.Add(filename.GetFullPath());
	LogViewer *viewer = new LogViewer(NULL, arr);
	viewer->Show();
}

/*!
 * Called when someone clicks "Exit" in the popup menu
 *
 * \param event The menu selection event
 */
void GamessQdWindowsApp::OnExitEvent(wxCommandEvent &event)
{
	wxLogDebug(wxT("Exit"));
	if (mRunningJobs) {
		wxMessageDialog ask(NULL, wxT("Are you sure you want to quit GamessQ? There are still active jobs in the queue. These jobs will be restarted from the beginning the next time you start GamessQ."), wxT("Are you sure?"), wxOK | wxCANCEL);
		if (ask.ShowModal() == wxID_OK) {
			ExitMainLoop();
		}
	} else {
		ExitMainLoop();
	}
}

/*!
 * Called when someone double-clicks the taskbar icon
 *
 * Same as OnShowManager()
 *
 * \param event The double-click event
 */
void GamessQdWindowsApp::OnDClick(wxTaskBarIconEvent &event)
{
	wxLogDebug(wxT("Show Manager"));
	wxFileName filename(wxTheApp->argv[0]);
	filename.SetFullName(wxT("gamessq.exe"));
	wxExecute(filename.GetFullPath());
}

/*!
 * Called when a client disconnects
 *
 * Decrements the connection counter and deletes the connection.
 *
 * \param conn the connection that was disconnected
 */
void GamessQdWindowsApp::OnDisconnect(wxConnection *conn)
{
	mNumConnections --;
}

/*!
 * Called when a new connection is received.
 * 
 * \param topic the DDE topic from the client
 *
 * \return A new GamessQWindowsServerConn object
 */
wxConnectionBase *GamessQdWindowsApp::OnAcceptConnection(
		const wxString& topic)
{
	mNumConnections ++;
	return new ServerConn(this);
}

/*
 * GamessQdWindowsApp::ServerConn member functions
 */

/*!
 * Creates a new GamessQWindowsServerConn object associated with the
 * given application and GamessQServer object.
 *
 * \param app The GamessQdWindowsApp object for the currently running
 * program
 */
GamessQdWindowsApp::ServerConn::ServerConn(GamessQdWindowsApp *app)
{
	mApp = app;
}

GamessQdWindowsApp::ServerConn::~ServerConn()
{
	Disconnect();
}

/*!
 * This function is called upon receiving a command from the client and relays
 * this command to GamessQServer::OnExecute()
 *
 * \param topic The DDE topic for this command (unused)
 * \param data The command data
 * \param size The size of the command (unused)
 * \param format The format of the command (unused)
 *
 * \return True if the command succeeded, false otherwise
 */
bool GamessQdWindowsApp::ServerConn::OnExecute(const wxString& topic,
		wxChar *data, int size, wxIPCFormat format)
{
	if (mApp->mServer) {
		return mApp->mServer->OnExecute(wxString(data));
	}
	return false;
}

/*! This function is called upon receiving a request from the client and realys
 * this query to GamessQServer::OnRequest()
 *
 * \param topic The DDE topic for this request (unused)
 * \param data The request data
 * \param size The size of the request (unused)
 * \param format The format of the request (unused)
 *
 * \return The result of the query
 */
wxChar *GamessQdWindowsApp::ServerConn::OnRequest(const wxString& topic,
		const wxString& data, int *size, wxIPCFormat format)
{
	if (! mApp->mServer) {
		return NULL;
	}

	retString = mApp->mServer->OnRequest(data);

	if (size) {
		*size = (retString.Len() + 1) * sizeof(wxChar);
	}

	return (wxChar *)retString.c_str();
}

/*!
 * This function is called when the client associated with this connection
 * disconnects. It calls GamessQdWindowsApp::OnDisconnect() and deletes the
 * connection
 *
 * \return True
 */
bool GamessQdWindowsApp::ServerConn::OnDisconnect()
{
	mApp->OnDisconnect(this);
	delete this;
	return true;
}

/*
 * GamessQTaskbarIcon implementation
 */
GamessQTaskbarIcon::GamessQTaskbarIcon() : wxTaskBarIcon()
{
	SetIcon(GetGamessQIcon(), wxT("GamessQ"));
}

/*!
 * \return The newly created popup menu.
 */
wxMenu *GamessQTaskbarIcon::CreatePopupMenu()
{
	wxLogDebug(wxT("CreatePopupMenu()"));
	wxMenu *menu = new wxMenu;
	menu->Append(ID_MANAGER, wxT("Show &Manager"), _("Show the queue manager"));
	menu->Append(ID_LOGS, wxT("View &Log"), _("View the queue log"));
	menu->AppendSeparator();
	menu->Append(ID_EXIT, wxT("E&xit"), _("Quit GamessQ"));
	return menu;
}

