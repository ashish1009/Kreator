//
//  iKanCore.h
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

// Engine Core file. All header that are used by all engine files to be included here.
// Any preprocessor for entire engine to be defined here

// Preprocessor
#include <iKanCommon.h>

// Engine core file - Interface
#include "Core/Debug/Logger.hpp"
#include "Core/Debug/Profiler.hpp"
#include "Core/Utils/Asserts.h"

// Engine core file - Implementation
#include "Core/Debug/LoggerCoreAPI.h"
#include "Core/Utils/AssertsCoreAPI.h"
