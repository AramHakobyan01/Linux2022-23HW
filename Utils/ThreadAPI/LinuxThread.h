#ifndef THREADAPI_LINUXTHREAD_H
#define THREADAPI_LINUXTHREAD_H

#include <cstring>
#include <typeinfo>
#include <functional>
#include <mutex>
#include "details/LinuxThreadDetails.h"

namespace rau
{

    class LinuxThread
    {
    public:
        using native_handle_type = pthread_t;

    public:

        // The class LinuxThread::id is a lightweight, trivially copyable class that serves as
        // a unique identifier of LinuxThread.
        //
        // Instances of this class may also hold the special distinct value that
        // does not represent any thread. Once a thread has finished,
        // the value of LinuxThread::id may be reused by another thread.
        //
        // This class is designed for use as key in associative containers, both ordered and unordered.
        class id
        {
            native_handle_type m_threadID{0ull};
        public:

            friend bool operator==( id lhs, id rhs ) noexcept
            {
                return 0 != pthread_equal(lhs.m_threadID, rhs.m_threadID);
            }

            friend bool operator!=( id lhs, id rhs ) noexcept
            {
                return !(lhs == rhs);
            }

            friend bool operator< ( id lhs, id rhs ) noexcept
            {
                return lhs.m_threadID < rhs.m_threadID;
            }

            friend bool operator<=( id lhs, id rhs ) noexcept
            {
                //TODO
                return !(lhs > rhs);
            }

            friend bool operator> ( id lhs, id rhs ) noexcept
            {
                //TODO
                return rhs < lhs;
            }

            friend bool operator>=( id lhs, id rhs ) noexcept
            {
                //TODO
                return !(lhs < rhs);
            }

            friend std::ostream& operator<<(std::ostream& out, id obj)
            {
                //TODO
                return out << obj.m_threadID;
            }

            friend std::hash<LinuxThread::id>;

        };

    private:
        id m_ID;

    public:

        native_handle_type native_handle()
        {
            return *reinterpret_cast<native_handle_type*>(&m_ID);
        }

        LinuxThread() noexcept = default;

        template <typename Callable, typename ...Args>
        explicit LinuxThread(Callable&& c, Args&&... args) noexcept
        {
            auto* wrapper = new detail::ThreadRoutineWrapper<Callable&&, Args&&...>
                    {std::forward<Callable&&>(c), std::forward<Args&&>(args)...};
            using lambda_type = std::remove_reference_t<decltype(*wrapper)>;
            native_handle_type temp_id;
            auto result = pthread_create(&temp_id, nullptr,
                                         &detail::posix_thread_routine<lambda_type>, wrapper);
            if(result != 0)
            {
                delete wrapper;
                throw std::runtime_error{strerror(errno)};
            }
            m_ID = *reinterpret_cast<id*>(temp_id);
        }

        LinuxThread( LinuxThread&& other ) noexcept
        {
            // TODO
            // Move constructor. Constructs the thread object to represent
            // the thread of execution that was represented by "other".
            // After this call other no longer represents a thread of execution.
            this->m_ID = other.m_ID;
            other.m_ID = id{};
        }

        LinuxThread( const LinuxThread& ) = delete;
        ~LinuxThread()
        {
            // TODO
            // If *this has an associated thread (joinable() == true), std::terminate() is called.
            // A thread object does not have an associated thread (and is safe to destroy) after
            //      1. it was default-constructed
            //      2. it was moved from
            //      3. join() has been called
            //      4. detach() has been called
            if(joinable()){
                std::logic_error{"a thread was left not joined/not detached"};
            }
        }

        LinuxThread& operator=(LinuxThread&& other)
        {
            if (joinable()) {
                throw std::logic_error{"trying to drop not joined/not detached thread"};
            }
            m_ID = other.m_ID;
            other.m_ID = id{};
            // TODO
            // If *this still has an associated running thread (i.e. joinable() == true), calls std::terminate().
            // Otherwise, assigns the state of other to *this and sets other to a default constructed state.
            // After this call, this->get_id() is equal to the value of other.get_id() prior to the call,
            // and other no longer represents a thread of execution.
            return *this;
        }

        LinuxThread& operator=(LinuxThread& other) = delete;

        [[nodiscard]]
        id get_id() const
        {
            //TODO
            // Returns a value of std::thread::id identifying the thread associated with *this.
            return m_ID;
        }

        [[nodiscard]]
        bool joinable() const
        {
            // TODO
            // Checks if the LinuxThread object identifies an active thread of execution.
            // Specifically, returns true if get_id() != LinuxThread::id().
            // So a default constructed thread is not joinable.
            //
            // A thread that has finished executing code, but has not yet been joined
            // is still considered an active thread of execution and is therefore joinable.
            if(get_id() != LinuxThread::id()){
                return true;
            }
            return false;
        }

        void join()
        {
            // TODO
            // Blocks the current thread until the thread identified by *this finishes its execution.
            // The completion of the thread identified by *this synchronizes with the corresponding
            // successful return from join(). No synchronization is performed on
            // *this itself. Concurrently calling join() on the same thread object from multiple
            // threads constitutes a data race that results in undefined behavior.
            if (joinable()) {
                auto join_result = pthread_join(native_handle(), nullptr);
                if (join_result != 0) {
                    throw std::runtime_error{strerror(join_result)};
                }
            }
            m_ID = id{};
        }

        void detach()
        {
            // TODO
            // Separates the thread of execution from the thread object, allowing execution to continue
            // independently. Any allocated resources will be freed once the thread exits.
            // After calling detach *this no longer owns any thread.
            pthread_detach(native_handle());
            this->m_ID = id{};
        }

    };

}

namespace std
{

    template <>
    struct hash<rau::LinuxThread::id>
    {
        size_t operator()(rau::LinuxThread::id&& id)
        {
            return id.m_threadID;
        }
    };

} // namespace std


#endif //THREADAPI_LINUXTHREAD_H
