RELATIVE_PATH_TO_CLASSIFIER=../testing/
RELATIVE_PATH_TO_ADAPTERS=../Classifiers/
RELATIVE_PATH_TO_SILHOUETTE_EXTRACTION=../silhouette_extraction/

RPTC=$(RELATIVE_PATH_TO_CLASSIFIER)
RPTSE=$(RELATIVE_PATH_TO_SILHOUETTE_EXTRACTION)
OPENCVLIBS=`pkg-config --libs opencv`
AP=$(RELATIVE_PATH_TO_ADAPTERS)
OBJFILES=Granlund.o HWMatching.o Classifier.o BayesAdapter.o SVMAdapter.o KNNAdapter.o AbstractAdapter.o


all: demo silhouetteExtraction

demo: demoMain.cpp $(OBJFILES)
		g++ demoMain.cpp $(OBJFILES) -o demo ${OPENCVLIBS}
		
silhouetteExtraction: $(RPTSE)main.cpp silhouetteExtraction.o
		g++ -o silhouetteExtraction $(RPTSE)main.cpp silhouetteExtraction.o ${OPENCVLIBS}

silhouetteExtraction.o: $(RPTSE)silhouetteExtraction.cpp $(RPTSE)silhouetteExtraction.hpp
		g++ -c $(RPTSE)silhouetteExtraction.cpp -o silhouetteExtraction.o ${OPENCVLIBS}


Classifier.o: $(RPTC)Classifier.cpp $(RPTC)Classifier.hpp
		g++ -c $(RPTC)Classifier.cpp -o Classifier.o ${OPENCVLIBS}


# Classifiers
GR=../Granlund/
Granlund.o: $(GR)MinimumDistanceClassifier.cpp $(GR)MinimumDistanceClassifier.hpp
		g++ -c $(GR)MinimumDistanceClassifier.cpp -o Granlund.o ${OPENCVLIBS}

HW=../height_width_matching/
HWMatching.o: $(HW)HWMatching.cpp $(HW)HWMatching.hpp
		g++ -c $(HW)HWMatching.cpp -o HWMatching.o ${OPENCVLIBS}

$(HW)HWMatching.hpp: $(RPTC)Classifier.hpp
#############


# Adapters
BayesAdapter.o: $(AP)BayesAdapter.cpp $(AP)BayesAdapter.hpp
		g++ -c $(AP)BayesAdapter.cpp -o BayesAdapter.o ${OPENCVLIBS}

SVMAdapter.o: $(AP)SVMAdapter.cpp $(AP)SVMAdapter.hpp
		g++ -c $(AP)SVMAdapter.cpp -o SVMAdapter.o ${OPENCVLIBS}

KNNAdapter.o: $(AP)KNNAdapter.cpp $(AP)KNNAdapter.hpp
		g++ -c $(AP)KNNAdapter.cpp -o KNNAdapter.o ${OPENCVLIBS}

AbstractAdapter.o: $(AP)AbstractAdapter.cpp $(AP)AbstractAdapter.hpp
		g++ -c $(AP)AbstractAdapter.cpp -o AbstractAdapter.o ${OPENCVLIBS}

$(AP)BayesAdapter.hpp: $(AP)AbstractAdapter.hpp

$(AP)SVMAdapter.hpp: $(AP)AbstractAdapter.hpp

$(AP)KNNAdapter.hpp: $(AP)AbstractAdapter.hpp
##########

clean:
		rm *.o
