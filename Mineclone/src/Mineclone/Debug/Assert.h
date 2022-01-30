#pragma once

#include "Log.h"

#define ASSERT(assertion, message) { if(!(assertion)) { Mineclone::Log::error("Assert", "Assertion failed: " + (message)); __debugbreak(); } }
#define ASSERT_NOT_REACHED(message) { Mineclone::Log::error("Assert", "Hit assertion: " + (message)); __debugbreak(); }
