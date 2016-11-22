namespace gothreads
{
    template<class FuncT, class ...Args>
    go::go(FuncT function, Args && ...arguments) : base()
    {
        _thread_pool->schedule_task(std::move(detail::task(function, std::forward<Args>(arguments)...)));
    }
}
