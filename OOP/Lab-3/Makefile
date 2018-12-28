clean: main.exe
	rm *.o
main.exe: main.o Rectangle.o Rhombus.o Trapezium.o SListItem.o SList.o
	g++ -Wall main.o Rectangle.o Rhombus.o Trapezium.o SListItem.o SList.o -o main.exe
main.o: main.cpp Rectangle.h Rhombus.h Trapezium.h SListItem.h SList.h
	g++ -c -Wall main.cpp
Rectangle.o: Rectangle.cpp Rectangle.h
	g++ -c -Wall Rectangle.cpp
Rhombus.o: Rhombus.cpp Rhombus.h
	g++ -c -Wall Rhombus.cpp
Trapezium.o: Trapezium.cpp Trapezium.h
	g++ -c -Wall Trapezium.cpp
SListItem.o: SListItem.cpp SListItem.h
	g++ -c -Wall SListItem.cpp 
SList.o: SList.cpp SList.h
	g++ -c -Wall SList.cpp