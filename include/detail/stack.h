#pragma once
#include <memory>
#include <vector>

namespace gothreads {
    namespace detail {
        
        
        class stack {
            
        public:
            typedef char Type;
            
            template<class Allocator = std::allocator<Type>>
            static std::unique_ptr<stack> create(size_t size = 0);

            virtual ~stack() = default;
            
            virtual size_t capacity() const = 0;
            virtual size_t size() const = 0;
            virtual void reserve(size_t i) = 0;

            virtual void resize(size_t i) = 0;
            virtual void resize(size_t i, Type const& val) = 0;

            virtual Type* data() = 0;
            virtual Type const* data() const = 0;
            
        };

        template<class Type, class Allocator>
        class stack_impl : public stack {
            std::vector<Type, Allocator> _container;

        public:
            explicit stack_impl(size_t size);

            virtual ~stack_impl() = default;

            size_t capacity() const override;
            size_t size() const override;
            void reserve(size_t i) override;

            void resize(size_t i) override;
            void resize(size_t i, Type const& val) override;

            Type* data() override;
            Type const* data() const override;
        };
    }
}

#include "../../src/detail/stack.inl"
