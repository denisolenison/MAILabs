#include "TQueue.h"
#include <exception>

template <class T> TQueue<T>::TQueue() : head(nullptr), count(0) {
	std::cout << "Queue created" << std::endl;
}


template <class T> std::shared_ptr<T> TQueue<T>::operator[](size_t i) {
	std::lock_guard<std::recursive_mutex> lock(queue_mutex);
	if (i > size() - 1) throw std::invalid_argument("index greater then stack size");
	size_t j = 0;

	for (std::shared_ptr<T> a : *this) {
		if (j == i) return a;
		j++;
	}

	return std::shared_ptr<T>(nullptr);
}



template <class T> std::ostream& operator<<(std::ostream& os, const TQueue<T>& queue) {
	for (auto i : queue) os << *i << std::endl;
	return os;

}

template <class T> void TQueue<T>::push(T* item) {
	std::lock_guard<std::recursive_mutex> lock(queue_mutex);
	std::shared_ptr<TQueueItem<T>> other(new TQueueItem<T>(item, &queue_mutex));
	std::shared_ptr<TQueueItem<T>> current;
	current = head;
	if (current != nullptr) {
		while (current->GetNext() != nullptr) {
			current = current->GetNext();
		}
		current->SetNext(other);
	}
	else {
		current = other;
		head = current;
	}
}


template <class T> void TQueue<T>::push(std::shared_ptr<T> item) {
	std::lock_guard<std::recursive_mutex> lock(queue_mutex);
	std::shared_ptr<TQueueItem<T>> other(new TQueueItem<T>(item, &queue_mutex));
	std::shared_ptr<TQueueItem<T>> current;
	current = head;
	if (current != nullptr) {
		while (current->GetNext() != nullptr) {
			current = current->GetNext();
		}
		current->SetNext(other);
	}
	else {
		current = other;
		head = current;
	}
}


template <class T> bool TQueue<T>::empty() {
	std::lock_guard<std::recursive_mutex> lock(queue_mutex);
	return head == nullptr;
}

template <class T> std::shared_ptr<T> TQueue<T>::pop_front() {
	std::lock_guard<std::recursive_mutex> lock(queue_mutex);
	std::shared_ptr<T> result;
	if (head != nullptr) {
		result = head->GetValue();
		head = head->GetNext();
	}
	count--;

	return result;
}

template <class T> size_t TQueue<T>::size() {
	std::lock_guard<std::recursive_mutex> lock(queue_mutex);
	int result = 0;
	for (auto i : *this) result++;
	return result;
}

template <class T> TQueueItem<T> TQueue<T>::GetElement(int pos) {
	std::lock_guard<std::recursive_mutex> lock(queue_mutex);
	// Позиция от 1 до Count?
	if (pos < 1 || pos > count)
	{
		// Неверная позиция
		std::cout << "Incorrect position !!!\n";
		pos = count;
	}

	// Счетчик
	int i = 1;

	std::shared_ptr<TQueueItem<T>> Cur = head;

	while (i < pos)
	{
		// Доходим до элемента, 
		Cur = Cur->GetNext();
		i++;
	}


	return *Cur;
}

template <class T> int TQueue<T>::GetCount() {
	std::lock_guard<std::recursive_mutex> lock(queue_mutex);
	return count;
}


template <class T> TIterator<TQueueItem<T>, T> TQueue<T>::begin() const {
	return TIterator<TQueueItem<T>, T>(head);
}

template <class T> TIterator<TQueueItem<T>, T> TQueue<T>::end() const {
	return TIterator<TQueueItem<T>, T>(nullptr);
}

template <class T> TQueue<T>::~TQueue() {
	std::cout << "Queue deleted" << std::endl;
}

#include <functional>
template class TQueue<std::function<void(void)>>;