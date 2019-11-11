#include "base.h"
#include "gui/Document.h"

namespace Monju {

class App : public JUCEApplication
{
public:
    App() {};
    ~App() {};

    const String getApplicationName() override { return ProjectInfo::projectName; }
    const String getApplicationVersion() override { return ProjectInfo::versionString; }

    void initialise (const String& /*commandLine*/) override
    {

        sol::state lua;
        int x = 0;
        lua.open_libraries(sol::lib::base, sol::lib::package);
        lua.set_function("beep", [&x]{ ++x; });
        lua.script(
            "beep()"
            "beep()"
            "print('hello world')"
            "print(x)"
        );
        assert(x == 2);

        mainWindow.reset (
            new MainWindow (getApplicationName(), new Document())
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

    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name, Component* component)  :
            DocumentWindow (name,
                Desktop::getInstance()
                    .getDefaultLookAndFeel()
                    .findColour (ResizableWindow::backgroundColourId),
                DocumentWindow::allButtons
            )
        {
            setUsingNativeTitleBar (true);

            setContentOwned (component, true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (App)

}
