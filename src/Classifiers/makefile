OPENCVLIBS=`pkg-config --libs opencv`

all: test

test: test.cpp BayesAdapter.o SVMAdapter.o AbstractAdapter.o KNNAdapter.o
		g++ -o test test.cpp BayesAdapter.o SVMAdapter.o KNNAdapter.o AbstractAdapter.o ${OPENCVLIBS}

BayesAdapter.o: BayesAdapter.cpp BayesAdapter.hpp
		g++ -c BayesAdapter.cpp ${OPENCVLIBS}

SVMAdapter.o: SVMAdapter.cpp SVMAdapter.hpp
		g++ -c SVMAdapter.cpp ${OPENCVLIBS}

KNNAdapter.o: KNNAdapter.cpp KNNAdapter.hpp
		g++ -c KNNAdapter.cpp ${OPENCVLIBS}

AbstractAdapter.o: AbstractAdapter.cpp AbstractAdapter.hpp
		g++ -c AbstractAdapter.cpp ${OPENCVLIBS}

BayesAdapter.hpp: AbstractAdapter.hpp

SVMAdapter.hpp: AbstractAdapter.hpp

KNNAdapter.hpp: AbstractAdapter.hpp

clean:
		rm *.o
