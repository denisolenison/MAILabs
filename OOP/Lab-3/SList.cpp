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


void SList::push_back(std::shared_ptr<Figure> &&figure) {
	std::shared_ptr<SListItem> temp(new SListItem(figure));
	
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


void SList::insert(std::shared_ptr<Figure> &&figure, int pos) {
	if (pos < 1 || pos > count + 1)
	{
		// Неверная позиция
		std::cout << "Incorrect position !!!\n";
		return;
	}

	// Если вставка в конец списка
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

	// Счетчик
	int i = 1;

	// Отсчитываем от головы n - 1 элементов
	std::shared_ptr<SListItem> ins = head;

	while (i < pos)
	{
		// Доходим до элемента, 
		// перед которым вставляемся
		ins = ins->GetNext();
		i++;
	}

	// Доходим до элемента, 
	// который предшествует
	std::shared_ptr<SListItem> PrevIns = ins->GetPrev();

	// Создаем новый элемент
	std::shared_ptr<SListItem> temp(new SListItem(figure));

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
	std::shared_ptr<SListItem> ins = head;

	while (i < pos)
	{
		// Доходим до элемента, 
		// перед которым вставляемся
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

	// Позиция от 1 до Count?
	if (pos < 1 || pos > count)
	{
		// Неверная позиция
		std::cout << "Incorrect position !!!\n";
		return;
	}

	// Счетчик
	int i = 1;

	std::shared_ptr<SListItem> Del = head;

	while (i < pos)
	{
		// Доходим до элемента, 
		// который удаляется
		Del = Del->GetNext();
		i++;
	}

	// Доходим до элемента, 
	// который предшествует удаляемому
	std::shared_ptr<SListItem> PrevDel = Del->GetPrev();
	// Доходим до элемента, который следует за удаляемым
	std::shared_ptr<SListItem> AfterDel = Del->GetNext();

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

