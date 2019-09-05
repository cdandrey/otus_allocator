
#pragma once

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
            if (n == 1)
            {
                if (_buff.empty())
                {
                    for (size_type i = 0; i < BUFF_SIZE; ++i)
                    {
                        auto p = std::malloc(sizeof(T));

                        if (!p)
                            throw std::bad_alloc();

                        _buff.emplace_back(reinterpret_cast<pointer>(p));
                    }
                }

                pointer p = _buff.at(_buff.size() - 1);

				_buff.pop_back();

                return p;
            }
            else
            {
                auto p = std::malloc(n*sizeof(T));

                if (!p)
                    throw std::bad_alloc();

                return reinterpret_cast<pointer>(p);
            }
            
        }

        void deallocate(pointer p,size_type n)
        {
            if (n == 1)
            {
                _buff.emplace_back(p);
            }
            else
            {
                std::free(p);
            }
            
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
}
