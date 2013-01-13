OPENCVLIBS=`pkg-config --libs opencv`

all: test

test: test.cpp BayesAdapter.o SVMAdapter.o
		g++ -o test test.cpp BayesAdapter.o SVMAdapter.o ${OPENCVLIBS}

BayesAdapter.o: BayesAdapter.cpp BayesAdapter.hpp
		g++ -c BayesAdapter.cpp ${OPENCVLIBS}

SVMAdapter.o: SVMAdapter.cpp SVMAdapter.hpp
		g++ -c SVMAdapter.cpp ${OPENCVLIBS}

clean:
		rm *.o
