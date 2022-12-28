//
// Created by mikayel on 18/12/22.
//

#ifndef THREADAPI_CONDITIONVARIABLE_H
#define THREADAPI_CONDITIONVARIABLE_H
#include <mutex>
#include <pthread.h>
#include "LinuxMutex.h"

namespace rau
{

class LinuxConditionVariable {
public:
    using native_handle_type = pthread_cond_t;
    //using native_handle_type = //TODO
private:
    pthread_cond_t m_cond;
public:
    LinuxConditionVariable() {
        //TODO
        int result = pthread_cond_init(&m_cond, nullptr);
        if (result != 0) {
            throw std::runtime_error{strerror(result)};
        }
    }

    LinuxConditionVariable(const LinuxConditionVariable &) = delete;

    ~LinuxConditionVariable() {
        //TODO
        // Destroys the object of type std::condition_variable.
        //
        // It is only safe to invoke the destructor if all threads have been notified.
        // It is not required that they have exited their respective wait functions:
        // some threads may still be waiting to reacquire the associated lock,
        // or may be waiting to be scheduled to run after reacquiring it.
        //
        // The programmer must ensure that no threads attempt to wait on *this once
        // the destructor has been started, especially when the waiting threads are calling
        // the wait functions in a loop or are using the overloads of the wait functions that take a predicate.
        pthread_cond_destroy(&m_cond);
    }

    void notify_one() noexcept
    {
        //TODO
        // If any threads are waiting on *this, calling notify_one unblocks one of the waiting threads.
        pthread_cond_signal(&m_cond);
    }

    void notify_all() noexcept
    {
        // TODO
        // Unblocks all threads currently waiting for *this.
        pthread_cond_broadcast(&m_cond);
    }

    void wait( std::unique_lock<LinuxMutex>& lock )
    {
        // TODO
        // Atomically unlocks lock, blocks the current executing thread, and adds it to the list of
        // threads waiting on *this. The thread will be unblocked when notify_all() or notify_one()
        // is executed. It may also be unblocked spuriously. When unblocked, regardless of the reason,
        // lock is reacquired and wait exits.

        // If this function fail to meet the postconditions
        // (lock.owns_lock()==true and lock.mutex() is locked by the calling thread),
        // std::terminate is called. For example, this could happen if relocking the mutex throws an exception.
        int result = pthread_cond_wait(&m_cond, lock.mutex()->native_handle());
        if (result != 0) {
            throw std::runtime_error{strerror(result)};
        }
    }

    template< class Predicate >
    void wait( std::unique_lock<LinuxMutex>& lock, Predicate stop_waiting )
    {
        // TODO Makes thread wait on *this on while !stop_waiting().
        // If this function fail to meet the postconditions
        // (lock.owns_lock()==true and lock.mutex() is locked by the calling thread),
        // std::terminate is called. For example, this could happen if relocking the mutex throws an exception.
        while (!stop_waiting()) {
            wait(lock);
        }
    }

    native_handle_type native_handle()
    {
        // TODO
        // Accesses the native handle of *this.
        //
        //The meaning and the type of the result of this function is implementation-defined.
        // On a POSIX system, this may be a value of type pthread_cond_t*.
        // On a Windows system, this may be a PCONDITION_VARIABLE.
        return m_cond;
    }


};

} // namespace rau
#endif //THREADAPI_CONDITIONVARIABLE_H


