#ifndef SLIST_H
#define	SLIST_H
#include <memory>

#include "Rectangle.h"
#include "Rhombus.h"
#include "Trapezium.h"
#include "Figure.h"
#include "SListItem.h"
#include "TIterator.h"
#include "IRemoveCriteria.h"

template <class T, class TT> class SList {
public:
	SList();
    
	void InsertSubitem(TT *value);
	void RemoveSubitem(IRemoveCriteria<TT>  *criteria);

	void push_back(T *value);

	size_t Size();

	bool Remove(T* value);


	T GetElement(int pos);
	int GetCount();

	TIterator<SListItem<T>, T> begin() const;
	TIterator<SListItem<T>, T> end() const;

	template <class A, class AA> friend std::ostream& operator<<(std::ostream& os,const SList<A,AA>& list);
    virtual ~SList();
private:
    
	std::shared_ptr<SListItem<T>> head;
	std::shared_ptr<SListItem<T>> tail;
	int count;
};

#endif

