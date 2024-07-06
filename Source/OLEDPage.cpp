/*
  ==============================================================================

    OLEDPage.cpp
    Created: 6 Jul 2024 3:41:44pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/


#include "OLEDPage.h"

namespace juce
{
    OLEDPage::OLEDPage() {}

    void OLEDPage::setFont(const Font& font)
    {
        for (auto* label : labels)
        {
            label->setFont(font);
        }
    }

    void OLEDPage::addLabel(const String& text, const Rectangle<int>& bounds)
    {
        auto* label = new Label();
        label->setText(text,dontSendNotification);
        label->setBounds(bounds);
        labels.add(label);
    }

    void OLEDPage::clearLabels()
    {
        labels.clear();
    }

    void OLEDPage::addToComponent(Component& parent)
    {
        for (auto* label : labels)
        {
            parent.addAndMakeVisible(label);
        }
    }
}
