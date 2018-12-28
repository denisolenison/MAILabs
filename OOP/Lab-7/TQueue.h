#ifndef TQUEUE_H
#define	TQUEUE_H

#include "TIterator.h"
#include "TQueueItem.h"
#include <memory>

template <class T> class TQueue {
public:
	TQueue();

	void push_back(T *item);

	bool empty();
	size_t size();

	TIterator<TQueueItem<T>, T> begin();
	TIterator<TQueueItem<T>, T> end();


	TQueueItem<T> GetElement(int pos);
	int GetCount();

	std::shared_ptr<T> pop_front();
	template <class A> friend std::ostream& operator<<(std::ostream& os, const TQueue<A>& queue);
	virtual ~TQueue();
private:

	std::shared_ptr<TQueueItem<T>> head;
	int count = 0;
};

#endif
