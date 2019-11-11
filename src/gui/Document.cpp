#include "Document.h"

namespace Monju {

using namespace juce;

//==============================================================================
Document::Document()
{

    // Add child components

    setSize (800, 600);
}

Document::~Document()
{
}

void Document::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void Document::resized()
{
}

}
