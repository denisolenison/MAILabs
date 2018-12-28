#ifndef TQUEUEITEM_H
#define	TQUEUEITEM_H
#include <memory>
#include <thread>
#include <mutex>
#include "TAllocationBlock.h"

template<class T> class TQueueItem {
public:
	TQueueItem(T *item, std::recursive_mutex *parent);
	TQueueItem(std::shared_ptr<T> item, std::recursive_mutex *parent);
	template<class A> friend std::ostream& operator<<(std::ostream& os, TQueueItem<A>& obj);

	std::shared_ptr<TQueueItem<T>> SetNext(std::shared_ptr<TQueueItem> next);
	std::shared_ptr<TQueueItem<T>> GetNext();
	std::shared_ptr<T> GetValue() const;

	virtual ~TQueueItem();
private:
	std::shared_ptr<T> item;
	std::shared_ptr<TQueueItem<T>> next;
	std::recursive_mutex *queue_mutex;
};

#endif