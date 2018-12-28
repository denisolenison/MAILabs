#include <cstdlib>
#include <iostream>
#include "Rectangle.h"
#include "SListItem.h"
#include "SList.h"
#include <memory>


int main(int argc, char** argv) {

	int endProg = 0;
	int a, b, c;
	int Fig;

	SList<Figure> list;
	std::shared_ptr<Figure> f;

	while (endProg == 0) {
		int action = 0;
		int action2 = 0;
		int amounts = 0;
		std::cout << "Choose Action: \n";
		std::cout << "1) Add Elements \n";
		std::cout << "2) Delete Element \n";
		std::cout << "3) Destructor \n";
		std::cout << "4) Print Container \n";
		std::cout << "5) Get Square \n";
		std::cout << "6) End Program \n";
		
		std::cin >> action;

		if (action == 1) {
			std::cout << "How many elements? \n";
			std::cin >> amounts;
			for (int i = 0; i < amounts; ++i) {
				std::cout << "1 - Add Head \n";
				std::cout << "2 - Add Tail \n";
				std::cout << "3 - Insert Element \n";
				std::cin >> action2;
				if (action2 == 1) {
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
					if (Fig == 1) f = std::make_shared<Rectangle>(Rectangle(a, b));
					if (Fig == 2) f = std::make_shared<Rhombus>(Rhombus(a, b));
					if (Fig == 3) f = std::make_shared<Trapezium>(Trapezium(a, b, c));
					list.push_front(std::move(f));
				}
				if (action2 == 2) {
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
					if (Fig == 1) f = std::make_shared<Rectangle>(Rectangle(a, b));
					if (Fig == 2) f = std::make_shared<Rhombus>(Rhombus(a, b));
					if (Fig == 3) f = std::make_shared<Trapezium>(Trapezium(a, b, c));
					list.push_back(std::move(f));
				}
				if (action2 == 3) {
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
					if (Fig == 1) f = std::make_shared<Rectangle>(Rectangle(a, b));
					if (Fig == 2) f = std::make_shared<Rhombus>(Rhombus(a, b));
					if (Fig == 3) f = std::make_shared<Trapezium>(Trapezium(a, b, c));
					int position;
					std::cout << "Set Position: \n";
					std::cin >> position;
					list.insert(std::move(f), position);
				}
			}
		}
		if (action == 2) {
			int position;
			std::cout << "Set Position: \n";
			std::cin >> position;
			list.Delete(position);
		}
		if (action == 3) {
			int cond;
			std::cout << "Are you sure? 1 - yes , 0 - no \n";
			std::cin >> cond;
			if (cond == 1) list.~SList();
		}
		if (action == 4) {
			std::cout << "Container: \n";
			for (auto i : list)  i->Print() ;
		}
		if (action == 5) {
			int position;
			std::cout << "Which Element? \n";
			std::cin >> position;
			std::cout << "Square = " << list.getSquare(position) << std::endl;
		}
		if (action == 6) {
			endProg = 1;
		}
		else {}

	}


    return 0;
}

