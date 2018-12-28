#include "SListItem.h"
#include <iostream>
#include <memory>

template <class T> SListItem<T>::SListItem(T* _item) {
    item = std::shared_ptr<T> (_item);
    next = nullptr;
	prev = nullptr;
    std::cout << "List item: created" << std::endl;
}

template <class T> std::shared_ptr<SListItem<T>> SListItem<T>::SetNext(std::shared_ptr<SListItem<T>> _next) {
	std::shared_ptr<SListItem<T>> old = next;
    next = _next;
    return old;
}

template <class T> std::shared_ptr<SListItem<T>> SListItem<T>::SetPrev(std::shared_ptr<SListItem<T>> _prev) {
	std::shared_ptr<SListItem<T>> old = prev;
	prev = _prev;
	return old;
}

template <class T> std::shared_ptr<T> SListItem<T>::GetValue() {
    return item;
}

template <class T> std::shared_ptr<SListItem<T>> SListItem<T>::GetNext() {
    return next;
}

template <class T> std::shared_ptr<SListItem<T>> SListItem<T>::GetPrev() {
	return prev;
}

template <class T> void SListItem<T>::push_back(std::shared_ptr<SListItem> _next) {
	if (next != nullptr) {
		next->push_back(_next);
	}
	else {
		next = _next;
	}
}



template <class T> SListItem<T>::~SListItem() {
    std::cout << "List item: deleted" << std::endl;
    //delete next;
}

template <class A> std::ostream& operator<<(std::ostream& os, SListItem<A>& obj) {
	obj.GetFigure()->Print();
    return os;
}


#include "TQueue.h"
#include "Figure.h"
template class SListItem<TQueue<Figure>>;
template class SListItem<Figure>;
