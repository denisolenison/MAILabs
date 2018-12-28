#ifndef SLIST_H
#define	SLIST_H

#include "Rectangle.h"
#include "SListItem.h"

class SList {
public:
	SList();
	SList(const SList& orig);
    
    void push_front(Rectangle &&rectangle);
	void push_back(Rectangle &&rectangle);
	void insert(Rectangle &&rectangle, int pos);
	size_t getSquare(int pos);
    bool empty();
	void Delete(int pos);
    friend std::ostream& operator<<(std::ostream& os,const SList& list);
    virtual ~SList();
private:
    
    SListItem *head;
	SListItem *tail;
	int count;
};

#endif

