//
//  Asserts.h
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

// Internal API Marco used by main MACRO
#define IK_EXPAND_VARGS(x) x
#define ASSERT(condition) assert(condition);
#define IK_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { IK_ERROR("Assertion Failed"); ASSERT(condition); } }
#define IK_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { IK_ERROR("Assertion Failed: {0}", __VA_ARGS__); ASSERT(condition); } }
#define IK_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro
#define IK_GET_ASSERT_MACRO(...) IK_EXPAND_VARGS(IK_ASSERT_RESOLVE(__VA_ARGS__, IK_ASSERT_MESSAGE, IK_ASSERT_NO_MESSAGE))

// API Macro to trigger the Abort with and without any prints
#define IK_ASSERT(...) IK_EXPAND_VARGS( IK_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#define IK_ABORT(condition) IK_ASSERT_NO_MESSAGE(condition);
