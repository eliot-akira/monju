#pragma once

#include "base.h"

namespace monju {

class LogListBox  : public ListBox,
                    public ListBoxModel,
                    public AsyncUpdater
{
public:
    LogListBox()
    {
        setModel (this);
        Component::setColour (ListBox::ColourIds::backgroundColourId, Colours::black);
        setRowHeight(20); // 22 default
    }
    ~LogListBox() override = default;

    int getNumRows() override
    {
        return logList.size();
    }

    void paintListBoxItem (int row, Graphics& g, int width, int height, bool rowIsSelected) override
    {
        ignoreUnused (rowIsSelected);
        if (! isPositiveAndBelow (row, logList.size())) return;

        // logStatusList[row] != nullptr &&
        Colour textColor = logStatusList[row]=="error" ? Colours::red : Colours::white;

        g.setColour (textColor);

        // ListBox does not support rows of varying heights..

//         auto rect = Rectangle<int>(width, height).reduced (5, height);
//         g.drawMultiLineText(logList[row],
//             rect.getX(), rect.getY(),
//             width,
//             Justification::centredLeft,
//             true
//         );

        g.drawText (logList[row],
            Rectangle<int> (width, height).reduced (5, 3),
            Justification::centredLeft, true
        );
    }

    void setMaxMessages (int newMax)
    {
        if (newMax <= 0 || newMax == maxMessages)
            return;
        maxMessages = newMax;
        triggerAsyncUpdate();
    }


    void addErrorMessage (const String& message)
    {
        addMessage(message, "error");
    }

    void addMessage (const String& message)
    {
        addMessage(message, "ok");
    }

    void addMessage (const String& message, const String& status)
    {
        if (logList.size() > maxMessages)
        {
            logList.remove (0);
            logStatusList.remove (0);
        }

        logList.add (message);
        logStatusList.add (status);

        triggerAsyncUpdate();
    }

    void clear()
    {
        logList.clear();
        logStatusList.clear();
        triggerAsyncUpdate();
    }

    void handleAsyncUpdate() override
    {
        updateContent();
        scrollToEnsureRowIsOnscreen (logList.size() - 1);
        repaint();
    }

private:
    int maxMessages { 100 };
    StringArray logStatusList;
    StringArray logList;
};

}
