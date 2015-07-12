/////////////////////////////////////////////////////////////////////////////
// Name:        about_gamessq.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     Thu 03 Jul 2008 01:44:43 PM CDT
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _ABOUT_GAMESSQ_H_
#define _ABOUT_GAMESSQ_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "about_gamessq.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/hyperlink.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_ABOUTGAMESSQ 10024
#define ID_HYPERLINKCTRL 10000
#define ID_HYPERLINKCTRL1 10001
#define SYMBOL_ABOUTGAMESSQ_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_ABOUTGAMESSQ_TITLE _("About GamessQ")
#define SYMBOL_ABOUTGAMESSQ_IDNAME ID_ABOUTGAMESSQ
#define SYMBOL_ABOUTGAMESSQ_SIZE wxSize(480, 300)
#define SYMBOL_ABOUTGAMESSQ_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * AboutGamessQ class declaration
 */

class AboutGamessQ: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( AboutGamessQ )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    AboutGamessQ();
    AboutGamessQ( wxWindow* parent, wxWindowID id = SYMBOL_ABOUTGAMESSQ_IDNAME, const wxString& caption = SYMBOL_ABOUTGAMESSQ_TITLE, const wxPoint& pos = SYMBOL_ABOUTGAMESSQ_POSITION, const wxSize& size = SYMBOL_ABOUTGAMESSQ_SIZE, long style = SYMBOL_ABOUTGAMESSQ_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ABOUTGAMESSQ_IDNAME, const wxString& caption = SYMBOL_ABOUTGAMESSQ_TITLE, const wxPoint& pos = SYMBOL_ABOUTGAMESSQ_POSITION, const wxSize& size = SYMBOL_ABOUTGAMESSQ_SIZE, long style = SYMBOL_ABOUTGAMESSQ_STYLE );

    /// Destructor
    ~AboutGamessQ();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin AboutGamessQ event handler declarations

////@end AboutGamessQ event handler declarations

////@begin AboutGamessQ member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end AboutGamessQ member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin AboutGamessQ member variables
////@end AboutGamessQ member variables
};

#endif
    // _ABOUT_GAMESSQ_H_
