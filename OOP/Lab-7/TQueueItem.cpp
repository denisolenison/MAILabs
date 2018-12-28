#include "TQueueItem.h"
#include <iostream>

template <class T> TQueueItem<T>::TQueueItem(T* item) {
	this->item = std::shared_ptr<T>(item);
	this->next = nullptr;
	std::cout << "Queue item: created" << std::endl;
}

template <class T>  TAllocationBlock TQueueItem<T>::TQueueItem_Allocator(sizeof(TQueueItem<T>), 4);

template <class T> std::shared_ptr<TQueueItem<T>> TQueueItem<T>::SetNext(std::shared_ptr<TQueueItem<T>> &next) {
	std::shared_ptr<TQueueItem <T>> old = this->next;
	this->next = next;
	return old;
}

template <class T> std::shared_ptr<T> TQueueItem<T>::GetValue() const {
	return this->item;
}

template <class T> std::shared_ptr<TQueueItem<T>> TQueueItem<T>::GetNext() {
	return this->next;
}

template <class T> void TQueueItem<T>::SetThis(std::shared_ptr<TQueueItem> _this) {
	item = _this->item;
}

template <class T> TQueueItem<T>::~TQueueItem() {
	std::cout << "Queue item: deleted" << std::endl;
}

template <class A> std::ostream& operator<<(std::ostream& os, TQueueItem<A>& obj) {
	obj.GetValue()->Print();
	if (obj.GetNext() != nullptr) os << " - ";
    return os;
}

template <class T> void TQueueItem<T>::push_back(std::shared_ptr<TQueueItem> _next) {
	if (next != nullptr) {
		if (_next->GetValue()->Square() >= GetValue()->Square()) {
			next->push_back(_next);
		}
		else {
			std::shared_ptr<TQueueItem> new_T(new TQueueItem(*this));
			SetThis(_next);
			new_T->next = next->next;
			push_back(new_T);
		}
	}
	else {
		if (_next->GetValue()->Square() >= GetValue()->Square()) {
			next = _next;
		}
		else {
			std::shared_ptr<TQueueItem> new_T(new TQueueItem(*this));
			SetThis(_next);
			next = new_T;
		}
	}
}


template <class T> void * TQueueItem<T>::operator new (size_t size) {
	if (TQueueItem_Allocator.has_free_blocks()) {
		return TQueueItem_Allocator.allocate();
	}
	else {
		TQueueItem_Allocator.reallocate();
		return TQueueItem_Allocator.allocate();
	}
}


template <class T> void TQueueItem<T>::operator delete(void *p) {
	TQueueItem_Allocator.deallocate(p);
}


#include "Figure.h"
template class TQueueItem<Figure>;
template <class A> std::ostream& operator<<(std::ostream& os, TQueueItem<Figure>& obj);
