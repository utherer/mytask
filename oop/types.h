#pragma once

#include <windows.h>

inline void DebugCheck(bool x){
#ifdef _DEBUG
    if(!x) ::DebugBreak();
#endif
}

#define CHECK(x) DebugCheck(x)