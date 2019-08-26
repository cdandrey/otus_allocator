
#pragma once

#include <vector>

#define UNUSED(x) (void)(x)

namespace alc
{
    template <typename T>
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

        buffering_allocator() = default;

        ~buffering_allocator()
        {
            for (const auto &p : buff)
                free(p);
        }
        
        template<typename U>
        buffering_allocator(const buffering_allocator<U>&) {}

        T *allocate(std::size_t n)
        {
            if (buff.empty())
            {
                auto p = malloc(n * sizeof(T));

                if (!p)
                    throw std::bad_alloc();

                return reinterpret_cast<T *>(p);
            }
            else
            {
                auto p = buff.at(buff.size() - 1);
                buff.pop_back();

                return p;
            }
        }

        void deallocate(T *p,std::size_t n)
        {
            UNUSED(n);
            buff.emplace_back(p);
        }

        template<typename U, typename ...Args>
        void construct(U *p, Args &&...args)
        {
            new(p) U(std::forward<Args>(args)...);
        }

        void destroy(T *p)
        {
            p->~T();
        }

        private:

            std::vector<T *> buff;
    };
}
