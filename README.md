# Text-Analyzer-using-Associative-Containers
## Project 5 of COP 4530: Implementing Ordered Associative Array

### Files
- **oaa.h**           the ordered associative array class template
- **wordbench2.h**    defines wordbench refactored to use the OAA API
- **wordbench2.cpp**  wordbench implementation
- **wordify.cpp**     used to clean string data
- **log.txt**         work log
- **main2.cpp**       driver program for wordbench
- **foaa.cpp**	  functionality test for OAA
- **rantable.cpp** 	  random table file generator
- **makefile**	  builds wb2.x, foaa.x, and moaa.x

## Required Implementations
1. Define and implement the class template OAA<K,D,P> within OAA.h.
2. Use the default value P = fsu::LessThan<K> for the third template parameter, so that OAA<K,D> is automatically defined.
3. Define the app WordBench, refactored as a client of OAA<fsu::String, size_t>, in the header file wordbench2.h, and implement the refactored WordBench in the file wordbench2.cpp.

### About Associative Containers
An associative container is one in which the client can access elements by value rather than position such as an array or vector where a position is specified by an index. The client cannot specify the position of an element in an associative container. The container implementation decides how an element is stored. Some associative containers maintain a sorted order which may be preferred and convenient for some clients. Also, because the internal structure of an associative container depends on the values of the elements, it would be destructive to allow the client to modify element values. Thus, to prevent altering the structure, iterators for the container are essentially constant types.


### Resources
