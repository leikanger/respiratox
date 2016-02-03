CC=clang++
CFLAGS=-std=c++11 -Wall -lpthread
LIBR=-lboost_system -lboost_thread

all: test

# Neste lager og kjører testen!
test: test_without_running
	./testAll.out

test_without_running: serialInterface.o serialInterfaceTEST.cpp
	$(CC) $(CFLAGS) $(LIBR) -lboost_unit_test_framework serialInterfaceTEST.cpp serialInterface.o -o testAll.out

serialInterface.o: serialInterface.cpp serialInterface.h
	$(CC) $(CFLAGS) $(LIBR) serialInterface.cpp -c



test_old_all_files: *cpp *h
	$(CC) $(CFLAGS) -lboost_unit_test_framework $(LIBR) serialInterfaceTEST.cpp serialInterface.cpp -o testAll.out 

clean:
	rm *.out *.o

#serial: serialInterface.cpp serialInterface.h
#	clang++ serialInterface.cpp
