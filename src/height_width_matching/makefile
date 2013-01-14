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

$(MAIN): $(MAIN).cpp $(MC).o Classifier.o BayesAdapter.o AbstractAdapter.o SVMAdapter.o KNNAdapter.o RandomForestAdapter.o
		g++ $(MAIN).cpp $(MC).o Classifier.o BayesAdapter.o SVMAdapter.o KNNAdapter.o RandomForestAdapter.o AbstractAdapter.o -o $(MAIN) ${OPENCVLIBS}


Classifier.o: $(RPTC)Classifier.cpp $(RPTC)Classifier.hpp
		g++ -c $(RPTC)Classifier.cpp -o Classifier.o ${OPENCVLIBS}


$(MC).o: $(MC).cpp $(MC).hpp
		g++ -c $(MC).cpp ${OPENCVLIBS}

$(MC).hpp: $(RPTC)Classifier.hpp

BayesAdapter.o: $(AP)BayesAdapter.cpp $(AP)BayesAdapter.hpp
		g++ -c $(AP)BayesAdapter.cpp -o BayesAdapter.o ${OPENCVLIBS}

SVMAdapter.o: $(AP)SVMAdapter.cpp $(AP)SVMAdapter.hpp
		g++ -c $(AP)SVMAdapter.cpp -o SVMAdapter.o ${OPENCVLIBS}

KNNAdapter.o: $(AP)KNNAdapter.cpp $(AP)KNNAdapter.hpp
		g++ -c $(AP)KNNAdapter.cpp -o KNNAdapter.o ${OPENCVLIBS}

RandomForestAdapter.o: $(AP)RandomForestAdapter.cpp $(AP)RandomForestAdapter.hpp
		g++ -c $(AP)RandomForestAdapter.cpp -o RandomForestAdapter.o ${OPENCVLIBS}

AbstractAdapter.o: $(AP)AbstractAdapter.cpp $(AP)AbstractAdapter.hpp
		g++ -c $(AP)AbstractAdapter.cpp -o AbstractAdapter.o ${OPENCVLIBS}

$(AP)BayesAdapter.hpp: $(AP)AbstractAdapter.hpp

$(AP)SVMAdapter.hpp: $(AP)AbstractAdapter.hpp

$(AP)KNNAdapter.hpp: $(AP)AbstractAdapter.hpp

clean:
		rm *.o
