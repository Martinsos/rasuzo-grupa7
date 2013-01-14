OPENCVLIBS=`pkg-config --libs opencv`
OBJFILES=BayesAdapter.o SVMAdapter.o AbstractAdapter.o KNNAdapter.o RandomForestAdapter.o

all: test

test: test.cpp ${OBJFILES}
		g++ -o test test.cpp ${OBJFILES} ${OPENCVLIBS}

BayesAdapter.o: BayesAdapter.cpp BayesAdapter.hpp
		g++ -c BayesAdapter.cpp ${OPENCVLIBS}

SVMAdapter.o: SVMAdapter.cpp SVMAdapter.hpp
		g++ -c SVMAdapter.cpp ${OPENCVLIBS}

KNNAdapter.o: KNNAdapter.cpp KNNAdapter.hpp
		g++ -c KNNAdapter.cpp ${OPENCVLIBS}

RandomForestAdapter.o: RandomForestAdapter.cpp RandomForestAdapter.hpp
		g++ -c RandomForestAdapter.cpp ${OPENCVLIBS}

AbstractAdapter.o: AbstractAdapter.cpp AbstractAdapter.hpp
		g++ -c AbstractAdapter.cpp ${OPENCVLIBS}

BayesAdapter.hpp: AbstractAdapter.hpp

SVMAdapter.hpp: AbstractAdapter.hpp

KNNAdapter.hpp: AbstractAdapter.hpp

RandomForestAdapter.hpp: AbstractAdapter.hpp

clean:
		rm *.o
