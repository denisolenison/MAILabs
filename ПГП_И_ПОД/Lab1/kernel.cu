
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <chrono>

#include <windows.h>


using namespace std;

#define FIND_ERR(call) { gpuAssert((call), __FILE__, __LINE__); }

inline void gpuAssert(cudaError_t cudaStatus, const char* file, int line, bool abort = true)
{
	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "ERROR: CUDA failed in %s:%d: %s\n", file, line, cudaGetErrorString(cudaStatus));
		exit(0);
	}
}

__global__ void findMin(int N, double* v1, double* v2) {
	int tId = threadIdx.x + blockIdx.x * blockDim.x;
	while (tId < N) {
		if (v1[tId] < v2[tId]) {
			v2[tId] = v1[tId];
		}
		tId += blockDim.x * gridDim.x;
	}
}

//cpu
void findMinCPU(int N, double* v1, double* v2) {
	for (int i = 0 ; i < N ; ++i) {
		v2[i] = min(v1[i], v2[i]);
	}
}

int main()
{
	std::ios::sync_with_stdio(false);

	int N;
	cin >> N;
	if (N <= 0) {
		return 0;
	}

	double* vector1 = new double[N];
	double* vector2 = new double[N];

	double* fastV1, *fastV2;


	// Инициализируем память на GPU
	FIND_ERR(cudaMalloc(&fastV1, N * sizeof(double)));
	FIND_ERR(cudaMalloc(&fastV2, N * sizeof(double)));

	for (int i = 0; i < N; i++) {
		vector1[i] = i;
	}

	for (int i = 0; i < N; i++) {
		vector2[i] = (i*i)%N;
	}


	//CPU FUNCT
	//запись времени
	/*
	LARGE_INTEGER perfCntStart, perfCntStop, proc_freq;
	::memset(&proc_freq, 0x00, sizeof(proc_freq));
	::memset(&perfCntStart, 0x00, sizeof(perfCntStart));
	::memset(&perfCntStop, 0x00, sizeof(perfCntStop));
	::QueryPerformanceCounter(&perfCntStart);
	::QueryPerformanceFrequency(&proc_freq);

	findMinCPU(N, vector1, vector2);
	
	::QueryPerformanceCounter(&perfCntStop);
	printf(": %f\n", float(perfCntStop.QuadPart - perfCntStart.QuadPart) / float(proc_freq.QuadPart) * 1000);
	*/


	//Копируем в память GPU
	FIND_ERR(cudaMemcpy(fastV1, vector1, N * sizeof(double), cudaMemcpyHostToDevice));
	FIND_ERR(cudaMemcpy(fastV2, vector2, N * sizeof(double), cudaMemcpyHostToDevice));

	//запись времени 

	cudaEvent_t start, end;
	FIND_ERR(cudaEventCreate(&start));
	FIND_ERR(cudaEventCreate(&end));
	FIND_ERR(cudaEventRecord(start));
	

	findMin <<<8, 8>>> (N, fastV1, fastV2); //функция
	FIND_ERR(cudaGetLastError()); //просмотр ошибок
	
	FIND_ERR(cudaEventRecord(end));
	FIND_ERR(cudaEventSynchronize(end));
	float t;
	FIND_ERR(cudaEventElapsedTime(&t, start, end));
	FIND_ERR(cudaEventDestroy(start));
	FIND_ERR(cudaEventDestroy(end));
	//конец записи

	printf("time = %f\n", t);

	// Копируем результат с ГПУ
	FIND_ERR(cudaMemcpy(vector2, fastV2, N * sizeof(double), cudaMemcpyDeviceToHost));
	/*
	for (int i = 0; i < N; i++) {
		printf("%.10e ", vector2[i]);
	}*/

	// Освобождение памяти
	FIND_ERR(cudaFree(fastV1));
	FIND_ERR(cudaFree(fastV2));
	

	delete[] vector1;
	delete[] vector2;


    return 0;
}
