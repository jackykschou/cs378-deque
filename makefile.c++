all:
	make Deque.zip

clean:
	rm -f Deque.log
	rm -f Deque.zip
	rm -f TestDeque

doc: Deque.h
	doxygen Doxyfile

turnin-list:
	turnin --list dlessin cs378pj4

turnin-submit: Deque.zip
	turnin --submit dlessin cs378pj4 Deque.zip

turnin-verify:
	turnin --verify dlessin cs378pj4

Deque.log:
	git log > Deque.log

Deque.zip: Deque.h Deque.log TestDeque.c++ TestDeque.out
	zip -r Deque.zip html/ Deque.h Deque.log TestDeque.c++ TestDeque.out

TestDeque: Deque.h TestDeque.c++
	g++ -pedantic -std=c++0x -Wall TestDeque.c++ -o TestDeque -lcppunit -ldl

TestDeque.out: TestDeque
	valgrind TestDeque > TestDeque.out
