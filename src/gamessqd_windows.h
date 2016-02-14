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

#ifndef __GAMESSQD_WINDOWS_H__
#define __GAMESSQD_WINDOWS_H__

#include <wx/app.h>
#include <wx/timer.h>
#include <wx/taskbar.h>
#include <wx/string.h>
#include <wx/ipc.h>
#include <wx/menu.h>

#include "gamessqd.h"

/*! Provides IDs for all the gamessqd events
 */
enum {
	ID_TIMER = wxID_HIGHEST + 1,
	ID_MANAGER,
	ID_LOGS,
	ID_EXIT
};

//! Taskbar icon
/*!
 * Provides the little icon down in the system tray on windows.
 */
class GamessQTaskbarIcon: public wxTaskBarIcon
{
	public:
		//! Generic constructor
		GamessQTaskbarIcon();

		//! Creates the popup menu.
		virtual wxMenu *CreatePopupMenu();
};

//! Main backend application for windows
/*!
 * This class contains a version of the application that uses Windows DDE as
 * the backbone for communication between the client and server.
 */
class GamessQdWindowsApp: public wxApp, public wxServer
{
	DECLARE_EVENT_TABLE()

	public:
		//! Initiation Function
		virtual bool OnInit();

		//! Cleanup function
		virtual int OnExit();

		//! Timer event handler
		void OnTimer(wxTimerEvent &event);

		//! "Show Manager" event handler
		void OnShowManager(wxCommandEvent &event);

		//! "View Logs" event handler
		void OnViewLogs(wxCommandEvent &event);

		//! "Exit" event handler
		void OnExitEvent(wxCommandEvent &event);

		//! Double-click event handler
		void OnDClick(wxTaskBarIconEvent &event);

		//! Client disconnect
		void OnDisconnect(wxConnection *conn);

		//! Client connect
		wxConnectionBase *OnAcceptConnection(const wxString& topic);

	private:
		//! Helper implementation of wxConnection
		/*!
		 * A helper class necessary for using the wxWidgets interfaces to DDE
		 */
		class ServerConn: public wxConnection
		{
			public:
				//! Constructor
				ServerConn(GamessQdWindowsApp *app);
		
				//! Destructor
				~ServerConn();
		
				//! Execute Command
				virtual bool OnExec(const wxString& topic, const wxString & data);
		
				//! Query for information
				virtual const void *OnRequest(const wxString& topic,
						const wxString& data, size_t *size, wxIPCFormat format);
		
				//! Client disconnect
				virtual bool OnDisconnect();
		
			private:
				//! GamessQdWindowsApp object
				GamessQdWindowsApp *mApp;
		
				//! Return value
				/*!
				 * String used for storing return values so we don't have a
				 * memory leak when we need to leave stuff on the heap.
				 */
				wxString retString;
		};

		//! Number of connected clients.
		int mNumConnections;

		//! GamessQServer object.
		GamessQServer *mServer;

		//! Timer object.
		wxTimer *mTimer;

		//! GamessQTaskbarIcon object.
		GamessQTaskbarIcon *mIcon;

		//! If there are active jobs
		/*!
		 * Stores whether or not there are jobs paused pending or running.
		 *
		 * \sa GamessQServer::CheckQueue()
		 */
		bool mRunningJobs;
};

#endif /* __GAMESSQD_WINDOWS_H__ */

