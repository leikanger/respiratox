CC=clang++
CFLAGS=-std=c++11 -Wall 
BOOST_LIBR=-lboost_system

all: test_without_running

main: main.cpp serialInterface.o
	$(CC) $(CFLAGS) $(BOOST_LIBR) main.cpp serialInterface.o -o main.out





# Neste lager og kjører testen!
test: test_without_running
	@echo "\n\n" ; echo "_______________________ Run unit test compilation _______________________" ; echo "";
	./testAll.out
	@echo "" ; echo "_______________________ End unit test compilation _______________________" ; echo "";

test_without_running: serialInterfaceTEST.cpp serialInterface.o serialBOOST.o 
	$(CC) $(CFLAGS) -lboost_unit_test_framework -lpthread $(BOOST_LIBR) serialInterfaceTEST.cpp serialInterface.o serialBOOST.o -o testAll.out

serialInterface.o: serialInterface.cpp serialInterface.h
	$(CC) $(CFLAGS) serialInterface.cpp -c

serialBOOST.o: serialBOOST.cpp serialBOOST.h
	$(CC) $(CFLAGS) $(BOOST_LIBR) serialBOOST.cpp -c



test_old_all_files: *cpp *h
	$(CC) $(CFLAGS) -lboost_unit_test_framework $(BOOST_LIBR) serialInterfaceTEST.cpp serialInterface.cpp serialBOOST.cpp -o testAll.out 

resetclear:
	rm *.out *.o

clean:
	make resetclear ; make 
