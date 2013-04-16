#if !defined(INTERPOSE_PTHREAD_HPP)
#define INTERPOSE_PTHREAD_HPP

#if defined(__APPLE__)
#include <interpose/osx/pthread.hpp>
#elif defined(__linux__)
#include <interpose/linux/pthread.hpp>
#endif

#endif
