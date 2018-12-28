#ifndef SLIST_H
#define	SLIST_H
#include <memory>
#include <future>
#include <mutex>
#include <thread>

#include "Rectangle.h"
#include "Rhombus.h"
#include "Trapezium.h"
#include "SListItem.h"
#include "TIterator.h"

template <class T> class SList {
public:
	SList();
	void push_front(T* item);
	void push_back(T* item);
    void push_front(std::shared_ptr<T> item);
	void push_back(std::shared_ptr<T> item);
	void insert(T* item, int pos);
	void insert(std::shared_ptr<T> item, int pos);
	size_t getSquare(int pos);
    bool empty();
	void Delete(int pos);
	size_t size();

	TIterator<SListItem<T>, T> begin();
	TIterator<SListItem<T>, T> end();

	std::shared_ptr<T> operator[] (size_t i);
	void sort();
	void sort_parallel();

	std::shared_ptr<T> pop_front();
	std::shared_ptr<T> pop_back();



	template <class A> friend std::ostream& operator<<(std::ostream& os,const SList<A>& list);
    virtual ~SList();
private:
	std::future<void> sort_in_background();
	std::shared_ptr<SListItem<T>> head;
	std::shared_ptr<SListItem<T>> tail;
	int count;
};

#endif

