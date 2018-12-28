clean: main.exe
	rm *.o
main.exe: main.o Rectangle.o Rhombus.o Trapezium.o TAllocationBlock.o SListItem.o SList.o
	g++ -Wall -lthread main.o Rectangle.o Rhombus.o Trapezium.o TAllocationBlock.o SListItem.o SList.o -o main.exe
main.o: main.cpp Rectangle.h Rhombus.h Trapezium.h TAllocationBlock.h SListItem.h SList.h
	g++ -c -Wall -lthread main.cpp
Rectangle.o: Rectangle.cpp Rectangle.h
	g++ -c -Wall -lthread Rectangle.cpp
Rhombus.o: Rhombus.cpp Rhombus.h
	g++ -c -Wall -lthread Rhombus.cpp
TAllocationBlock.o: TAllocationBlock.cpp TAllocationBlock.h
	g++ -c -Wall -lthread TAllocationBlock.cpp
Trapezium.o: Trapezium.cpp Trapezium.h
	g++ -c -Wall -lthread Trapezium.cpp
SListItem.o: SListItem.cpp SListItem.h
	g++ -c -Wall -lthread SListItem.cpp 
SList.o: SList.cpp SList.h
	g++ -c -Wall -lthread SList.cpp
