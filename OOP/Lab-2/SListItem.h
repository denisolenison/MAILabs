#ifndef SLISTITEM_H
#define	SLISTITEM_H

#include "Rectangle.h"
class SListItem {
public:
	SListItem(const Rectangle& rectangle);
	SListItem(const SListItem& orig);
    friend std::ostream& operator<<(std::ostream& os, const SListItem& obj);
    
	SListItem* SetNext(SListItem* next);
	SListItem* GetNext();

	SListItem* SetPrev(SListItem* prev);
	SListItem* GetPrev();

	Rectangle GetRectangle() const;

    virtual ~SListItem();
private:
	Rectangle rectangle;
	SListItem *next;
	SListItem *prev;
};

#endif

