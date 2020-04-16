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

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "gamessq.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "gamessq.h"
#include "gamessq_common.h"
#include "job.h"
#include <wx/filename.h>
#include <wx/filefn.h>
#include <wx/cmdline.h>
#include <stdio.h>

////@begin XPM images

////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( GamessQApp )
////@end implement app


/*!
 * GamessQApp type definition
 */

IMPLEMENT_CLASS( GamessQApp, wxApp )


/*!
 * GamessQApp event table definition
 */

BEGIN_EVENT_TABLE( GamessQApp, wxApp )

////@begin GamessQApp event table entries
    EVT_IDLE( GamessQApp::OnIdle )

////@end GamessQApp event table entries

END_EVENT_TABLE()


/*!
 * Constructor for GamessQApp
 */

GamessQApp::GamessQApp()
{
    Init();
}


/*!
 * Member initialisation
 */

void GamessQApp::Init()
{
////@begin GamessQApp member initialisation
////@end GamessQApp member initialisation
}

/*!
 * Initialisation for GamessQApp
 */

bool GamessQApp::OnInit()
{    
	if (! mGui) {
		return true;
	}

////@begin GamessQApp initialisation
    // Remove the comment markers above and below this block
    // to make permanent changes to the code.

#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif
    GamessQFrame* mainWindow = new GamessQFrame( NULL );
    mainWindow->Show(true);
////@end GamessQApp initialisation

	mFrame = mainWindow;

    return true;
}


/*!
 * Cleanup for GamessQApp
 */

int GamessQApp::OnExit()
{    
	if (mGui) {
		return wxApp::OnExit();
	} else {
		return wxAppConsole::OnExit();
	}
}


/*!
 * On MacOS, Any filenames left in mFilenames are opened.
 */

void GamessQApp::OnIdle( wxIdleEvent& event )
{
	if (mFilenames.Count() > 0 && mFrame) {
		wxArrayString tmp(mFilenames);
		mFilenames.Clear();
		mFrame->OnDropFiles(tmp);
	} else {
		event.Skip();
	}
}

/*!
 * Called when MacOS sends an open event (such as dragging onto the icon). The
 * file name is appended to mFilenames, and all the dragged files are opened
 * upon the next IdleEvent. This ensures that you won't get a bunch of
 * different job options dialogs when multiple files are drug onto the icon.
 */
#ifdef __WXMAC__
void GamessQApp::MacOpenFile(const wxString &filename)
{
	mFilenames.Add(filename, 1);
}
#endif

/*!
 * Replaces wxApp::Initialize, so that command line arguments are parsed before
 * wxWidgets tries to initialize any GUI stuff. Once the command line parser
 * determines whether or not the GUI should be loaded, then it calls either
 * wxApp::Initialize() or wxAppConsole::Initialize().
 *
 * \param argc The number of command line arguments
 * \param argv The command line arguments
 *
 * \return True upon success, false otherwise
 */
bool GamessQApp::Initialize(int& argc, wxChar **argv)
{
	mRetVal = 0;
	HandleCmd(argc, argv);
	if (mRetVal != 0) {
		mGui = false;
	}

	if (mGui) {
		return wxApp::Initialize(argc, argv);
	} else {
		return wxAppConsole::Initialize(argc, argv);
	}
}

#if not wxCHECK_VERSION(2, 8, 0)
/*!
 * Replaces wxApp::OnInitGui() so that if we don't have a gui we won't
 * initialize GTK.
 */
bool GamessQApp::OnInitGui()
{
	if (mGui) {
		return wxApp::OnInitGui();
	} else {
		return true;
	}
}
#endif

/*!
 * This function deals with the command line commands, and if the GUI is
 * enabled, it then calls wxApp::OnRun()
 */
int GamessQApp::OnRun() {
	if (mHelp) {
		return mRetVal;
	}

	// if there is no command, start in GUI mode
	if (mCommand == NONE) {
		return wxApp::OnRun();
	}

	int i, j;
	bool found;
	int *queueIds;
	JobQueueManager queue;
	switch (mCommand) {
	case ADD:
		queue.Init();
		for (i = 0; i < mNumItems; i ++) {
			queue.Add(mFilenames.Item(i));
		}
		mFilenames.Clear();
		break;
	case REMOVE:
	case PAUSE:
	case RESUME:
		queue.Init();
		queueIds = queue.GetJobIds();
		for (i = 0; i < mNumItems; i ++) {
			found = false;
			for (j = 0; j < mNumItems; j ++) {
				if (mIds[i] == mIds[j]) {
					found = true;
					break;
				}
			}
			if (! found) {
				fprintf(stderr, "%d Is not a valid job ID\n", mIds[i]);
				mNumItems --;
				for (j = i; j < mNumItems; j ++) {
					mIds[j] = mIds[j + 1];
				}
			}
		}
		
		switch (mCommand) {
		case REMOVE:
			if (mNumItems == 0) {
				fprintf(stderr, "No jobs to remove\n");
			}
			for (i = 0; i < mNumItems; i ++) {
				if (! queue.Remove(mIds[i])) {
					fprintf(stderr, "Could not remove: %d\n", mIds[i]);
				}
			}
			break;
		case PAUSE:
			if (mNumItems == 0) {
				fprintf(stderr, "No jobs to pause\n");
			}
			for (i = 0; i < mNumItems; i ++) {
				if (! queue.Pause(mIds[i])) {
					fprintf(stderr, "Could not pause: %d\n", mIds[i]);
				}
			}
			break;
		case RESUME:
			if (mNumItems == 0) {
				fprintf(stderr, "No jobs to resume\n");
			}
			for (i = 0; i < mNumItems; i ++) {
				if (! queue.Resume(mIds[i])) {
					fprintf(stderr, "Could not resume: %d\n", mIds[i]);
				}
			}
			break;
            default:
                break;
		}
		delete queueIds;
		delete mIds;
		mIds = NULL;
		break;
	case LIST:
		ListJobs();
		break;
	case CLEAR:
		queue.Init();
		queue.Clear();
		break;
	case PAUSEALL:
		queue.Init();
		queue.SetActive(false);
		break;
	case RESUMEALL:
		queue.Init();
		queue.SetActive(true);
		break;
    default:
        break;
	}

	if (mGui) {
		return wxApp::OnRun();
	} else {
		return mRetVal;
	}
}

//! Command line description
#if wxCHECK_VERSION(2, 9, 0)
static const wxCmdLineEntryDesc cmdLineDesc[] =
{
	{ wxCMD_LINE_SWITCH, "g", "gui", "enable the gui" },

	{ wxCMD_LINE_SWITCH, "h", "help",	
			"print this help and exit" },

	{ wxCMD_LINE_OPTION, "p", "procs",
			"number of processors for added jobs", wxCMD_LINE_VAL_NUMBER },

	{ wxCMD_LINE_PARAM, NULL, NULL, "command", wxCMD_LINE_VAL_STRING,
			wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE },

	{ wxCMD_LINE_NONE }
};
#else
static const wxCmdLineEntryDesc cmdLineDesc[] =
{
    { wxCMD_LINE_SWITCH, wxT("g"), wxT("gui"), wxT("enable the gui") },
    
    { wxCMD_LINE_SWITCH, wxT("h"), wxT("help"),
        wxT("print this help and exit") },
    
    { wxCMD_LINE_OPTION, wxT("p"), wxT("procs"),
        wxT("number of processors for added jobs"), wxCMD_LINE_VAL_NUMBER },
    
    { wxCMD_LINE_PARAM, NULL, NULL, wxT("command"), wxCMD_LINE_VAL_STRING,
        wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE },
    
    { wxCMD_LINE_NONE }
};
#endif

/*! Parses the command line arguments, and stores the results in member
 * variables
 *
 * \param argc The number of command line arguments
 * \param argv The command line arguments
 */
void GamessQApp::HandleCmd(int argc, wxChar **argv)
{
	mHelp = false;
	mGui = false;
	mProcs = 1;
	mIds = NULL;
	mCommand = NONE;
	mRetVal = 0;
	long tmp;

	wxCmdLineParser parser(cmdLineDesc, argc, argv);
	parser.Parse();
	
	if (parser.Found(wxT("h"))) {
		parser.Usage();
		mHelp = true;
		return;
	}

	if (parser.GetParamCount() == 0) {
		mGui = true;
		return;
	}
	
	if (parser.Found(wxT("g"))) {
		mGui = true;
	}

	if (parser.Found(wxT("p"), &tmp)) {
		mProcs = (int)tmp;
	}

	// we won't get to this point unless we have at least one parameter
	wxString cmd = parser.GetParam(0);

	// convert the given string into a comand
	if (cmd == wxT("add")) {
		mCommand = ADD;
	} else if (cmd == wxT("remove") || cmd == wxT("rm")) {
		mCommand = REMOVE;
	} else if (cmd == wxT("pause")) {
		mCommand = PAUSE;
	} else if (cmd == wxT("resume")) {
		mCommand = RESUME;
	} else if (cmd == wxT("clear")) {
		mCommand = CLEAR;
	} else if (cmd == wxT("list") || cmd == wxT("ls")) {
		mCommand = LIST;
	} else if (cmd == wxT("pause-all")) {
		mCommand = PAUSEALL;
	} else if (cmd == wxT("resume-all")) {
		mCommand = RESUMEALL;
	}

	int i = 1;
	wxString arg;
	wxFileName filename;
	mNumItems = 0;
	switch (mCommand) {
	case NONE:
		// if no command is given GUI mode is assumed
		mGui = true;
		i = 0;
	case ADD:
		mFilenames.Alloc(parser.GetParamCount() - i);
		for (; i < parser.GetParamCount(); i ++) {
			arg = parser.GetParam(i);
			filename = wxFileName(arg);
			if (wxFileExists(arg) && filename.GetExt() == wxT("inp")) {
				mFilenames.Add(arg);
				mNumItems ++;
			} else {
				fprintf(stderr, "Invalid input file: ");
				fprintf(stderr, arg.ToAscii());
				fprintf(stderr, "\n");
			}
		}
		break;
	case REMOVE:
	case PAUSE:
	case RESUME:
		mIds = new int[parser.GetParamCount() - 1];
		for (; i < parser.GetParamCount(); i ++) {
			arg = parser.GetParam(i);
			if (arg.ToLong(&tmp, 10)) {
				mIds[mNumItems] = (int)tmp;
				mNumItems++;
			} else {
				fprintf(stderr, "Invalid job ID: ");
				fprintf(stderr, arg.ToAscii());
				fprintf(stderr, "\n");
			}
		}
		break;
	case CLEAR:
	case LIST:
	case PAUSEALL:
	case RESUMEALL:
		if (parser.GetParamCount() != 1) {
			fprintf(stderr, cmd.ToAscii());
			fprintf(stderr, " does not take any arguments.\n");
			mRetVal = 1;
			return;
		}
		break;
	}
}

/*!
 * Gets a full list of jobs from the server, and dumps them to stdout
 */
void GamessQApp::ListJobs()
{
	JobQueueManager queue;
	if (! queue.Init()) {
		printf("Could not connect to queue!\n");
		return;
	}

	int *ids = queue.GetJobIds();
	JobList list;
	list.DeleteContents(true);
	int numJobs = ids[0];
	for (int i = 1; i <= numJobs; i ++) {
		wxString name = queue.GetName(ids[i]);
		Job::Status status = queue.GetStatus(ids[i]);
		int procs = queue.GetNumProcessors(ids[i]);
		Job *job = new Job(ids[i], name, wxT(""), status, procs);
		list.Append(job);
	}
	delete ids;

	if (queue.IsActive()) {
		printf("Queue Status: Active\n");
	} else {
		printf("Queue Status: Paused\n");
	}
	int width = 20;
	JobList::compatibility_iterator node;
	for (node = list.GetFirst(); node; node = node->GetNext()) {
		if (node->GetData()->GetName().Len() > width) {
			width = (int) node->GetData()->GetName().Len();
		}
	}

	wxArrayString *rows = new wxArrayString[numJobs + 1];
	rows[0].Add(wxT("ID:"));
	rows[0].Add(wxT("Name:"));
	rows[0].Add(wxT("Processors:"));
	rows[0].Add(wxT("Status:"));
	int i = 1;
	for (node = list.GetFirst(); node; node = node->GetNext()) {
		Job *job = node->GetData();
		wxString id;
		id << job->GetId();
		rows[i].Add(id);
		rows[i].Add(job->GetName());
		wxString numProcs;
		numProcs << job->GetNumProcessors();
		rows[i].Add(numProcs);
		rows[i].Add(job->GetStatusString());
		i ++;
	}
	printTable(numJobs + 1, rows);
}

void printTable(int numRows, wxArrayString *rows)
{
	int columns = 0;
	for (int i = 0; i < numRows; i ++) {
		if (rows[i].Count() > columns) {
			columns = (int) rows[i].Count();
		}
	}

	int *width = new int[columns];
	for (int i = 0; i < columns; i ++) {
		width[i] = 0;
	}

	for (int i = 0; i < numRows; i ++) {
		for (int j = 0; j < rows[i].Count(); j ++) {
			if (width[j] < rows[i].Item(j).Len()) {
				width[j] = (int) rows[i].Item(j).Len();
			}
		}
	}

	wxString format;
	for (int i = 0; i < numRows; i ++) {
		for (int j = 0; j < rows[i].Count(); j++) {
			printf(" ");
			printf(rows[i].Item(j).ToAscii());
            int spaces = (int) (width[j] - rows[i].Item(j).Len());
			for (int k = 0; k < spaces; k ++) {
				printf(" ");
			}
		}
		printf("\n");
	}
	delete[] width;
}

