# ajdust this variables
	# there must be MYCLASSIFIER.cpp and MYCLASSIFIER.hpp in current directory
	MYCLASSIFIER=HWMatching
	#
	RELATIVE_PATH_TO_CLASSIFIER=../testing/
	# there must be MAIN.cpp in current directory
	MAIN=mainSample
#######################


MC=$(MYCLASSIFIER)
RPTC=$(RELATIVE_PATH_TO_CLASSIFIER)
OPENCVLIBS=`pkg-config --libs opencv`

all: $(MAIN)

$(MAIN): $(MAIN).cpp $(MC).o Classifier.o
		g++ $(MAIN).cpp $(MC).o Classifier.o -o $(MAIN) ${OPENCVLIBS}


Classifier.o: $(RPTC)Classifier.cpp $(RPTC)Classifier.hpp
		g++ -c $(RPTC)Classifier.cpp -o Classifier.o ${OPENCVLIBS}


$(MC).o: $(MC).cpp $(MC).hpp
		g++ -c $(MC).cpp ${OPENCVLIBS}

$(MC).hpp: $(RPTC)Classifier.hpp

clean:
		rm *.o
