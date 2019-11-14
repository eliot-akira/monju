#include "base.h"
#include "App.h"
#include "LogListBox.h"

namespace monju {

class App;

class Document :
    public Component
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
    std::string originalPackagePath;

    std::unique_ptr<CodeEditorComponent> editor;
    CodeDocument document;
    LuaTokeniser tokens;

    std::unique_ptr<FileChooser> fileChooser;
    File currentFile;

    TextButton selectFileButton;
    TextButton reloadButton;
    TextButton runButton;
    TextButton clearButton;

    LogListBox logList;

    void onSelectFile ();
    void updatePackagePath ();

    void readFile (const File& fileToRead);
    void readFile (const File& fileToRead, bool doRun);

    void onRun ();
    // Separate function to allow std::bind to onRun with no argument
    void onRunWithContent (std::string content);

    void onReload ();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Document)
};

}
