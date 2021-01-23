
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <cuda.h>
#include <thrust/functional.h>
#include <thrust/swap.h>
#include <thrust/extrema.h>
#include <thrust/execution_policy.h>
#include <thrust/iterator/transform_iterator.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/reduce.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <algorithm>
#include <string>

using namespace std;
using namespace thrust;

#define CSC(call)  													\
do {																\
	cudaError_t res = call;											\
	if (res != cudaSuccess) {										\
		fprintf(stderr, "ERROR in %s:%d. Message: %s\n",			\
				__FILE__, __LINE__, cudaGetErrorString(res));		\
		exit(0);													\
	}															    \
} while (0)	



const int BLOCK = 256;
const int THREAD = 256;

const int BLOCK2 = 16;
const int THREAD2 = 16;


__constant__ int N[1];
__constant__ int M[1];
__constant__ int K[1];

struct abs_fun : public thrust::unary_function<double, double> {
	__host__ __device__
		double operator()(double elem) const {
		return elem < 0 ? -elem : elem;
	}
};

struct abs_comp {
	abs_fun fabs;
	__host__ __device__ double operator()(double a, double b) {
		return fabs(a) < fabs(b);
	}
};



__global__ void fastSwap(int row, int col, int max_id, double* AB) {
	unsigned tx = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned ts = blockDim.x * gridDim.x;

	for (int el = tx; el < M[0] + K[0]; el += ts) {
		double temp = AB[el * N[0] + row];
		AB[el * N[0] + row] = AB[el * N[0] + max_id];
		AB[el * N[0] + max_id] = temp;
	}
}

__global__ void triagStep(double* AB, int r, int c) {
	unsigned tx = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned txs = blockDim.x * gridDim.x;

	unsigned ty = blockIdx.y * blockDim.y + threadIdx.y;
	unsigned tys = blockDim.y * gridDim.y;

	for (int col_ = ty + c + 1; col_ < M[0] + K[0]; col_ += tys) {
		for (int row_ = tx + r + 1; row_ < N[0]; row_ += txs) {

			double rowDivisor = AB[row_ + N[0] * c] / AB[r + N[0] * c];
			AB[row_ + N[0] * col_] -= AB[r + N[0] * col_] * rowDivisor;

		}
	}
}



__global__ void backStep(double* AB, int row, int col) {
	unsigned tx = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned txs = blockDim.x * gridDim.x;

	unsigned ty = blockIdx.y * blockDim.y + threadIdx.y;
	unsigned tys = blockDim.y * gridDim.y;

	for (int col_ = ty + col + 1; col_ < M[0] + K[0]; col_ += tys) {
		for (int row_ = tx; row_ < row; row_ += txs) {
			double rowDivisor = AB[row_ + col * N[0]] / AB[row + col * N[0]];
			AB[row_ + col_ * N[0]] -= AB[row + col_ * N[0]] * rowDivisor;
		}
	}
}


__global__ void backKill(double* AB, int row, int col) {
	unsigned tx = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned txs = blockDim.x * gridDim.x;

	for (int row_ = tx; row_ < row; row_ += txs) {
		double rowDivisor = AB[row_ + col * N[0]] / AB[row + col * N[0]];
		AB[row_ + col * N[0]] -= AB[row + col * N[0]] * rowDivisor;
	}


}

__global__ void division(double* AB, double* diag, int* indices, int* indices2, int freeX) {

	unsigned tx = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned txs = blockDim.x * gridDim.x;

	unsigned ty = blockIdx.y * blockDim.y + threadIdx.y;
	unsigned tys = blockDim.y * gridDim.y;


	for (int col_ = ty; col_ < K[0]; col_ += tys) {
		for (int i = tx; i < freeX; i += txs) {
			int curRow = indices2[i];

			double rowDivisor = diag[i];
			AB[curRow + col_ * N[0] + N[0] * M[0]] = AB[curRow + col_ * N[0] + N[0] * M[0]] / rowDivisor;

		}
	}
}




__global__ void triagKill(double* AB, int r, int c) {
	unsigned tx = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned txs = blockDim.x * gridDim.x;

	for (int row_ = tx + r + 1; row_ < N[0]; row_ += txs) {

		double rowDivisor = AB[row_ + N[0] * c] / AB[r + N[0] * c];
		AB[row_ + N[0] * c] -= AB[r + N[0] * c] * rowDivisor;
	}
}


int main() {

	int n, m, k;
	scanf("%d%d%d", &n, &m, &k);

	cudaMemcpyToSymbol(N, &n, sizeof(int));
	cudaMemcpyToSymbol(M, &m, sizeof(int));
	cudaMemcpyToSymbol(K, &k, sizeof(int));

	int nm, nk;
	nm = n * m;
	nk = n * k;

	host_vector<double> matrAB_host(nm + nk);

	device_vector<double> matrAB_dev;

	srand(time(NULL));

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			//double rNum = (double)((rand() * rand()) % 1000000) / 2783;
			//cin >> matrAB_host[i + j * n];
			scanf("%lf", &matrAB_host[i + j * n]);
			//matrAB_host[i + j * n] = rNum;
		}
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < k; ++j) {
			double rNum = (double)((rand() * rand()) % 1000000) / 5466;
			//cin >> matrAB_host[i + j * n + nm];
			scanf("%lf", &matrAB_host[i + j * n + nm]);
			//matrAB_host[i + j * n + nm] = rNum;
		}
	}


	//cerr << "N: " << n << " M: " << m << " K: " << k << "\n";


	//to GPU memory
	matrAB_dev = matrAB_host;



	// pointers to mem:
	double* AB_ptr = thrust::raw_pointer_cast(matrAB_dev.data());


	dim3 gridSz(BLOCK2, BLOCK2);
	dim3 blockSz(THREAD2, THREAD2);

	int row = 0; //i - col


	host_vector<int> indices(0);
	host_vector<int> indices2(0);


	cudaEvent_t start, stop;
	float gpu_time = 0.0;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);



	for (int i = 0; i < m && row < n; ++i) {

		auto iter = matrAB_dev.begin() + i * n;


		auto max_el = thrust::max_element(
			iter + row, iter + n, abs_comp()
		);


		int max_id = max_el - iter;


		if (fabs(*max_el) <= 1e-7) {
			continue;
		}
		else {
			if (max_id != row) {
				fastSwap << <BLOCK, THREAD >> > (row, i, max_id, AB_ptr);
				CSC(cudaThreadSynchronize());
			}
		}



		triagStep << <gridSz, blockSz >> > (AB_ptr, row, i);


		CSC(cudaThreadSynchronize());

		triagKill << <BLOCK, THREAD >> > (AB_ptr, row, i);

		CSC(cudaThreadSynchronize());


		indices.push_back(i);
		indices2.push_back(row);
		++row;

	}

	matrAB_host = matrAB_dev;




	host_vector<bool> i_bool(m, false);
	device_vector<bool> i_bool_dev(m);

	int freeX = indices.size();

	for (int i = 0; i < freeX; ++i) {
		i_bool[indices[i]] = true;
	}


	i_bool_dev = i_bool;

	matrAB_dev = matrAB_host;

	AB_ptr = thrust::raw_pointer_cast(matrAB_dev.data());


	// reversing step
	for (int i = 0; i < freeX - 1; ++i) {
		int curRow = indices2[freeX - i - 1];
		int curCol = indices[freeX - i - 1];


		backStep << <gridSz, blockSz >> > (AB_ptr, curRow, curCol);

		//CSC(cudaThreadSynchronize());

		backKill << <BLOCK, THREAD >> > (AB_ptr, curRow, curCol);

		CSC(cudaThreadSynchronize());
	}


	device_vector<int> indices_dev;
	device_vector<int> indices2_dev;


	host_vector<double> diags(freeX);
	device_vector<double> diags_dev;

	matrAB_host = matrAB_dev;

	for (int i = 0; i < freeX; ++i) {
		diags[i] = matrAB_host[indices2[i] + n * indices[i]];
	}

	indices_dev = indices;
	indices2_dev = indices2;
	diags_dev = diags;

	int* i_ptr = thrust::raw_pointer_cast(indices_dev.data());
	int* i2_ptr = thrust::raw_pointer_cast(indices2_dev.data());
	double* d_ptr = thrust::raw_pointer_cast(diags_dev.data());

	//divide rights

	division << <gridSz, blockSz >> > (AB_ptr, d_ptr, i_ptr, i2_ptr, freeX);

	CSC(cudaThreadSynchronize());

	matrAB_host = matrAB_dev;


	//AtoE
	for (int i = 0; i < freeX; ++i) {
		int curRow = indices2[i];
		int curCol = indices[i];

		matrAB_host[curRow + curCol * n] = 1;
	}


	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&gpu_time, start, stop);

	// open log:
	ofstream log("logs.log", ios::app);
	log << "F" << endl;
	// threads
	log << BLOCK << "x" << THREAD << endl;
	// size:
	log << n << "x" << m << "x" << k << endl;
	// time:
	log << gpu_time << endl;
	log.close();





	int t = 0;
	//filling XMatrix
	
	for (int i = 0; i < m; ++i) {
		int curXRow;
		if (indices.size() > t) {
			curXRow = indices[t];
			if (i == curXRow) {
				for (int j = 0; j < k; ++j) {
					printf("%.10e ", matrAB_host[t + n * j + n * m]);
				}
				printf("\n");
				t += 1;
			}
			else {
				for (int j = 0; j < k; ++j) {
					printf("%.10e ", 0.0);
				}
				printf("\n");
			}
		}
		else {
			for (int j = 0; j < k; ++j) {
				printf("%.10e ", 0.0);
			}
			printf("\n");
		}

	}


	return 0;
}
