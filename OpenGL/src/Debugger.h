#pragma once

void GlClearError();
bool GlLogCall_(const char* function, const char* file, int line);

#if _DEBUG == 1
#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GlClearError();\
        x;\
        ASSERT(GlLogCall_(#x, __FILE__, __LINE__))
#else
#define ASSERT(x) x
#define GlCall(x) x
#endif
