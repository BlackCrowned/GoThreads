#pragma once
#include <memory>

namespace gothreads {
    
    template<class T>
    class chan_impl_base {
        chan_impl_base() = default;
        virtual ~chan_impl_base() = default;

        virtual void send(T&& data) = 0;
        virtual T receive() = 0;

        virtual void close() = 0;
        virtual bool closed() = 0;
    };

    template<class T>
    class chan_impl_unbuffered : public chan_impl_base<T> {
        
    };

    template<class T>
    class chan_impl_buffered : public chan_impl_base<T> {
        
    };

    template<class T>
    class chan {
        std::shared_ptr<chan_impl_base<T>> _chan;

    public:
        chan();
        chan(size_t buffer_size);

        ~chan();

        void send(T&& data);
        T receive();

        void close();
        bool closed();
    };
}

#include "../src/channel.inl"