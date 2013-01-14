# ajdust this variables
	# there must be MYCLASSIFIER.cpp and MYCLASSIFIER.hpp in current directory
	MYCLASSIFIER=HWMatching
	#
	RELATIVE_PATH_TO_CLASSIFIER=../testing/
	# there must be MAIN.cpp in current directory
	MAIN=mainSample
	# path to Classifiers
	RELATIVE_PATH_TO_ADAPTERS=../Classifiers/
#######################


MC=$(MYCLASSIFIER)
RPTC=$(RELATIVE_PATH_TO_CLASSIFIER)
OPENCVLIBS=`pkg-config --libs opencv`
AP=$(RELATIVE_PATH_TO_ADAPTERS)

all: $(MAIN)

$(MAIN): $(MAIN).cpp $(MC).o Classifier.o $(AP)BayesAdapter.o $(AP)AbstractAdapter.o $(AP)SVMAdapter.o
		g++ $(MAIN).cpp $(MC).o Classifier.o $(AP)BayesAdapter.o $(AP)SVMAdapter.o $(AP)AbstractAdapter.o -o $(MAIN) ${OPENCVLIBS}


Classifier.o: $(RPTC)Classifier.cpp $(RPTC)Classifier.hpp
		g++ -c $(RPTC)Classifier.cpp -o Classifier.o ${OPENCVLIBS}


$(MC).o: $(MC).cpp $(MC).hpp
		g++ -c $(MC).cpp ${OPENCVLIBS}

$(MC).hpp: $(RPTC)Classifier.hpp

BayesAdapter.o: $(AP)BayesAdapter.cpp $(AP)BayesAdapter.hpp
		g++ -c $(AP)BayesAdapter.cpp ${OPENCVLIBS}

SVMAdapter.o: $(AP)SVMAdapter.cpp $(AP)SVMAdapter.hpp
		g++ -c $(AP)SVMAdapter.cpp ${OPENCVLIBS}

AbstractAdapter.o: $(AP)AbstractAdapter.cpp $(AP)AbstractAdapter.hpp
		g++ -c $(AP)AbstractAdapter.cpp ${OPENCVLIBS}

BayesAdapter.hpp: $(AP)AbstractAdapter.hpp

SVMAdapter.hpp: $(AP)AbstractAdapter.hpp


clean:
		rm *.o
