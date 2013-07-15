// ----------------------------
// projects/deque/TestDeque.c++
// Copyright (C) 2013
// Glenn P. Downing
// ----------------------------

/*
To test the program:
    % ls /usr/include/cppunit/
    ...
    TestFixture.h
    ...
    % locate libcppunit.a
    /usr/lib/libcppunit.a
    % g++ -pedantic -std=c++0x -Wall Deque.c++ TestDeque.c++ -o TestDeque -lcppunit -ldl
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

        T empty;
        T non_full;
        T full_of_1;
        T full_of_0;
        T random_packed;
        T random_loose;


        virtual void SetUp() 
        {
            srand (time(NULL));

            non_full.resize(100);
            for(int i = 1; i <= 50; ++i)
            {
                non_full.push_back(i);
            }

            full_of_0.resize(100);
            for(int i = 1; i <= 100)
            {
                full_of_0.push_back(0);
            }

            full_of_1.resize(100);
            for(int i = 1; i <= 100)
            {
                full_of_1.push_back(1);
            }

            for(int i = 100; i != 0; --i)
            {
                random_packed.push_back(rand() % INT_MAX + 1);
            }

            random2_loose.resize(50);
            for(int i = 50; i != 0; --i)
            {
                random_loose.insert(rand() % 50, rand() % INT_MAX + 1);
            }
            
        }
};

typedef testing::MyDeque<int> MY_Q;
typedef testing::deque<int> REAL_Q;

TYPED_TEST(TypeTest, TEST_DEFAULT_CONSTRUCTOR) 
{
    ASSERT_TRUE(empty.empty());
    ASSERT_TRUE(empty.size() == 0);
}

TYPED_TEST(TypeTest, TEST_COPY_CONSTRUCTOR_1) 
{
    T x(random_loose);
    ASSERT_TRUE(equal(random_loose.cbegin(), random_loose.cend(), x));
    ASSERT_TRUE(random2_loose == x);
}

TYPED_TEST(TypeTest, TEST_COPY_CONSTRUCTOR_2) 
{
    T x(random_packed);
    ASSERT_TRUE(equal(random_packed.cbegin(), random_packed.cend(), x));
    ASSERT_TRUE(random_packed == x);
}

TYPED_TEST(TypeTest, TEST_COPY_EQUAL_ASSIGN_1) 
{
    T x = random_loose;
    ASSERT_TRUE(equal(random_loose.cbegin(), random_loose.cend(), x));
    ASSERT_TRUE(random_loose == x);
}

TYPED_TEST(TypeTest, TEST_COPY_EQUAL_ASSIGN_2) 
{
    T x = random_packed;
    ASSERT_TRUE(equal(random_packed.cbegin(), random_packed.cend(), x));
    ASSERT_TRUE(random_packed == x);
}

TYPED_TEST(TypeTest, TEST_SIZE_ONLY_CONSTRUCTOR_1) 
{
    T x(0);
    ASSERT_TRUE(x.empty());
    ASSERT_TRUE(x.size() == 0);
}

TYPED_TEST(TypeTest, TEST_SIZE_ONLY_CONSTRUCTOR_2) 
{
    T x(100);
    ASSERT_FALSE(x.empty());
    ASSERT_TRUE(x.size() == 100);

    ASSERT_TRUE(equal(full_of_0.cbegin(), full_of_0.cend(), x));
    ASSERT_TRUE(full_of_0 == x);
}

TYPED_TEST(TypeTest, TEST_FILL_CONSTRUCTOR_1) 
{
    T x(0, 8);
    ASSERT_TRUE(x.empty());
    ASSERT_TRUE(x.size() == 0);
}

TYPED_TEST(TypeTest, TEST_FILL_CONSTRUCTOR_2) 
{
    T x(100, 1);
    ASSERT_FALSE(x.empty());
    ASSERT_TRUE(x.size() == 100);
    ASSERT_TRUE(equal(full_of_1.cbegin(), full_of_1.cend(), x));
    ASSERT_TRUE(full_of_1 == x);
    
}

TYPED_TEST(TypeTest, TEST_FILL_CONSTRUCTOR_3) 
{
    T x(100, 0);
    ASSERT_FALSE(x.empty());
    ASSERT_TRUE(x.size() == 100);
    ASSERT_TRUE(equal(full_of_0.cbegin(), full_of_0.cend(), x));
    ASSERT_TRUE(full_of_0 == x);
}

TYPED_TEST(TypeTest, TEST_SIZE_1) 
{
    ASSERT_TRUE(non_full.size() == 50);
}

TYPED_TEST(TypeTest, TEST_SIZE_2) 
{
    ASSERT_TRUE(full_of_1.size() == 100);
}


TYPED_TEST(TypeTest, TEST_SIZE_3) 
{
    ASSERT_TRUE(empty.size() == 0);
}

TYPED_TEST(TypeTest, TEST_EMPTY_1) 
{
    ASSERT_TRUE(empty.empty());
}


TYPED_TEST(TypeTest, TEST_EMPTY_2) 
{
    ASSERT_FALSE(non_full.empty());
}


TYPED_TEST(TypeTest, TEST_EMPTY_3) 
{
    ASSERT_FALSE(full_of_0.empty());
}


TYPED_TEST(TypeTest, TEST_EQUALS_1) 
{
    T x = empty;
    ASSERT_TRUE(x == empty);
}


TYPED_TEST(TypeTest, TEST_EQUALS_2) 
{
    T x(100, 5);
    ASSERT_FALSE(x == full_of_1);
}


TYPED_TEST(TypeTest, TEST_EQUALS_3) 
{
    T x(100, 1);
    ASSERT_TRUE(x == full_of_1);
}

TYPED_TEST(TypeTest, TEST_NOT_EQUALS_1) 
{
    T x = empty;
    ASSERT_FALSE(x != empty);
}


TYPED_TEST(TypeTest, TEST_NOT_EQUALS_2) 
{
    T x(100, 5);
    ASSERT_TRUE(x != full_of_1);
}


TYPED_TEST(TypeTest, TEST_NOT_EQUALS_3) 
{
    T x(100, 1);
    ASSERT_FALSE(x != full_of_1);
}


TYPED_TEST(TypeTest, TEST_LESS_THAN_1) 
{
    T x(10, 1);
    ASSERT_FALSE(x < empty);
}


TYPED_TEST(TypeTest, TEST_LESS_THAN_2) 
{
    T x(10, 1);
    ASSERT_FALSE(x < full_of_0);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_3) 
{
    T x(10, 1);
    ASSERT_TRUE(x < full_of_1);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_4) 
{
    T x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(99);
    T y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_TRUE(x < y);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_5) 
{
    T x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(100);
    T y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_FALSE(x < y);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_1) 
{
    T x(10, 1);
    ASSERT_TRUE(x > empty);
}


TYPED_TEST(TypeTest, TEST_MORE_THAN_2) 
{
    T x(10, 1);
    ASSERT_TRUE(x > full_of_0);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_3) 
{
    T x(10, 1);
    ASSERT_FALSE(x > full_of_1);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_4) 
{
    T x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(99);
    T y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_FALSE(x > y);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_5) 
{
    T x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(100);
    T y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_FALSE(x > y);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_EQUAL_1) 
{
    T x(10, 1);
    ASSERT_FALSE(x <= empty);
}


TYPED_TEST(TypeTest, TEST_LESS_THAN_EQUAL_2) 
{
    T x(10, 1);
    ASSERT_FALSE(x <= full_of_0);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_EQUAL_3) 
{
    T x(10, 1);
    ASSERT_TRUE(x <= full_of_1);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_EQUAL_4) 
{
    T x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(99);
    T y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_TRUE(x <= y);
}

TYPED_TEST(TypeTest, TEST_LESS_THAN_EQUAL_5) 
{
    T x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(100);
    T y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_TRUE(x <= y);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_EQUAL_1) 
{
    T x(10, 1);
    ASSERT_TRUE(x >= empty);
}


TYPED_TEST(TypeTest, TEST_MORE_THAN_EQUAL_2) 
{
    T x(10, 1);
    ASSERT_TRUE(x >= full_of_0);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_EQUAL_3) 
{
    T x(10, 1);
    ASSERT_FALSE(x >= full_of_1);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_EQUAL_4) 
{
    T x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(99);
    T y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_FALSE(x >= y);
}

TYPED_TEST(TypeTest, TEST_MORE_THAN_EQUAL_5) 
{
    T x;
    x.push_back(50);
    x.push_back(15);
    x.push_back(40);
    x.push_back(1);
    x.push_back(100);
    T y;
    y.push_back(50);
    y.push_back(15);
    y.push_back(40);
    y.push_back(1);
    y.push_back(100);
    
    ASSERT_TRUE(x >= y);
}
