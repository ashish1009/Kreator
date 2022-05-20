//
//  iKanHeaders.h
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

// Include any core headers and Macros here that should be included in client side.
// Client should include just one file to compile Core engine library

// C++ Files
#include <iostream>

// Preprocessor
#include <iKanCommon.h>

// Core Files
#include <Core/Debug/Logger.hpp>
#include <Core/Debug/Profiler.hpp>
#include <Core/Utils/Asserts.h>

#include <Core/Event/ApplicationEvent.h>
#include <Core/Event/KeyEvent.h>
#include <Core/Event/MouseEvent.h>

#include <Core/Application.hpp>
#include <Core/Layer.hpp>

// Renderer Files
#include <Renderer/Utils/Renderer.hpp>
