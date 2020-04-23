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
#pragma implementation "gamessq_frame.h"
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

#include "gamessq_frame.h"
#include "gamessq_common.h"
#include "log_viewer.h"
#include "resources.h"
#include "about_gamessq.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>

////@begin XPM images
////@end XPM images


/*!
 * GamessQFrame type definition
 */

IMPLEMENT_CLASS( GamessQFrame, wxFrame )


/*!
 * GamessQFrame event table definition
 */

BEGIN_EVENT_TABLE( GamessQFrame, wxFrame )

////@begin GamessQFrame event table entries
    EVT_MENU( wxID_PREFERENCES, GamessQFrame::OnSETUPClick )

    EVT_MENU( ID_PAUSEALL, GamessQFrame::OnPauseallClick )

    EVT_MENU( wxID_EXIT, GamessQFrame::OnEXITClick )

    EVT_MENU( wxID_ADD, GamessQFrame::OnADDClick )

    EVT_MENU( wxID_REMOVE, GamessQFrame::OnRemoveClick )

    EVT_MENU( wxID_CANCEL, GamessQFrame::OnCancelClick )

    EVT_MENU( ID_PAUSE, GamessQFrame::OnPauseClick )

    EVT_MENU( ID_RESUME, GamessQFrame::OnResumeClick )

    EVT_MENU( ID_SAVEFOLDER, GamessQFrame::OnSavefolderClick )

    EVT_MENU( ID_SAVEAS, GamessQFrame::OnSaveasClick )

    EVT_MENU( ID_MACMOLPLT, GamessQFrame::OnMacmolpltClick )

    EVT_MENU( ID_VIEWLOGS, GamessQFrame::OnViewlogsClick )

    EVT_MENU( ID_REFRESH, GamessQFrame::OnRefreshClick )

    EVT_MENU( wxID_CLEAR, GamessQFrame::OnClearClick )

    EVT_MENU( wxID_ABOUT, GamessQFrame::OnAboutClick )

    EVT_LIST_ITEM_SELECTED( ID_JOBLISTCTRL, GamessQFrame::OnJoblistctrlSelected )
    EVT_LIST_ITEM_DESELECTED( ID_JOBLISTCTRL, GamessQFrame::OnJoblistctrlDeselected )
    EVT_LIST_BEGIN_LABEL_EDIT( ID_JOBLISTCTRL, GamessQFrame::OnJoblistctrlBeginLabelEdit )
    EVT_LIST_END_LABEL_EDIT( ID_JOBLISTCTRL, GamessQFrame::OnJoblistctrlEndLabelEdit )
    EVT_LIST_ITEM_RIGHT_CLICK( ID_JOBLISTCTRL, GamessQFrame::OnJoblistctrlRightClick )

////@end GamessQFrame event table entries
    EVT_TIMER( ID_TIMER, GamessQFrame::OnTimer )
	EVT_MENU( ID_SETUP_OK, GamessQFrame::OnSETUPOk )

END_EVENT_TABLE()


/*!
 * GamessQFrame constructors
 */

GamessQFrame::GamessQFrame()
{
    Init();
}

GamessQFrame::GamessQFrame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*!
 * GamessQFrame creator
 */

bool GamessQFrame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetGamessQIcon());
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
    return true;
}


/*!
 * GamessQFrame destructor
 */

GamessQFrame::~GamessQFrame()
{
////@begin GamessQFrame destruction
////@end GamessQFrame destruction
}


/*!
 * Member initialisation
 */

void GamessQFrame::Init()
{
////@begin GamessQFrame member initialisation
    mRefreshFrequency = 10000;
    AppMenu = NULL;
    jobsMenu = NULL;
    jobsToolbar = NULL;
    jobListCtrl = NULL;
////@end GamessQFrame member initialisation
}


/*!
 * Control creation for GamessQFrame
 */

void GamessQFrame::CreateControls()
{    
////@begin GamessQFrame content construction
    GamessQFrame* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    AppMenu = new wxMenu;
    AppMenu->Append(wxID_PREFERENCES, _("&Preferences"), _("Change the way GamessQ behaves"), wxITEM_NORMAL);
    AppMenu->Append(ID_PAUSEALL, _("&Pause All"), _("Pause Everything"), wxITEM_CHECK);
    AppMenu->AppendSeparator();
    AppMenu->Append(wxID_EXIT, _("E&xit"), _("Exit GamessQ"), wxITEM_NORMAL);
    menuBar->Append(AppMenu, _("&Queue"));
    jobsMenu = new wxMenu;
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, wxID_ADD, _("&Add Jobs\tCtrl+A"), _("Add new GAMESS jobs to the end of the queue"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/add-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, wxID_REMOVE, _("Remove Jobs"), _("Remove the selected GAMESS jobs from the queue"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/remove-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    jobsMenu->AppendSeparator();
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, wxID_CANCEL, _("&Cancel Jobs"), _("Cancel the selected GAMESS jobs"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/cancel-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, ID_PAUSE, _("&Pause Jobs\tCtrl+P"), _("Pause the selected GAMESS jobs"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/pause-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, ID_RESUME, _("&Resume Jobs\tCtrl+R"), _("Resume the selected paused GAMESS jobs"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/resume-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    jobsMenu->AppendSeparator();
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, ID_SAVEFOLDER, _("Save Output to Folder\tCtrl+S"), _("Save GAMESS output for selected jobs to a folder"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("menu/wxSave")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, ID_SAVEAS, _("Save Output As..."), _("Save GAMESS output for this job"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("menu/wxSaveAs")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    jobsMenu->Append(ID_MACMOLPLT, _("Open in wxMacMolPlt\tCtrl+O"), _T(""), wxITEM_NORMAL);
    jobsMenu->Append(ID_VIEWLOGS, _("View &Logs"), _("View the GAMESS log files for the selected jobs"), wxITEM_NORMAL);
    jobsMenu->AppendSeparator();
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, ID_REFRESH, _("Refresh\tAlt-R"), _("Refresh the display of the queue status"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/refresh-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    jobsMenu->AppendSeparator();
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, wxID_CLEAR, _("Clean Up"), _("Clear all finished GAMESS jobs from the queue"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/clear-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    menuBar->Append(jobsMenu, _("&Jobs"));
    
    //Yes this is a complete copy of the menu that was just added to the Menubar. At least in
    //wx 3.0 wx throws an assertion saying that a popupmenu is not supposed to be attached to
    //another menu or menubar. So after attaching the previous copy recreate it. The copy
    //attached to the menu is now "owned" by the Menu/window.
    
    jobsMenu = new wxMenu;
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, wxID_ADD, _("&Add Jobs\tCtrl+A"), _("Add new GAMESS jobs to the end of the queue"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/add-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, wxID_REMOVE, _("Remove Jobs"), _("Remove the selected GAMESS jobs from the queue"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/remove-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    jobsMenu->AppendSeparator();
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, wxID_CANCEL, _("&Cancel Jobs"), _("Cancel the selected GAMESS jobs"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/cancel-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, ID_PAUSE, _("&Pause Jobs\tCtrl+P"), _("Pause the selected GAMESS jobs"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/pause-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, ID_RESUME, _("&Resume Jobs\tCtrl+R"), _("Resume the selected paused GAMESS jobs"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/resume-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    jobsMenu->AppendSeparator();
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, ID_SAVEFOLDER, _("Save Output to Folder\tCtrl+S"), _("Save GAMESS output for selected jobs to a folder"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("menu/wxSave")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, ID_SAVEAS, _("Save Output As..."), _("Save GAMESS output for this job"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("menu/wxSaveAs")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    jobsMenu->Append(ID_MACMOLPLT, _("Open in wxMacMolPlt\tCtrl+O"), _T(""), wxITEM_NORMAL);
    jobsMenu->Append(ID_VIEWLOGS, _("View &Logs"), _("View the GAMESS log files for the selected jobs"), wxITEM_NORMAL);
    jobsMenu->AppendSeparator();
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, ID_REFRESH, _("Refresh\tAlt-R"), _("Refresh the display of the queue status"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/refresh-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }
    jobsMenu->AppendSeparator();
    {
        wxMenuItem* menuItem = new wxMenuItem(jobsMenu, wxID_CLEAR, _("Clean Up"), _("Clear all finished GAMESS jobs from the queue"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("icons/clear-16.png")));
        menuItem->SetBitmap(bitmap);
        jobsMenu->Append(menuItem);
    }

    wxMenu* itemMenu24 = new wxMenu;
    itemMenu24->Append(wxID_ABOUT, _("&About"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu24, _("&Help"));
    itemFrame1->SetMenuBar(menuBar);

    jobsToolbar = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL|wxTB_TEXT|wxTB_NODIVIDER, ID_TOOLBAR1 );
    jobsToolbar->SetToolBitmapSize(wxSize(24, 24));
    wxBitmap itemtool27Bitmap(itemFrame1->GetBitmapResource(wxT("icons/add-24.png")));
    wxBitmap itemtool27BitmapDisabled;
    jobsToolbar->AddTool(wxID_ADD, _("Add"), itemtool27Bitmap, itemtool27BitmapDisabled, wxITEM_NORMAL, _T(""), _("Add a new GAMESS jobs to the end of the queue"));
    wxBitmap itemtool28Bitmap(itemFrame1->GetBitmapResource(wxT("icons/remove-24.png")));
    wxBitmap itemtool28BitmapDisabled;
    jobsToolbar->AddTool(wxID_REMOVE, _("Remove"), itemtool28Bitmap, itemtool28BitmapDisabled, wxITEM_NORMAL, _T(""), _("Remove the selected GAMESS jobs from the queue"));
    jobsToolbar->AddSeparator();
    wxBitmap itemtool30Bitmap(itemFrame1->GetBitmapResource(wxT("icons/cancel-24.png")));
    wxBitmap itemtool30BitmapDisabled;
    jobsToolbar->AddTool(wxID_CANCEL, _("Cancel"), itemtool30Bitmap, itemtool30BitmapDisabled, wxITEM_NORMAL, _T(""), _("Cancel the selected GAMESS jobs"));
    wxBitmap itemtool31Bitmap(itemFrame1->GetBitmapResource(wxT("icons/pause-24.png")));
    wxBitmap itemtool31BitmapDisabled;
    jobsToolbar->AddTool(ID_PAUSE, _("Pause"), itemtool31Bitmap, itemtool31BitmapDisabled, wxITEM_NORMAL, _T(""), _("Pause the selected GAMESS jobs"));
    wxBitmap itemtool32Bitmap(itemFrame1->GetBitmapResource(wxT("icons/resume-24.png")));
    wxBitmap itemtool32BitmapDisabled;
    jobsToolbar->AddTool(ID_RESUME, _("Resume"), itemtool32Bitmap, itemtool32BitmapDisabled, wxITEM_NORMAL, _T(""), _("Resume the selected paused GAMESS jobs"));
    jobsToolbar->AddSeparator();
    wxBitmap itemtool34Bitmap(itemFrame1->GetBitmapResource(wxT("icons/refresh-24.png")));
    wxBitmap itemtool34BitmapDisabled;
    jobsToolbar->AddTool(ID_REFRESH, _("Refresh"), itemtool34Bitmap, itemtool34BitmapDisabled, wxITEM_NORMAL, _T(""), _("Refresh the display of the queue status"));
    jobsToolbar->AddSeparator();
    wxBitmap itemtool36Bitmap(itemFrame1->GetBitmapResource(wxT("icons/clear-24.png")));
    wxBitmap itemtool36BitmapDisabled;
    jobsToolbar->AddTool(wxID_CLEAR, _("Clean Up"), itemtool36Bitmap, itemtool36BitmapDisabled, wxITEM_NORMAL, _T(""), _("Clear all finished GAMESS jobs from the queue"));
    jobsToolbar->Realize();
    itemFrame1->SetToolBar(jobsToolbar);

    wxBoxSizer* itemBoxSizer37 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer37);

    jobListCtrl = new wxListCtrl( itemFrame1, ID_JOBLISTCTRL, wxDefaultPosition, wxSize(590, 300), wxLC_REPORT|wxLC_EDIT_LABELS|wxLC_HRULES|wxLC_VRULES|wxSUNKEN_BORDER );
    itemBoxSizer37->Add(jobListCtrl, 1, wxGROW|wxALL, 0);

    wxStatusBar* itemStatusBar39 = new wxStatusBar( itemFrame1, ID_STATUSBAR1, wxNO_BORDER );
    itemStatusBar39->SetFieldsCount(1);
    itemFrame1->SetStatusBar(itemStatusBar39);

////@end GamessQFrame content construction

	// create the dialog objects
	mConfigDialog = new ConfigurationDialog(this);
	mJobOptionsDialog = new JobOptionsDialog(this);
	
	// initiate the queue manager, if this fails, we're toast
	if (! mQueueManager.Init()) {
//        wxLogFatalError(wxT("Failed to start the backend!"));
        // The above calls abort which appears to exit without a useful message on the Mac.
        // Simply posting an alert here and letting the frontend run seems ok and at least
        // gets the message to the user.
        wxLogError(wxT("Failed to start the backend! Most functions will not work!"));
	}
	mActive = mQueueManager.IsActive();
	AppMenu->Check(ID_PAUSEALL, ! mActive);

	// load settings
	mConfig = new wxConfig(WX_CONFIG_APPNAME);
	wxString stringFreq;
	long freq=0;
	//Attempt to read the only Gui specific pref. If successful a prefs file must exist.
	if (mConfig->Read(wxT("Refresh Frequency"), &freq)) {
		mRefreshFrequency = (int) freq;
	}
	//If no prefs file or the spoolDir is invalid through up the prefs
	bool path_test = ValidatePath(mQueueManager.GetSpoolDir());
	if (!path_test)
		wxLogError(wxT("Error: Non-allowed characters in the spool Dir. Please correct before continuing!"));
	if ((freq==0)|| ! path_test ){
		mConfigDialog->SetRefreshFrequency(mRefreshFrequency / 1000);
		mConfigDialog->SetGamessPath(mQueueManager.GetGamessDir());
		mConfigDialog->SetSpoolDir(mQueueManager.GetSpoolDir());
		mConfigDialog->Show();
	}

	// start the timer
	mRefreshTimer = new wxTimer(this, ID_TIMER);
	mRefreshTimer->Start(mRefreshFrequency, wxTIMER_ONE_SHOT);

	// set up the queue list
	jobListCtrl->InsertColumn(0, wxT("Name"));
	jobListCtrl->InsertColumn(1, wxT("Processors"));
	jobListCtrl->InsertColumn(2, wxT("Status"));
	jobListCtrl->SetColumnWidth(0, 400);
	jobListCtrl->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
	jobListCtrl->SetColumnWidth(2, 80);

	RefreshList();
	RefreshButtons();
	SetDropTarget(new DropTarget(this));
}

/*!
 * This function is called when the user clicks "Configuration". It loads the
 * values into the configuration dialog and shows it.
 *
 * \sa ConfigurationDialog
 */

void GamessQFrame::OnSETUPClick( wxCommandEvent& event )
{
	mConfigDialog->SetRefreshFrequency(mRefreshFrequency / 1000);
	mConfigDialog->SetGamessPath(mQueueManager.GetGamessDir());
	mConfigDialog->SetSpoolDir(mQueueManager.GetSpoolDir());
	mConfigDialog->Show();
}

/*!
 * This function is called whenever the user clicks "ok" on the configuration
 * dialog. It reads the values from the configuration dialog, applies them and
 * updates the settings file/registry.
 *
 * \sa ConfigurationDialog::OnOkClick()
 */
void GamessQFrame::OnSETUPOk( wxCommandEvent& event)
{
	mRefreshFrequency = mConfigDialog->GetRefreshFrequency() * 1000;
	mQueueManager.SetSpoolDir(mConfigDialog->GetSpoolDir());
	mConfig->Write(wxT("Spool Directory"), mConfigDialog->GetSpoolDir());
	mQueueManager.SetGamessDir(mConfigDialog->GetGamessPath());
	mConfig->Write(wxT("GAMESS path"), mConfigDialog->GetGamessPath());
	mConfig->Write(wxT("Refresh Frequency"), mRefreshFrequency);
	mConfig->Flush();
}


/*!
 * This function is called when the user clicks "Exit", it closes the
 * application.
 */
void GamessQFrame::OnEXITClick( wxCommandEvent& event )
{
	Close();
}


/*!
 * This function is called whenever the user clicks "Add". It calls NewJob().
 */
void GamessQFrame::OnADDClick( wxCommandEvent& event )
{
	GreyButtons();
	NewJob();
	RefreshList();
	RefreshButtons();
}


/*!
 * This function is called when the user clicks "Cancel". It looks through the
 * selected items and tells the server to cancel whichever ones can be
 * canceled.
 *
 * \sa CanCancel(job *)
 */
void GamessQFrame::OnCancelClick( wxCommandEvent& event )
{
	GreyButtons();
	int index = (int) jobListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED);
	while (index != -1) {
		Job *job = mJobList.Item(index)->GetData();
		if (CanCancel(job)) {
			mQueueManager.Cancel(job->GetId());
		}
		index = (int) jobListCtrl->GetNextItem(index, wxLIST_NEXT_ALL,
				wxLIST_STATE_SELECTED);
	}
	RefreshList();
	RefreshButtons();
}


/*!
 * This function is called when the user clicks "Pause". It looks through the
 * selected items and tells the server to pause whichever ones can be paused.
 *
 * \sa CanPause(job *)
 */
void GamessQFrame::OnPauseClick( wxCommandEvent& event )
{
	GreyButtons();
	int index = (int) jobListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED);
	while (index != -1) {
		Job *job = mJobList.Item(index)->GetData();
		if (CanPause(job)) {
			mQueueManager.Pause(job->GetId());
		}
		index = (int) jobListCtrl->GetNextItem(index, wxLIST_NEXT_ALL,
				wxLIST_STATE_SELECTED);
	}
	RefreshList();
	RefreshButtons();
}


/*!
 * This function is called when the user clicks "Resume". It looks through the
 * selected items and tells the server to resume whichever ones can be resumed.
 *
 * \sa CanResume(job *)
 */
void GamessQFrame::OnResumeClick( wxCommandEvent& event )
{
	GreyButtons();
	int index = (int) jobListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED);
	while (index != -1) {
		Job *job = mJobList.Item(index)->GetData();
		if (CanResume(job)) {
			mQueueManager.Resume(job->GetId());
		}
		index = (int) jobListCtrl->GetNextItem(index, wxLIST_NEXT_ALL,
				wxLIST_STATE_SELECTED);
	}
	RefreshList();
	RefreshButtons();
}

/*!
 * This function is called when the refresh timer goes off (at a
 * user-determined interval). It calls RefreshList().
 */
void GamessQFrame::OnTimer( wxTimerEvent& event )
{
	if (mConnected) {
		GreyButtons();
		RefreshList();
		RefreshButtons();
	}
}

/*!
 * This function is called when the user clicks "Refresh", it calls
 * RefreshList().
 */

void GamessQFrame::OnRefreshClick( wxCommandEvent& event )
{
	GreyButtons();
	RefreshList();
	RefreshButtons();
}


/*!
 * Should we show tooltips?
 */

bool GamessQFrame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap GamessQFrame::GetBitmapResource( const wxString& name )
{
	return GetGamessQBitmap(name);
}

/*!
 * Get icon resources
 */

wxIcon GamessQFrame::GetIconResource( const wxString& name )
{
    return wxNullIcon;
}

/*!
 * Looks at the statuses of all the selected jobs and puts them into an array.
 * The array has an entry for each possible status. The array entry contains a
 * zero if there are no jobs with that status, and a positive number if there
 * is at least one selected job with that status.
 */
int *GamessQFrame::GetSelectedStatus() {
	int *status = new int[8];
	for (int i = 0; i < 8; i ++) {
		status[i] = 0;
	}
	int index = (int) jobListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED);
	while (index != -1) {
		status[mJobList.Item(index)->GetData()->GetStatus()] ++;
		index = (int) jobListCtrl->GetNextItem(index, wxLIST_NEXT_ALL,
				wxLIST_STATE_SELECTED);
	}
	return status;
}

/*!
 * Determines if it makes sense to remove the selected jobs. Since any job can
 * be removed, this returns true if at least one job is selected.
 *
 * \return True if it makes sense to remove the selected jobs, false otherwise.
 *
 * \sa CanCancel(int *)
 */
bool GamessQFrame::CanRemove()
{
	return jobListCtrl->GetSelectedItemCount() > 0;
}

/*!
 * Determines if it makes sense to cancel the given job. Jobs that are
 * done, canceled or ended with an error can't be canceled, so this function
 * returns true if at least one of the given job is pending paused or
 * running.
 *
 * \param job The job in question
 *
 * \return True if it makes sense to cancel given job.
 */
bool GamessQFrame::CanCancel(Job *job)
{
	return job->GetStatus() == Job::STATUS_PENDING ||
			job->GetStatus() == Job::STATUS_RUNNING ||
			job->GetStatus() == Job::STATUS_PAUSED;
}

/*!
 * Determines if it makes sense to cancel the selected jobs. Jobs that are
 * done, canceled or ended with an error can't be canceled, so this function
 * only returns true if at least one of the selected jobs is pending paused or
 * running.
 *
 * \par Notes concerning job actions
 * When it comes to what actions can be performed on a given selection of jobs,
 * there are two ways to look at it. Either the user should only be allowed to
 * perform actions that can be don on all the jobs, or the user should be
 * allowed to perform any action that can be performed on any one of the jobs.
 * I chose the latter of the two options, so if it can be done to any of the
 * selected jobs we allow it for the selection. This means that upon execution
 * of the command, we must only run the command on the applicable jobs.
 *
 * \param status Status array returned by GetSelectedStatus()
 *
 * \return True if it makes sense to cancel the selected jobs, false otherwise.
 */
bool GamessQFrame::CanCancel(int *status)
{
	return status[Job::STATUS_PENDING] || status[Job::STATUS_RUNNING] ||
			status[Job::STATUS_PAUSED];
}

/*!
 * Determines if it makes sense to pause the given job. Pending or running jobs
 * are the only ones that can be paused, so this function returns true if the
 * given job is pending or running.
 *
 * \param job The job in question
 *
 * \return True if it makes sense to pause given job.
 */
bool GamessQFrame::CanPause(Job *job)
{
	return job->GetStatus() == Job::STATUS_RUNNING ||
			job->GetStatus() == Job::STATUS_PENDING;
}

/*!
 * Determines if it makes sense to pause the selected jobs. Pending or running
 * jobs are the only ones that can be paused, so this function only returns
 * true if at least one of the selected jobs is pending or running.
 *
 * \param status Status array returned by GetSelectedStatus()
 *
 * \return True if it makes sense to pause the selected jobs, false otherwise.
 *
 * \sa CanCancel(int *)
 */
bool GamessQFrame::CanPause(int *status)
{
	return status[Job::STATUS_PENDING] || status[Job::STATUS_RUNNING];
}

/*!
 * Determines if it makes sense to resume the given job. Paused jobs are the
 * only ones that can be resumed, so this function returns true if the given
 * job is paused.
 *
 * \param job The job in question
 *
 * \return True if it makes sense to resume given job.
 */
bool GamessQFrame::CanResume(Job *job)
{
	return job->GetStatus() == Job::STATUS_PAUSED;
}

/*!
 * Determines if it makes sense to resume the selected jobs. Paused jobs are
 * the only ones that can be resumed, so this function only returns true if at
 * least one of the selected jobs is paused.
 *
 * \param status Status array returned by GetSelectedStatus()
 *
 * \return True if it makes sense to resume the selected jobs, false otherwise.
 *
 * \sa CanCancel(int *)
 */
bool GamessQFrame::CanResume(int *status)
{
	return status[Job::STATUS_PAUSED];
}

/*!
 * Determines if it makes sense to view the logs of the given job. Non-pending
 * jobs are the only ones that potentially have log files, so this function
 * only returns true if the given job isn't pending.
 *
 * \param job The job in question
 *
 * \return True if it makes sense to view the logs of the given job.
 */
bool GamessQFrame::CanViewLogs(Job *job)
{
	int status = job->GetStatus();
	return status == Job::STATUS_DONE || status == Job::STATUS_CANCELED ||
			status == Job::STATUS_ERROR || status == Job::STATUS_RUNNING ||
			status == Job::STATUS_PAUSED;
}

/*!
 * Determines if it makes sense to view the logs of the selected jobs.
 * Non-pending jobs are the only ones that potentially have log files, so this
 * only returns true if at least one of the selected jobs is not pending.
 *
 * \param status Status array returned by GetSelectedStatus()
 *
 * \return True if it makes sense to view the logs of the selected jobs, false
 * otherwise.
 *
 * \sa CanCancel(int *)
 */
bool GamessQFrame::CanViewLogs(int *status)
{
	return status[Job::STATUS_DONE] || status[Job::STATUS_CANCELED] ||
			status[Job::STATUS_ERROR] || status[Job::STATUS_RUNNING] ||
			status[Job::STATUS_PAUSED];
}

/*!
 * Determines if it makes sense to save the given job to a folder. Saving only
 * makes sense for done jobs, so it returns true if the job is done.
 *
 * \param job The job in question
 *
 * \return True if it makes sense to save the given job to a folder.
 */
bool GamessQFrame::CanSaveFolder(Job *job)
{
	return job->GetStatus() == Job::STATUS_DONE;
}

/*!
 * Determines if it makes sense to save the selected jobs to a file. Saving
 * only makes sense for done jobs, so it returns true if at least one of the
 * jobs is done.
 *
 * \param status Status array returned by GetSelectedStatus()
 *
 * \return True if it makes sense to save the selected jobs to a folder, false
 * otherwise.
 *
 * \sa CanCancel(int *)
 */
bool GamessQFrame::CanSaveFolder(int *status)
{
	return status[Job::STATUS_DONE];
}

/*!
 * Determines if it makes sense to save the selected jobs to a file. Since you
 * can only save one job to a given file at a time, this only make sense if
 * there is one job selected and that job is done.
 *
 * \param status Status array returned by GetSelectedStatus()
 *
 * \return True if it makes sense to save the selected jobs to a file, false
 * otherwise.
 */
bool GamessQFrame::CanSaveAs(int *status)
{
	return status[Job::STATUS_DONE] == 1 &&
			jobListCtrl->GetSelectedItemCount() == 1;
}

/*!
 * Enables the buttons that are valid based on the current selection in the
 * list control. For example, if only done jobs are selected, it would leave
 * the "Pause" button greyed out, since pausing makes no sense for done jobs.
 */
void GamessQFrame::RefreshButtons()
{
	int *status = GetSelectedStatus();
	// everything should be grey if we can't connect
	mConnected = mQueueManager.IsConnected();

	// you can always add, refresh and clear
	jobsToolbar->EnableTool(wxID_ADD, mConnected);
	jobsMenu->Enable(wxID_ADD, mConnected);
	jobsToolbar->EnableTool(ID_REFRESH, mConnected);
	jobsMenu->Enable(ID_REFRESH, mConnected);
	jobsToolbar->EnableTool(wxID_CLEAR, mConnected);
	jobsMenu->Enable(wxID_CLEAR, mConnected);

	jobsToolbar->EnableTool(wxID_REMOVE, mConnected && CanRemove());
	jobsMenu->Enable(wxID_REMOVE, mConnected && CanRemove());

	jobsToolbar->EnableTool(ID_PAUSE, mConnected && mActive &&
			CanPause(status));
	jobsMenu->Enable(ID_PAUSE, mConnected && mActive && CanPause(status));

	jobsToolbar->EnableTool(ID_RESUME, mConnected && mActive &&
			CanResume(status));
	jobsMenu->Enable(ID_RESUME, mConnected && mActive && CanResume(status));

	jobsToolbar->EnableTool(wxID_CANCEL, mConnected && mActive &&
			CanCancel(status));
	jobsMenu->Enable(wxID_CANCEL, mConnected && mActive && CanCancel(status));

	jobsMenu->Enable(ID_VIEWLOGS, mConnected && CanViewLogs(status));

	jobsMenu->Enable(ID_SAVEFOLDER, mConnected && CanSaveFolder(status));

	jobsMenu->Enable(ID_SAVEAS, mConnected && CanSaveAs(status));

	jobsMenu->Enable(ID_MACMOLPLT, mConnected && CanSaveFolder(status));

	delete status;
}

/*!
 * Disables all the buttons to keep the user from messing anything up in the
 * middle of an operation.
 */
void GamessQFrame::GreyButtons()
{
	mConnected = false;
	// stop the timer as well
	jobsToolbar->EnableTool(wxID_ADD, false);
	jobsMenu->Enable(wxID_ADD, false);
	jobsToolbar->EnableTool(wxID_REMOVE, false);
	jobsMenu->Enable(wxID_REMOVE, false);
	jobsToolbar->EnableTool(wxID_CANCEL, false);
	jobsMenu->Enable(wxID_CANCEL, false);
	jobsToolbar->EnableTool(ID_PAUSE, false);
	jobsMenu->Enable(ID_PAUSE, false);
	jobsToolbar->EnableTool(ID_RESUME, false);
	jobsMenu->Enable(ID_RESUME, false);
	jobsToolbar->EnableTool(ID_REFRESH, false);
	jobsMenu->Enable(ID_REFRESH, false);
	jobsToolbar->EnableTool(wxID_CLEAR, false);
	jobsMenu->Enable(wxID_CLEAR, false);
	jobsMenu->Enable(ID_SAVEFOLDER, false);
	jobsMenu->Enable(ID_SAVEAS, false);
	jobsMenu->Enable(ID_MACMOLPLT, false);
}

/*!
 * This function refreshes the job list. It queries the server for the required
 * information and then updates the list control.
 *
 * \par Notes on the implementation of RefreshList()
 * For efficiency reasons, after the initial refresh, all refreshes are
 * incremental. Because IPC is the slowest part of the whole program the client
 * only gathers the only the information that is subject to have changed from
 * the server. The list control is also left alone during the process so that
 * it is greyed out for as short a period as possible.
 *
 * \par
 * The list control is also updated incrementally, yet for a completely
 * different reason. Whenever the list control is cleared entirely, it not only
 * takes time to reload the entire list, but it resets the current position of
 * the list in the scroller, so every time the client refreshes the user is
 * looking at the top (and most likely irrelevant) portion of the queue again.
 * 
 * \par
 * Due to the incremental nature of this code, it is rather complicated and
 * should be left alone, unless there is a good reason to change it.
 */
void GamessQFrame::RefreshList()
{
	int *ids = mQueueManager.GetJobIds();
	int num = ids[0];

	wxString name;
	int procs;
	Job::Status status;
	Job *job;

	// update any jobs that are out of date, and remove any jobs that no longer
	// exist on the server
    JobList::compatibility_iterator node = mJobList.GetFirst();
	int i = 1;
	while (node) {
		job = node->GetData();
		if (i <= num && job->GetId() == ids[i]) {
			// update the status
			status = job->GetStatus();
			if (status != Job::STATUS_DONE && status != Job::STATUS_ERROR
					&& status != Job::STATUS_CANCELED) {
				job->SetStatus(mQueueManager.GetStatus(ids[i]));
			}
			node = node->GetNext();
			i++;
		} else {
			JobList::compatibility_iterator nextNode = node->GetNext();
			mJobList.DeleteNode(node);
			node = nextNode;
		}
	}

	// add any new jobs for which we do not yet have local copies
	for (; i <= num; i++) {
		name = mQueueManager.GetName(ids[i]);
		procs = mQueueManager.GetNumProcessors(ids[i]);
		status = mQueueManager.GetStatus(ids[i]);
		job = new Job(ids[i], name, wxT(""), status, procs);
		mJobList.Append(job);
	}

	// update the interface
	jobListCtrl->Hide();

	// update the jobs that are in the control and delete any jobs that no
	// longer exist
	node = mJobList.GetFirst();
	i = 0;
	while (i < jobListCtrl->GetItemCount()) {
		if (node && jobListCtrl->GetItemData(i) == node->GetData()->GetId()) {
			jobListCtrl->SetItem(i, 0, node->GetData()->GetName());
			jobListCtrl->SetItem(i, 2, node->GetData()->GetStatusString());
			node = node->GetNext();
			i++;
		} else {
			jobListCtrl->DeleteItem(i);
		}
	}

	// add new entries to the control for the new jobs
	for (;node; i ++, node = node->GetNext()) {
		Job *job = node->GetData();
		int index = (int) jobListCtrl->InsertItem(i, job->GetName());
		jobListCtrl->SetItemData(index, job->GetId());
		wxString procs;
		procs << job->GetNumProcessors();
		jobListCtrl->SetItem(index, 1, procs);
		jobListCtrl->SetItem(index, 2, job->GetStatusString());
	}
	jobListCtrl->Show();

	delete ids;
	mRefreshTimer->Start(mRefreshFrequency, wxTIMER_ONE_SHOT);
}

/*!
 * This function polls the user for a bunch of input files to add. It then asks
 * the user for a few job options and then adds the files to the queue.
 */
void GamessQFrame::NewJob()
{
	wxFileDialog *fileChooser = new wxFileDialog(this, wxT("Choose a file"),
			wxT(""), wxT(""), wxT("GAMESS input file (*.inp)|*.inp"),
#if wxCHECK_VERSION(2,9,0)
            wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE
#else
            wxOPEN | wxFILE_MUST_EXIST | wxMULTIPLE
#endif
                                                 );
	if (fileChooser->ShowModal() == wxID_OK) {
		if (mJobOptionsDialog->ShowModal() == wxID_OK) {
			int procs = mJobOptionsDialog->GetNumProcessors();
			wxArrayString paths;
			fileChooser->GetPaths(paths);
			int count = (int) paths.GetCount();
			for(int i = 0; i < count; i ++) {
				mQueueManager.Add(paths.Item(i), procs);
			}
		}
	}
	delete fileChooser;
}

/*!
 * This function is called every time the user selects an item in the queue. It
 * simply calls RefreshButtons()
 */

void GamessQFrame::OnJoblistctrlSelected( wxListEvent& event )
{
	RefreshButtons();
}

/*!
 * This function is called when the user deselects an item in the list. It
 * calls RefreshButtons()
 */

void GamessQFrame::OnJoblistctrlDeselected( wxListEvent& event )
{
	RefreshButtons();
}


/*!
 * This function is called when the user clicks "Remove". It removes the
 * selected jobs.
 *
 * \sa Job::Remove()
 */

void GamessQFrame::OnRemoveClick( wxCommandEvent& event )
{
	GreyButtons();
	int index = (int) jobListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED);
	while (index != -1) {
		Job *job = mJobList.Item(index)->GetData();
		mQueueManager.Remove(job->GetId());
		index = (int) jobListCtrl->GetNextItem(index, wxLIST_NEXT_ALL,
				wxLIST_STATE_SELECTED);
	}
	RefreshList();
	RefreshButtons();
}


/*!
 * This function is called when the user clicks "Clear". It deletes all the
 * done, canceled or error jobs.
 */
void GamessQFrame::OnClearClick( wxCommandEvent& event )
{
	GreyButtons();
	mQueueManager.Clear();
	RefreshList();
	RefreshButtons();
}


/*!
 * This function is called when the user right-clicks the queue frame. It makes
 * the menu pop up.
 */
void GamessQFrame::OnJoblistctrlRightClick( wxListEvent& event )
{
	PopupMenu(jobsMenu);
}


/*!
 * This function is called when someone clicks "View Logs". It creates and
 * shows a new LogViewer with the log files of all the jobs that have valid
 * logs.
 */
void GamessQFrame::OnViewlogsClick( wxCommandEvent& event )
{
	wxArrayString filenames;
	int index = (int) jobListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED);
	if (index == -1) {
		return;
	}
	while (index != -1) {
		Job *job = mJobList.Item(index)->GetData();
		if (CanViewLogs(job)) {
			wxFileName name = (mQueueManager.GetFileName(job->GetId()));
			name.SetExt(wxT("log"));
			filenames.Add(name.GetFullPath());
		}
		index = (int) jobListCtrl->GetNextItem(index, wxLIST_NEXT_ALL,
				wxLIST_STATE_SELECTED);
	}
	LogViewer *viewer = new LogViewer(this, filenames);
	viewer->Show();
}


/*!
 * This function is called when someone tries to edit a field in the queue. It
 * veto's the event unless they are trying to edit a job's name.
 */
void GamessQFrame::OnJoblistctrlBeginLabelEdit( wxListEvent& event )
{
	if (event.GetColumn() != 0) {
		event.Veto();
	} else {
		GreyButtons();
	}
}


/*!
 * This function is called when the user stops editing the name of a job. It
 * changes the name in the local queue list and tells the server to make the
 * change as well.
 */

void GamessQFrame::OnJoblistctrlEndLabelEdit( wxListEvent& event )
{
	Job *job = mJobList.Item(event.GetIndex())->GetData();
	if (mQueueManager.SetName(job->GetId(), event.GetLabel())) {
		job->SetName(event.GetLabel());
	}
	RefreshButtons();
}

/*!
 * This function queries the server for the filename of the job, so that it
 * knows what all file types will be saved. It then checks to see what files
 * will be overwritten by the proposed copy and appends those file names to the
 * array of files to get overwritten.
 *
 * \param[out]	overwrites	Files being overwritten
 * \param[in]	jobId		The unique ID of the job in question
 * \param[in]	savename	The base name to which the files will be saved
 */
void GamessQFrame::FindOverwrites(wxArrayString &overwrites, int jobId,
		wxString savename)
{
	wxFileName final(savename);
	wxFileName source(mQueueManager.GetFileName(jobId));

	final.SetExt(wxT("log"));
	source.SetExt(wxT("log"));
	if (wxFileExists(final.GetFullPath()) &&
			wxFileExists(source.GetFullPath())) {
		overwrites.Add(final.GetFullPath());
	}

	final.SetExt(wxT("dat"));
	source.SetExt(wxT("dat"));
	if (wxFileExists(final.GetFullPath()) &&
			wxFileExists(source.GetFullPath())) {
		overwrites.Add(final.GetFullPath());
	}

	final.SetExt(wxT("irc"));	//Pre 2009 versions of GAMESS
	source.SetExt(wxT("irc"));
	if (wxFileExists(final.GetFullPath()) &&
			wxFileExists(source.GetFullPath())) {
		overwrites.Add(final.GetFullPath());
	}

	final.SetExt(wxT("trj"));	//2009 and later versions of GAMESS
	source.SetExt(wxT("trj"));
	if (wxFileExists(final.GetFullPath()) &&
		wxFileExists(source.GetFullPath())) {
		overwrites.Add(final.GetFullPath());
	}

	final.SetExt(wxT("efp"));
	source.SetExt(wxT("efp"));
	if (wxFileExists(final.GetFullPath()) &&
		wxFileExists(source.GetFullPath())) {
		overwrites.Add(final.GetFullPath());
	}

	final.SetExt(wxT("gamma"));
	source.SetExt(wxT("gamma"));
	if (wxFileExists(final.GetFullPath()) &&
		wxFileExists(source.GetFullPath())) {
		overwrites.Add(final.GetFullPath());
	}
	
	final.SetExt(wxT("rst"));
	source.SetExt(wxT("rst"));
	if (wxFileExists(final.GetFullPath()) &&
		wxFileExists(source.GetFullPath())) {
		overwrites.Add(final.GetFullPath());
	}
}

/*!
 * This function asks the user for a folder, and then saves all the output
 * files for the selected jobs to that folder. The jobs are saved as
 * JobName.Extension.
 */
void GamessQFrame::OnSavefolderClick( wxCommandEvent& event )
{
	wxDirDialog dialog(this, wxT("Select Output Folder"), wxT(""),
			wxDD_NEW_DIR_BUTTON);
	if (dialog.ShowModal() == wxID_OK) {
		int index = (int) jobListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
				wxLIST_STATE_SELECTED);
		if (index == -1) {
			return;
		}
		wxArrayString savenames;
		int num = jobListCtrl->GetSelectedItemCount();
		savenames.Alloc(num);
		int *ids = new int[num];

		for (int i = 0; index != -1 && i < num; i++) {
			Job *job = mJobList.Item(index)->GetData();
			if (CanSaveFolder(job)) {
				ids[i] = job->GetId();
				savenames.Add(wxFileName(dialog.GetPath(),
						job->GetName()).GetFullPath());
			} else {
				i --;
				num --;
			}
			index = (int) jobListCtrl->GetNextItem(index, wxLIST_NEXT_ALL,
					wxLIST_STATE_SELECTED);
		}
		for (int i = 0; i < num; i ++) {
			int k = 1;
			for (int j = i + 1; j < num; j ++) {
				if (savenames.Item(j) == savenames.Item(i)) {
					wxString format;
					format << savenames.Item(i) << wxT("_%03d");
					savenames.Item(j).Printf(format, k);
					k++;
				}
			}
		}
		wxArrayString overwrites;
		for (int i = 0; i < num; i ++) {
			FindOverwrites(overwrites, ids[i], savenames.Item(i));
		}
		if (overwrites.GetCount() > 0) {
			ConfirmOverwrite confirm(this, overwrites);
			if (confirm.ShowModal() != wxID_OK) {
                delete[] ids;
				return;
			}
		}
		for (int i = 0; i < num; i ++) {
			mQueueManager.Save(ids[i], savenames.Item(i));
		}

		delete[] ids;
	}
}

/*!
 * This function checks the files to make sure they are ".inp" files, and then
 * asks the user for job options and adds the ".inp" files to the queue.
 *
 * \param filenames Names of the dropped files
 */
bool GamessQFrame::OnDropFiles(const wxArrayString& filenames)
{
	if (! mConnected) {
		return false;
	}

	// check file types
	bool valid = false;
	for (int i = 0; i < filenames.GetCount(); i ++) {
		wxFileName file(filenames.Item(i));
		if (file.GetExt() == wxT("inp")) {
			valid = true;
		}
	}

	if (! valid) {
		return false;
	}

	GreyButtons();
	// add the .inp files
	if (mJobOptionsDialog->ShowModal() == wxID_OK) {
		int procs = mJobOptionsDialog->GetNumProcessors();
		for(int i = 0; i < filenames.GetCount(); i ++) {
			wxFileName file(filenames.Item(i));
			if (file.GetExt() == wxT("inp")) {
				mQueueManager.Add(filenames.Item(i), procs);
			}
		}
	}
	RefreshList();
	RefreshButtons();
	return true;
}

/*!
 * This Function asks the user for a file name and saves the output files with
 * that file name and the appropriate extensions.
 */

void GamessQFrame::OnSaveasClick( wxCommandEvent& event )
{
	wxFileDialog dialog(this, wxT("Select Output Folder"), wxT(""), wxT(""),
			wxT("GAMESS Output Files (*.dat)|*.dat"),
#if wxCHECK_VERSION(2,9,0)
                    wxFD_SAVE
#else
                    wxSAVE
#endif
                        );
	if (dialog.ShowModal() == wxID_OK) {
		// TODO: I should probably check more stuff here.
		Job *job = mJobList.Item(jobListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
				wxLIST_STATE_SELECTED))->GetData();
		wxArrayString overwrites;
		FindOverwrites(overwrites, job->GetId(), dialog.GetPath());
		if (overwrites.GetCount() > 0) {
			ConfirmOverwrite confirm(this, overwrites);
			if (confirm.ShowModal() != wxID_OK) {
				return;
			}
		}
		mQueueManager.Save(job->GetId(), dialog.GetPath());
	}
}


/*!
 * Launches wxMacMolPlt (assuming it's in the PATH) with the .dat files
 *
 * \par Notes on launching wxMacMolPlt on a Mac
 * In order to launch wxmacmolplt on a mac, we had to run the open command
 * manually because for some reason wxExecute doesn't work quite right.
 */
void GamessQFrame::OnMacmolpltClick( wxCommandEvent& event )
{
	wxArrayString filenames;
	int index = (int) jobListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED);
	fprintf(stdout, "received request for MacMolPlt using index = %d\n", index);
	if (index == -1) {
		return;
	}

#ifdef __WXMAC__
	// open will need to take a fiew more arguments when working with a mac
	filenames.Add(wxT("open"));
	filenames.Add(wxT("-a"));
	filenames.Add(wxT("wxmacmolplt"));
#endif /* __WXMAC__ */

	while (index != -1) {
		Job *job = mJobList.Item(index)->GetData();
		int jobId = job->GetId();
		if (job->GetStatus() == Job::STATUS_DONE) {
			wxFileName name = (mQueueManager.GetFileName(jobId));
			name.SetExt(wxT("log"));
			filenames.Add(name.GetFullPath());
			wxLogMessage(name.GetFullPath());
		}
		index = (int) jobListCtrl->GetNextItem(index, wxLIST_NEXT_ALL,
				wxLIST_STATE_SELECTED);
	}

#ifdef __WXMAC__
	// this fires of open manually, the wx launcher doesn't work for some
	// reason.
	char **args = new char*[filenames.GetCount() + 1];
	args[filenames.GetCount()] = NULL;

	args[0] = "open";
	args[0] = "-a";
	args[0] = "wxmacmolplt";

	for (int i = 0; i < filenames.GetCount(); i ++) {
		wxString arg = filenames.Item(i);
		args[i] = new char[arg.Len() + 1];
		args[i][arg.Len()] = (char)0;
		memcpy(args[i], arg.ToAscii(), arg.Len());
		printf("%s\n", args[i]);
	}

	pid_t pid = fork();
	if (pid == 0) {
		execvp("open", args);
		exit(1);
	}
#else /* __WXMAC__ */

	// we'll just use the standard method for linux and windows
	wxString command = wxT("wxmacmolplt");
	for (int i = 0; i < filenames.GetCount(); i ++) {
		command << wxT(" ") << filenames.Item(i);
	}
	wxLogDebug(command);
	wxExecute(command);
#endif /* __WXMAC__ */
}

/*!
 * Pauses or activates the server (whichever is applicable)
 */

void GamessQFrame::OnPauseallClick( wxCommandEvent& event )
{
	mActive = ! event.IsChecked();
	mQueueManager.SetActive(! event.IsChecked());
	RefreshButtons();
}

/*
 * Stuff for GamessQFrame::DropTarget (yes, it has to be it's own class)
 */

/*!
 * \param parent The parent GamessQFrame object
 */
GamessQFrame::DropTarget::DropTarget(GamessQFrame *parent)
{
	mParent = parent;
}

/*!
 * Calls GamessQFrame::OnDropFiles()
 *
 * \param x			X coordinate of the drop (unused)
 * \param y			Y coordinate of the drop (unused)
 * \param filenames	The names of all the files dropped
 */
bool GamessQFrame::DropTarget::OnDropFiles(wxCoord x, wxCoord y,
		const wxArrayString& filenames)
{
	return mParent->OnDropFiles(filenames);
}


/*!
 * Shows an about dialog with a little useful information.
 */

void GamessQFrame::OnAboutClick( wxCommandEvent& event )
{
	AboutGamessQ about(this);
	about.ShowModal();
}

