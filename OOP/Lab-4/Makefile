clean: main.exe
	rm *.o
main.exe: main.o Rectangle.o Rhombus.o Trapezium.o SListItem.o SList.o
	g++ -std=c++11 -Wall main.o Rectangle.o Rhombus.o Trapezium.o SListItem.o SList.o -o main.exe
main.o: main.cpp Figure.h Rectangle.h Rhombus.h Trapezium.h SListItem.h SList.h
	g++ -c -std=c++11 -Wall main.cpp
Rectangle.o: Rectangle.cpp Rectangle.h
	g++ -c -std=c++11 -Wall Rectangle.cpp
Rhombus.o: Rhombus.cpp Rhombus.h
	g++ -c -std=c++11 -Wall Rhombus.cpp
Trapezium.o: Trapezium.cpp Trapezium.h
	g++ -c -std=c++11 -Wall Trapezium.cpp
SListItem.o: SListItem.cpp SListItem.h
	g++ -c -std=c++11 -Wall SListItem.cpp 
SList.o: SList.cpp SList.h
	g++ -c -std=c++11 -Wall SList.cpp
