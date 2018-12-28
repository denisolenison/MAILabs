#ifndef SLISTITEM_H
#define	SLISTITEM_H

#include "Figure.h"
#include "TAllocationBlock.h"
#include <memory>


template<class T> class SListItem {
public:
	SListItem(T *figure);
	SListItem(std::shared_ptr<T> figure);
	template<class A> friend std::ostream& operator<<(std::ostream& os, const SListItem<A>& obj);
    
	std::shared_ptr<T> GetPFigure() const;

	std::shared_ptr<SListItem<T>> SetNext(std::shared_ptr<SListItem> next);
	std::shared_ptr<SListItem<T>> GetNext();

	std::shared_ptr<SListItem<T>> SetPrev(std::shared_ptr<SListItem> prev);
	std::shared_ptr<SListItem<T>> GetPrev();

	std::shared_ptr<T>GetValue() const;

	void * operator new (size_t size);
	void operator delete(void *p);

    virtual ~SListItem();
private:
	std::shared_ptr<T> figure;
	std::shared_ptr<SListItem<T>> next;
	std::shared_ptr<SListItem<T>> prev;

	size_t max_blocks = 4;

	static TAllocationBlock SListItem_Allocator;
};

#endif

