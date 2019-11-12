#include "base.h"
#include "gui/Window.h"
#include "gui/Document.h"

namespace monju {

class App : public JUCEApplication
{
public:
    App() {};
    ~App() {};

    const String getApplicationName() override { return ProjectInfo::projectName; }
    const String getApplicationVersion() override { return ProjectInfo::versionString; }

    void initialise (const String& commandLine) override
    {

        // Lua environment

        lua.open_libraries(sol::lib::base, sol::lib::package);

        // GUI

        Window::Config config { false };

        mainWindow.reset (
            new Window (getApplicationName(), new Document(this, &lua), config)
        );
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

    bool moreThanOneInstanceAllowed() override { return false; }

    /** Indicates that the user has tried to start up another instance of the app.
        This will get called even if moreThanOneInstanceAllowed() is false.
    */
    void anotherInstanceStarted (const String& commandLine) override {};

    /** Called when the operating system is trying to close the application.

        The default implementation of this method is to call quit(), but it may
        be overloaded to ignore the request or do some other special behaviour
        instead. For example, you might want to offer the user the chance to save
        their changes before quitting, and give them the chance to cancel.

        If you want to send a quit signal to your app, this is the correct method
        to call, because it means that requests that come from the system get handled
        in the same way as those from your own application code. So e.g. you'd
        call this method from a "quit" item on a menu bar.
    */
    void systemRequestedQuit() override
    {
        quit();
    }

    /** This method is called when the application is being put into background mode
        by the operating system.
    */
    void suspended() override {};

    /** This method is called when the application is being woken from background mode
        by the operating system.
    */
    void resumed() override {};

    sol::state lua;

private:
    std::unique_ptr<Window> mainWindow;
};

START_JUCE_APPLICATION (App)

}
