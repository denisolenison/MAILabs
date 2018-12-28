#include "SList.h"
#include <memory>

SList::SList() : head(nullptr), tail(nullptr), count(0) {
}


std::ostream& operator<<(std::ostream& os, const SList& list) {

	std::shared_ptr<SListItem> item = list.head;
    
    while(item!=nullptr)
    {
      item->GetPFigure()->Print();
      item = item->GetNext();
    }
    
    return os;
}

void SList::push_front(std::shared_ptr<Figure> &&figure) {
	std::shared_ptr<SListItem> temp(new SListItem(figure));

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


void SList::push_back(std::shared_ptr<Figure> &&figure) {
	std::shared_ptr<SListItem> temp(new SListItem(figure));
	
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


void SList::insert(std::shared_ptr<Figure> &&figure, int pos) {
	if (pos < 1 || pos > count + 1)
	{
		// �������� �������
		std::cout << "Incorrect position !!!\n";
		return;
	}

	// ���� ������� � ����� ������
	if (pos == count + 1)
	{
		push_back(std::move(figure));
		return;
	}
	else if (pos == 1)
	{
		push_front(std::move(figure));
		return;
	}

	// �������
	int i = 1;

	// ����������� �� ������ n - 1 ���������
	std::shared_ptr<SListItem> ins = head;

	while (i < pos)
	{
		// ������� �� ��������, 
		// ����� ������� �����������
		ins = ins->GetNext();
		i++;
	}

	// ������� �� ��������, 
	// ������� ������������
	std::shared_ptr<SListItem> PrevIns = ins->GetPrev();

	// ������� ����� �������
	std::shared_ptr<SListItem> temp(new SListItem(figure));

	// ��������� ������
	if (PrevIns != nullptr && count != 1)
		PrevIns->SetNext(temp);

	temp->SetNext(ins);
	temp->SetPrev(PrevIns);
	ins->SetPrev(temp);

	count++;



}


size_t SList::getSquare(int pos) {
	if (pos < 1 || pos > count + 1)
	{
		// �������� �������
		std::cout << "Incorrect position !!!\n";
		return 0;
	}

	// �������
	int i = 1;

	// ����������� �� ������ n - 1 ���������
	std::shared_ptr<SListItem> ins = head;

	while (i < pos)
	{
		// ������� �� ��������, 
		// ����� ������� �����������
		ins = ins->GetNext();
		i++;
	}

	std::shared_ptr<Figure> curRect = ins->GetFigure();

	return curRect->Square();
}


bool SList::empty() {
    return (head == nullptr);
}

void SList::Delete(int pos) {

	// ������� �� 1 �� Count?
	if (pos < 1 || pos > count)
	{
		// �������� �������
		std::cout << "Incorrect position !!!\n";
		return;
	}

	// �������
	int i = 1;

	std::shared_ptr<SListItem> Del = head;

	while (i < pos)
	{
		// ������� �� ��������, 
		// ������� ���������
		Del = Del->GetNext();
		i++;
	}

	// ������� �� ��������, 
	// ������� ������������ ����������
	std::shared_ptr<SListItem> PrevDel = Del->GetPrev();
	// ������� �� ��������, ������� ������� �� ���������
	std::shared_ptr<SListItem> AfterDel = Del->GetNext();

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

SList::~SList() {
	while (head != nullptr)
	{
		tail = head->GetNext();
		head = nullptr;
		head = tail;
		count--;
	}
}

