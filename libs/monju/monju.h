/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.txt file.

 BEGIN_JUCE_MODULE_DECLARATION

  ID:               monju
  vendor:
  version:          0.0.1
  name:             monju
  description:      Monju = Lua + Sol2 + JUCE
  website:
  license:          MIT
  searchpaths:      ../lua ../sol2/single/include/sol ../JuceLibraryCode

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#pragma once

#define SOL_USING_CXX_LUA 1

#include "sol.hpp"

#define DONT_SET_USING_JUCE_NAMESPACE 1

#include "JuceHeader.h"
