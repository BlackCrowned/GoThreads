namespace gothreads
{
    template<class FuncT, class ...Args>
    go::go(FuncT function, Args && ...arguments) : 
    _thread_pool(_service_locator.get_thread_pool())
    {
        _thread_pool->schedule_task(std::move(detail::task(function, std::forward<Args>(arguments)...)));
    }
}
