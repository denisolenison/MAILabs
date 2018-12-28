#include "TQueueItem.h"
#include <iostream>

template <class T> TQueueItem<T>::TQueueItem(T* item, std::recursive_mutex *parent) {
	this->queue_mutex = parent;
	this->item = std::shared_ptr<T>(item);
	this->next = nullptr;
	//std::cout << "Queue item: created" << std::endl;
}


template <class T> TQueueItem<T>::TQueueItem(std::shared_ptr<T> item, std::recursive_mutex *parent) {
	this->queue_mutex = parent;
	this->item = std::shared_ptr<T>(item);
	this->next = nullptr;
	//std::cout << "Queue item: created" << std::endl;
}


template <class T> std::shared_ptr<TQueueItem<T>>TQueueItem<T>::SetNext(std::shared_ptr<TQueueItem<T>> next) {
	std::unique_lock<std::recursive_mutex> lock(*queue_mutex);
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


template <class T> TQueueItem<T>::~TQueueItem() {
	//std::cout << "Queue item: deleted" << std::endl;
}

template <class A> std::ostream& operator<<(std::ostream& os, TQueueItem<A>& obj) {
	std::lock_guard<std::recursive_mutex> lock(*obj.queue_mutex);
	obj.GetValue()->Print();
	if (obj.GetNext() != nullptr) os << " - ";
    return os;
}

#include <functional>
template class TQueueItem<std::function<void(void)>>;