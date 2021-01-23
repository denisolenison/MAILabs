#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
//for __syncthreads()
#ifndef __CUDACC_RTC__ 
#define __CUDACC_RTC__
#endif // !(__CUDACC_RTC__)

#include <device_functions.h>

#define SHARED_MEMORY 512
#define MAXIMAL_INTEGER 2147483647 // 2^31 - 1

using namespace std;


#define CSC(call)  													\
do {																\
	cudaError_t res = call;											\
	if (res != cudaSuccess) {										\
		fprintf(stderr, "ERROR in %s:%d. Message: %s\n",			\
				__FILE__, __LINE__, cudaGetErrorString(res));		\
		exit(0);													\
	}															    \
} while (0)	

using namespace std;


class ClassSort {
public:
	int* intArray;
	int size = 0;

	ClassSort()
	{
		freopen(NULL, "rb", stdin);
		fread(&size, sizeof(int), 1, stdin);
		cerr << "Len: " << size << "\n";

		intArray = (int*)malloc(sizeof(*intArray) * size);

		fread(intArray, sizeof(int), size, stdin);

		//for (int i = 0; i < size; i++) {
		//	cerr << (int)intArray[i] << " ";
		//}

		fclose(stdin);
	};

	ClassSort(string inpFile) {
		FILE* file;
		if ((file = fopen(inpFile.c_str(), "rb")) == NULL) {
			cerr << "Can't load file" << "\n";
			exit(1);
		}

		fread(&size, sizeof(size), 1, file);
		cerr << "Len: " << size << "\n";
		intArray = (int*)malloc(sizeof(*intArray) * size);

		fread(intArray, sizeof(int), size, file);

		fclose(file);
	}

	void PrintRSS() {
		for (int i = 0; i < size - 1; i++) {
			if ((int)intArray[i + 1] < (int)intArray[i]) {
				cerr << (int)intArray[i] << " : " << (int)intArray[i+1] << "| ";
			}
		}
		cerr << "\n";
		freopen(NULL, "wb", stdout);
		fwrite(intArray, sizeof(*intArray), size, stdout);
		fclose(stdout);
	}

	void PrintRSS(string outFile) {
		for (int i = 0; i < size; i++) {
			cerr << (int)intArray[i] << " ";
		}
		cerr << "\n";

		FILE* file = fopen(outFile.c_str(), "wb");
		fwrite(intArray, sizeof(*intArray), size, file);
		fclose(file);
	}

	~ClassSort() {
		fclose(stdin);
		fclose(stdout);
		free(intArray);
	};
};


__device__ void oddEvenOne(const int tid, int* tmp, int shift, int len) {
	if ((tid + shift) % 2 == 0) {
		int a = tid;
		int b = tid + 1;
		if (b < len && tmp[a] > tmp[b]) {
			int temp = tmp[a];
			tmp[a] = tmp[b];
			tmp[b] = temp;
		}
	}
}


__global__ void OddEvenBlocks(int* theArray, int size, int shift2) {

	int tx = gridDim.x * blockDim.x;
	int tt = blockDim.x;
	int tid = threadIdx.x;
	int bid = blockDim.x * blockIdx.x;

	int tid_full = bid + threadIdx.x;

	__shared__ int tmp[SHARED_MEMORY];
	__shared__ int shift;


	for (int begin = bid + shift2; begin < size; begin += tx) {

		tid_full = begin + threadIdx.x;

		int end = (size > begin + tt ? begin + tt : size);
		int len = end - begin;

		if (tid < len) {
			tmp[tid] = theArray[tid_full];
		}
		else {
			tmp[tid] = MAXIMAL_INTEGER;
		}

		__syncthreads();

		for (int j = 0; j < len; ++j) {
			if (j % 2 == 0) {
				shift = 0;
			}
			else {
				shift = 1;
			}
			oddEvenOne(tid, tmp, shift, len);
			__syncthreads();
		}

		if (tid < len) {
			theArray[tid_full] = tmp[tid];
		}

		__syncthreads();
	}
}



__global__ void BitonicMerge(int* theArray, int size, int shift2) {


	int tx = gridDim.x * blockDim.x;
	int tt = blockDim.x; // 2 * SHARED_MEMORY
	int tid = threadIdx.x;
	int bid = blockDim.x * blockIdx.x;

	int shiftt = shift2;

	int tid_full = bid + threadIdx.x;
	__shared__ int tmp[2 * SHARED_MEMORY];



	for (int begin = bid + shiftt; begin < size; begin += tx) {
		tid_full = begin + threadIdx.x;

		int end = (size > begin + tt ? begin + tt : size);
		int len = end - begin;

		if ((tid < tt/2) && (tid < len)) {
			tmp[tid] = theArray[tid_full];
		}
		else if (tid < len) {
			tmp[tt + tt/2 - tid - 1] = theArray[tid_full];
		}
		else if ((tid < tt) && (tid >= tt / 2)) {
			tmp[tt + tt / 2 - tid - 1] = MAXIMAL_INTEGER;
		}
		else {
			tmp[tid] = MAXIMAL_INTEGER;
		}

		__syncthreads();

		int base = tt / 2;
		int shift = 0;
		int n_tid = tid;

		while (base >= 1) {
			if (n_tid >= base) {
				int opTid = n_tid - base;
				if (tmp[opTid + shift] > tmp[n_tid + shift]) {
					int temp = tmp[opTid + shift];
					tmp[opTid + shift] = tmp[n_tid + shift];
					tmp[n_tid + shift] = temp;
				}
				if (base >= 1) {
					base = base / 2;
					n_tid = n_tid - base;
					shift = shift + base;
				}
			}
			else {
				if (base >= 1) {
					base = base / 2;
				}
			}
			__syncthreads();
		}

		__syncthreads();


		if (tid < len) {
			theArray[tid_full] = tmp[tid];
		}

		__syncthreads();

	}
}


void Sorting(ClassSort* sort) {
	int* D_theArray;

	CSC(cudaMalloc((void**)& D_theArray, sizeof(*D_theArray) * sort->size));

	CSC(cudaMemcpy(D_theArray, sort->intArray, sizeof(*sort->intArray) * sort->size, cudaMemcpyHostToDevice));

	//pre-entry odd-even sort

	OddEvenBlocks << <1024, SHARED_MEMORY >> > (D_theArray, sort->size, 0);

	CSC(cudaGetLastError());


	//cycle bitonic-merge / odd-even sort
	int iters = (sort->size + 1) / SHARED_MEMORY + 2;
	for (int i = 0; i < iters; ++i) {
		int evOdd = i % 2;


		BitonicMerge << <1024, 2 * SHARED_MEMORY >> > (D_theArray, sort->size, evOdd * SHARED_MEMORY);
		CSC(cudaGetLastError());


		//cerr << "Iter: " << i << "\n";
	}

	CSC(cudaMemcpy(sort->intArray, D_theArray, sizeof(int) * sort->size, cudaMemcpyDeviceToHost));
	CSC(cudaFree(D_theArray));
}




int main(void) {

	//ClassSort theSort = ClassSort("test.data");
	//Sorting(&theSort);
	//theSort.PrintRSS("test_out.data");

	ClassSort theSort = ClassSort();
	Sorting(&theSort);
	theSort.PrintRSS();

	return 0;
}