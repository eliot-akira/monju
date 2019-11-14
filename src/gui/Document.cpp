#include "Document.h"

namespace monju {

static CodeEditorComponent::ColourScheme luaColors()
{
    static const CodeEditorComponent::ColourScheme::TokenType types[] =
    {
        { "Error",          Colour (0xffcc0000) },
        { "Comment",        Colour (0xff3c3c3c) },
        { "Keyword",        Colour (0xff0000cc) },
        { "Operator",       Colour (0xff225500) },
        { "Identifier",     Colour (0xff000000) },
        { "Integer",        Colour (0xff880000) },
        { "Float",          Colour (0xff885500) },
        { "String",         Colour (0xff990099) },
        { "Bracket",        Colour (0xff000055) },
        { "Punctuation",    Colour (0xff004400) }
    };

    CodeEditorComponent::ColourScheme cs;

    for (auto& t : types)
        cs.set (t.name, Colour (t.colour));

    return cs;
}

Document::Document(monju::App* app) :
    app(app),
    lua(app->lua),
    originalPackagePath(app->lua["package"]["path"])
{
    // GUI

    int width = 900;
    int height = 580;

    editor.reset (new CodeEditorComponent (document, &tokens));
    editor->setColour (CodeEditorComponent::backgroundColourId, Colours::white);
    editor->setColour (CodeEditorComponent::lineNumberTextId, Colours::white);
    editor->setTabSize (2, true);
    editor->setFont (editor->getFont().withHeight (15));
    editor->loadContent ("print(os.date())");

    selectFileButton.setButtonText ("File");
    runButton.setButtonText ("Run");
    reloadButton.setButtonText ("Reload");
    clearButton.setButtonText ("Clear");

    fileChooser.reset (new FileChooser ("Select an Lua file...", File(), "*.lua"));

    // GUI: Show

    updateBounds (width, height);

    setOpaque (false);
    addAndMakeVisible (editor.get());
    addAndMakeVisible (selectFileButton);
    addAndMakeVisible (runButton);
    addAndMakeVisible (reloadButton);
    addAndMakeVisible (clearButton);
    addAndMakeVisible (logList);

    setSize (width, height);

    // Lua

    lua.set_function ("print", [this](std::string str) {
        logList.addMessage (str);
    });

    auto m = lua.create_table();

    m["x"] = 1;

    m["ping"] = [this, m]() {
        int x = m["x"];
        logList.addMessage ( "x=" + String(x));
        return "pong";
    };

    lua["m"] = m;

    onRun();

    // Bind handlers

    selectFileButton.onClick = std::bind (&Document::onSelectFile, this);
    reloadButton.onClick = std::bind (&Document::onReload, this);
    runButton.onClick = std::bind (&Document::onRun, this);
    clearButton.onClick = [this]()
    {
        logList.clear();
    };
}

Document::~Document()
{
    // Unbind handlers
    selectFileButton.onClick = nullptr;
    reloadButton.onClick = nullptr;
    runButton.onClick = nullptr;
    clearButton.onClick = nullptr;
    editor = nullptr;
}

void Document::updateBounds (int fullWidth, int fullHeight)
{
    int halfWidth = fullWidth / 2;
    int halfHeight = fullHeight / 2;
    int margin = 5;
    int w = 0;
    int h = 22;
    int x = 0;
    int y = margin;

    editor->setBounds (x, 0, halfWidth, fullHeight);

    // Action buttons, from right

    w = 60;
    x = fullWidth - w - margin;
    reloadButton.setBounds (x, y, w, h);

    x -= w + margin;
    clearButton.setBounds (x, y, w, h);

    x -= w + margin;
    runButton.setBounds (x, y, w, h);

    // Fill the rest
    w = x - halfWidth - (margin * 2);
    x = halfWidth + margin;
    selectFileButton.setBounds (x, y, w, h);

    // Console
    h += margin * 2;
    logList.setBounds (halfWidth, h, halfWidth, fullHeight - h);
}

void Document::paint (Graphics& g)
{
    g.fillAll(
        getLookAndFeel().findColour (ResizableWindow::backgroundColourId)
    );
        //(Colours::black);
}

void Document::resized()
{
    updateBounds(getWidth(), getHeight());
}

void Document::onSelectFile ()
{
    bool ok = fileChooser->browseForFileToOpen();
    if (!ok) return;
    currentFile = fileChooser->getResult();
    readFile (currentFile, true);

    selectFileButton.setButtonText(currentFile.getRelativePathFrom(app->homePath));

    updatePackagePath();
}

void Document::updatePackagePath ()
{
    if (! currentFile.existsAsFile()) return;

    // Update package path to include current file's containing folder

    std::string folder = currentFile.getParentDirectory().getFullPathName().toStdString();
    lua["package"]["path"] = folder + "/?.lua;" + folder + "/?/init.lua;" + originalPackagePath;

    std::string currentPackagePath = lua["package"]["path"];
}

void Document::readFile (const File& fileToRead)
{
    readFile (fileToRead, false);
}

void Document::readFile (const File& fileToRead, bool doRun)
{
    if (! fileToRead.existsAsFile()) return;

    auto script = fileToRead.loadFileAsString().toStdString();
    editor->loadContent (script);
    if (doRun)
    {
        onRunWithContent (script);
    }
}

void Document::onRun ()
{
    onRunWithContent (document.getAllContent().toStdString());
}

// Separate function to allow std::bind to onRun with no argument
void Document::onRunWithContent (std::string content)
{
    auto result = lua.safe_script(
        content,
        sol::script_pass_on_error
    );

    if (!result.valid()) {
        sol::error err = result;
        logList.addErrorMessage ( err.what() );
    }
}

void Document::onReload ()
{
    if (currentFile.existsAsFile())
        readFile (currentFile, true);
    else
        onRun();
}

}
