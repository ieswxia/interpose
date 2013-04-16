#if !defined(INTERPOSE_SIGNALS_HPP)
#define INTERPOSE_SIGNALS_HPP

#if defined(__APPLE__)
#include <interpose/osx/signals.hpp>
#elif defined(__linux__)
#include <interpose/linux/signals.hpp>
#endif

#endif