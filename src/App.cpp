#include "App.h"
#include "gui/Document.h"

namespace monju {

App::App() :
    homePath(
        File::getSpecialLocation(File::SpecialLocationType::userHomeDirectory)
    )
{

};
App::~App() {};

void App::initialise (const String& commandLine)
{

    // Lua environment

    lua.open_libraries(); // sol::lib::base, sol::lib::package, sol::lib::table

    // Package require paths
    std::string homePathStr = homePath.getFullPathName().toStdString();
    std::string packagePath = lua["package"]["path"];
    lua["package"]["path"] =  homePathStr + "/?.lua;" + homePathStr + "/?/init.lua" + ";" + packagePath;

    // GUI

    Window::Config config;
    config.isMainWindow = true;

    App* app = this;

    mainWindow.reset (
        new Window (getApplicationName(), new Document(app), config)
    );

    mainWindow->setResizeLimits(860, 480, 9999, 9999);
}

void App::shutdown()
{
	lua.collect_garbage();
    mainWindow = nullptr;
}

START_JUCE_APPLICATION (App)

}
