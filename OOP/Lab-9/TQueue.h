#ifndef TQUEUE_H
#define	TQUEUE_H

#include "TIterator.h"
#include "TQueueItem.h"
#include <memory>
#include <future>
#include <mutex>
#include <thread>

template <class T> class TQueue {
public:
	TQueue();

	void push(T *item);
	void push(std::shared_ptr<T> item);

	bool empty();
	size_t size();

	TIterator<TQueueItem<T>, T> begin() const;
	TIterator<TQueueItem<T>, T> end() const;

	std::shared_ptr<T> operator[] (size_t i);


	TQueueItem<T> GetElement(int pos);
	int GetCount();

	std::shared_ptr<T> pop_front();
	template <class A> friend std::ostream& operator<<(std::ostream& os, const TQueue<A>& queue);
	virtual ~TQueue();
private:
	std::recursive_mutex queue_mutex;
	std::shared_ptr<TQueueItem<T>> head;
	int count = 0;
};

#endif
