#ifndef SLIST_H
#define	SLIST_H
#include <memory>

#include "Rectangle.h"
#include "Rhombus.h"
#include "Trapezium.h"
#include "SListItem.h"
#include "TIterator.h"

template <class T> class SList {
public:
	SList();
    
    void push_front(std::shared_ptr<T> &&item);
	void push_back(std::shared_ptr<T> &&item);
	void insert(std::shared_ptr<T> &&item, int pos);
	size_t getSquare(int pos);
    bool empty();
	void Delete(int pos);

	TIterator<SListItem<T>, T> begin();
	TIterator<SListItem<T>, T> end();

	template <class A> friend std::ostream& operator<<(std::ostream& os,const SList<A>& list);
    virtual ~SList();
private:
    
	std::shared_ptr<SListItem<T>> head;
	std::shared_ptr<SListItem<T>> tail;
	int count;
};

#endif

