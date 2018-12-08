clean: main.exe
	rm *.o
main.exe: main.o rectangle.o rhombus.o trapezium.o
	g++ -Wall main.o rectangle.o rhombus.o trapezium.o -o main.exe
main.o: main.cpp rectangle.h rhombus.h trapezium.h
	g++ -c -Wall main.cpp
rectangle.o: rectangle.cpp rectangle.h
	g++ -c -Wall rectangle.cpp 
rhombus.o: rhombus.cpp rhombus.h
	g++ -c -Wall rhombus.cpp 
trapezium.o: trapezium.cpp trapezium.h
	g++ -c -Wall trapezium.cpp