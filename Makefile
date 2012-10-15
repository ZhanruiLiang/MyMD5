CCFLAG=-DDEBUG -g
CC=g++
md5sum: md5.o md5sum.o
	$(CC) $(CCFLAG) md5.o md5sum.o -o md5sum
md5: md5.o md5_test.o
	$(CC) $(CCFLAG) md5.o md5_test.o -o md5
md5_test.o: md5.hpp md5_test.cpp
	$(CC) $(CCFLAG) -c md5_test.cpp 
md5.o: md5.cpp md5.hpp
	$(CC) $(CCFLAG) -c md5.cpp 
