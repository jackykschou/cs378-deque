#include <algorithm> // copy, equal, lexicographical_compare, max, swap
#include <cassert>   // assert
#include <iterator>  // iterator, bidirectional_iterator_tag
#include <memory>    // allocator
#include <stdexcept> // out_of_range
#include <utility>   // !=, <=, >, >=

#include "Deque.h"

using namespace std;

int main()
{	
	MyDeque<int> x(10);

	/*
	MyDeque<int> y;

	
	for(int i = 0; i < 10; ++i)
	{
		y.push_back(1);
	}
	
	cout << "***********************************************" << endl;

	for_each(y.begin(), y.end(), [](int elem)
	{
		cout << elem << " ";
	});

	cout << y.size()  << endl;
	*/

	cout << endl;
}