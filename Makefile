test: *cpp *h
	g++ -std=c++11 -lboost_unit_test_framework -lboost_filesystem serialInterfaceTEST.cpp serialInterface.cpp -o testAll.out && ./testAll.out

#serial: serialInterface.cpp serialInterface.h
#	clang++ serialInterface.cpp
