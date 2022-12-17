main: TTRP.o funciones.o
	g++ funciones.o TTRP.o -o TTRP

funciones.o: funciones.hpp funciones.cpp
	g++ -c funciones.cpp

TTRP.o: TTRP.cpp
	g++ -c TTRP.cpp

clean:
	rm *.o TTRP
