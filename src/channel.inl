
namespace gothreads {
    template <class T>
    chan<T>::chan() :
    _chan(new chan_impl_unbuffered<T>())
    {
        
    }

    template <class T>
    chan<T>::chan(size_t buffer_size) :
    _chan()
    {
        if (buffer_size > 0) {
            _chan.reset(new chan_impl_buffered<T>(buffer_size));
        }
        else {
            _chan.reset(new chan_impl_unbuffered<T>());
        }
    }

    template <class T>
    chan<T>::~chan() {

    }

    template <class T>
    void chan<T>::send(T&& data) {
        _chan->send(std::forward<T>(data));
    }

    template <class T>
    T chan<T>::receive() {
        return _chan->receive();
    }

    template <class T>
    void chan<T>::close() {
        _chan->close();
    }

    template <class T>
    bool chan<T>::closed() {
        return _chan->closed();
    }
}