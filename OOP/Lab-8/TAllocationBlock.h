#ifndef TALLOCATIONBLOCK_H
#define	TALLOCATIONBLOCK_H

#include <cstdlib>
class TAllocationBlock {
public:
	TAllocationBlock(size_t size, size_t count);
	void *allocate();
	void *reallocate();
	void deallocate(void *pointer);
	bool has_free_blocks();

	virtual ~TAllocationBlock();
private:
	size_t size;
	size_t count;

	char  *used_blocks;
	void  **free_blocks;

	size_t free_count;
	size_t max_count;
};

#endif
