#pragma once

#include "../base.h"

namespace Monju {

class Document   : public Component
{
public:
    Document();
    ~Document();

    void paint (Graphics& g) override;
    void resized() override;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Document)
};

}
