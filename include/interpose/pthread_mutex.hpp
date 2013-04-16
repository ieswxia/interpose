#if !defined(INTERPOSE_PTHREAD_MUTEX_HPP)
#define INTERPOSE_PTHREAD_MUTEX_HPP

#if defined(__APPLE__)
#include <interpose/osx/pthread_mutex.hpp>
#elif defined(__linux__)
#include <interpose/linux/pthread_mutex.hpp>
#endif

#endif