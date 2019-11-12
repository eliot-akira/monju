#include "base.h"
#include "LogListBox.h"

namespace monju {

class Document   : public Component
{
public:
    Document(const JUCEApplication* app, sol::state* lua);
    ~Document();

    void paint (Graphics& g) override;
    void resized() override;

    void updateBounds(int fullWidth, int fullHeight);

private:
    CodeDocument document;
    std::unique_ptr<CodeEditorComponent> editor;
    TextButton compileButton;
    LuaTokeniser tokens;

    LogListBox logList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Document)
};

}
