#include "SListItem.h"
#include <iostream>
#include <memory>

template <class T> SListItem<T>::SListItem(const std::shared_ptr<T>& figure) {
    this->figure = figure;
    this->next = nullptr;
	this->prev = nullptr;
    std::cout << "List item: created" << std::endl;
}

template <class T> TAllocationBlock SListItem<T>::SListItem_Allocator(sizeof(SListItem<T>), 4);


template <class T> std::shared_ptr<SListItem<T>> SListItem<T>::SetNext(std::shared_ptr<SListItem<T>> next) {
	std::shared_ptr<SListItem<T>> old = this->next;
    this->next = next;
    return old;
}

template <class T> std::shared_ptr<SListItem<T>> SListItem<T>::SetPrev(std::shared_ptr<SListItem<T>> prev) {
	std::shared_ptr<SListItem<T>> old = this->prev;
	this->prev = prev;
	return old;
}

template <class T> std::shared_ptr<T> SListItem<T>::GetFigure() const {
    return this->figure;
}

template <class T> std::shared_ptr<SListItem<T>> SListItem<T>::GetNext() {
    return this->next;
}

template <class T> std::shared_ptr<SListItem<T>> SListItem<T>::GetPrev() {
	return this->prev;
}

template <class T> std::shared_ptr<T> SListItem<T>::GetPFigure() const
{
	return this->figure;
}


template <class T> SListItem<T>::~SListItem() {
    std::cout << "List item: deleted" << std::endl;
    //delete next;
}

template <class A> std::ostream& operator<<(std::ostream& os, SListItem<A>& obj) {
	obj.GetFigure()->Print();
    return os;
}

template <class T> void * SListItem<T>::operator new (size_t size) {
	if (SListItem_Allocator.has_free_blocks()) {
		return SListItem_Allocator.allocate();
	}
	else {
		SListItem_Allocator.reallocate();
		return SListItem_Allocator.allocate();
	}
}


template <class T> void SListItem<T>::operator delete(void *p) {
	SListItem_Allocator.deallocate(p);
}


#include "Figure.h"
template class SListItem<Figure>;
template <class A> std::ostream& operator<<(std::ostream& os, const SListItem<Figure>& obj);
