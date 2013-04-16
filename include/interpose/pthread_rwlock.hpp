#if !defined(INTERPOSE_PTHREAD_RWLOCK_HPP)
#define INTERPOSE_PTHREAD_RWLOCK_HPP

#if defined(__APPLE__)
#include <interpose/osx/pthread_rwlock.hpp>
#elif defined(__linux__)
#include <interpose/linux/pthread_rwlock.hpp>
#endif

#endif
