#include "App.h"
#include "gui/Document.h"

namespace monju {

App::App() {};
App::~App() {};

void App::initialise (const String& commandLine)
{

    // Lua environment

    lua.open_libraries(); // sol::lib::base, sol::lib::package, sol::lib::table

    // GUI

    Window::Config config { false };

    App* app = this;

    mainWindow.reset (
        new Window (getApplicationName(), new Document(app), config)
    );
}

void App::shutdown()
{
    mainWindow = nullptr;
}

START_JUCE_APPLICATION (App)

}
