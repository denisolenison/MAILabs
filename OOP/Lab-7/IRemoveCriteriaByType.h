#ifndef IREMOVECRITERIABYTYPE_H
#define	IREMOVECRITERIABYTYPE_H
#include "IRemoveCriteria.h"

template <class T> class IRemoveCriteriaByType : public IRemoveCriteria<T> {
public:
	IRemoveCriteriaByType(int type) : Type(type) {};
	bool isIt(T* figure) override {
		return figure->GetFType() == Type;
	}

private:
	int Type;
};

#endif
