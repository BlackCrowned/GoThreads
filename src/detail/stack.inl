
namespace gothreads {
    namespace detail {
        
        template <class Allocator>
        std::unique_ptr<stack> stack::create(size_t size) {
            return std::make_unique<stack_impl<Type, Allocator>>(size);
        }

        template <class Allocator>
        std::unique_ptr<stack> stack::copy(std::unique_ptr<stack> const& s) {
            return std::make_unique<stack_impl<Type, Allocator>>(*dynamic_cast<stack_impl<Type, Allocator>*>(s.get()));
        }

        template <class Type, class Allocator>
        stack_impl<Type, Allocator>::stack_impl(size_t size) : 
        _container(size) {
            
        }

        template <class Type, class Allocator>
        size_t stack_impl<Type, Allocator>::capacity() const {
            return _container.capacity();
        }

        template <class Type, class Allocator>
        size_t stack_impl<Type, Allocator>::size() const {
            return _container.size();
        }

        template <class Type, class Allocator>
        void stack_impl<Type, Allocator>::reserve(size_t i) {
            return _container.reserve(i);
        }

        template <class Type, class Allocator>
        void stack_impl<Type, Allocator>::resize(size_t i) {
            return _container.resize(i);
        }

        template <class Type, class Allocator>
        void stack_impl<Type, Allocator>::resize(size_t i, stack::Type const& val) {
            return _container.resize(i, val);
        }

        template <class Type, class Allocator>
        void stack_impl<Type, Allocator>::shrink_to_fit() {
            return _container.shrink_to_fit();
        }

        template <class Type, class Allocator>
        stack::Type* stack_impl<Type, Allocator>::data() {
            return _container.data();
        }

        template <class Type, class Allocator>
        stack::Type const* stack_impl<Type, Allocator>::data() const {
            return _container.data();
        }
    }
}
