namespace gothreads
{
    template<class FuncT, class ...Args>
    go::go(FuncT function, Args && ...arguments)
    {
        _thread_pool = _service_locator.get_thread_pool();
    }
}
