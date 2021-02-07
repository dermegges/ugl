
#include "BasicViewer.hpp"

#include <ugl/wx/Canvas.hpp>

#include <wx/app.h>
#include <wx/defs.h>
#include <wx/frame.h>
#include <wx/gdicmn.h>
#include <wx/window.h>

/**
 * The wxWidgets App needed for BasicViewerCanvas.
 */
class BasicViewerApp : public wxApp
{
private:
    virtual bool OnInit()
    {
        wxFrame* frame = new wxFrame( nullptr, wxID_ANY, "Basic Viewer (wxWidgets)",
                wxDefaultPosition, wxSize( 960, 540 ) );
        frame->SetBackgroundStyle( wxBG_STYLE_PAINT );

        new ugl::wx::Canvas(new BasicViewer(), frame );

        frame->Show( true );
        return true;
    }
};

// -------------------------------------------------------------------------

wxIMPLEMENT_APP( BasicViewerApp );
