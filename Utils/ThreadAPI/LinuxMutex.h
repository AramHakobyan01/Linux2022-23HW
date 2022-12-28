#ifndef THREADAPI_LINUXMUTEX_H
#define THREADAPI_LINUXMUTEX_H
#include <pthread.h>
#include <iostream>
#include <cstring>

namespace rau
{
// The LinuxMutex class is a synchronization primitive that can be used to protect
// shared data from being simultaneously accessed by multiple threads.

// LinuxMutex offers exclusive, non-recursive ownership semantics:
// 1. A calling thread owns a LinuxMutex from the time that it successfully
//    calls either lock or try_lock until it calls unlock.
// 2. When a thread owns a LinuxMutex, all other threads will block (for calls to lock)
//    or receive a false return value (for try_lock) if they attempt to claim ownership of the mutex.
// 3. A calling thread must not own the mutex prior to calling lock or try_lock.
class LinuxMutex
{
public:
    using native_handle_type = pthread_mutex_t;
public:
    constexpr LinuxMutex() noexcept
    {
        pthread_mutex_init(&mutex, nullptr);
        // TODO
        // Constructs the mutex. The mutex is in unlocked state after the constructor completes.
    }

    LinuxMutex( const LinuxMutex& ) = delete;

    LinuxMutex& operator=(const LinuxMutex&) = delete;

    ~LinuxMutex()
    {
        pthread_mutex_destroy(&mutex);
        // TODO
        // Destroys the mutex. The behavior is undefined if the mutex is owned by
        // any thread or if any thread terminates while holding any ownership of the mutex.
    }

    [[nodiscard]]
    pthread_mutex_t native_handle() const noexcept
    {
        // TODO
        return mutex;
    }

    void lock()
    {
        // TODO
        // Locks the mutex. If another thread has already locked the mutex, a call
        // to lock will block execution until the lock is acquired.
        // If lock is called by a thread that already owns the mutex, the behavior
        // is undefined: for example, the program may deadlock.
        pthread_mutex_lock(&mutex);
    }

    void unlock()
    {
        // TODO
        // Unlocks the mutex. The mutex must be locked by the current
        // thread of execution, otherwise, the behavior is undefined.
        pthread_mutex_unlock(&mutex);
    }

    bool try_lock()
    {
        // TODO
        // Tries to lock the mutex. Returns immediately. On successful lock acquisition
        // returns true, otherwise returns false.
        return pthread_mutex_trylock(&mutex) == 0;
    }
private:
    native_handle_type mutex{};
};

// https://en.cppreference.com/w/cpp/thread/lock_guard
    class LinuxMutexGuard
    {
    public:
        // Construct the guard and lock the mutex
        explicit LinuxMutexGuard(LinuxMutex& mutex) : mutex_(mutex)
        {
            mutex_.lock();
        }

        // Destruct the guard and unlock the mutex
        ~LinuxMutexGuard()
        {
            mutex_.unlock();
        }

    private:
        LinuxMutex& mutex_;
    };


} // namespace rau

#endif //THREADAPI_LINUXMUTEX_H