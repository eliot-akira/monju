#pragma once

#include "base.h"

namespace monju {

class Window : public DocumentWindow
{
public:

    struct Config {
        bool isMainWindow = false;
    };

    Window (String name, Component* component, Config& config)  :
        DocumentWindow (name,
            Desktop::getInstance()
                .getDefaultLookAndFeel()
                .findColour (
                    ResizableWindow::backgroundColourId
                ),
            DocumentWindow::allButtons
        ),
        config(config)
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
        if (config.isMainWindow)
        {
          JUCEApplication::getInstance()->systemRequestedQuit();
        }
        else
        {
          // Close window
        }
    }

    /* Note: Be careful if you override any DocumentWindow methods - the base
        class uses a lot of them, so by overriding you might break its functionality.
        It's best to do all your work in your content component instead, but if
        you really have to override any DocumentWindow methods, make sure your
        subclass also calls the superclass's method.
    */

private:
    Config& config;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Window)
};

}
