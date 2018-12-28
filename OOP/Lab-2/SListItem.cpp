#include "SListItem.h"
#include <iostream>

SListItem::SListItem(const Rectangle& rectangle) {
    this->rectangle = rectangle;
    this->next = nullptr;
	this->prev = nullptr;
    std::cout << "List item: created" << std::endl;
}

SListItem::SListItem(const SListItem& orig) {
    this->rectangle = orig.rectangle;
    this->next = orig.next;
	this->prev = orig.prev;
    std::cout << "List item: copied" << std::endl;
}

SListItem* SListItem::SetNext(SListItem* next) {
	SListItem* old = this->next;
    this->next = next;
    return old;
}

SListItem* SListItem::SetPrev(SListItem* prev) {
	SListItem* old = this->prev;
	this->prev = prev;
	return old;
}

Rectangle SListItem::GetRectangle() const {
    return this->rectangle;
}

SListItem* SListItem::GetNext() {
    return this->next;
}

SListItem* SListItem::GetPrev() {
	return this->prev;
}

SListItem::~SListItem() {
    std::cout << "List item: deleted" << std::endl;
    delete next;

}

std::ostream& operator<<(std::ostream& os, const SListItem& obj) {
    os << "[" << obj.rectangle << "]" << " - ";
    return os;
}
