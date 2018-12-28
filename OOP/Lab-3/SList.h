#ifndef SLIST_H
#define	SLIST_H
#include <memory>

#include "Rectangle.h"
#include "Rhombus.h"
#include "Trapezium.h"
#include "SListItem.h"

class SList {
public:
	SList();
    
    void push_front(std::shared_ptr<Figure> &&figure);
	void push_back(std::shared_ptr<Figure> &&figure);
	void insert(std::shared_ptr<Figure> &&figure, int pos);
	size_t getSquare(int pos);
    bool empty();
	void Delete(int pos);
    friend std::ostream& operator<<(std::ostream& os,const SList& list);
    virtual ~SList();
private:
    
	std::shared_ptr<SListItem> head;
	std::shared_ptr<SListItem> tail;
	int count;
};

#endif

