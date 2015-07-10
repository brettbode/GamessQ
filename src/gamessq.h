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

#ifndef _GAMESSQ_H_
#define _GAMESSQ_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "gamessq.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "gamessq_frame.h"
////@end includes

#include <wx/app.h>
#include "job_queue_manager.h"

void printTable(int numRows, wxArrayString *rows);
/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

//! GamessQ main client application
/*!
 * GamessQApp class declaration
 */

class GamessQApp: public wxApp
{    
    DECLARE_CLASS( GamessQApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    GamessQApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin GamessQApp event handler declarations
    /// wxEVT_IDLE event handler for ID_UNIDENTIFIED
    void OnIdle( wxIdleEvent& event );

////@end GamessQApp event handler declarations

////@begin GamessQApp member function declarations
////@end GamessQApp member function declarations

////@begin GamessQApp member variables
////@end GamessQApp member variables

	//! Initialization function
	virtual bool Initialize(int& argc, wxChar **argv);

	//! wxWidgets equivalent of main(), contains the actual program code.
	virtual int OnRun();

private:
	//! Command line argument parser
	void HandleCmd(int argc, wxChar **argv);

	//! Lists all the jobs in the queue
	void ListJobs();

	//! Represents the possible commands flags
	enum Command {
		NONE = 0,
		ADD,
		REMOVE,
		PAUSE,
		RESUME,
		LIST,
		CLEAR,
		PAUSEALL,
		RESUMEALL
	};

	//! True if the GUI should be enabled
	bool mGui;

	//! True if help should be printed
	bool mHelp;

	//! Number of processors to use for added jobs
	int mProcs;

	//! Stores the command flag
	Command mCommand;

	//! Stores the names of the intput files to be added
	wxArrayString mFilenames;

	//! Stores the unique ids of the jobs given on the command line
	int *mIds;

	//! Number of elements in mIds
	int mNumItems;

	//! Stores the final return value of OnRun()
	int mRetVal;

	//! Stores the GamessQFrame object (used when adding files via the icon)
	GamessQFrame *mFrame;

#ifdef __WXMAC__
public:
	//! Called when MacOS sends an open event
	virtual void MacOpenFile(const wxString &filename);
#endif

#if not wxCHECK_VERSION(2, 8, 0)
public:
	//! Called to initialize the GUI under wx 2.6
	virtual bool OnInitGui();
#endif
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(GamessQApp)
////@end declare app

#endif
    // _GAMESSQ_H_
