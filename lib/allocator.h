
#pragma once

#include <cstdlib>
#include <vector>

namespace alc
{
    template <typename T,unsigned BUFF_SIZE = 10>
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

        buffering_allocator() : _is_buff(true)
        {
            _buff.reserve(BUFF_SIZE);
        };

        ~buffering_allocator()
        {
            if (is_buff())
            {
                for (const auto &p : _head)
                    std::free(p);
            }
        }
        
		buffering_allocator(const buffering_allocator<T, BUFF_SIZE>& al)
		{
            _is_buff = al.is_buff();
		}

        template<typename U>
        buffering_allocator(const buffering_allocator<U>&) 
		{
            _is_buff = false;
		}

        pointer allocate(size_type n)
        {
            if (is_buff() && n == 1)
            {
                if (_buff.empty())
                {
                    auto p = std::malloc(BUFF_SIZE*sizeof(T));

                    if (!p)
                        throw std::bad_alloc();

                    pointer ptype = reinterpret_cast<pointer>(p);

                    for (size_type i = 1; i < BUFF_SIZE; ++i)
                        _buff.emplace_back(ptype + i);

                    _head.emplace_back(ptype);
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
            if (is_buff() && n == 1)
                _buff.emplace_back(p);
            else
                std::free(p);
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

        bool is_buff() const { return _is_buff; }

        private:

            bool _is_buff;
            std::vector<pointer> _buff;
            std::vector<pointer> _head;
    };
}
