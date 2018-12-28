#ifndef TQUEUEITEM_H
#define	TQUEUEITEM_H
#include <memory>
#include "TAllocationBlock.h"

template<class T> class TQueueItem {
public:
	TQueueItem(T *item);
	template<class A> friend std::ostream& operator<<(std::ostream& os, TQueueItem<A>& obj);

	std::shared_ptr<TQueueItem<T>> SetNext(std::shared_ptr<TQueueItem> &next);
	std::shared_ptr<TQueueItem<T>> GetNext();
	void SetThis(std::shared_ptr<TQueueItem<T>> _this);
	std::shared_ptr<T> GetValue() const;
	void * operator new (size_t size);
	void operator delete(void *p);

	void push_back(std::shared_ptr<TQueueItem<T>> next);
	template <class A> friend std::ostream& operator<<(std::ostream& os, TQueueItem<A>& obj);

	virtual ~TQueueItem();
private:
	std::shared_ptr<T> item;
	std::shared_ptr<TQueueItem<T>> next;
	size_t max_blocks = 4;

	static TAllocationBlock TQueueItem_Allocator;
};

#endif
