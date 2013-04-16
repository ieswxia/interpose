#if !defined(INTERPOSE_PLATFORM_HPP)
#define INTERPOSE_PLATFORM_HPP

#if defined(__APPLE__)
    #define _OSX(x) x
#else
    #define _OSX(x)
#endif

#if defined(__linux__)
    #define _LINUX(x) x
#else
    #define _LINUX(x)
#endif

#endif
