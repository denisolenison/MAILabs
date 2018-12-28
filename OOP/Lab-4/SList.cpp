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


template <class T> void SList<T>::push_back(std::shared_ptr<T> &&item) {
	std::shared_ptr<SListItem<T>> temp(new SListItem<T>(item));
	
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


template <class T> void SList<T>::insert(std::shared_ptr<T> &&item, int pos) {
	if (pos < 1 || pos > count + 1)
	{
		// Неверная позиция
		std::cout << "Incorrect position !!!\n";
		return;
	}

	// Если вставка в конец списка
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

	// Счетчик
	int i = 1;

	// Отсчитываем от головы n - 1 элементов
	std::shared_ptr<SListItem<T>> ins = head;

	while (i < pos)
	{
		// Доходим до элемента, 
		// перед которым вставляемся
		ins = ins->GetNext();
		i++;
	}

	// Доходим до элемента, 
	// который предшествует
	std::shared_ptr<SListItem<T>> PrevIns = ins->GetPrev();

	// Создаем новый элемент
	std::shared_ptr<SListItem<T>> temp(new SListItem<T>(item));

	// настройка связей
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
		// Неверная позиция
		std::cout << "Incorrect position !!!\n";
		return 0;
	}

	// Счетчик
	int i = 1;

	// Отсчитываем от головы n - 1 элементов
	std::shared_ptr<SListItem<T>> ins = head;

	while (i < pos)
	{
		// Доходим до элемента, 
		// перед которым вставляемся
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

	// Позиция от 1 до Count?
	if (pos < 1 || pos > count)
	{
		// Неверная позиция
		std::cout << "Incorrect position !!!\n";
		return;
	}

	// Счетчик
	int i = 1;

	std::shared_ptr<SListItem<T>> Del = head;

	while (i < pos)
	{
		// Доходим до элемента, 
		// который удаляется
		Del = Del->GetNext();
		i++;
	}

	// Доходим до элемента, 
	// который предшествует удаляемому
	std::shared_ptr<SListItem<T>> PrevDel = Del->GetPrev();
	// Доходим до элемента, который следует за удаляемым
	std::shared_ptr<SListItem<T>> AfterDel = Del->GetNext();

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
