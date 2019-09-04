
#pragma once

#define UNUSED(x) (void)(x)

#include <cstdlib>
#include <vector>

namespace alc
{
    template <typename T,unsigned BUFF_SIZE = 5>
    struct buffering_allocator
    {
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = std::size_t;

        template <typename U>
        struct rebind
        {
            using other = buffering_allocator<U>;
        };

        buffering_allocator()
        {
            _buff.reserve(BUFF_SIZE);
        };

        ~buffering_allocator()
        {
            for (const auto &p : _buff)
                std::free(p);
        }
        
        template<typename U>
        buffering_allocator(const buffering_allocator<U>&) {}

        pointer allocate(size_type n)
        {
            if (_buff.empty())
            {
                auto n_alloc = (n > BUFF_SIZE) ? n : BUFF_SIZE;

                auto p = std::malloc(n_alloc * sizeof(T));

                if (!p)
                    throw std::bad_alloc();

				auto ptype = reinterpret_cast<pointer>(p);

                for (size_type i = 0; i < n_alloc - n; ++i)
                    _buff.push_back(ptype + i);

                return (ptype + (n_alloc - n));
            }
            else
            {
                pointer p = _buff.at(_buff.size() - 1);
                _buff.pop_back();

                return p;
            }
        }

        void deallocate(pointer p,size_type n)
        {
            for (size_type i = 0; i < n; ++i)
                _buff.emplace_back(p + i);
        }

        template<typename U, typename ...Args>
        void construct(U *p, Args &&...args)
        {
            new(p) U(std::forward<Args>(args)...);
        }

        void destroy(pointer p)
        {
            p->~value_type();
        }

        private:

            std::vector<pointer> _buff;
    };

    template <typename T>
    struct my_allocator
    {
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = std::size_t;

        template <typename U>
        struct rebind
        {
            using other = my_allocator<U>;
        };

        my_allocator() = default;
        ~my_allocator() = default;
        
        template<typename U>
        my_allocator(const my_allocator<U>&) {}

        pointer allocate(size_type n)
        {
            auto p = std::malloc(n * sizeof(T));

            if (!p)
                throw std::bad_alloc();

            return reinterpret_cast<pointer>(p);
        }

        void deallocate(pointer p,size_type n)
        {
            std::free(p);
            UNUSED(n);
        }

        template<typename U, typename ...Args>
        void construct(U *p, Args &&...args)
        {
            new(p) U(std::forward<Args>(args)...);
        }

        void destroy(pointer p)
        {
            p->~value_type();
        }
    };
}
