#ifndef SLISTITEM_H
#define	SLISTITEM_H

#include "Figure.h"
#include <memory>


class SListItem {
public:
	SListItem(const std::shared_ptr<Figure>& figure);
    friend std::ostream& operator<<(std::ostream& os, const SListItem& obj);
    
	std::shared_ptr<Figure> GetPFigure() const;

	std::shared_ptr<SListItem> SetNext(std::shared_ptr<SListItem> next);
	std::shared_ptr<SListItem> GetNext();

	std::shared_ptr<SListItem> SetPrev(std::shared_ptr<SListItem> prev);
	std::shared_ptr<SListItem> GetPrev();

	std::shared_ptr<Figure>GetFigure() const;

    virtual ~SListItem();
private:
	std::shared_ptr<Figure> figure;
	std::shared_ptr<SListItem> next;
	std::shared_ptr<SListItem> prev;
};

#endif

