#if !defined(INTERPOSE_DL_HPP)
#define INTERPOSE_DL_HPP

#if defined(__APPLE__)
#include <interpose/osx/dl.hpp>
#elif defined(__linux__)
#include <interpose/linux/dl.hpp>
#endif

#endif