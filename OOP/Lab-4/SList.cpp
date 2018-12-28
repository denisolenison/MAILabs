#include "SList.h"
#include <memory>

template <class T> SList<T>::SList() : head(nullptr), tail(nullptr), count(0) {
}


template <class T> std::ostream& operator<<(std::ostream& os, const SList<T>& list) {

	std::shared_ptr<SListItem<T>> item = list.head;
    
    while(item!=nullptr)
    {
      item->GetPFigure()->Print();
      item = item->GetNext();
    }
    
    return os;
}

template <class T> void SList<T>::push_front(std::shared_ptr<T> &&item) {
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


template <class T> void SList<T>::push_back(std::shared_ptr<T> &&item) {
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


template <class T> void SList<T>::insert(std::shared_ptr<T> &&item, int pos) {
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
	if (pos < 1 || pos > count + 1)
	{
		// �������� �������
		std::cout << "Incorrect position !!!\n";
		return 0;
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

	std::shared_ptr<T> curRect = ins->GetFigure();

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

template <class T> SList<T>::~SList() {
	while (head != nullptr)
	{
		tail = head->GetNext();
		head = nullptr;
		head = tail;
		count--;
	}
}

#include "Figure.h"
template class SList<Figure>;
template std::ostream& operator<<(std::ostream& os, const SList<Figure>& list);
