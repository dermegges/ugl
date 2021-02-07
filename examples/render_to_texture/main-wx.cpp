
#include "RenderToTexture.hpp"

#include <ugl/wx/Canvas.hpp>

#include <wx/app.h>
#include <wx/defs.h>
#include <wx/frame.h>
#include <wx/gdicmn.h>
#include <wx/window.h>

/**
 * The wxWidgets App needed for Render to texture canvas.
 */
class RenderToTextureApp : public wxApp
{
private:
    virtual bool OnInit()
    {
        wxFrame* frame = new wxFrame( nullptr, wxID_ANY, "Render To Texture (wxWidgets)",
                wxDefaultPosition, wxSize( 960, 540 ) );
        frame->SetBackgroundStyle( wxBG_STYLE_PAINT );

        new ugl::wx::Canvas(new RenderToTexture(), frame );

        frame->Show( true );
        return true;
    }
};

// -------------------------------------------------------------------------

wxIMPLEMENT_APP( RenderToTextureApp );
