#include "SList.h"

SList::SList() : head(nullptr), tail(nullptr), count(0) {
}


SList::SList(const SList& orig) {
    head = orig.head;
	tail = orig.tail;
	count = 0;
}


std::ostream& operator<<(std::ostream& os, const SList& list) {

	SListItem *item = list.head;
    
    while(item!=nullptr)
    {
      os << *item;
      item = item->GetNext();
    }
    
    return os;
}

void SList::push_front(Rectangle &&rectangle) {
	SListItem *temp = new SListItem(rectangle);

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


void SList::push_back(Rectangle &&rectangle) {
	SListItem *temp = new SListItem(rectangle);
	
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


void SList::insert(Rectangle &&rectangle, int pos) {
	if (pos < 1 || pos > count + 1)
	{
		// �������� �������
		std::cout << "Incorrect position !!!\n";
		return;
	}

	// ���� ������� � ����� ������
	if (pos == count + 1)
	{
		push_back(std::move(rectangle));
		return;
	}
	else if (pos == 1)
	{
		push_front(std::move(rectangle));
		return;
	}

	// �������
	int i = 1;

	// ����������� �� ������ n - 1 ���������
	SListItem *ins = head;

	while (i < pos)
	{
		// ������� �� ��������, 
		// ����� ������� �����������
		ins = ins->GetNext();
		i++;
	}

	// ������� �� ��������, 
	// ������� ������������
	SListItem *PrevIns = ins->GetPrev();

	// ������� ����� �������
	SListItem *temp = new SListItem(rectangle);

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
	SListItem *ins = head;

	while (i < pos)
	{
		// ������� �� ��������, 
		// ����� ������� �����������
		ins = ins->GetNext();
		i++;
	}

	Rectangle curRect = ins->GetRectangle();

	return curRect.Square();
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

	SListItem *Del = head;

	while (i < pos)
	{
		// ������� �� ��������, 
		// ������� ���������
		Del = Del->GetNext();
		i++;
	}

	// ������� �� ��������, 
	// ������� ������������ ����������
	SListItem * PrevDel = Del->GetPrev();
	// ������� �� ��������, ������� ������� �� ���������
	SListItem * AfterDel = Del->GetNext();

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

