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
    lua(app->lua)
{
    // GUI

    int width = 900;
    int height = 580;

    editor.reset (new CodeEditorComponent (document, &tokens));



    editor->loadContent (R"(print(os.date())
)"
    );
    editor->setColour (CodeEditorComponent::backgroundColourId, Colours::white);
    editor->setColour (CodeEditorComponent::lineNumberTextId, Colours::white);
    editor->setTabSize (2, true);
    editor->setFont (editor->getFont().withHeight (15));

    compileButton.setButtonText ("Compile");
    clearButton.setButtonText ("Clear");

    // GUI show

    setOpaque (false);

    addAndMakeVisible (editor.get());
    addAndMakeVisible (logList);
    addAndMakeVisible (compileButton);
    addAndMakeVisible (clearButton);

    updateBounds (width, height);

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

    std::function<void()> onCompile = [this]()
    {
        auto result = lua.safe_script(
            document.getAllContent().toStdString(),
            sol::script_pass_on_error
        );

        if (!result.valid()) {
            sol::error err = result;
            logList.addErrorMessage ( err.what() );
        }
    };

    onCompile();

    // Bind handlers

    compileButton.onClick = onCompile;
    clearButton.onClick = [this]()
    {
        logList.clear();
    };
}

Document::~Document()
{
    // Unbind handlers
    compileButton.onClick = nullptr;
}

void Document::updateBounds (int fullWidth, int fullHeight)
{
    int halfWidth = fullWidth / 2;
    int halfHeight = fullHeight / 2;
    int margin = 5;
    int w = 0;
    int h = 20;
    int x = 0;
    int y = margin;

    editor->setBounds (x, 0, halfWidth, fullHeight);

    // From right

    w = 60;
    x = fullWidth - w - margin;
    clearButton.setBounds (x, y, w, h);

    x -= w + margin;
    compileButton.setBounds (x, y, w, h);

    // Console

    logList.setBounds (halfWidth, 0, halfWidth, fullHeight);
}

void Document::paint (Graphics& g)
{
    g.fillAll (Colours::black); // getLookAndFeel().findColour (ResizableWindow::backgroundColourId)
}

void Document::resized()
{
    updateBounds(getWidth(), getHeight());
}

}
