
#pragma once

namespace ffl
{
    template<typename T = int,class A = std::allocator<T> >
    class fifo
    {

        public:

            using value_type = T;
            using reference = value_type&;
            using const_reference = const reference;
            using pointer = value_type*;
            using const_pointer = const pointer;

            using allocator_type = A;
            using const_allocator_type = const allocator_type;

            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;

            // constructors
            explicit fifo() 
                : _alloc()
                , _size()
                ,_capacity()
                , _data()
                , _head()
                , _tail()
            {}

            explicit fifo(size_type n) 
                : _alloc()
                , _size(n)
                , _capacity(n*_CAPACITY_INCREASE)
                , _data(allocator().allocate(n*_CAPACITY_INCREASE))
                , _head()
                , _tail(n)
            {
                initializate(data_begin(),size(),value_type());
            }

            explicit fifo(size_type n,value_type v) 
                : _alloc()
                , _size(n)
                , _capacity(n*_CAPACITY_INCREASE)
                , _data(allocator().allocate(n*_CAPACITY_INCREASE))
                , _head()
                , _tail(n)
            {
                initializate(data_begin(),size(),v);
            }

            explicit fifo(fifo& f)
                : _alloc(f.allocator())
                , _size(f.size())
                , _capacity(f.capacity()) 
                , _data(allocator().allocate(f.capacity()))
                , _head(f.head())
                , _tail(f.tail())
            {
                memcpy(data_begin(),f.data_begin(),sizeof(value_type)*size());
            }

            explicit fifo(fifo&& f)
                : _alloc(std::move(f._alloc))
                , _size(std::move(f._size))
                , _capacity(std::move(f._capacity)) 
                , _data(std::move(f.data_begin()))
                , _head(std::move(f._head))
                , _tail(std::move(f._tail))
            {
                f._size = size_type();
                f._capacity = size_type();
                f._data = pointer();
            }

            // destructor
            ~fifo()
            {
                if (data_begin() != pointer())
                {
                    for (size_type i = 0; i < size(); ++i)
                        allocator().destroy(data_begin() + i);

                    allocator().deallocate(data_begin(),capacity());
                }
            }
            
            // iterator
            class iterator
            {
                friend class fifo;

                private:
                    
                    pointer _p;
                    const_pointer _begin;
                    const_pointer _end;

                    iterator(pointer p,pointer begin, pointer end) 
                        : _p(p)
                        , _begin(const_cast<const_pointer>(begin))
                        , _end(const_cast<const_pointer>(end))
                    {}

                public:
                    
                    iterator() = default;
                    iterator(const iterator&) = default;
                    ~iterator() = default;

                    bool operator!=(const iterator& other ) const { return other._p != _p; }
                    
                    pointer& operator++() 
                    { 
                        _p = ( _p == _end ) ? const_cast<pointer>(_begin) : ++_p;
                        return _p; 
                    }

                    reference operator*() const { return *_p; }
                    pointer operator->() const { return _p; }

            };

            // const iterator
            class const_iterator
            {
                friend class fifo;

                private:
                    
                    pointer _p;

                    const_iterator(pointer p) : _p(p) {}
                public:
                    
                    const_iterator() = default;
                    const_iterator(const const_iterator&) = default;
                    const_iterator(const iterator& it) : _p(it._p) {}
                    ~const_iterator() = default;

                    bool operator!=(const const_iterator& other ) const { return other._p != _p; }
                    const_reference operator*() const { return *_p; }
                    const_pointer operator->() { return _p; }

            };

            // info
            size_type     size()     const { return _size; }
            size_type     capacity() const { return _capacity; }
            bool          empty()    const { return _size == 0; }

            allocator_type& allocator() { return _alloc; }
            const_allocator_type& allocator() const { return _alloc; }

            // access
            iterator       begin()    const { return iterator((data_begin() + head()),data_begin(),(data_begin() + capacity() - 1)); }
            iterator       end()      const { return iterator((data_begin() + tail()),data_begin(),(data_begin() + capacity() - 1)); }

            const_iterator cbegin()   const { return static_cast<const_iterator>(begin()); }
            const_iterator cend()     const { return static_cast<const_iterator>(end()); }

            value_type pop() 
            {
                value_type tmp = value_type();

                if (!empty())
                {
                    tmp = *cbegin();
                    ++_head;
                    _head %= capacity();
                    --_size;
                }

                return tmp;
            }

            reference pop_peek() { return *cbegin(); }

            // modification
            void push(value_type value)
            {
                if (size() + 1 == capacity())
                {
                    pointer tmp = allocator().allocate(capacity()*_CAPACITY_INCREASE);

                    for (size_type i = 0; i < size(); ++i)
                    {
                        *(tmp + i) = *(data_begin() + head());
                        ++_head;
                        _head %= capacity();
                    }

                    _head = 0;
                    _tail = size();

                    allocator().deallocate(data_begin(),capacity());

                    _data = tmp;

                    _capacity *= _CAPACITY_INCREASE;
                }
                else if (size() == 0)
                {
                    _data = allocator().allocate(_CAPACITY_INCREASE);

                    _capacity = _CAPACITY_INCREASE;
                    _head = 0;
                    _tail = 0;
                }

                *(data_begin() + tail()) = value;

                ++_tail;
                _tail %= capacity();

                ++_size;
            }

        private:

            allocator_type _alloc;
            size_type _size;
            size_type _capacity;
            pointer _data;
            size_type _head;
            size_type _tail;

            static const size_type _CAPACITY_INCREASE = 2;

            inline void initializate(pointer d,size_type n,value_type v)
            {
                for (size_type i = 0; i < n; ++i)
                    allocator().construct((d + i),v);
            }

            pointer data_begin()  const { return _data; }
            pointer data_end() const { return _data + _size; }
            size_type head() const { return _head; }
            size_type tail() const { return _tail; }

    };
}
