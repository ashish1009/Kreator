//
//  AssertsCoreAPI.h
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

// Includes API for trigger assert only for Core Engine not visible to client

#include "Core/Utils/Asserts.h"

#define IK_CORE_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { IK_CORE_ERROR("Assertion Failed"); ASSERT(condition); } }
#define IK_CORE_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { IK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); ASSERT(condition); } }

#define IK_GET_CORE_ASSERT_MACRO(...) IK_EXPAND_VARGS(IK_ASSERT_RESOLVE(__VA_ARGS__, IK_CORE_ASSERT_MESSAGE, IK_CORE_ASSERT_NO_MESSAGE))

#define IK_CORE_ASSERT(...) IK_EXPAND_VARGS( IK_GET_CORE_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
