#home    = /home/courses/cop4530p/LIB
#For my windows code directory
home    = ../../CodeFiles
cpp     = $(home)/mycpp
tcpp    = $(home)/mytcpp
tests   = $(home)/tests
proj    = .
incpath = -I$(proj) -I$(cpp) -I$(tcpp)
CC      = g++ -std=c++11 -Wall -Wextra
#CC      = clang++ -std=c++11 -Wall -Wextra

project: wb2.x foaa.x moaa.x

wb2.x:   main2.o xstring.o wordbench2.o
	$(CC) -o wb2.x main2.o xstring.o wordbench2.o

main2.o: $(proj)/wordbench2.h $(proj)/main2.cpp
	$(CC) $(incpath)  -c $(proj)/main2.cpp

wordbench2.o: $(proj)/oaa.h $(proj)/wordbench2.h $(proj)/wordbench2.cpp $(proj)/wordify.cpp
	$(CC) $(incpath)  -c $(proj)/wordbench2.cpp

xstring.o: $(cpp)/xstring.h $(cpp)/xstring.cpp
	$(CC) $(incpath)  -c $(cpp)/xstring.cpp

foaa.x: $(proj)/oaa.h $(proj)/foaa.cpp
	$(CC) $(incpath) -o foaa.x $(proj)/foaa.cpp

foaa+.x: $(proj)/oaa.h $(proj)/foaa+.cpp
	$(CC) $(incpath) -o foaa+.x $(proj)/foaa+.cpp

moaa.x: $(proj)/oaa.h $(proj)/moaa.cpp
	$(CC) $(incpath) -o moaa.x $(proj)/moaa.cpp
