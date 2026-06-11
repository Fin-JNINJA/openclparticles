include(FetchContent)


FetchContent_Declare(
    wxwidgets
    GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
    GIT_TAG master
    )

set(wxWidgets_USE_STATIC ON CACHE BOOL "Use static wxWidgets libraries" FORCE)
set(wxWidgets_BUILD_SHARED OFF CACHE BOOL "Disable shared wxWidgets build" FORCE)
set(wxBUILD_SHARED OFF CACHE BOOL "Build shared libraries" FORCE)
FetchContent_MakeAvailable(wxwidgets)
