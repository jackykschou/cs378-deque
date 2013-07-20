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
         * <your documentation>
         */
        friend bool operator == (const MyDeque& lhs, const MyDeque& rhs) {
            // <your code>
            // you must use std::equal()
            return true;
        }

        // ----------
        // operator <
        // ----------

        /**
         * <your documentation>
         */
        friend bool operator < (const MyDeque& lhs, const MyDeque& rhs) {
            // <your code>
            // you must use std::lexicographical_compare()
            return true;}

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
                 * <your documentation>
                 */
                friend bool operator == (const iterator& lhs, const iterator& rhs) 
                {
                    return (lhs.current_block_index == rhs.current_block_index) && (*lhs.current_block == *rhs.current_block);
                }

                /**
                 * <your documentation>
                 */
                friend bool operator != (const iterator& lhs, const iterator& rhs) {
                    return !(lhs == rhs);}

                // ----------
                // operator +
                // ----------

                /**
                 * <your documentation>
                 */
                friend iterator operator + (iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * <your documentation>
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
                // operator &
                // -----------

                /**
                 * & operator
                 * return the address of the current element the iterator is at
                 */
                pointer operator & () 
                {
                    return (*current_block) + current_block_index;
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
                // operator &
                // -----------

                /**
                 * & operator
                 * return the address of the current element the iterator is at
                 */
                pointer operator & () 
                {
                    return (*current_block) + current_block_index;
                }

    public:
        // ------------
        // constructors
        // ------------

        /**
         * @param a the allocator the deque used
         * default constructor or constructor that takes in an allocator
         */
        explicit MyDeque (const allocator_type& a = allocator_type()) 
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
        explicit MyDeque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type()) 
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

            /*
            std::size_t construct_begin_index = block_size / 2 - 1;
            pointer* construct_begin_block = first_block + block_num / 2;
            std::size_t construct_end_index = block_size / 2;
            pointer* construct_end_block = first_block + block_num / 2;
            */

            begin = iterator(first_block + block_num / 2, 4);
            end = iterator(first_block + block_num / 2, 5);

            for(int i = 0; i < s / 2; ++i)
            {   

                _a.construct(&begin, v);
                _a.construct(&end, v);
                --begin;
                ++end;

                /*
                //wrap around if at the end of current block
                if(construct_begin_index + i - 1 > construct_begin_index)
                {
                    construct_begin_index = 9;
                    --*construct_begin_block;
                }
                if(construct_end_index - i + 1 == 10)
                {
                    construct_end_index = 0;
                    ++*construct_end_block;
                }
                

                _a.construct(*(construct_begin_block) + construct_begin_index, v);
                _a.construct(*(construct_end_block) + construct_end_index, v);
                --begin;
                ++end;

                --construct_begin_index;
                ++construct_end_index;
                */
            }


            //if size is odd, add a single element at the beginning
            if(s % 2 == 1)
            {
                _a.construct(&begin, v);

                /*
                if(construct_begin_index + i - 1 > construct_begin_index)
                {
                    construct_begin_index = 9;
                    --*construct_begin_block;
                }
                _a.construct(*(construct_begin_block) + construct_begin_index, v);
                --begin;
                */
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
            pointer* begin_block = first_block + (block_num / 2);
            std::size_t block_offset = distance_one_side / block_size;
            //get to the correct block
            while(block_offset-- != 0)
            {
                --begin_block;
            }
            std::size_t index_offset = (size % block_size) / 2;

            //if there is only one block, start from the begin
            if(block_num == 1)
            {
                begin = iterator(begin_block, (index_offset == 0) ? (0) : ((block_size / 2) - index_offset));
            }
            else //start from the end
            {
                begin = iterator(begin_block, (index_offset == 0) ? (9) : (block_size - index_offset));
            }
            //if size is odd, decrement begin by one
            if(size % 2 == 1)
            {
                --begin;
            }

            end = std::copy(that.begin, that.end, begin);

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
            for(iterator current = begin; current != end; ++current)
            {
                _a.destroy(&current);
            }
            for(pointer* current = first_block; first_block != end; ++current)
            {
                _a.deallocate(*current, block_size);
            }
            _a_outer.deallocate(first_block, last_block - first_block);

            assert(valid());
        }

        // ----------
        // operator =
        // ----------

        /**
         * = operator
         * @param rhs constant reference to rhs
         * @return the copy MyDeque
         */
        MyDeque& operator = (const MyDeque& rhs) 
        {
            if(size >= rhs.size) //if there are enough space, no reallocation is needed
            {
                iterator new_begin = begin + (size / 2);
                iterator new_end = end - (size / 2);
                if(size % 2 == 1)
                {
                    --end;
                }

                //copy element from rhs
                std::size_t element_offset = rhs.size / 2;
                new_begin -= element_offset;
                new_end += element_offset;
                //get new_begin to the next position if size of rhs is even
                if(rhs.size % 2 == 1)
                {
                    ++new_end;
                }
                //destroy excess elements if size of the old deque is larger than that of the new one and copy the elements
                for(iterator current = begin; current != new_begin; ++current)
                {
                    _a.destroy(&current);
                }
                for(iterator current = new_end; current != end; ++current)
                {
                    _a.destroy(&current);
                }
                begin = new_begin;
                end = new_end;
                copy(rhs.begin, rhs.end, begin);
            }
            else
            {
                for(iterator current = begin; current != end; ++current)
                {
                    _a.destroy(&current);
                }
                for(pointer* current = first_block; first_block != end; ++current)
                {
                    _a.deallocate(*current, block_size);
                }
                _a_outer.deallocate(first_block, last_block - first_block);
            }

            size = rhs.size;
            assert(valid());
            return *this;}

        // -----------
        // operator []
        // -----------

        /**
         * <your documentation>
         */
        reference operator [] (size_type index) {
            // <your code>
            // dummy is just to be able to compile the skeleton, remove it
            static value_type dummy;
            return dummy;}

        /**
         * <your documentation>
         */
        const_reference operator [] (size_type index) const {
            return const_cast<MyDeque*>(this)->operator[](index);}

        // --
        // at
        // --

        /**
         * <your documentation>
         */
        reference at (size_type index) {
            // <your code>
            // dummy is just to be able to compile the skeleton, remove it
            static value_type dummy;
            return dummy;}

        /**
         * <your documentation>
         */
        const_reference at (size_type index) const {
            return const_cast<MyDeque*>(this)->at(index);}

        // ----
        // back
        // ----

        /**
         * <your documentation>
         */
        reference back () {
            // <your code>
            // dummy is just to be able to compile the skeleton, remove it
            static value_type dummy;
            return dummy;}

        /**
         * <your documentation>
         */
        const_reference back () const {
            return const_cast<MyDeque*>(this)->back();}

        // -----
        // begin
        // -----

        /**
         * <your documentation>
         */
        iterator begin () {
            // <your code>
            return iterator(/* <your arguments> */);}

        /**
         * <your documentation>
         */
        const_iterator begin () const {
            // <your code>
            return const_iterator(/* <your arguments> */);}

        // -----
        // clear
        // -----

        /**
         * <your documentation>
         */
        void clear () {
            // <your code>
            assert(valid());}

        // -----
        // empty
        // -----

        /**
         * <your documentation>
         */
        bool empty () const {
            return !size();}

        // ---
        // end
        // ---

        /**
         * <your documentation>
         */
        iterator end () {
            // <your code>
            return iterator(/* <your arguments> */);}

        /**
         * <your documentation>
         */
        const_iterator end () const {
            // <your code>
            return const_iterator(/* <your arguments> */);}

        // -----
        // erase
        // -----

        /**
         * <your documentation>
         */
        iterator erase (iterator) {
            // <your code>
            assert(valid());
            return iterator();}

        // -----
        // front
        // -----

        /**
         * <your documentation>
         */
        reference front () {
            // <your code>
            // dummy is just to be able to compile the skeleton, remove it
            static value_type dummy;
            return dummy;}

        /**
         * <your documentation>
         */
        const_reference front () const {
            return const_cast<MyDeque*>(this)->front();}

        // ------
        // insert
        // ------

        /**
         * <your documentation>
         */
        iterator insert (iterator, const_reference) {
            // <your code>
            assert(valid());
            return iterator();}

        // ---
        // pop
        // ---

        /**
         * <your documentation>
         */
        void pop_back () {
            // <your code>
            assert(valid());}

        /**
         * <your documentation>
         */
        void pop_front () {
            // <your code>
            assert(valid());}

        // ----
        // push
        // ----

        /**
         * <your documentation>
         */
        void push_back (const_reference) {
            // <your code>
            assert(valid());}

        /**
         * <your documentation>
         */
        void push_front (const_reference) {
            // <your code>
            assert(valid());}

        // ------
        // resize
        // ------

        /**
         * @param s the size to be resized
         * @param v the element to get copied for the new space expanded
         */
        void resize (size_type s, const_reference v = value_type()) 
        {
            
            assert(valid());
        }

        // ----
        // size
        // ----

        /**
         * <your documentation>
         */
        size_type size () const {
            // <your code>
            return 0;}

        // ----
        // swap
        // ----

        /**
         * <your documentation>
         */
        void swap (MyDeque&) {
            // <your code>
            assert(valid());}
        };

        std::size_t Deque:: block_size = 10;

#endif // Deque_h