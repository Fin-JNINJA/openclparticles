// openclparticles.cpp : Defines the entry point for the application.
//

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 0
#endif

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/sizer.h>
#include <wx/wx.h>
#include <wx/timer.h>
#endif

#include <CL/cl.hpp>

enum
{
    ID_Hello = 1
};

using namespace std;

class wxImagePanel : public wxPanel
{
    wxBitmap image;

public:
    wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format);

    void paintEvent(wxPaintEvent& evt);
    void paintNow();

    void render(wxDC& dc);

    /*
         void mouseMoved(wxMouseEvent& event);
         void mouseDown(wxMouseEvent& event);
         void mouseWheelMoved(wxMouseEvent& event);
         void mouseReleased(wxMouseEvent& event);
         void rightClick(wxMouseEvent& event);
         void mouseLeftWindow(wxMouseEvent& event);
         void keyPressed(wxKeyEvent& event);
         void keyReleased(wxKeyEvent& event);
         */

    DECLARE_EVENT_TABLE()
};


wxImagePanel::wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format) :
    wxPanel(parent)
{
    // load the file... ideally add a check to see if loading was successful
    image.LoadFile(file, format);
}

void wxImagePanel::paintEvent(wxPaintEvent& evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
    //ADD DOUBLE BUFFER
}

void wxImagePanel::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

void wxImagePanel::render(wxDC& dc)
{
    dc.DrawBitmap(image, 0, 0, false);
}


BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)

EVT_PAINT(wxImagePanel::paintEvent)

END_EVENT_TABLE()

class RenderTimer : public wxTimer
{
    wxImagePanel* pane;
public:
    RenderTimer(wxImagePanel* pane);
    void Notify();
    void start();
};

RenderTimer::RenderTimer(wxImagePanel* pane) : wxTimer()
{
    RenderTimer::pane = pane;
}

void RenderTimer::Notify()
{
    pane->Refresh();
}

void RenderTimer::start()
{
    wxTimer::Start(16);
}

class MyFrame : public wxFrame
{
    RenderTimer* timer;
    wxImagePanel* drawPane;

public:
    MyFrame();
    ~MyFrame()
    {
        delete timer;
    }
    void onClose(wxCloseEvent& evt)
    {
        timer->Stop();
        evt.Skip();
    }
    DECLARE_EVENT_TABLE()
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnPaint(wxPaintEvent& event);
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_CLOSE(MyFrame::onClose)
END_EVENT_TABLE()

MyFrame::MyFrame() : wxFrame((wxFrame*)NULL, -1, wxT("Hello wxDC"), wxPoint(50, 50), wxSize(1920, 1080))
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    drawPane = new wxImagePanel(this, wxT("C:\\Users\\finla\\Documents\\CodeProjects\\openclparticles\\static\\teapot.xpm"), wxBITMAP_TYPE_XPM);
    sizer->Add(drawPane, 1, wxEXPAND);
    SetSizer(sizer);

    timer = new RenderTimer(drawPane);
    Show();
    timer->start();


    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
        "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();
    MyFrame* frame = new MyFrame();
    //pass wxImagePanel* drawPane;

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    //frame = new wxFrame(NULL, wxID_ANY, wxT("Hello wxDC"), wxPoint(50, 50), wxSize(800, 600));

    //// then simply create like this
    //drawPane = new wxImagePanel(frame, wxT("C:\\Users\\finla\\Documents\\CodeProjects\\openclparticles\\static\\teapot.xpm"), wxBITMAP_TYPE_XPM);
    //sizer->Add(drawPane, 1, wxEXPAND);

    frame->SetSizer(sizer);

    frame->Show(true);

    return true;
}

wxIMPLEMENT_APP(MyApp);

