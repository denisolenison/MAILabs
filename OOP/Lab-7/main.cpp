#include <cstdlib>
#include <iostream>
#include "Figure.h"
#include "SListItem.h"
#include "SList.h"
#include "TQueueItem.h"
#include "TQueue.h"
#include "TIterator.h"
#include "TAllocationBlock.h"
#include "IRemoveCriteriaByValue.h"
#include "IRemoveCriteriaByType.h"
#include "IRemoveCriteriaAll.h"

#include <memory>




int main(int argc, char** argv) {

	int endProg = 0;
	int a, b, c;
	int Fig;

	SList<TQueue<Figure>, Figure> list;

	while (endProg == 0) {
		int action = 0;
		int action2 = 0;
		int action3 = 0;
		int amounts = 0;
		std::cout << "Choose Action: \n";
		std::cout << "1) Add Elements \n";
		std::cout << "2) Delete Elements \n";
		std::cout << "3) Destructor \n";
		std::cout << "4) Print Container \n";
		std::cout << "5) Get Square \n";
		std::cout << "6) End Program \n";
		
		std::cin >> action;

		if (action == 1) {
			std::cout << "How many elements? \n";
			std::cin >> amounts;
			for (int i = 0; i < amounts; ++i) {
				std::cout << "Choose Figure: \n";
				std::cout << "1 - Rectangle \n";
				std::cout << "2 - Rhombus \n";
				std::cout << "3 - Trapezium: \n";
				std::cin >> Fig;
				if (Fig == 1) {
					std::cout << "Set Sizes: \n";
				}
				if (Fig == 2) {
					std::cout << "Set Heights: \n";
				}
				if (Fig == 3) {
					std::cout << "Set 2 Sizes and height: \n";
				}
				std::cin >> a >> b;
				if (Fig == 3) std::cin >> c;
				if (Fig == 1) list.InsertSubitem(new Rectangle(a, b));
				if (Fig == 2) list.InsertSubitem(new Rhombus(a, b));
				if (Fig == 3) list.InsertSubitem(new Trapezium(a, b, c));
			}
		}
		if (action == 2) {
			std::cout << "Choose Criteria? \n";
			std::cout << "1 - By type \n";
			std::cout << "2 - By square (below) \n";
			std::cin >> action2;
			if (action2 == 1) {
				std::cout << "What type? \n";
				std::cout << "1 - All rectangles \n";
				std::cout << "2 - All Rhombuses \n";
				std::cout << "3 - All Trapeziums \n";
				std::cin >> action3;
				if (action3 == 1) {
					IRemoveCriteriaByType<Figure> criteria(1);
					list.RemoveSubitem(&criteria);
				}
				else if (action3 == 2) {
					IRemoveCriteriaByType<Figure> criteria(2);
					list.RemoveSubitem(&criteria);
				}
				else if (action3 == 3) {
					IRemoveCriteriaByType<Figure> criteria(3);
					list.RemoveSubitem(&criteria);
				}
				
			}
			else {
				std::cout << "Insert Square: ";
				std::cin >> action3;
				IRemoveCriteriaByValue<Figure> criteria(action3);
				list.RemoveSubitem(&criteria);
			}
		}
		if (action == 3) {
			int cond;
			std::cout << "Are you sure? 1 - yes , 0 - no \n";
			std::cin >> cond;
			if (cond == 1) {
				IRemoveCriteriaAll<Figure> criteria;
				list.RemoveSubitem(&criteria);
			}
		}
		if (action == 4) {
			std::cout << "Container: \n";
			std::cout << list << std::endl;
		}
		if (action == 5) {
			int position;
			int position2;
			std::cout << "Which Container? \n";
			std::cin >> position;
			if (position >= 1 && position <= list.GetCount()) {
				TQueue<Figure> CurQueue = list.GetElement(position);
				std::cout << "Which Position? \n";
				std::cin >> position2;
				if (position2 >= 1 && position2 <= CurQueue.GetCount()) {
					TQueueItem<Figure> CurElement = CurQueue.GetElement(position2);
					std::cout << "Square = " << CurElement.GetValue()->Square() << std::endl;
				}
				else {
					std::cout << "Incorrect position!!!\n";
				}
			}
			else {
				std::cout << "Incorrect position!!!\n";
			}
		}
		if (action == 6) {
			list.~SList();
			endProg = 1;
		}
		else {}
		
	}


    return 0;
}
