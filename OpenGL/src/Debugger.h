#pragma once

void GlClearError();
bool GlLogCall_(const char* function, const char* file, int line);

#ifdef _DEBUG
#pragma message("---- _DEBUG defined.")
#else
#pragma message("---- _DEBUG NOT defined.")
#endif

#define ASSERT(x) if (!(x)) __debugbreak()
#define GlCall(x) GlClearError();\
        x;\
        ASSERT(GlLogCall_(#x, __FILE__, __LINE__))