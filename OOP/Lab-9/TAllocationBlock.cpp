#include "TAllocationBlock.h"
#include <iostream>

TAllocationBlock::TAllocationBlock(size_t size, size_t count) : size(size), count(count) {
	used_blocks = (char*)malloc(size*count);
	free_blocks = (void**)malloc(sizeof(void*)*count);

	for (size_t i = 0; i < count; i++) free_blocks[i] = used_blocks + i * size;
	free_count = count;
	max_count = count;
	//std::cout << "TAllocationBlock: Memory init" << std::endl;
}

void *TAllocationBlock::reallocate() {
	void *result = nullptr;
	max_count = max_count * 2;
	used_blocks = (char*)malloc(size*max_count);
	free_blocks = (void**)malloc(sizeof(void*)*max_count);

	for (size_t i = 0; i < max_count; i++) free_blocks[i] = used_blocks + i * size;
	free_count = max_count / 2;
	//std::cout << "TAllocationBlock: Memory reallocated" << std::endl;
	return result;
}

void *TAllocationBlock::allocate() {
	void *result = nullptr;

	if (free_count > 0)
	{
		result = free_blocks[free_count - 1];
		free_count--;
		//std::cout << "TAllocationBlock: Allocate " << (max_count - free_count) << " of " << max_count << std::endl;
	}
	else
	{

		//std::cout << "TAllocationBlock: Reallocated" << std::endl;
	}

	return result;
}

void TAllocationBlock::deallocate(void *pointer) {
	//std::cout << "TAllocationBlock: Deallocate block " << std::endl;

	free_blocks[free_count] = pointer;
	free_count++;

}

bool TAllocationBlock::has_free_blocks() {
	return free_count > 0;
}


TAllocationBlock::~TAllocationBlock() {

	if (free_count < count) std::cout << "TAllocationBlock: Memory leak?" << std::endl;
	else  std::cout << "TAllocationBlock: Memory freed" << std::endl;
	delete free_blocks;
	delete used_blocks;
}
