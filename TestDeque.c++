// ----------------------------
// projects/deque/TestDeque.c++
// CopyrighContainer (C) 2013
// Glenn P. Downing
// ----------------------------

/*
To Test the program:
    % ls /usr/include/cppunit/
    ...
    TestFixture.h
    ...
    % locate libcppunit.a
    /usr/lib/libcppunit.a
    % g++ -pedantic -std=c++0x -Wall Deque.c++ TestDeque.c++ -o TestDeque -lcppuniContainer -ldl
    % valgrind TestDeque > TestDeque.out
*/

// --------
// includes
// --------

#include <algorithm> // equal
#include <cstring>   // strcmp, NULL
#include <deque>     // deque
#include <sstream>   // ostringstream
#include <stdexcept> // invalid_argument
#include <string>    // ==
#include <cstdlib>   //rand
#include <climits>   //INT_MAX
#include <iostream>

#include "gtest/gtest.h" //g test

#include "Deque.h"

using namespace std;

// ---------
// TestDeque
// ---------

template <typename T>
class TypeTest : public testing::Test 
{


    protected:

    public:

        typedef T Container;

        Container empty;
        Container non_full;
        Container full_of_1;
        Container full_of_0;
        Container random_packed;
        Container random_loose;


        virtual void SetUp() 
        {
            srand (time(NULL));

            for(int i = 1; i <= 50; ++i)
            {
                non_full.push_back(i);
            }

            for(int i = 1; i <= 100; ++i)
            {
                full_of_0.push_back(0);
            }

            for(int i = 1; i <= 100; ++i)
            {
                full_of_1.push_back(1);
            }

            for(int i = 100; i != 0; --i)
            {
                random_packed.push_back(rand() % INT_MAX + 1);
            }
            
            this->random_loose.resize(50);
            for(int i = 50; i != 0; --i)
            {
                random_loose[rand() % 49] = rand() % INT_MAX;
            }
            
        }
};

typedef ::testing::Types<MyDeque<int> > MyTypes;

TYPED_TEST_CASE(TypeTest, MyTypes);

TYPED_TEST(TypeTest, TEST_DEFAULT_CONSTRUCTOR) 
{
    ASSERT_TRUE(this->empty.empty());
    ASSERT_TRUE(this->empty.size() == 0);
}

TYPED_TEST(TypeTest, TEST_COPY_CONSTRUCTOR_1) 
{
    typename TestFixture::Container x(this->random_loose);
    ASSERT_TRUE(equal(this->random_loose.begin(), this->random_loose.end(), x.begin()));
    ASSERT_TRUE(this->random_loose == x);
}

TYPED_TEST(TypeTest, TEST_COPY_CONSTRUCTOR_2) 
{
    typename TestFixture::Container x(this->random_packed);
    ASSERT_TRUE(equal(this->random_packed.begin(), this->random_packed.end(), x.begin()));
    ASSERT_TRUE(this->random_packed == x);
}

TYPED_TEST(TypeTest, TEST_COPY_EQUAL_ASSIGN_1) 
{
    typename TestFixture::Container x = this->random_loose;
    ASSERT_TRUE(equal(this->random_loose.begin(), this->random_loose.end(), x.begin()));
    ASSERT_TRUE(this->random_loose == x);
}

TYPED_TEST(TypeTest, TEST_COPY_EQUAL_ASSIGN_2) 
{
    typename TestFixture::Container x = this->random_packed;
    ASSERT_TRUE(equal(this->random_packed.begin(), this->random_packed.end(), x.begin()));
    ASSERT_TRUE(this->random_packed == x);
}

TYPED_TEST(TypeTest, TEST_SIZE_ONLY_CONSTRUCTOR_1) 
{
    typename TestFixture::Container x(0);
    ASSERT_TRUE(x.empty());
    ASSERT_TRUE(x.size() == 0);
}

TYPED_TEST(TypeTest, TEST_SIZE_ONLY_CONSTRUCTOR_2) 
{
    typename TestFixture::Container x(100);
    ASSERT_FALSE(x.empty());
    ASSERT_TRUE(x.size() == 100);
    ASSERT_TRUE(equal(this->full_of_0.begin(), this->full_of_0.end(), x.begin()));
    ASSERT_TRUE(this->full_of_0 == x);
}

TYPED_TEST(TypeTest, TEST_FILL_CONSTRUCTOR_1) 
{
    typename TestFixture::Container x(0, 8);
    ASSERT_TRUE(x.empty());
    ASSERT_TRUE(x.size() == 0);
}

TYPED_TEST(TypeTest, TEST_FILL_CONSTRUCTOR_2) 
{
    typename TestFixture::Container x(100, 1);
    ASSERT_FALSE(x.empty());
    ASSERT_TRUE(x.size() == 100);
    ASSERT_TRUE(equal(this->full_of_1.begin(), this->full_of_1.end(), x.begin()));
    ASSERT_TRUE(this->full_of_1 == x);
    
}

TYPED_TEST(TypeTest, TEST_FILL_CONSTRUCTOR_3) 
{
    typename TestFixture::Container x(100, 0);
    ASSERT_FALSE(x.empty());
    ASSERT_TRUE(x.size() == 100);
    ASSERT_TRUE(equal(this->full_of_0.begin(), this->full_of_0.end(), x.begin()));
    ASSERT_TRUE(this->full_of_0 == x);
}

TYPED_TEST(TypeTest, TEST_SIZE_1) 
{
    ASSERT_TRUE(this->non_full.size() == 50);
}

TYPED_TEST(TypeTest, TEST_SIZE_2) 
{
    ASSERT_TRUE(this->full_of_1.size() == 100);
}


TYPED_TEST(TypeTest, TEST_SIZE_3) 
{
    ASSERT_TRUE(this->empty.size() == 0);
}

TYPED_TEST(TypeTest, TEST_EMPTY_1) 
{
    ASSERT_TRUE(this->empty.empty());
}


TYPED_TEST(TypeTest, TEST_EMPTY_2) 
{
    ASSERT_FALSE(this->non_full.empty());
}


TYPED_TEST(TypeTest, TEST_EMPTY_3) 
{
    ASSERT_FALSE(this->full_of_0.empty());
}


TYPED_TEST(TypeTest, TEST_EQUALS_1) 
{
    typename TestFixture::Container x = this->empty;
    ASSERT_TRUE(x == this->empty);
}


TYPED_TEST(TypeTest, TEST_EQUALS_2) 
{
    typename TestFixture::Container x(100, 5);
    ASSERT_FALSE(x == this->full_of_1);
}


TYPED_TEST(TypeTest, TEST_EQUALS_3) 
{
    typename TestFixture::Container x(100, 1);
    ASSERT_TRUE(x == this->full_of_1);
}

TYPED_TEST(TypeTest, TEST_NOT_EQUALS_1) 
{
    typename TestFixture::Container x = this->empty;
    ASSERT_FALSE(x != this->empty);
}


TYPED_TEST(TypeTest, TEST_NOT_EQUALS_2) 
{
    typename TestFixture::Container x(100, 5);
    ASSERT_TRUE(x != this->full_of_1);
}


TYPED_TEST(TypeTest, TEST_NOT_EQUALS_3) 
{
    typename TestFixture::Container x(100, 1);
    ASSERT_FALSE(x != this->full_of_1);
}


TYPED_TEST(TypeTest, TEST_LESS_THAN_1) 
{
    typename TestFixture::Container x(10, 1);
    ASSERT_FALSE(x < this->empty);
}


TYPED_TEST(TypeTest, TEST_LESS_THAN_2) 
{
    typename TestFixture::Container x(10, 1);
    ASSERT_FALSE(x < this->full_of_0);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_3) 
{
    typename TestFixture::Container x(10, 1);
    ASSERT_TRUE(x < this->full_of_1);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_4) 
{
    typename TestFixture::Container x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(99);
    typename TestFixture::Container y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_TRUE(x < y);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_5) 
{
    typename TestFixture::Container x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(100);
    typename TestFixture::Container y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_FALSE(x < y);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_1) 
{
    typename TestFixture::Container x(10, 1);
    ASSERT_TRUE(x > this->empty);
}


TYPED_TEST(TypeTest, TEST_MORE_THAN_2) 
{
    typename TestFixture::Container x(10, 1);
    ASSERT_TRUE(x > this->full_of_0);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_3) 
{
    typename TestFixture::Container x(10, 1);
    ASSERT_FALSE(x > this->full_of_1);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_4) 
{
    typename TestFixture::Container x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(99);
    typename TestFixture::Container y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_FALSE(x > y);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_5) 
{
    typename TestFixture::Container x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(100);
    typename TestFixture::Container y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_FALSE(x > y);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_EQUAL_1) 
{
    typename TestFixture::Container x(10, 1);
    ASSERT_FALSE(x <= this->empty);
}


TYPED_TEST(TypeTest, TEST_LESS_THAN_EQUAL_2) 
{
    typename TestFixture::Container x(10, 1);
    ASSERT_FALSE(x <= this->full_of_0);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_EQUAL_3) 
{
    typename TestFixture::Container x(10, 1);
    ASSERT_TRUE(x <= this->full_of_1);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_EQUAL_4) 
{
    typename TestFixture::Container x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(99);
    typename TestFixture::Container y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_TRUE(x <= y);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_EQUAL_5) 
{
    typename TestFixture::Container x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(100);
    typename TestFixture::Container y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_TRUE(x <= y);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_EQUAL_1) 
{
    typename TestFixture::Container x(10, 1);
    ASSERT_TRUE(x >= this->empty);
}


TYPED_TEST(TypeTest, TEST_MORE_THAN_EQUAL_2) 
{
    typename TestFixture::Container x(10, 1);
    ASSERT_TRUE(x >= this->full_of_0);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_EQUAL_3) 
{
    typename TestFixture::Container x(10, 1);
    ASSERT_FALSE(x >= this->full_of_1);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_EQUAL_4) 
{
    typename TestFixture::Container x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(99);
    typename TestFixture::Container y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_FALSE(x >= y);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_EQUAL_5) 
{
    typename TestFixture::Container x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(100);
    typename TestFixture::Container y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_TRUE(x >= y);
}

TYPED_TEST(TypeTest, TEST_INDEX_OP_1) 
{
    ASSERT_TRUE(this->full_of_1[20] == 1);
    ASSERT_TRUE(this->full_of_1[30] == 1);
    ASSERT_TRUE(this->full_of_1[40] == 1);
}

TYPED_TEST(TypeTest, TEST_INDEX_OP_2) 
{
    ASSERT_TRUE(this->full_of_0[20] == 0);
    ASSERT_TRUE(this->full_of_0[30] == 0);
    ASSERT_TRUE(this->full_of_0[40] == 0);
}

TYPED_TEST(TypeTest, TEST_INDEX_OP_3) 
{
    this->empty.push_back(9999);
    ASSERT_TRUE(this->empty[0] == 9999);
}

TYPED_TEST(TypeTest, TEST_INDEX_OP_4) 
{
    this->full_of_0[10] = 100;
    ASSERT_TRUE(this->full_of_0[10] == 100);
}

TYPED_TEST(TypeTest, TEST_AT_1) 
{
    ASSERT_TRUE(this->full_of_1.at(20) == 1);
    ASSERT_TRUE(this->full_of_1.at(30) == 1);
    ASSERT_TRUE(this->full_of_1.at(40) == 1);
}

TYPED_TEST(TypeTest, TEST_AT_2) 
{
    ASSERT_TRUE(this->full_of_0.at(20)  == 0);
    ASSERT_TRUE(this->full_of_0.at(30) == 0);
    ASSERT_TRUE(this->full_of_0.at(40) == 0);
}

TYPED_TEST(TypeTest, TEST_AT_3) 
{
    this->empty.push_back(9999);
    ASSERT_TRUE(this->empty.at(0) == 9999);
}

TYPED_TEST(TypeTest, TEST_AT_4) 
{
    this->full_of_0.at(10) = 100;
    ASSERT_TRUE(this->full_of_0.at(10) == 100);
}

TYPED_TEST(TypeTest, TEST_AT_5) 
{
    ASSERT_THROW(this->empty.at(99), out_of_range);
}

TYPED_TEST(TypeTest, TEST_FRONT_1) 
{
    ASSERT_TRUE(this->full_of_0.front() == 0);
}

TYPED_TEST(TypeTest, TEST_FRONT_2) 
{
    ASSERT_TRUE(this->full_of_1.front() == 1);
}

TYPED_TEST(TypeTest, TEST_FRONT_3) 
{
    this->empty.push_back(99);
    this->empty.push_back(-99);
    ASSERT_TRUE(this->empty.front() == 99);
}

TYPED_TEST(TypeTest, TEST_BACK_1) 
{
    ASSERT_TRUE(this->full_of_0.back() == 0);
}

TYPED_TEST(TypeTest, TEST_BACK_2) 
{
    ASSERT_TRUE(this->full_of_1.back() == 1);
}

TYPED_TEST(TypeTest, TEST_BACK_3) 
{
    this->empty.push_back(99);
    this->empty.push_back(-99);
    ASSERT_TRUE(this->empty.back() == -99);
}

TYPED_TEST(TypeTest, TEST_BEGIN_1) 
{
    ASSERT_TRUE(*this->full_of_1.begin() == 1);
}

TYPED_TEST(TypeTest, TEST_BEGIN_2) 
{
    ASSERT_TRUE(*this->full_of_0.begin() == 0);
}

TYPED_TEST(TypeTest, TEST_BEGIN_3) 
{
    this->empty.push_back(99);
    this->empty.push_back(199);
    ASSERT_TRUE(*this->empty.begin() == 99);
}

TYPED_TEST(TypeTest, TEST_END_1) 
{
    ASSERT_TRUE(*(this->full_of_1.end() - 1) == 1);
}

TYPED_TEST(TypeTest, TEST_END_2) 
{
    ASSERT_TRUE(*(this->full_of_0.end() - 1) == 0);
}

TYPED_TEST(TypeTest, TEST_END_3) 
{
    this->empty.push_back(99);
    this->empty.push_back(199);
    ASSERT_TRUE(*(this->empty.end() - 1) == 199);
}

TYPED_TEST(TypeTest, TEST_EQUAL_ASSIGN_1) 
{
    typename TestFixture::Container x;
    x = this->random_loose;
    ASSERT_TRUE(equal(this->random_loose.begin(), this->random_loose.end(), x.begin()));
    ASSERT_TRUE(this->random_loose == x);
}

TYPED_TEST(TypeTest, TEST_EQUAL_ASSIGN_2) 
{
    typename TestFixture::Container x;
    x = this->random_packed;
    ASSERT_TRUE(equal(this->random_packed.begin(), this->random_packed.end(), x.begin()));
    ASSERT_TRUE(this->random_packed == x);
}

TYPED_TEST(TypeTest, TEST_SWAP_1) 
{
    typename TestFixture::Container full_of_1_copy(this->full_of_1);
    typename TestFixture::Container full_of_0_copy(this->full_of_0);
    this->full_of_0.swap(this->full_of_1);
    ASSERT_TRUE(this->full_of_1 == full_of_0_copy);
    ASSERT_TRUE(this->full_of_0 == full_of_1_copy);
}

TYPED_TEST(TypeTest, TEST_SWAP_2) 
{
    typename TestFixture::Container full_of_1_copy(this->full_of_1);
    typename TestFixture::Container empty_copy(this->empty);
    this->full_of_1.swap(this->empty);
    ASSERT_TRUE(this->full_of_1 == empty_copy);
    ASSERT_TRUE(this->full_of_1.empty());
    ASSERT_TRUE(this->empty == full_of_1_copy);
}

TYPED_TEST(TypeTest, TEST_SWAP_3) 
{
    typename TestFixture::Container x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    typename TestFixture::Container y;
    y.push_back(8);
    y.push_back(9);
    y.push_back(10);

    x.swap(y);

    ASSERT_TRUE(x[0] == 8);
    ASSERT_TRUE(x[1] == 9);
    ASSERT_TRUE(x[2] == 10);
    ASSERT_TRUE(y[0] == 1);
    ASSERT_TRUE(y[1] == 2);
    ASSERT_TRUE(y[2] == 3);
}

TYPED_TEST(TypeTest, TEST_PUSH_BACK_1) 
{
    typename TestFixture::Container x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    typename TestFixture::Container y;
    y.push_back(8);
    y.push_back(9);
    y.push_back(10);

    ASSERT_TRUE(y[0] == 8);
    ASSERT_TRUE(y[1] == 9);
    ASSERT_TRUE(y[2] == 10);
    ASSERT_TRUE(x[0] == 1);
    ASSERT_TRUE(x[1] == 2);
    ASSERT_TRUE(x[2] == 3);
}

TYPED_TEST(TypeTest, TEST_POP_BACK_1) 
{
    typename TestFixture::Container x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    typename TestFixture::Container y;
    y.push_back(8);
    y.push_back(9);
    y.push_back(10);

    x.pop_back();
    y.pop_back();

    ASSERT_TRUE(x.back() == 2);
    ASSERT_TRUE(y.back() == 9);
}

TYPED_TEST(TypeTest, TEST_POP_BACK_2) 
{
    while(this->full_of_1.size() != 0)
    {
        this->full_of_1.pop_back();
    }
    ASSERT_TRUE(this->full_of_1.empty());
}

TYPED_TEST(TypeTest, TEST_POP_BACK_3) 
{
    while(this->full_of_0.size() != 0)
    {
        this->full_of_0.pop_back();
    }
    ASSERT_TRUE(this->full_of_0.empty());
}

TYPED_TEST(TypeTest, TEST_PUSH_FRONT_1) 
{
    typename TestFixture::Container x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    typename TestFixture::Container y;
    y.push_front(8);
    y.push_front(9);
    y.push_front(10);

    ASSERT_TRUE(y[0] == 10);
    ASSERT_TRUE(y[1] == 9);
    ASSERT_TRUE(y[2] == 8);
    ASSERT_TRUE(x[0] == 3);
    ASSERT_TRUE(x[1] == 2);
    ASSERT_TRUE(x[2] == 1);
}

TYPED_TEST(TypeTest, TEST_POP_FRONT_1) 
{
    typename TestFixture::Container x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    typename TestFixture::Container y;
    y.push_front(8);
    y.push_front(9);
    y.push_front(10);

    x.pop_front();
    y.pop_front();

    ASSERT_TRUE(x.front() == 2);
    ASSERT_TRUE(y.front() == 9);

    x.pop_front();
    y.pop_front();

    ASSERT_TRUE(x.front() == 1);
    ASSERT_TRUE(y.front() == 8);
}

TYPED_TEST(TypeTest, TEST_POP_FRONT_2) 
{
    while(this->full_of_1.size() != 0)
    {
        this->full_of_1.pop_front();
    }
    ASSERT_TRUE(this->full_of_1.empty());
}

TYPED_TEST(TypeTest, TEST_POP_FRONT_3) 
{
    while(this->full_of_0.size() != 0)
    {
        this->full_of_0.pop_front();
    }
    ASSERT_TRUE(this->full_of_0.empty());
}

TYPED_TEST(TypeTest, TEST_INSERT_1) 
{
    typename TestFixture::Container x;
    for(int i = 0; i < 100; ++i)
    {
        x.insert(x.begin(), 1);
    }
    ASSERT_TRUE(x == this->full_of_1);
}

TYPED_TEST(TypeTest, TEST_INSERT_2) 
{
    typename TestFixture::Container x;
    for(int i = 0; i < 100; ++i)
    {
        x.insert(x.begin(), 0);
    }
    ASSERT_TRUE(x == this->full_of_0);
}

TYPED_TEST(TypeTest, TEST_INSERT_3) 
{
    ASSERT_TRUE(*this->full_of_0.insert(next(this->full_of_0.begin(), 1), 10) == 10);
    ASSERT_TRUE(this->full_of_0[1] == 10);

    ASSERT_TRUE(*this->full_of_0.insert(next(this->full_of_0.begin(), 33), 99) == 99);
    ASSERT_TRUE(this->full_of_0[33] == 99);

    ASSERT_TRUE(*this->full_of_0.insert(this->full_of_0.end(), 88) == 88);
    ASSERT_TRUE(this->full_of_0.back() == 88);
}

TYPED_TEST(TypeTest, TEST_ERASE_1) 
{
    for(int i = 0; i < 100; ++i)
    {
        this->full_of_0.erase(this->full_of_0.begin());
    }
    ASSERT_TRUE(this->full_of_0.empty());
}

TYPED_TEST(TypeTest, TEST_ERASE_2) 
{
    for(int i = 0; i < 100; ++i)
    {
        this->full_of_1.erase(this->full_of_1.begin());
    }
    ASSERT_TRUE(this->full_of_1.empty());
}

TYPED_TEST(TypeTest, TEST_ERASE_3) 
{
    typename TestFixture::Container x;
    x.push_back(999);
    x.push_back(888);
    x.push_back(777);
    x.push_back(666);
    x.push_back(555);
    x.push_back(444);

    ASSERT_TRUE(*(x.erase(x.begin())) == 888);
    ASSERT_TRUE(x.front() == 888);
    ASSERT_TRUE(*(x.erase(x.begin())) == 777);
    ASSERT_TRUE(x.front() == 777);
    ASSERT_TRUE(*(x.erase(x.end() - 2)) == 444);
}

TYPED_TEST(TypeTest, TEST_RESIZE_1) 
{
    this->empty.resize(100);
    ASSERT_TRUE(this->empty.size() == 100);
    for(int i = 0; i < 100; ++i)
    {
        ASSERT_TRUE(this->empty[i] == 0);
    }
}

TYPED_TEST(TypeTest, TEST_RESIZE_2) 
{
    this->empty.resize(100, 5);
    ASSERT_TRUE(this->empty.size() == 100);
    for(int i = 0; i < 100; ++i)
    {
        ASSERT_TRUE(this->empty[i] == 5);
    }
}

TYPED_TEST(TypeTest, TEST_RESIZE_3) 
{
    this->full_of_1.resize(0);
    ASSERT_TRUE(this->full_of_1.empty());
}


TYPED_TEST(TypeTest, TEST_clear_1) 
{
    this->full_of_1.clear();
    ASSERT_TRUE(this->full_of_1.empty());
}

TYPED_TEST(TypeTest, TEST_clear_2) 
{
    this->full_of_0.clear();
    ASSERT_TRUE(this->full_of_0.empty());
}


TYPED_TEST(TypeTest, TEST_clear_3) 
{
    this->empty.push_back(100);
    this->empty.push_front(121);
    this->empty.clear();
    ASSERT_TRUE(this->empty.empty());
}

