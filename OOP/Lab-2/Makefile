clean: main.exe
	rm *.o
main.exe: main.o Rectangle.o SListItem.o SList.o
	g++ -Wall main.o Rectangle.o SListItem.o SList.o -o main.exe
main.o: main.cpp Rectangle.h SListItem.h SList.h
	g++ -c -Wall main.cpp
Rectangle.o: Rectangle.cpp Rectangle.h
	g++ -c -Wall Rectangle.cpp 
SListItem.o: SListItem.cpp SListItem.h
	g++ -c -Wall SListItem.cpp 
SList.o: SList.cpp SList.h
	g++ -c -Wall SList.cpp