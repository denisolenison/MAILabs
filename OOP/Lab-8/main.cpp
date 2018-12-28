#include <cstdlib>
#include <iostream>
#include "Rectangle.h"
#include "SListItem.h"
#include "SList.h"
#include "TAllocationBlock.h"
#include <memory>
#include <random>
#include <thread>
#include <mutex>


int main(int argc, char** argv) {

	int endProg = 0;
	int a, b, c;
	int Fig;

	int allocated_blocks = 5;

	SList<Figure> list;
	std::shared_ptr<Figure> f;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> genFigure(1, 3);


	while (endProg == 0) {
		int action = 0;
		int action2 = 0;
		int amounts = 0;
		int Fig_ = 0;
		std::cout << "Choose Action: \n";
		std::cout << "1) Add Elements \n";
		std::cout << "2) Delete Element \n";
		std::cout << "3) Destructor \n";
		std::cout << "4) Print Container \n";
		std::cout << "5) Get Square \n";
		std::cout << "6) Add Random Elements \n";
		std::cout << "7) Sort \n";
		std::cout << "8) End Program \n";
		
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
					if (Fig <= 3) {
						int position;
						std::cout << "Set Position: \n";
						std::cin >> position;
						list.insert(std::move(f), position);
					}
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
			for (auto i : list) {
				i->Print();
				std::cout << " - ";
			}
			std::cout << "\n";
		}
		if (action == 5) {
			int position;
			std::cout << "Which Element? \n";
			std::cin >> position;
			std::cout << "Square = " << list.getSquare(position) << std::endl;
		}
		if (action == 6) {
			int amount_;
			int range_side;
			std::cout << "How many elements? \n";
			std::cin >> amount_;
			std::cout << "Set side range: from 1 to ";
			std::cin >> range_side;
			std::cout << "\n";
			std::uniform_int_distribution<int> genSide(1, range_side);
			for (int i = 0; i < amount_; ++i) {
				Fig_ = genFigure(generator);
				a = genSide(generator);
				b = genSide(generator);
				c = genSide(generator);
				if (Fig_ == 1) f = std::make_shared<Rectangle>(Rectangle(a, b));
				if (Fig_ == 2) f = std::make_shared<Rhombus>(Rhombus(a, b));
				if (Fig_ == 3) f = std::make_shared<Trapezium>(Trapezium(a, b, c));
				list.push_front(std::move(f));
			}
		}
		if (action == 7) {
			int sortType;
			std::cout << "Choose sort type:\n";
			std::cout << "1) Usual Sort \n";
			std::cout << "2) Parallel Sort \n";
			std::cin >> sortType;
			std::cout << "Sort -------------" << std::endl;
			if (sortType == 1) {
				list.sort();
			}
			else {
				list.sort_parallel();
			}
			std::cout << "Done -------------" << std::endl;
		}
		if (action == 8) {
			list.~SList();
			endProg = 1;
		}
		else {}

	}


    return 0;
}
