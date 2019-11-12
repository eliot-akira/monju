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

Document::Document(const JUCEApplication* app, sol::state* lua)
{
    // GUI

    int width = 420;
    int height = 340;

    editor.reset (new CodeEditorComponent (document, &tokens));

    editor->setColour (CodeEditorComponent::backgroundColourId, Colours::white);
    editor->setColour (CodeEditorComponent::lineNumberTextId, Colours::white);
    editor->setTabSize (2, true);
    editor->setFont (editor->getFont().withHeight (15));
    editor->loadContent (
        "beep()"
        "print('hello world')"
    );

    compileButton.setButtonText ("Compile");

    // Lua

    int x = 0;

    lua->set_function ("beep", [&x] {
        ++x;
    });

    lua->set_function ("print", [this, logList  = &logList](std::string str) {
        logList->addMessage (str);
    });

    // GUI show

    setOpaque (false);

    addAndMakeVisible (logList);
    addAndMakeVisible (editor.get());
    addAndMakeVisible (compileButton);

    updateBounds (width, height);
    setSize (width, height);

    // Bind handlers
    compileButton.onClick = [this, lua, logList = &logList]()
    {
        const auto script = document.getAllContent();
        std::string str = script.toStdString();

        try
        {
            auto result = lua->script(str);
        }
        catch (const std::exception& e)
        {
            logList->addErrorMessage ( e.what() );
        }
    };
}

Document::~Document()
{
    // Unbind handlers
    compileButton.onClick = nullptr;
}

void Document::updateBounds (int fullWidth, int fullHeight)
{
    int halfHeight = fullHeight / 2;
    int w = 0;
    int h = 20;

    editor->setBounds (0, 0, fullWidth, halfHeight);

    w = 60;
    compileButton.setBounds (fullWidth - w, 0, w, h);

    logList.setBounds (0, halfHeight, fullWidth, halfHeight);
}

void Document::paint (Graphics& g)
{
    g.fillAll (Colours::transparentWhite); // getLookAndFeel().findColour (ResizableWindow::backgroundColourId)
}

void Document::resized()
{
    updateBounds(getWidth(), getHeight());
}

}
