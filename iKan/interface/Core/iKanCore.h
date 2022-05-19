//
//  iKanCore.h
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

// Engine Core file. All header that are used by all engine files to be included here.
// Any preprocessor for entire engine to be defined here

// Engine core file - Interface
#include "Core/Debug/Logger.hpp"
#include "Core/Utils/Asserts.h"

// Engine core file - Implementation
#include "Core/Debug/LoggerCoreAPI.h"
#include "Core/Utils/AssertsCoreAPI.h"

// Core Preprocessors
#define BIT(x) (1 << x) // Shift bit 1 at position x
#define ToString(x) #x  // Convert X to string
