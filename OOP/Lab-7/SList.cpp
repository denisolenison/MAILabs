#include "SList.h"
#include <memory>

template <class T, class TT> SList<T, TT>::SList() {
	head = nullptr;
	tail = nullptr;
	count = 0;
}



template <class T, class TT> void SList<T, TT>::RemoveSubitem(IRemoveCriteria<TT> *criteria) {
	std::cout << "---------------------->" << std::endl;
	for (auto i : *this) {
		T copy;
		while (!i->empty()) {
			std::shared_ptr<TT> value = i->pop_front();

			if (criteria->isIt(&*value)) {
				std::cout << "List: Delete element ";
				value->Print();
				std::cout << std::endl;
			}
			else {
				if (value->GetFType() == 1) {
					copy.push_back(new Rectangle(value->GetA(), value->GetB()));
				}
				else if (value->GetFType() == 2) {
					copy.push_back(new Rhombus(value->GetA(), value->GetB()));
				}
				else if (value->GetFType() == 3) {
					copy.push_back(new Trapezium(value->GetA(), value->GetB(), value->GetC()));
				}
			}
		}

		while (!copy.empty()) {
			std::shared_ptr<TT> value2 = copy.pop_front();
			if (value2->GetFType() == 1) {
				i->push_back(new Rectangle(value2->GetA(), value2->GetB()));
			}
			else if (value2->GetFType() == 2) {
				i->push_back(new Rhombus(value2->GetA(), value2->GetB()));
			}
			else if (value2->GetFType() == 3) {
				i->push_back(new Trapezium(value2->GetA(), value2->GetB(), value2->GetC()));
			}
		}

	}
	for (auto i : *this) {
		if (i->empty()) {
			Remove(&*i);
		}
	}
	std::cout << "!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
}


template <class T, class TT> void SList<T, TT>::InsertSubitem(TT* value) {

	bool inserted = false;
	if (head != nullptr) {
		std::cout << "A" << std::endl;
		for (auto i : *this) {
			if (i->size() < 5) {
				if (inserted == false) {
					i->push_back(value);
					std::cout << "List: Add Element in list:" << i->size() << std::endl;
					inserted = true;
				}
			}
		}
	}

	if (!inserted) {
		std::cout << "B" << std::endl;
		std::cout << "List: New list element created" << std::endl;
		T* t_value = new T();
		t_value->push_back(value);
		push_back(t_value);
		count++;
	}
}



template <class T, class TT> void SList<T, TT>::push_back(T *value) {
	std::shared_ptr<SListItem<T>> value_item(new SListItem<T>(value));
	std::cout << "List: Added to list" << std::endl;
	if (head != nullptr) {
		head->push_back(value_item);
	}
	else {
		head = value_item;
	}
}



template <class T, class TT> bool SList<T, TT>::Remove(T* value) {
	std::shared_ptr<SListItem < T>> item = head;
	std::shared_ptr<SListItem < T>> prev_item = nullptr;
	bool result = false;

	while ((item != nullptr) && (!result)) {
		if (item->GetValue().get() == value) {
			if (prev_item != nullptr) prev_item->SetNext(item->GetNext());
			else head = item->GetNext();
			result = true;
		}
		else {
			prev_item = item;
			item = item->GetNext();
		}
	}

	return result;
}



template <class T, class TT> T SList<T, TT>::GetElement(int pos) {

	int curPos = 1;
	for (auto i : *this) {
		if (curPos == pos) {
			return *i;
		}
		curPos++;
	}
}

template <class T, class TT> int SList<T, TT>::GetCount() {
	return count;
}

template <class T, class TT> size_t SList<T, TT>::Size() {
	size_t result = 0;
	for (auto a : *this) result++;
	return result;
}


template <class T, class TT> TIterator<SListItem<T>, T> SList<T, TT>::begin()
const {
	return TIterator<SListItem<T>, T>(head);
}

template <class T, class TT> TIterator<SListItem<T>, T> SList<T, TT>::end()
const {
	return TIterator<SListItem<T>, T>(nullptr);
}


template <class T, class TT> SList<T, TT>::~SList() {
	std::cout << "List: deleted" << std::endl;
}

template <class A, class AA> std::ostream& operator<<(std::ostream& os, const SList<A, AA>& list) {
	std::cout << "Queues:" << std::endl;
	int ListLevel = 1;
	for (auto i : list) {
		std::cout << "{" << ListLevel << "}{" << *i << "}" << std::endl;
		ListLevel++;
	}
	return os;
}

#include "TQueue.h"
#include "Figure.h"

template class SList<TQueue<Figure>, Figure>;
template <class A, class AA> std::ostream& operator<<(std::ostream& os,const SList<TQueue<Figure>, Figure>& list);
