#if !defined(INTERPOSE_PROC_HPP)
#define INTERPOSE_PROC_HPP

#if defined(__APPLE__)
#include <interpose/osx/proc.hpp>
#elif defined(__linux__)
#include <interpose/linux/proc.hpp>
#endif

#endif
