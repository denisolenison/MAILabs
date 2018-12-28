#include "SList.h"
#include <exception>
#include <memory>
#include <thread>
#include <mutex>

std::mutex the_mutex;

template <class T> SList<T>::SList() : head(nullptr), tail(nullptr), count(0) {
}


template <class T> std::shared_ptr<T> SList<T>::operator[](size_t i) {
	if (i > size() - 1) throw std::invalid_argument("index greater then list size");
	size_t j = 0;

	for (std::shared_ptr<T> a : *this) {
		if (j == i) return a;
		j++;
	}

	return std::shared_ptr<T>(nullptr);
}


template <class T> void SList<T>::sort() {
	if (size() > 1) {
		std::shared_ptr<T> middle = pop_front();
		SList<T> left, right;

		while (!empty()) {
			std::shared_ptr<T> item = pop_front();
			if (*item < *middle) {
				left.push_front(item);
			}
			else {
				right.push_front(item);
			}
		}

		left.sort();
		right.sort();

		while (!left.empty()) push_front(left.pop_back());
		push_front(middle);
		while (!right.empty()) push_front(right.pop_back());

	}
}

template<class T > std::future<void> SList<T>::sort_in_background() {
	std::lock_guard<std::mutex> locker(the_mutex);
	std::packaged_task<void(void) > task(std::bind(std::mem_fn(&SList<T>::sort_parallel), this));
	std::future<void> res(task.get_future());
	std::thread th(std::move(task));
	th.detach();
	return res;
}

template <class T> void SList<T>::sort_parallel() {
	if (size() > 1) {
		std::shared_ptr<T> middle = pop_back();
		SList<T> left, right;

		while (!empty()) {
			std::shared_ptr<T> item = pop_back();
			if (*item < *middle) {
				left.push_front(item);
			}
			else {
				right.push_front(item);
			}
		}

		std::future<void> left_res = left.sort_in_background();
		std::future<void> right_res = right.sort_in_background();

		left_res.get();

		while (!left.empty()) push_front(left.pop_back());
		push_front(middle);

		right_res.get();
		while (!right.empty()) push_front(right.pop_back());
	}
}


template <class T> std::ostream& operator<<(std::ostream& os, const SList<T>& list) {

	std::shared_ptr<SListItem<T>> item = list.head;
    
    while(item!=nullptr)
    {
      item->GetPFigure()->Print();
	  if (item->GetNext() != nullptr) os << " - ";
      item = item->GetNext();
    }
    
    return os;
}
template <class T> void SList<T>::push_front(T* item) {
	std::shared_ptr<SListItem<T>> temp(new SListItem<T>(item));

	// ����������� ���
	temp->SetPrev(nullptr);
	temp->SetNext(head);

	// ���� �������� ����?
	if (head != nullptr)
		head->SetPrev(temp);

	// ���� ������� ������, �� �� ������������ � ������ � �����
	if (count == 0)
		head = tail = temp;
	else
		// ����� ����� ������� - ��������
		head = temp;

	count++;
}

template <class T> void SList<T>::push_front(std::shared_ptr<T> item) {
	std::shared_ptr<SListItem<T>> temp(new SListItem<T>(item));

	// ����������� ���
	temp->SetPrev(nullptr);
	temp->SetNext(head);

	// ���� �������� ����?
	if (head != nullptr)
		head->SetPrev(temp);

	// ���� ������� ������, �� �� ������������ � ������ � �����
	if (count == 0)
		head = tail = temp;
	else
		// ����� ����� ������� - ��������
		head = temp;

	count++;
}

template <class T> void SList<T>::push_back(T* item) {
	std::shared_ptr<SListItem<T>> temp(new SListItem<T>(item));

	temp->SetNext(nullptr);
	temp->SetPrev(tail);

	// ���� �������� ����?
	if (tail != nullptr)
		tail->SetNext(temp);

	// ���� ������� ������, �� �� ������������ � ������ � �����
	if (count == 0)
		head = tail = temp;
	else
		// ����� ����� ������� - ���������
		tail = temp;

	count++;
}

template <class T> void SList<T>::push_back(std::shared_ptr<T> item) {
	std::shared_ptr<SListItem<T>> temp(new SListItem<T>(item));
	
	temp->SetNext(nullptr);
	temp->SetPrev(tail);

	// ���� �������� ����?
	if (tail != nullptr)
		tail->SetNext(temp);

	// ���� ������� ������, �� �� ������������ � ������ � �����
	if (count == 0)
		head = tail = temp;
	else
		// ����� ����� ������� - ���������
		tail = temp;

	count++;
}

template <class T> void SList<T>::insert(T* item, int pos) {
	if (pos < 1 || pos > count + 1)
	{
		// �������� �������
		std::cout << "Incorrect position !!!\n";
		return;
	}

	// ���� ������� � ����� ������
	if (pos == count + 1)
	{
		push_back(std::move(item));
		return;
	}
	else if (pos == 1)
	{
		push_front(std::move(item));
		return;
	}

	// �������
	int i = 1;

	// ����������� �� ������ n - 1 ���������
	std::shared_ptr<SListItem<T>> ins = head;

	while (i < pos)
	{
		// ������� �� ��������, 
		// ����� ������� �����������
		ins = ins->GetNext();
		i++;
	}

	// ������� �� ��������, 
	// ������� ������������
	std::shared_ptr<SListItem<T>> PrevIns = ins->GetPrev();

	// ������� ����� �������
	std::shared_ptr<SListItem<T>> temp(new SListItem<T>(item));

	// ��������� ������
	if (PrevIns != nullptr && count != 1)
		PrevIns->SetNext(temp);

	temp->SetNext(ins);
	temp->SetPrev(PrevIns);
	ins->SetPrev(temp);

	count++;



}

template <class T> void SList<T>::insert(std::shared_ptr<T> item, int pos) {
	if (pos < 1 || pos > count + 1)
	{
		// �������� �������
		std::cout << "Incorrect position !!!\n";
		return;
	}

	// ���� ������� � ����� ������
	if (pos == count + 1)
	{
		push_back(std::move(item));
		return;
	}
	else if (pos == 1)
	{
		push_front(std::move(item));
		return;
	}

	// �������
	int i = 1;

	// ����������� �� ������ n - 1 ���������
	std::shared_ptr<SListItem<T>> ins = head;

	while (i < pos)
	{
		// ������� �� ��������, 
		// ����� ������� �����������
		ins = ins->GetNext();
		i++;
	}

	// ������� �� ��������, 
	// ������� ������������
	std::shared_ptr<SListItem<T>> PrevIns = ins->GetPrev();

	// ������� ����� �������
	std::shared_ptr<SListItem<T>> temp(new SListItem<T>(item));

	// ��������� ������
	if (PrevIns != nullptr && count != 1)
		PrevIns->SetNext(temp);

	temp->SetNext(ins);
	temp->SetPrev(PrevIns);
	ins->SetPrev(temp);

	count++;



}


template <class T> size_t SList<T>::getSquare(int pos) {
	if (pos < 1 || pos > count)
	{
		// �������� �������
		std::cout << "Incorrect position !!!\n";
		pos = count;
	}

	// �������
	int i = 1;

	// ����������� �� ������ n - 1 ���������
	std::shared_ptr<SListItem<T>> ins = head;

	while (i < pos)
	{
		// ������� �� ��������, 
		// ����� ������� �����������
		ins = ins->GetNext();
		i++;
	}

	std::shared_ptr<T> curRect = ins->GetValue();

	return curRect->Square();
}


template <class T> bool SList<T>::empty() {
    return (head == nullptr);
}

template <class T> void SList<T>::Delete(int pos) {

	// ������� �� 1 �� Count?
	if (pos < 1 || pos > count)
	{
		// �������� �������
		std::cout << "Incorrect position !!!\n";
		return;
	}

	// �������
	int i = 1;

	std::shared_ptr<SListItem<T>> Del = head;

	while (i < pos)
	{
		// ������� �� ��������, 
		// ������� ���������
		Del = Del->GetNext();
		i++;
	}

	// ������� �� ��������, 
	// ������� ������������ ����������
	std::shared_ptr<SListItem<T>> PrevDel = Del->GetPrev();
	// ������� �� ��������, ������� ������� �� ���������
	std::shared_ptr<SListItem<T>> AfterDel = Del->GetNext();

	// ���� ������� �� ������
	if (PrevDel != nullptr && count != 1)
		PrevDel->SetNext(AfterDel);
	// ���� ������� �� �����
	if (AfterDel != 0 && count != 1)
		AfterDel->SetPrev(PrevDel);

	// ��������� �������?
	if (pos == 1)
		head = AfterDel;
	if (pos == count)
		tail = PrevDel;

	// �������� ��������
	Del = nullptr;

	count--;
	return;
}

template <class T> std::shared_ptr<T> SList<T>::pop_front() {
	std::shared_ptr<T> result;
	if (head != nullptr) {
		result = head->GetValue();
		head = head->GetNext();
	}
	count--;
	return result;
}

template <class T> std::shared_ptr<T> SList<T>::pop_back() {
	std::shared_ptr<T> result;

	if (head != nullptr) {
		std::shared_ptr<SListItem<T>> element = head;
		std::shared_ptr<SListItem<T>> previ = nullptr;

		while (element->GetNext() != nullptr) {
			previ = element;
			element = element->GetNext();
		}

		if (previ != nullptr) {
			previ->SetNext(nullptr);
			tail = previ;
			result = element->GetValue();

		}
		else {
			result = element->GetValue();
			head = nullptr;
			tail = nullptr;
		}
	}
	count--;
	return result;

}


template <class T> size_t SList<T>::size() {

	int result = 0;
	for (auto i : *this) result++;
	return result;
}



template <class T> TIterator<SListItem<T>, T> SList<T>::begin()
{
	return TIterator<SListItem<T>, T>(head);
}

template <class T> TIterator<SListItem<T>, T> SList<T>::end()
{
	return TIterator<SListItem<T>, T>(nullptr);
}


template <class T> SList<T>::~SList() {
	while (head != nullptr)
	{
		Delete(1);
	}
	tail = nullptr;
}

#include "Figure.h"
template class SList<Figure>;
template <class T> std::ostream& operator<<(std::ostream& os, const SList<Figure>& list);
