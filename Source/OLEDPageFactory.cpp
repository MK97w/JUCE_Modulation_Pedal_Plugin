/*
  ==============================================================================

    OLEDLookAndFeel.cpp
    Created: 6 Jul 2024 12:46:48am
    Author:  MK

  ==============================================================================
*/

#include "OLEDPageFactory.h"

std::unique_ptr<OLEDPage> PageFactory::create(pageType type, const juce::String& title, apvtsInfo& info)
{
	if (type == pageType::BASIC_EDIT)
	{
		return std::make_unique<BasicEditPage>(title, info);
	}
	else if (type == pageType::FULL_EDIT)
	{
		return std::make_unique<EditPage>(title,info);
	}
	else
	{
		return nullptr;
	}
}
