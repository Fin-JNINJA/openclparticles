// openclparticles.cpp : Defines the entry point for the application.
//


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 0
#endif

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <CL/cl.hpp>

//#include "../static/teapot.xpm"

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

BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
// some useful events
/*
EVT_MOTION(wxImagePanel::mouseMoved)
EVT_LEFT_DOWN(wxImagePanel::mouseDown)
EVT_LEFT_UP(wxImagePanel::mouseReleased)
EVT_RIGHT_DOWN(wxImagePanel::rightClick)
EVT_LEAVE_WINDOW(wxImagePanel::mouseLeftWindow)
EVT_KEY_DOWN(wxImagePanel::keyPressed)
EVT_KEY_UP(wxImagePanel::keyReleased)
EVT_MOUSEWHEEL(wxImagePanel::mouseWheelMoved)
*/

// catch paint events
EVT_PAINT(wxImagePanel::paintEvent)

END_EVENT_TABLE()

wxImagePanel::wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format) :
    wxPanel(parent)
{
    // load the file... ideally add a check to see if loading was successful
    image.LoadFile(file, format);
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

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

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnPaint(wxPaintEvent& event);
};

enum
{
    ID_Hello = 1
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    wxImagePanel* drawPane;

    wxInitAllImageHandlers();

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    //frame = new wxFrame(NULL, wxID_ANY, wxT("Hello wxDC"), wxPoint(50, 50), wxSize(800, 600));

    // then simply create like this
    drawPane = new wxImagePanel(frame, wxT("C:\\Users\\Fin\\Documents\\place to clone repositories\\openclparticles\\static\\teapot.xpm"), wxBITMAP_TYPE_XPM);
    sizer->Add(drawPane, 1, wxEXPAND);

    frame->SetSizer(sizer);

    frame->Show(true);

    //OPENCL SETUP 

    cl_uint numPlatforms;
    clGetPlatformIDs(0, nullptr, &numPlatforms);

    // Step 2: Get the first available platform
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, nullptr);

    // Step 3: Get number of GPU devices available on the platform
    cl_uint numDevices;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &numDevices);

    // Step 4: Get the first GPU device ID
    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);

    // Step 5: Define buffers to store device info
    char name[128], vendor[128];
    cl_ulong mem;
    cl_uint units;

    // Step 6: Retrieve device name, vendor, memory size, and compute units
    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(name), name, nullptr);
    clGetDeviceInfo(device, CL_DEVICE_VENDOR, sizeof(vendor), vendor, nullptr);
    clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(mem), &mem, nullptr);
    clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(units), &units, nullptr);

    // Step 7: Print the collected device information
    std::cout << "Device Name: " << name << "\n";
    std::cout << "Vendor: " << vendor << "\n";
    std::cout << "Memory: " << mem / (1024 * 1024) << " MB\n";
    std::cout << "Compute Units: " << units << "\n";

    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
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

