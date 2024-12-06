/*
  ==============================================================================

    layout.h
    Created: 6 Dec 2024 11:17:36am
    Author:  mert.kabukcuoglu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum class pageType
{
	BASIC_EDIT = 0,
	FULL_EDIT,
    PRESET,
	amountofPages
};

enum class effect
{
	VIBRATO = 0,
	FLANGER,
	amountOfEffects
};

//std::array<const char*, static_cast<int>(effect::amountOfEffects)> effectNames = { "Vibrato", "Flanger" };