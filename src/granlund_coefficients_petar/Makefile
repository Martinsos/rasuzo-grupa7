OPENCVLIBS=`pkg-config --libs opencv`

all: grancof

grancof: granlundCoefficients.cpp
	g++ granlundCoefficients.cpp ../testing/Classifier.cpp ../Classifiers/BayesAdapter.cpp \
		../Classifiers/AbstractAdapter.cpp ../Classifiers/SVMAdapter.cpp granlundClassifiers.cpp \
		../Classifiers/KNNAdapter.cpp ../Classifiers/RandomForestAdapter.cpp -o cv ${OPENCVLIBS}
