// ----------------------
// projects/deque/Deque.h
// Copyright (C) 2013
// Glenn P. Downing
// ----------------------

#ifndef Deque_h
#define Deque_h

// --------
// includes
// --------

#include <algorithm> // copy, equal, lexicographical_compare, max, swap
#include <cassert>   // assert
#include <iterator>  // iterator, bidirectional_iterator_tag
#include <memory>    // allocator
#include <stdexcept> // out_of_range
#include <utility>   // !=, <=, >, >=

// -----
// using
// -----

using std::rel_ops::operator!=;
using std::rel_ops::operator<=;
using std::rel_ops::operator>;
using std::rel_ops::operator>=;

// -------
// destroy
// -------

template <typename A, typename BI>
BI destroy (A& a, BI b, BI e) {
    while (b != e) {
        --e;
        a.destroy(&*e);}
    return b;}

// ------------------
// uninitialized_copy
// ------------------

template <typename A, typename II, typename BI>
BI uninitialized_copy (A& a, II b, II e, BI x) {
    BI p = x;
    try {
        while (b != e) {
            a.construct(&*x, *b);
            ++b;
            ++x;}}
    catch (...) {
        destroy(a, p, x);
        throw;}
    return x;}

// ------------------
// uninitialized_fill
// ------------------

template <typename A, typename BI, typename U>
BI uninitialized_fill (A& a, BI b, BI e, const U& v) {
    BI p = b;
    try {
        while (b != e) {
            a.construct(&*b, v);
            ++b;}}
    catch (...) {
        destroy(a, p, b);
        throw;}
    return e;}

// -------
// MyDeque
// -------

template < typename T, typename A = std::allocator<T> >
class MyDeque {
    public:
        // --------
        // typedefs
        // --------

        typedef A                                        allocator_type;
        typedef typename allocator_type::value_type      value_type;

        typedef typename allocator_type::size_type       size_type;
        typedef typename allocator_type::difference_type difference_type;

        typedef typename allocator_type::pointer         pointer;
        typedef typename allocator_type::const_pointer   const_pointer;

        typedef typename allocator_type::reference       reference;
        typedef typename allocator_type::const_reference const_reference;

    public:
        // -----------
        // operator ==
        // -----------

        /**
         * == operator
         * @param lhs a MyDeque
         * @param rhs a MyDeque
         * @return whether the two deques are equal
         */
        friend bool operator == (const MyDeque& lhs, const MyDeque& rhs) {
            return lhs.size == rhs .size && std::equal(lhs.begin, lhs.end, rhs.begin);
        }

        // ----------
        // operator <
        // ----------

        /**
         * < operator
         * @param lhs a MyDeque
         * @param rhs a MyDeque
         * @return whether the lhs is smaller than rhs
         */
        friend bool operator < (const MyDeque& lhs, const MyDeque& rhs) {
            return std::lexicographical_compare(lhs.begin, lhs.end, rhs.begin, rhs.end);
        }

    private:
        // ----
        // data
        // ----

        allocator_type _a;
        allocator_type<T>::rebind<*T>::other _a_outer;
        pointer* first_block;
        pointer* last_block;
        iterator begin;
        iterator end;
        std::size_t size;
        static std::size_t block_size;

    private:
        // -----
        // valid
        // -----

        bool valid () const {
            // <your code>
            return true;}

    public:
        // --------
        // iterator
        // --------

        class iterator {
            public:
                // --------
                // typedefs
                // --------

                typedef std::bidirectional_iterator_tag   iterator_category;
                typedef typename MyDeque::value_type      value_type;
                typedef typename MyDeque::difference_type difference_type;
                typedef typename MyDeque::pointer         pointer;
                typedef typename MyDeque::reference       reference;

            public:
                // -----------
                // operator ==
                // -----------

                /**
                 * ==operator
                 * @param lhs an iterator to compare
                 * @param rhs an iterator to compare
                 * @return whether the iterators are equal
                 */
                friend bool operator == (const iterator& lhs, const iterator& rhs) 
                {
                    return (lhs.current_block_index == rhs.current_block_index) && (*lhs.current_block == *rhs.current_block);
                }

                /**
                 * !=operator
                 * @param lhs an iterator to compare
                 * @param rhs an iterator to compare
                 * @return whether the iterators are not equal
                 */
                friend bool operator != (const iterator& lhs, const iterator& rhs) {
                    return !(lhs == rhs);}

                // ----------
                // operator +
                // ----------

                /**
                 * +operator
                 * @param lhs an iterator to compare
                 * @param rhs integral type to change the position of the iterator
                 * @return lhs + rhs
                 */
                friend iterator operator + (iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * -operator
                 * @param lhs an iterator to compare
                 * @param rhs integral type to change the position of the iterator
                 * @return lhs - rhs
                 */
                friend iterator operator - (iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                pointer *current_block;
                std::size_t current_block_index;

            private:
                // -----
                // valid
                // -----

                bool valid () const 
                {

                    return true;
                }


            public:
                // -----------
                // constructor
                // -----------

                /**
                 * constructor for iterator
                 * @param block the pointer a block in the deque
                 * @param index the index of the block
                 */
                iterator (pointer* block, std::size_t index) 
                {
                    current_block = block;
                    current_block_index = index;
                    assert(valid());
                }

                // Default copy, destructor, and copy assignment.
                // iterator (const iterator&);
                // ~iterator ();
                // iterator& operator = (const iterator&);

                // ----------
                // operator *
                // ----------

                /**
                 * deference operator
                 * @return a reference to the current element the iterator at
                 */
                reference operator * () const 
                {
                    return *current_block[current_block_index];
                }

                // -----------
                // operator ->
                // -----------

                /**
                 * structure dereference operator
                 * @return the desired member of the current element the iterator at 
                 */
                pointer operator -> () const 
                {
                    return &**this;
                }

                // -----------
                // operator ++
                // -----------

                /**
                 * increment operator (pre)
                 * @return reference of the iterator after incrementation
                 */
                iterator& operator ++ () 
                {
                    //wrap around if at the end of current block
                    if(++current_block_index == block_size)
                    {
                        current_block_index = 0;
                        ++current_block;
                    }

                    assert(valid());
                    return *this;
                }

                /**
                 * increment operator (post)
                 * @return reference of the iterator before incrementation
                 */
                iterator operator ++ (int) 
                {
                    iterator x = *this;
                    ++(*this);
                    assert(valid());
                    return x;
                }

                // -----------
                // operator --
                // -----------

                /**
                 * decrement operator (pre)
                 * @return reference of the iterator after decrementation
                 */
                iterator& operator -- () 
                {
                    //wrap around if at the end of current block
                    if(current_block_index-- == 0)
                    {
                        current_block_index = 9;
                        --*current_block;
                    }
                    assert(valid());
                    return *this;
                }

                /**
                 * decrement operator (post)
                 * @return reference of the iterator before decrementation
                 */
                iterator operator -- (int) 
                {
                    iterator x = *this;
                    --(*this);
                    assert(valid());
                    return x;
                }

                // -----------
                // operator +=
                // -----------

                /**
                 * += operator
                 * @param d the number of incrememtation
                 * @return reference of the iterator after incrementation
                 */
                iterator& operator += (difference_type d) 
                {
                    //wrap around blocks
                    current_block += d / block_size;

                    int index_inc = d % block_size;
                    //warp around an additional block if necessary
                    if((current_block_index + index_inc) >= block_size)
                    {
                        current_block_index = (current_block_index + index_inc) % block_size;
                        ++current_block;
                    }
                    else
                    {
                        current_block_index += index_inc;
                    }

                    assert(valid());
                    return *this;
                }

                // -----------
                // operator -=
                // -----------

                /**
                 * -= operator
                 * @param d the number of decrememtation
                 * @return reference of the iterator after decrementation
                 */
                iterator& operator -= (difference_type d) 
                {
                    //wrap around blocks
                    current_block -= d / block_size;

                    int index_dec = d % block_size;
                    //warp around an additional block if necessary
                    if((current_block_index - index_dec) > current_block_index)
                    {
                        current_block_index = block_size - (index_dec - current_block_index);
                        --current_block;
                    }
                    else
                    {
                        current_block_index -= index_dec;
                    }
                    assert(valid());
                    return *this;
                }

                // -----------
                // get_block_address
                // -----------

                /**
                 * return the address of the current block the iterator is at
                 */
                pointer* get_block_address () 
                {
                    return current_block;
                }

                // -----------
                // get_block_index
                // -----------

                /**
                 * return the address of the current block the iterator is at
                 */

                std::size_t get_block_index () 
                {
                    return current_block_index;
                }


                // -----------
                // set_block_address
                // -----------

                /**
                 * @param new_current_block the new block address
                 * return the address of the current block the iterator is at
                 */
                void set_block_address (pointer* new_current_block) 
                {
                    current_block = new_current_block;
                }

                };

                


    public:
        // --------------
        // const_iterator
        // --------------

        class const_iterator {
            public:
                // --------
                // typedefs
                // --------

                typedef std::bidirectional_iterator_tag   iterator_category;
                typedef typename MyDeque::value_type      value_type;
                typedef typename MyDeque::difference_type difference_type;
                typedef typename MyDeque::const_pointer   pointer;
                typedef typename MyDeque::const_reference reference;

            public:
                // -----------
                // operator ==
                // -----------

                /**
                 * == Operator
                 * @param lhs constant iterator
                 * @param rhs constant iterator
                 * @return bool if they are equal
                 */
                friend bool operator == (const const_iterator& lhs, const const_iterator& rhs) 
                {

                    return (lhs.current_block_index == rhs.current_block_index) && (*lhs.current_block == *rhs.current_block);


                }

                /**
                 * != Operator
                 * @param lhs constant iterator
                 * @param rhs constant iterator
                 * @return bool if they are not equal
                 */
                friend bool operator != (const const_iterator& lhs, const const_iterator& rhs) {
                    return !(lhs == rhs);}

                // ----------
                // operator +
                // ----------

                /**
                 * + Operator
                 * @param lhs constant iterator
                 * @param rhs constant iterator
                 * @return const_iterator with new position after addition
                 */
                friend const_iterator operator + (const_iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * - Operator
                 * @param lhs constant iterator
                 * @param rhs constant iterator
                 * @return const_iterator with new position after subtraction
                 */
                friend const_iterator operator - (const_iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                pointer *current_block;
                std::size_t current_block_index;


            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    // <your code>
                    return true;}

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * constructor for iterator
                 * @param block the pointer a block in the deque
                 * @param index the index of the block
                 */
                const_iterator (pointer* x, std::size_t index) {
                    current_block = x;
                    index = current_block_index;
                    assert(valid());}

                // Default copy, destructor, and copy assignment.
                // const_iterator (const const_iterator&);
                // ~const_iterator ();
                // const_iterator& operator = (const const_iterator&);

                // ----------
                // operator *
                // ----------

                /**
                 * deference operator
                 * @return a reference to the current element the iterator at
                 */
                reference operator * () const {
                    
                    return *current_block[current_block_index];}

                // -----------
                // operator ->
                // -----------

                /**
                 * structure dereference operator
                 * @return the desired member of the current element the iterator at 
                 */
                pointer operator -> () const {
                    return &**this;}

                // -----------
                // operator ++
                // -----------

                /**
                 * increment operator (pre)
                 * @return const_reference of the iterator after incrementation
                 */
                const_iterator& operator ++ () {
                    //wrap around if at the end of current block
                    if(++current_block_index == block_size)
                    {
                        current_block_index = 0;
                        ++current_block;
                    }

                    assert(valid());
                    return const_cast<iterator&>(*this);
                }

                /**
                 * increment operator (post)
                 * @return const_reference of the iterator after incrementation
                 */
                const_iterator operator ++ (int) {
                    const_iterator x = *this;
                    ++(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator --
                // -----------

                /**
                 * decrement operator (pre)
                 * @return constant reference of the iterator after decrementation
                 */
                const_iterator& operator -- () {
                    //wrap around if at the end of current block
                    if(current_block_index-- == 0)
                    {
                        current_block_index = 9;
                        --current_block;
                    }
                    assert(valid());
                    return const_cast<iterator&>(*this);
                }

                /**
                 * decrement operator (post)
                 * @return constant reference of the iterator before decrementation
                 */
                const_iterator operator -- (int) {
                    const_iterator x = *this;
                    --(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator +=
                // -----------

                /**
                 * += operator
                 * @param d the number of incrememtation
                 * @return constant reference of the iterator after incrementation
                 */
                const_iterator& operator += (difference_type) {
                    //wrap around blocks
                    current_block += d / block_size;

                    int index_inc = d % block_size;
                    //warp around an additional block if necessary
                    if((current_block_index + index_inc) >= block_size)
                    {
                        current_block_index = (current_block_index + index_inc) % block_size;
                        ++current_block;
                    }
                    else
                    {
                        current_block_index += index_inc;
                    }

                    assert(valid());
                    return const_cast<iterator&>(*this);
                }

                // -----------
                // operator -=
                // -----------

                /**
                 * -= operator
                 * @param d the number of decrememtation
                 * @return constant reference of the iterator after decrementation
                 */
                const_iterator& operator -= (difference_type) {
                    //wrap around blocks
                    current_block -= d / block_size;

                    int index_dec = d % block_size;
                    //warp around an additional block if necessary
                    if((current_block_index - index_dec) > current_block_index)
                    {
                        current_block_index = block_size - (index_dec - current_block_index);
                        --current_block;
                    }
                    else
                    {
                        current_block_index -= index_dec;
                    }
                    assert(valid());
                    return const_cast<iterator&>(*this);}};

                // -----------
                // get_block_address
                // -----------

                /**
                 * return the address of the current block the iterator is at
                 */
                pointer* get_block_address () 
                {
                    return current_block;
                }

                // -----------
                // get_block_index
                // -----------

                /**
                 * return the address of the current block the iterator is at
                 */

                std::size_t get_block_index () 
                {
                    return current_block_index;
                }

                // -----------
                // set_block_address
                // -----------

                /**
                 * @param new_current_block the new block address
                 * return the address of the current block the iterator is at
                 */
                void set_block_address (pointer* new_current_block) 
                {
                    current_block = new_current_block;
                }
            };

    public:
        // ------------
        // constructors
        // ------------

        /**
         * @param a the allocator the deque used
         * default constructor or constructor that takes in an allocator
         */
        explicit MyDeque (const allocator_type& a = allocator_type()) : _a(a)
        {
            size = 0;

            //allocate outer array
            first_block = _a_outer.allocate(5);

            //allocate inner array
            for(int i = 0; i < 5; ++i)
            {
                *(first_block + i) = _a.allocate(block_size);
            }

            last_block = first_block + 5;
            begin = iterator(first_block + 2, 5);
            end = iterator(first_block + 2, 5);


            assert(valid());
        }

        /**
         * fill constructor that optionally takes in an allocator
         * @param s the number of elements
         */
        explicit MyDeque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type()) : _a(a)
        {
            size = s;
            std::size_t block_num = s / 10 + 1;

            //allocate outer array
            first_block = _a_outer.allocator(block_num);

            //allocate inner array
            for(int i = 0; i < block_num; ++i)
            {
                *(first_block + i) = _a.allocate(block_size);
            }

            last_block = first_block + block_num;

            begin = iterator(first_block + block_num / 2, 5);
            end = iterator(first_block + block_num / 2, 5);

            for(int i = 0; i < s / 2; ++i)
            {   

                _a.construct(&(*(--begin)), v);
                _a.construct(&*end, v);
                ++end;
            }


            //if size is odd, add a single element at the beginning
            if(s % 2 == 1)
            {
                _a.construct(&*begin, v);
            }
            else
            {
                ++begin;
            }

            assert(valid());
        }

        /**
         * copy constructor
         * @param that MyDeque to be get copied
         */
        MyDeque (const MyDeque& that) 
        {
            int block_num = this.last_block - this.first_block;
            size = that.size;

            //allocate memory
            first_block = _a_outer.allocate(block_num);
            last_block = first_block + block_num;
            for(int i = 0; i < (block_num); ++i)
            {
                (*first_block + i) = _a.allocate(block_size); 
            }

            //find the position of begin
            begin = iterator(first_block + (that.begin.get_block_address - that.first_block), that.begin.get_block_index());

            end = uninitialized_copy(_a, that.begin, that.end, begin);

            assert(valid());
        }

        // ----------
        // destructor
        // ----------

        /**
         * destructor
         */
        ~MyDeque () 
        {
            //destroy all elements
            destroy(_a, begin, end);
            //deallocate inner arrays
            for(pointer* current = first_block; first_block != end; ++current)
            {
                _a.deallocate(*current, block_size);
            }
            //dellocate outer arrays
            _a_outer.deallocate(first_block, last_block - first_block);

            assert(valid());
        }

        // ----------
        // operator =
        // ----------

        /**
         * = operator
         * @param that constant reference to that
         * @return the copy MyDeque
         */
        MyDeque& operator = (const MyDeque& that) 
        {
            if (this == &that) //same deque
            {
                return *this;
            }
            if(size >= that.size) //if there are enough space, no reallocation is needed
            {
                iterator new_begin = begin + (size / 2);
                iterator new_end = end - (size / 2);
                if(size % 2 == 1)
                {
                    --end;
                }

                //copy element from that
                std::size_t element_offset = that.size / 2;
                new_begin -= element_offset;
                new_end += element_offset;
                //get new_begin to the next position if size of that is even
                if(that.size % 2 == 1)
                {
                    ++new_end;
                }
                //destroy excess elements if size of the old deque is larger than that of the new one and copy the elements
                destroy(_a, begin, new_begin );
                destroy(_a, new_end, end);

                begin = new_begin;
                end = new_end;
                //copy elements 
                std::copy(that.begin, that.end, begin);
            }
            else //reallocation is needed
            {
                //destroy old elements and deallocate both inner the outer arrays
                destroy(_a, begin, end);
                for(pointer* current = first_block; first_block != end; ++current)
                {
                    _a.deallocate(*current, block_size);
                }
                _a_outer.deallocate(first_block, last_block - first_block);

                std::size_t block_num = (that.size / block_size + 1) * 2;

                //allocate outer array
                pointer* new_first_block = _a_outer.allocate(block_num);

                //allocate inner arrays
                for (pointer* current = new_first_block, int i = 0; i < block_size; ++i, ++current)
                {
                    *current = _a.allocate(block_size);
                }

                begin = iterator(new_first_block + ((that.size / block_size + 1) / 2), that.begin.get_block_index);

                end = uninitialized_copy(_a, that.begin, that.end, begin);

                first_block = new_first_block;
                last_block = new_first_block + block_num;
            }

            size = that.size;
            assert(valid());
            return *this;}

        // -----------
        // operator []
        // -----------

        /**
         * [] operator
         * @param index a copy of the value indexing
         * @return reference of the element accesed by the index
         */
        reference operator [] (size_type index) {

            return *(begin + index);}

        /**
         * [] constant_reference operator
         * @param index a copy of the value indexing
         * @return constant reference of the element accesed by the index
         */
        const_reference operator [] (size_type index) const {
            return const_cast<MyDeque*>(this)->operator[](index);}

        // --
        // at
        // --

        /**
         * at funciton
         * @param index a copy of the value indexing
         * @return reference of the element accesed at index
         * @throw out_of_range
         */
        reference at (size_type index) {
            if (index >= size || index < 0)
                throw std::out_of_range;

            return *(begin + index);}

        /**
         * at constant funciton
         * @param index a copy of the value indexing
         * @return constant reference of the element accesed at index
         * @throw out_of_range
         */
        const_reference at (size_type index) const {
            return const_cast<MyDeque*>(this)->at(index);}

        // ----
        // back
        // ----

        /**
         * back function
         * @return reference to back element in dequeu
         */
        reference back () {
            
            return *(end-1);}

        /**
         * back funciton
         * @return constant reference to back element in dequeu
         */
        const_reference back () const {
            return const_cast<MyDeque*>(this)->back();}

        // -----
        // begin
        // -----

        /**
         * begin function
         * @return iterator to the beginning of dequeu
         */
        iterator begin () {
            
            return begin;}

        /**
         * begin funciton
         * @return constant iterator to the beginning of dequeu
         */
        const_iterator begin () const {
           
            return const_iterator(begin.get_block_address(), begin.get_block_index());}

        // -----
        // clear
        // -----

        /**
         * clear function
         */
        void clear () {
            this->resize(0);
            assert(valid());}

        // -----
        // empty
        // -----

        /**
         * empty funciton 
         * @return bool true if empty, false if not
         */
        bool empty () const {
            return !size();}

        // ---
        // end
        // ---

        /**
         * end function
         * @return iterator to the end of deque
         */
        iterator end () {
            
            return end;}

        /**
         * end function
         * @return iterator to the end of deque
         */
        const_iterator end () const {
            
            return const_iterator(end.get_block_address(), end.get_block_index());}

        // -----
        // erase
        // -----

        /**
         * erase (remove an element)
         * @param it an iterator of the position of removal
         * @return an iterator follow the removal position
         */
        iterator erase (iterator it) 
        {
            //check if it is closer to the begin or closer to the end (and shift elements to the shorter side)
            if((it - begin) <= (end - it))
            {

            }
            else
            {
                
            }


            assert(valid());
            return iterator();}

        // -----
        // front
        // -----

        /**
         * front function
         * @return reference to the front element of the dequeu
         */
        reference front () {
            
            return *begin;}

        /**
         * front function
         * @return  constant reference to the front element of the dequeu
         */
        const_reference front () const {
            return const_cast<MyDeque*>(this)->front();}

        // ------
        // insert
        // ------

        /**
         * insert (remove an element)
         * @param it an iterator of the position of insertion
         * @param b an element to get insert
         * @return an iterator to the new element get inserted
         */
        iterator insert (iterator it, const_reference v) {



            assert(valid());
            return iterator();}

        // ---
        // pop
        // ---

        /**
         * pop_back function (remove element at the end)
         */
        void pop_back () 
        {
            _a.destroy(&*(--end));
            assert(valid());}

        /**
         * pop_front function (remove element at the front)
         */
        void pop_front () 
        {
            _a.destroy(&*(begin++));
            assert(valid());}

        // ----
        // push
        // ----

        /**
         * push_back function (add element to the end)
         * @param v the value to be pushed
         */
        void push_back (const_reference v) 
        {
            //reallocate if not enough memory at the end
            if((last_block - end.get_block_address) == 1 && end.get_block_index == 9)
            {
                resize(size + 1,v);
            }
            else
            {
                _a.construct(*end, v);
                ++end;
            }

            assert(valid());}

        /**
         * push_back function (add element to the front)
         * @param v the value to be pushed
         */
        void push_front (const_reference v) 
        {
            //reallocate if not enough memory at the front
            if((begin.get_block_address - first_block) == 0 && begin.get_block_index == 0)
            {
                std::size_t block_num = ((size + 1) / block_size + 1) * 2;
                pointer* new_first_block = _a_outer.allocate(block_num);
                begin.set_block_address(new_first_block + (((size + 1) / block_size + 1) / 2) );

                for(pointer* current = new_first_block + (((size + 1) / block_size + 1) / 2), pointer* old_begin_block = begin.get_block_address(),  *old_end_block = end.get_block_address(); 
                    old_first_block != old_end_block; ++current, ++old_first_block)
                {
                    *current = *old_first_block; 
                }
                end = begin + (size + 1);

                _a.construct(*(--begin), v);

            }
            else
            {
                _a.construct(*(--begin), v);
            }
            assert(valid());}

        // ------
        // resize
        // ------

        /**
         * @param s the size to be resized
         * @param v the element to get copied for the new space expanded
         * change the 
         */
        void resize (size_type s, const_reference v = value_type()) 
        {
            //destroy elements at the end if s is less than the current size
            if(s <= size)
            {
                std::size_t count = size - s;
                while(count-- != 0)
                {
                    _a.destroy(&(*end));
                    --end;
                }
            }
            /*
            //the capacity is still enough, destroy all elements and copy, but no reallocation is needed
            else if(((last_block - first_block) * block_size) >= s)
            {
                int new_first_block_offset = ((((last_block - first_block) * block_size) - s) / 2);
                new_begin = (first_block + new_first_block_offset, ((new_first_block_offset % block_size) == 0) ? 0 : (new_first_block_offset - 1));
                //assign construct
                int count = 0;
                for(iterator current = new_begin; current >= begin && current < end; ++current)
                {

                }

            }
            */
            //reallocation is not enough capacity
            else
            {
                std::size_t block_num = (s / block_size + 1) * 2;
                pointer* new_first_block = _a_outer.allocate(block_num);
                begin.set_block_address(new_first_block + ((s / block_size + 1) / 2));

                for(pointer* current = new_first_block + ((s / block_size + 1) / 2), pointer* old_begin_block = begin.get_block_address(),  *old_end_block = end.get_block_address(); 
                    old_first_block != old_end_block; ++current, ++old_first_block)
                {
                    *current = *old_first_block; 
                }
                end = begin + s;


                //append new elements
                std::size_t count = s - size;
                while(count-- != 0)
                {
                    this->push(v);
                }

                _a_outer.deallocate(first_block, last_block - first_block);
                first_block = new_first_block;
                last_block = new_first_block + block_num;
            }
            size = s;
            assert(valid());
        }

        // ----
        // size
        // ----

        /**
         * size function
         * @return size_type the size of the dequeu
         */
        size_type size () const {
            
            return size;}

        // ----
        // swap
        // ----

        /**
         * swap
         * @param that reference to MyDeque to be swapped
         */
        void swap (MyDeque& that) {

            pointer* temp_first = that.first_block;
            pointer* temp_last = that.last_block;
            iterator temp_begin = that.begin;
            iterator temp_end = that.end;

            that.first_block = first_block;
            that.last_block = last_block;
            that.begin = begin;
            that.end = end;

            first_block = temp_first;
            last_block = temp_last;
            begin = temp_begin;
            end = temp_end; 

            that.size ^= size;
            size ^= that.size;
            that.size ^= size;


            assert(valid());}
        };

        std::size_t Deque:: block_size = 10;

#endif // Deque_h