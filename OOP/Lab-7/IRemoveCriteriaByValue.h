#ifndef IREMOVECRITERIABYVALUE_H
#define	IREMOVECRITERIABYVALUE_H
#include "IRemoveCriteria.h"

template <class T> class IRemoveCriteriaByValue : public IRemoveCriteria<T> {
public:
	IRemoveCriteriaByValue(size_t value) : Value(value) {};
	bool isIt(T* figure) override {
		return figure->Square() < Value;
	}

private:
	size_t Value;
};

#endif
