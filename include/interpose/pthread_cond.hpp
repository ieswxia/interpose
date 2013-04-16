#if !defined(INTERPOSE_PTHREAD_COND_HPP)
#define INTERPOSE_PTHREAD_COND_HPP

#if defined(__APPLE__)
#include <interpose/osx/pthread_cond.hpp>
#elif defined(__linux__)
#include <interpose/linux/pthread_cond.hpp>
#endif

#endif