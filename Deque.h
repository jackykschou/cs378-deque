// ----------------------
// projects/deque/Deque.h
// Copyright (C) 2013
// Glenn P. Downing
// ----------------------

#ifndef Deque_
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

#include <iostream>

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
    return e;
}


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
            return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
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
            return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
        }



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
                 * +operator for iterator and number
                 * @param lhs an iterator
                 * @param rhs integral type to change the position of the iterator
                 * @return lhs + rhs
                 */
                friend iterator operator + (iterator lhs, difference_type rhs) {
                    return lhs += rhs;
                }

                // ----------
                // operator -
                // ----------

                /**
                 * -operator
                 * @param lhs an iterator
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

                    return (current_block_index >= 0) && current_block_index <= (block_size - 1);
                }


            public:
                // -----------
                // constructor
                // -----------

                /**
                 * default constructor
                 */
                iterator () 
                {
                    current_block  = static_cast<pointer*>(0);
                    current_block_index = 0;
                    assert(valid());
                }

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
                    return (*current_block)[current_block_index];
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
                    //wrap around if at the end_iterator of current block
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
                    //wrap around if at the end_iterator of current block
                    if(current_block_index-- == 0)
                    {
                        current_block_index = (block_size - 1);
                        --current_block;
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
                pointer* get_block_address () const
                {
                    return current_block;
                }

                // -----------
                // get_block_index
                // -----------

                /**
                 * return the address of the current block the iterator is at
                 */

                std::size_t get_block_index () const
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

                pointer* current_block;
                std::size_t current_block_index;


            private:
                // -----
                // valid
                // -----

                bool valid () const 
                {
                    return (current_block_index >= 0) && current_block_index <= (block_size - 1);
                }

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * default constructor
                 */
                const_iterator () 
                {
                    current_block  = static_cast<const_pointer>(0);
                    current_block_index = 0;
                    assert(valid());
                }

                /**
                 * constructor for iterator
                 * @param block the pointer a block in the deque
                 * @param index the index of the block
                 */
                const_iterator (pointer* x, std::size_t index) {
                    current_block = x;
                    index = current_block_index;
                    assert(valid());}

                /**
                 * constructor talking a non constant iterator
                 */
                const_iterator(iterator it) 
                {
                    current_block = const_cast<pointer*>(&(*it.get_block_address()));
                    current_block_index = it.get_block_index();
                    assert(valid());
                }

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
                    //wrap around if at the end_iterator of current block
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
                    //wrap around if at the end_iterator of current block
                    if(current_block_index-- == 0)
                    {
                        current_block_index = (block_size - 1);
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
                const_iterator& operator += (difference_type d) {
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
                const_iterator& operator -= (difference_type d) {
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
                    return const_cast<iterator&>(*this);}


                // -----------
                // get_block_address
                // -----------

                /**
                 * return the address of the current block the iterator is at
                 */
                pointer* get_block_address () const
                {
                    return current_block;
                }

                // -----------
                // get_block_index
                // -----------

                /**
                 * return the address of the current block the iterator is at
                 */

                std::size_t get_block_index () const
                {
                    return current_block_index;
                }

            };

            private:
            // ----
            // data
            // ----

            allocator_type _a;
            typename allocator_type::template rebind<pointer>::other _a_outer;
            pointer* first_block;
            pointer* last_block;
            iterator begin_iterator; 
            iterator end_iterator;
            size_type size_num;
            static std::size_t block_size;

             private:
            // -----
            // valid
            // -----

            bool valid () const 
            {   

                // pointer* temp_first = first_block;
                // pointer* temp_last = last_block;

                // std::cout << "Deque :" << std::endl;
                // while(temp_first != temp_last)
                // {
                //     for (int i = 0 ; i < 10; i++){
                //         std::cout << (*temp_first)[i] << " ";
                        
                //     }
                //     std::cout << std::endl;
                //     ++temp_first;
                // }


                return (begin_iterator.get_block_address() <= end_iterator.get_block_address()) && (size_num >= 0) && (first_block <= last_block);
            }


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
            size_num = 0;

            //allocate outer array
            first_block = _a_outer.allocate(5);

            //allocate inner array
            for(int i = 0; i < 5; ++i)
            {
                *(first_block + i) = _a.allocate(block_size);
            }

            last_block = first_block + 5;
            begin_iterator = iterator(first_block + 2, 5);
            end_iterator = iterator(first_block + 2, 5);

        }

        void printd()
        {
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "block difference: " << last_block - first_block << " iterator difference: " << std::distance(begin_iterator, end_iterator) << " size: " << size_num << std::endl;
            std::cout << "number of block begin far from first_block: " << begin_iterator.get_block_address() - first_block << std::endl;
            std::cout << "number of block filled with elements: " << end_iterator.get_block_address() -  begin_iterator.get_block_address() << std::endl;
            std::cout << "number of empty blocks at the end: " << last_block - end_iterator.get_block_address() << std::endl;
            std::cout << "begin index: " << begin_iterator.get_block_index() << "  end index: " << end_iterator.get_block_index() << std::endl;
            std::for_each(begin_iterator, end_iterator, [](value_type elem)
            {
                std::cout << elem << ", ";
            });
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
        }

        /**
         * fill constructor that optionally takes in an allocator
         * @param s the number of elements
         */
        explicit MyDeque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type()) : _a(a)
        {
            size_num = s;
            std::size_t block_num = s / 10 + 1;

            //allocate outer array
            first_block = _a_outer.allocate(block_num);

            //allocate inner array
            for(std::size_t i = 0; i < block_num; ++i)
            {
                *(first_block + i) = _a.allocate(block_size);
            }

            last_block = first_block + block_num;

            begin_iterator = iterator(first_block + block_num / 2, 5);
            end_iterator = iterator(first_block + block_num / 2, 5);

            for(std::size_t i = 0; i < s / 2; ++i)
            {   

                _a.construct(&(*(--begin_iterator)), v);
                _a.construct(&*end_iterator, v);
                ++end_iterator;
            }


            //if size is odd, add a single element at the beginning
            if(s % 2 == 1)
            {
                _a.construct(&*begin_iterator, v);
            }
            else
            {
                ++begin_iterator;
            }

            assert(valid());
        }

        /**
         * copy constructor
         * @param that MyDeque to be get copied
         */
        MyDeque (const MyDeque& that) 
        {
            int block_num = that.last_block - that.first_block;
            size_num = that.size_num;

            //allocate memory
            first_block = _a_outer.allocate(block_num);
            last_block = first_block + block_num;
            for(int i = 0; i < (block_num); ++i)
            {
                *(first_block + i) = _a.allocate(block_size); 
            }

            //find the position of begin
            begin_iterator = iterator(first_block + (that.begin_iterator.get_block_address() - that.first_block), that.begin_iterator.get_block_index());

            end_iterator = uninitialized_copy(_a, that.begin_iterator, that.end_iterator, begin_iterator);
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
            destroy(_a, begin_iterator, end_iterator);
            std::cout << "start delete" << std::endl;

            //deallocate inner arrays
            for(pointer* current = first_block; current != last_block; ++current)
            {
                _a.deallocate(*current, block_size);
            }
            //dellocate outer arrays
            _a_outer.deallocate(first_block, last_block - first_block);

            //assert(valid());
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
            if(size_num >= that.size_num) //if there are enough space, no reallocation is needed
            {
                iterator new_begin = begin_iterator + (size_num / 2);
                iterator new_end = end_iterator - (size_num / 2);
                if(size_num % 2 == 1)
                {
                    --end_iterator;
                }

                //copy element from that
                std::size_t element_offset = that.size_num / 2;
                new_begin -= element_offset;
                new_end += element_offset;
                //get new_begin to the next position if size of that is even
                if(that.size_num % 2 == 1)
                {
                    ++new_end;
                }
                //destroy excess elements if size of the old deque is larger than that of the new one and copy the elements
                destroy(_a, begin_iterator, new_begin );
                destroy(_a, new_end, end_iterator);

                begin_iterator = new_begin;
                end_iterator = new_end;

                //copy elements
                iterator that_current = that.begin_iterator;
                iterator current = begin_iterator;
                while(that_current != that.end_iterator)
                {
                    *current = *that_current;
                    ++that_current;
                    ++current;
                }
            }
            else //reallocation is needed
            {
                //destroy old elements and deallocate both inner the outer arrays
                destroy(_a, begin_iterator, end_iterator);
                for(pointer* current = first_block; first_block != last_block; ++current)
                {
                    _a.deallocate(*current, block_size);
                }
                _a_outer.deallocate(first_block, last_block - first_block);

                std::size_t block_num = (that.size_num / block_size + 1) * 2;

                //allocate outer array
                pointer* new_first_block = _a_outer.allocate(block_num);
                pointer* current = new_first_block;

                //allocate inner arrays
                for (std::size_t i = 0; i < block_size; ++i, ++current)
                {
                    *current = _a.allocate(block_size);
                }

                begin_iterator = iterator(new_first_block + ((that.size_num / block_size + 1) / 2), that.begin_iterator.get_block_index());

                end_iterator = uninitialized_copy(_a, that.begin_iterator, that.end_iterator, begin_iterator);

                first_block = new_first_block;
                last_block = new_first_block + block_num;
            }

            size_num = that.size_num;
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

            return *(begin_iterator + index);}

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
            if (index >= size_num || index < 0)
                throw std::out_of_range("invalid index");

            return *(begin_iterator + index);}

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
            
            return *(end_iterator - 1);}

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
         * begin_iterator function
         * @return iterator to the beginning of dequeu
         */
        iterator begin () {
            
            return begin_iterator; }

        /**
         * begin_iterator funciton
         * @return constant iterator to the beginning of dequeu
         */
        const_iterator begin () const {
           
            return const_iterator(begin_iterator);
        }

        // -----
        // clear
        // -----

        /**
         * clear function
         */
        void clear () {
            resize(0);
            assert(valid());}

        // -----
        // empty
        // -----

        /**
         * empty funciton 
         * @return bool true if empty, false if not
         */
        bool empty () const {
            return !size_num;}

        // ---
        // end
        // ---

        /**
         * end_iterator function
         * @return iterator to the end_iterator of deque
         */
        iterator end () {
            
            return end_iterator;}

        /**
         * end_iterator function
         * @return iterator to the end_iterator of deque
         */
        const_iterator end () const {
            
            return const_iterator(end_iterator);}

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
            iterator result = it + 1;
            //check if it is closer to the begin_iterator or closer to the end_iterator (and shift elements to the shorter side)
            if(distance(it, begin_iterator) <= distance(end_iterator, it))
            {
                while(it != begin_iterator)
                {
                    *it = *(it - 1);
                    --it;
                }
                _a.destroy(&*it);
                ++begin_iterator;
            }
            else
            {
                while(it != (end_iterator - 1))
                {
                    *it = *(it + 1);
                    ++it;
                }
                _a.destroy(&*it);
                --end_iterator;
            }

            assert(valid());
            return result;
        }

        // -----
        // front
        // -----

        /**
         * front function
         * @return reference to the front element of the dequeu
         */
        reference front () {
            
            return *begin_iterator;}

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
        iterator insert (iterator it, const_reference v) 
        {

            iterator result;
            //if the dequeu is already full, reisze it
            if((size_num + 1) > ((last_block - first_block) * block_size))
            {
                std::size_t it_offset = std::distance(it, begin_iterator);  //keep track of the insertion position
                resize(size_num + 1);
                it = begin_iterator + it_offset;
                iterator temp = end_iterator - 2;

                while(temp != (it - 1))
                {
                    *(temp + 1) = *temp;
                    --temp;
                }
                *(it) = v;
                result = it;
            }

            //check if there is no space on the front, if so, construct new element at the end
            else if(begin_iterator.get_block_address() == first_block && begin_iterator.get_block_index() == 0)
            {
                _a.construct(&*end_iterator, value_type());
                ++end_iterator;
                iterator temp = end_iterator - 2;

                while(temp != (it - 1))
                {
                    *(temp + 1) = *temp;
                    --temp;
                }
                *it = v;
                result = it;

            }
            //check if there is no space on the back, if so, construct new element at the begin
            else if(end_iterator.get_block_address() == last_block && end_iterator.get_block_index() == (block_size - 1))
            {
                --begin_iterator;
                _a.construct(&*begin_iterator, value_type());
                iterator temp = begin_iterator + 1;

                while(temp != (it + 1))
                {
                    *(temp - 1) = *temp;
                    ++temp;
                }
                *it = v;
                result = it;
            }
            //else pick the shorter distance to shift
            else if (std::distance(it, begin_iterator) <= std::distance(end_iterator, it))
            {
                --begin_iterator;
                _a.construct(&*begin_iterator, value_type());
                iterator temp = begin_iterator + 1;

                while(temp != (it + 1))
                {
                    *(temp - 1) = *temp;
                    ++temp;
                }
                *it = v;
                result = it;
            }
            else
            {
                _a.construct(&*end_iterator, value_type());
                ++end_iterator;
                iterator temp = end_iterator - 2;

                while(temp != (it - 1))
                {
                    *(temp + 1) = *temp;
                    --temp;
                }
                *it = v;
                result = it;
            }

            assert(valid());
            return result;
        }

        // ---
        // pop
        // ---

        /**
         * pop_back function (remove element at the end_iterator)
         */
        void pop_back () 
        {
            _a.destroy(&*(--end_iterator));
            --size_num;
            assert(valid());

        }

        /**
         * pop_front function (remove element at the front)
         */
        void pop_front () 
        {
            _a.destroy(&*(begin_iterator++));
            --size_num;
            assert(valid());}

        // ----
        // push
        // ----

        /**
         * push_back function (add element to the end_iterator)
         * @param v the value to be pushed
         */
        void push_back (const_reference v) 
        {
            //std::cout << v << " get pushed" << std::endl;
            //reallocate if not enough memory at the end
            if((last_block == end_iterator.get_block_address()) && end_iterator.get_block_index() == 0)
            {   
                resize(size_num + 1,v);
            }
            else
            {
                _a.construct(&*end_iterator, v);
                ++end_iterator;
                ++size_num;
            }
            
            assert(valid());}

        /**
         * push_back function (add element to the front)
         * @param v the value to be pushed
         */
        void push_front (const_reference v) 
        {   
            //reallocate if not enough memory at the front
            if((begin_iterator.get_block_address() - first_block) == 0 && begin_iterator.get_block_index() == 0)
            {
                std::size_t block_num = ((size_num + 1) / block_size + 1) * 2;
                pointer* new_first_block = _a_outer.allocate(block_num);
                begin_iterator.set_block_address(new_first_block + (((size_num + 1) / block_size + 1) / 2) );

                pointer* current = new_first_block + (((size_num + 1) / block_size + 1) / 2);
                pointer* old_begin_block = begin_iterator.get_block_address();
                pointer* old_end_block = end_iterator.get_block_address();
                for(; old_begin_block != old_end_block; ++current, ++old_begin_block)
                {
                    *current = *old_begin_block; 
                }
                end_iterator = begin_iterator + (size_num + 1);

                _a.construct(&*(--begin_iterator), v);

            }
            else
            {
                _a.construct(&*(--begin_iterator), v);
            }
            ++size_num;
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

            //std::cout << "Before Resize  **************" << std::endl;

            //printd();

            //destroy elements at the end_iterator if s is less than the current size
            if(s <= size_num)
            {   
                std::size_t count = size_num - s;
                while(count-- != 0)
                {
                    _a.destroy(&(*end_iterator));
                    --end_iterator;
                }
                size_num = s;
            }
            //reallocation is not enough capacity
            else
            {


                std::size_t block_num = (s / block_size + 1) * 2;
                pointer* new_first_block = _a_outer.allocate(block_num);
                pointer* old_begin_block = begin_iterator.get_block_address();
                pointer* old_end_block = end_iterator.get_block_address();





                pointer* temp_first = first_block;
                pointer* temp_last = last_block;
                //deallocate the unused inner arrays
                while(temp_first != begin_iterator.get_block_address())
                {
                 _a.deallocate(*temp_first, block_size);
                 ++temp_first;
                }
                //deallocate the unsude inner arrays
                while(end_iterator.get_block_address() != temp_last){
                    --temp_last;
                    _a.deallocate(*temp_last, block_size);
                    
                }


                begin_iterator.set_block_address(new_first_block + ((s / block_size + 1) / 2));
                pointer* current =  new_first_block;



                //deallocate the unsude inner arrays
                while(end_iterator.get_block_address() != temp_last){
                    --temp_last;
                    _a.deallocate(*temp_last, block_size);
                    
                }


                pointer* old_first_block = first_block;
                pointer* old_last_block = last_block;

                first_block = new_first_block;
                last_block = new_first_block + block_num;

                //allocate new inner array at the front
                while(current != begin_iterator.get_block_address())
                {
                    *current = _a.allocate(block_size);
                    ++current;
                }

                //copy old inner array
                while(old_begin_block != old_end_block)
                {   
                    *current = *old_begin_block; 
                    ++current;
                    ++old_begin_block;
                }
                
                //allocate new inner array at the end
                while(current != last_block)
                {   
                    *current = _a.allocate(block_size);
                    ++current;
                }
                
                end_iterator = begin_iterator + size_num;

                //append new elements
                std::size_t count = s - size_num;


                while(count-- != 0)
                {   //std::cout << "constructing : " << v << std::endl;
                    _a.construct(&*end_iterator, v);
                    ++end_iterator;
                }
                size_num = s;
                _a_outer.deallocate(old_first_block, old_last_block - old_first_block);
            }
            
            //std::cout << "After Resize**************" << std::endl;
            //printd();
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
            
            return size_num;}

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
            iterator temp_begin = that.begin_iterator;
            iterator temp_end = that.end_iterator;

            that.first_block = first_block;
            that.last_block = last_block;
            that.begin_iterator = begin_iterator; 
            that.end_iterator = end_iterator;

            first_block = temp_first;
            last_block = temp_last;
            begin_iterator = temp_begin;
            end_iterator = temp_end; 

            that.size_num ^= size_num;
            size_num ^= that.size_num;
            that.size_num ^= size_num;


            assert(valid());}
        };




        template<typename T, typename A>
        typename std::size_t MyDeque<T, A>::block_size = 10;

#endif // Deque_h