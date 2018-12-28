#ifndef SLISTITEM_H
#define	SLISTITEM_H

#include "Figure.h"
#include "TAllocationBlock.h"
#include <memory>


template<class T> class SListItem {
public:
	SListItem(T *item);
    
	std::shared_ptr<SListItem<T>> SetNext(std::shared_ptr<SListItem> next);
	std::shared_ptr<SListItem<T>> GetNext();

	std::shared_ptr<SListItem<T>> SetPrev(std::shared_ptr<SListItem> prev);
	std::shared_ptr<SListItem<T>> GetPrev();

	void push_back(std::shared_ptr<SListItem<T>> next);


	std::shared_ptr<T>GetValue();
	template <class A> friend std::ostream& operator<<(std::ostream& os, SListItem<A>& obj);
    virtual ~SListItem();
private:
	std::shared_ptr<T> item;
	std::shared_ptr<SListItem> next;
	std::shared_ptr<SListItem> prev;

};

#endif

