clean: main.exe
	rm *.o
main.exe: main.o Rectangle.o Rhombus.o Trapezium.o TAllocationBlock.o TQueueItem.o TQueue.o SListItem.o SList.o
	g++ -Wall main.o Rectangle.o Rhombus.o Trapezium.o TAllocationBlock.o TQueueItem.o TQueue.o SListItem.o SList.o -o main.exe
main.o: main.cpp Rectangle.h Rhombus.h Trapezium.h TAllocationBlock.h TQueueItem.h TQueue.h SListItem.h SList.h
	g++ -c -Wall main.cpp
Rectangle.o: Rectangle.cpp Rectangle.h
	g++ -c -Wall Rectangle.cpp
Rhombus.o: Rhombus.cpp Rhombus.h
	g++ -c -Wall Rhombus.cpp
TAllocationBlock.o: TAllocationBlock.cpp TAllocationBlock.h
	g++ -c -Wall TAllocationBlock.cpp
Trapezium.o: Trapezium.cpp Trapezium.h
	g++ -c -Wall Trapezium.cpp
TQueueItem.o: TQueueItem.cpp TQueueItem.h
	g++ -c -Wall TQueueItem.cpp 
TQueue.o: TQueue.cpp TQueue.h
	g++ -c -Wall TQueue.cpp
SListItem.o: SListItem.cpp SListItem.h
	g++ -c -Wall SListItem.cpp 
SList.o: SList.cpp SList.h
	g++ -c -Wall SList.cpp