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
	MyDeque<int> x;
	
	for(int i = 0; i <100 ; ++i){
		x.push_back(i);
	}

	
	cout << "end loop" << endl;
}