#include "base.h"
#include "App.h"
#include "LogListBox.h"

namespace monju {

class App;

class Document   : public Component
{
public:
    Document(monju::App* app);
    ~Document();

    void paint (Graphics& g) override;
    void resized() override;

    void updateBounds(int fullWidth, int fullHeight);

private:

    monju::App* app;
    sol::state& lua;

    std::unique_ptr<CodeEditorComponent> editor;
    CodeDocument document;
    LuaTokeniser tokens;

    TextButton compileButton;
    TextButton clearButton;

    LogListBox logList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Document)
};

}
