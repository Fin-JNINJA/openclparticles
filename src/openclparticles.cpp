// openclparticles.cpp : Defines the entry point for the application.

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 0
#endif

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/sizer.h>
#include <wx/wx.h>
#include <wx/timer.h>
#endif

#include <wx/dcbuffer.h>

#include <wx/generic/numdlgg.h>

#include "../static/Image.xpm"
#include "../static/teapot.xpm"


using namespace std;

wxBitmap cat;


struct particle {
    float size;
    float x, y, vx, vy;

};

vector<particle> particles;

enum
{
    ID_Hello = 1
};



class wxImagePanel : public wxPanel
{
    wxBitmap image;

public:
    wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format);
    wxImagePanel(wxFrame* parent);

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
    image.LoadFile(file, format);
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

wxImagePanel::wxImagePanel(wxFrame* parent) :
    wxPanel(parent)
{

    image = wxBitmap(teapot_xpm);
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void wxImagePanel::paintEvent(wxPaintEvent& evt) //RENDER LOOP IS HERE
{

    //image = cat;
    wxPaintDC pdc(this);
    wxAutoBufferedPaintDC dc(this);
    render(dc);
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
    //const char* test = "test";
    drawPane = new wxImagePanel(this);
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
    srand(static_cast <unsigned> (time(0)));

    //cat.LoadFile("C:\\Users\\finla\\Documents\\CodeProjects\\openclparticles\\static\\Image.xpm", wxBITMAP_TYPE_XPM);
    //tea.LoadFile("C:\\Users\\finla\\Documents\\CodeProjects\\openclparticles\\static\\teapot.xpm", wxBITMAP_TYPE_XPM);
    cat = wxBitmap(Image_xpm);
    

    MyFrame* frame = new MyFrame();

    frame->Show(true);

    wxNumberEntryDialog dialog(frame, "message", "prompt: ", "caption", 50, 0, LONG_MAX);

    if (dialog.ShowModal() == wxID_OK) {
        long value = dialog.GetValue();
        wxMessageBox(wxString::Format("You entered: %ld", value));
        particles.resize(value);
    }

    static int yLO = 900;
    static int yHI = 1080;
    static int xLO = 0;
    static int xHI = 1920;
    for (auto& element : particles) {

        element.x = xLO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (xHI - xLO)));
        element.y = yLO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (yHI - yLO)));
        element.vx = 0;
        element.vy = 0;
    }

    return true;
}

wxIMPLEMENT_APP(MyApp);
