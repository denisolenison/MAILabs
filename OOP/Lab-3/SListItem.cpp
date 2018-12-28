#include "SListItem.h"
#include <iostream>
#include <memory>

SListItem::SListItem(const std::shared_ptr<Figure>& figure) {
    this->figure = figure;
    this->next = nullptr;
	this->prev = nullptr;
    std::cout << "List item: created" << std::endl;
}


std::shared_ptr<SListItem> SListItem::SetNext(std::shared_ptr<SListItem> next) {
	std::shared_ptr<SListItem> old = this->next;
    this->next = next;
    return old;
}

std::shared_ptr<SListItem> SListItem::SetPrev(std::shared_ptr<SListItem> prev) {
	std::shared_ptr<SListItem> old = this->prev;
	this->prev = prev;
	return old;
}

std::shared_ptr<Figure> SListItem::GetFigure() const {
    return figure;
}

std::shared_ptr<SListItem> SListItem::GetNext() {
    return this->next;
}

std::shared_ptr<SListItem> SListItem::GetPrev() {
	return this->prev;
}

std::shared_ptr<Figure> SListItem::GetPFigure() const
{
	return this->figure;
}


SListItem::~SListItem() {
    std::cout << "List item: deleted" << std::endl;
    //delete next;
}

std::ostream& operator<<(std::ostream& os, SListItem& obj) {
	obj.GetPFigure()->Print();
    return os;
}
