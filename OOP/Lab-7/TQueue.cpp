#include "TQueue.h"

template <class T> TQueue<T>::TQueue() : head(nullptr), count(0) {
	std::cout << "Queue created" << std::endl;
}


template <class T> std::ostream& operator<<(std::ostream& os, const TQueue<T>& queue) {

	std::shared_ptr<TQueueItem<T>> item = queue.head;

	while (item != nullptr)
	{
		os << *item;
		item = item->GetNext();
	}

	return os;
}

template <class T> void TQueue<T>::push_back(T* value) {
	std::shared_ptr<TQueueItem<T>> value_item(new TQueueItem<T>(value));
	std::cout << "List: Added to list" << std::endl;
	count++;
	if (head != nullptr) {
		head->push_back(value_item);
	}
	else {
		head = value_item;
	}
}


template <class T> bool TQueue<T>::empty() {
	return head == nullptr;
}

template <class T> std::shared_ptr<T> TQueue<T>::pop_front() {
	std::shared_ptr<T> result;
	if (head != nullptr) {
		result = head->GetValue();
		head = head->GetNext();
	}
	count--;

	return result;
}

template <class T> size_t TQueue<T>::size() {

	int result = 0;
	for (auto i : *this) result++;
	return result;
}

template <class T> TQueueItem<T> TQueue<T>::GetElement(int pos) {

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
	return count;
}


template <class T> TIterator<TQueueItem<T>, T> TQueue<T>::begin()
{
	return TIterator<TQueueItem<T>, T>(head);
}

template <class T> TIterator<TQueueItem<T>, T> TQueue<T>::end()
{
	return TIterator<TQueueItem<T>, T>(nullptr);
}

template <class T> TQueue<T>::~TQueue() {
	std::cout << "Queue deleted" << std::endl;
}

#include "Figure.h"
template class TQueue<Figure>;
template <class T> std::ostream& operator<<(std::ostream& os, TQueue<Figure>& queue);
