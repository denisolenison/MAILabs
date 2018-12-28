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

	// Предыдущего нет
	temp->SetPrev(nullptr);
	temp->SetNext(head);

	// Если элементы есть?
	if (head != nullptr)
		head->SetPrev(temp);

	// Если элемент первый, то он одновременно и голова и хвост
	if (count == 0)
		head = tail = temp;
	else
		// иначе новый элемент - головной
		head = temp;

	count++;
}


void SList::push_back(Rectangle &&rectangle) {
	SListItem *temp = new SListItem(rectangle);
	
	temp->SetNext(nullptr);
	temp->SetPrev(tail);

	// Если элементы есть?
	if (tail != nullptr)
		tail->SetNext(temp);

	// Если элемент первый, то он одновременно и голова и хвост
	if (count == 0)
		head = tail = temp;
	else
		// иначе новый элемент - хвостовой
		tail = temp;

	count++;
}


void SList::insert(Rectangle &&rectangle, int pos) {
	if (pos < 1 || pos > count + 1)
	{
		// Неверная позиция
		std::cout << "Incorrect position !!!\n";
		return;
	}

	// Если вставка в конец списка
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

	// Счетчик
	int i = 1;

	// Отсчитываем от головы n - 1 элементов
	SListItem *ins = head;

	while (i < pos)
	{
		// Доходим до элемента, 
		// перед которым вставляемся
		ins = ins->GetNext();
		i++;
	}

	// Доходим до элемента, 
	// который предшествует
	SListItem *PrevIns = ins->GetPrev();

	// Создаем новый элемент
	SListItem *temp = new SListItem(rectangle);

	// настройка связей
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
		// Неверная позиция
		std::cout << "Incorrect position !!!\n";
		return 0;
	}

	// Счетчик
	int i = 1;

	// Отсчитываем от головы n - 1 элементов
	SListItem *ins = head;

	while (i < pos)
	{
		// Доходим до элемента, 
		// перед которым вставляемся
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

	// Позиция от 1 до Count?
	if (pos < 1 || pos > count)
	{
		// Неверная позиция
		std::cout << "Incorrect position !!!\n";
		return;
	}

	// Счетчик
	int i = 1;

	SListItem *Del = head;

	while (i < pos)
	{
		// Доходим до элемента, 
		// который удаляется
		Del = Del->GetNext();
		i++;
	}

	// Доходим до элемента, 
	// который предшествует удаляемому
	SListItem * PrevDel = Del->GetPrev();
	// Доходим до элемента, который следует за удаляемым
	SListItem * AfterDel = Del->GetNext();

	// Если удаляем не голову
	if (PrevDel != nullptr && count != 1)
		PrevDel->SetNext(AfterDel);
	// Если удаляем не хвост
	if (AfterDel != 0 && count != 1)
		AfterDel->SetPrev(PrevDel);

	// Удаляются крайние?
	if (pos == 1)
		head = AfterDel;
	if (pos == count)
		tail = PrevDel;

	// Удаление элемента
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

