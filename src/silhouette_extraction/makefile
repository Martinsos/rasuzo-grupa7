OPENCVLIB=`pkg-config --libs opencv`

all: silhouetteExtraction 

silhouetteExtraction: main.cpp silhouetteExtraction.o
		g++ -o silhouetteExtraction main.cpp silhouetteExtraction.o ${OPENCVLIB}

silhouetteExtraction.o: silhouetteExtraction.cpp silhouetteExtraction.hpp
		g++ -c silhouetteExtraction.cpp ${OPENCVLIB}

clean:
		rm *.o
