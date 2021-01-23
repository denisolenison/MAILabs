#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda.h>
#include <thrust/extrema.h>
#include <thrust/device_vector.h>
#include <iostream>
#include <mpi.h>
#include <string>
#include <algorithm>

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

int p1, p2, p3;
int g1, g2, g3;

// Index inside the block
#define _i(i, j, k) ((k + 1) * ((g2 + 2) * (g1 + 2)) + (j + 1) * (g1 + 2) + i + 1)
#define _ix(id) (((id) % (g1 + 2)) - 1)
#define _iy(id) ((((id) % ((g1 + 2) * (g2 + 2))) / (g1 + 2)) - 1)
#define _iz(id) (((id) / ((g1 + 2)*(g2 + 2))) - 1)

// Index by processes
#define _ib(i, j, k) ((k) * (p1 * p2) + (j) * p1 + (i))
#define _ibx(id) ((id) % p1)
#define _iby(id) (((id) % (p1 * p2)) / p1)
#define _ibz(id) ((id) / (p1 * p2))


#define printf(...) fprintf(File, __VA_ARGS__)



__global__ void ArrInits(double* fastData, double* fastNext, int g1, int g2, int g3, double U0) {
	int tX = blockIdx.x * blockDim.x + threadIdx.x;
	int tY = blockIdx.y * blockDim.y + threadIdx.y;
	int tZ = blockIdx.z * blockDim.z + threadIdx.z;
	int offsetX = gridDim.x * blockDim.x;
	int offsetY = gridDim.y * blockDim.y;
	int offsetZ = gridDim.z * blockDim.z;

	for (int a = tX - 1; a < g1 + 1; a += offsetX) {
		for (int b = tY - 1; b < g2 + 1; b += offsetY) {
			for (int c = tZ - 1; c < g3 + 1; c += offsetZ) {
				fastData[_i(a, b, c)] = U0;
				fastNext[_i(a, b, c)] = U0;
			}
		}
	}

}


//1 - left , 2 - right, 3 - up, 4 - down, 5 - front, 6 - back
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4
#define FRONT 5
#define BACK 6


__global__ void cuda_get_side(double* fastData, double* fastSide, int side, int g1, int g2, int g3) {
	int tX = blockIdx.x * blockDim.x + threadIdx.x;
	int tY = blockIdx.y * blockDim.y + threadIdx.y;
	int offsetX = gridDim.x * blockDim.x;
	int offsetY = gridDim.y * blockDim.y;
	if (side == LEFT) {
		for (int k = tX; k < g3; k += offsetX) {
			for (int j = tY; j < g2; j += offsetY) {
				fastSide[j + k * g2] = fastData[_i(0, j, k)];
			}
		}
	}
	else if (side == RIGHT) {
		for (int k = tX; k < g3; k += offsetX) {
			for (int j = tY; j < g2; j += offsetY) {
				fastSide[j + k * g2] = fastData[_i(g1 - 1, j, k)];
			}
		}
	}
	else if (side == UP) {
		for (int k = tX; k < g3; k += offsetX) {
			for (int i = tY; i < g1; i += offsetY) {
				fastSide[i + k * g1] = fastData[_i(i, 0, k)];
			}
		}
	}
	else if (side == DOWN) {
		for (int k = tX; k < g3; k += offsetX) {
			for (int i = tY; i < g1; i += offsetY) {
				fastSide[i + k * g1] = fastData[_i(i, g2 - 1, k)];
			}
		}
	}
	else if (side == FRONT) {
		for (int j = tX; j < g2; j += offsetX) {
			for (int i = tY; i < g1; i += offsetY) {
				fastSide[i + j * g1] = fastData[_i(i, j, 0)];
			}
		}
	}
	else {
		for (int j = tX; j < g2; j += offsetX) {
			for (int i = tY; i < g1; i += offsetY) {
				fastSide[i + j * g1] = fastData[_i(i, j, g3 - 1)];
			}
		}
	}
}


__global__ void cuda_set_side(double* fastData, double* fastSide, int side, int g1, int g2, int g3) {
	int tX = blockIdx.x * blockDim.x + threadIdx.x;
	int tY = blockIdx.y * blockDim.y + threadIdx.y;
	int offsetX = gridDim.x * blockDim.x;
	int offsetY = gridDim.y * blockDim.y;
	if (side == LEFT) {
		for (int k = tX; k < g3; k += offsetX) {
			for (int j = tY; j < g2; j += offsetY) {
				fastData[_i(-1, j, k)] = fastSide[j + k * g2];
			}
		}
	}
	else if (side == RIGHT) {
		for (int k = tX; k < g3; k += offsetX) {
			for (int j = tY; j < g2; j += offsetY) {
				fastData[_i(g1, j, k)] = fastSide[j + k * g2];
			}
		}
	}
	else if (side == UP) {
		for (int k = tX; k < g3; k += offsetX) {
			for (int i = tY; i < g1; i += offsetY) {
				fastData[_i(i, -1, k)] = fastSide[i + k * g1];
			}
		}
	}
	else if (side == DOWN) {
		for (int k = tX; k < g3; k += offsetX) {
			for (int i = tY; i < g1; i += offsetY) {
				fastData[_i(i, g2, k)] = fastSide[i + k * g1];
			}
		}
	}
	else if (side == FRONT) {
		for (int j = tX; j < g2; j += offsetX) {
			for (int i = tY; i < g1; i += offsetY) {
				fastData[_i(i, j, -1)] = fastSide[i + j * g1];
			}
		}
	}
	else {
		for (int j = tX; j < g2; j += offsetX) {
			for (int i = tY; i < g1; i += offsetY) {
				fastData[_i(i, j, g3)] = fastSide[i + j * g1];
			}
		}
	}
}


__global__ void cuda_side_edge_values(double* fastData, int side, int g1, int g2, int g3, double value) {
	int tX = blockIdx.x * blockDim.x + threadIdx.x;
	int tY = blockIdx.y * blockDim.y + threadIdx.y;
	int offsetX = gridDim.x * blockDim.x;
	int offsetY = gridDim.y * blockDim.y;
	if (side == LEFT) {
		for (int k = tX; k < g3; k += offsetX) {
			for (int j = tY; j < g2; j += offsetY) {
				fastData[_i(-1, j, k)] = value;
			}
		}
	}
	else if (side == RIGHT) {
		for (int k = tX; k < g3; k += offsetX) {
			for (int j = tY; j < g2; j += offsetY) {
				fastData[_i(g1, j, k)] = value;
			}
		}
	}
	else if (side == UP) {
		for (int k = tX; k < g3; k += offsetX) {
			for (int i = tY; i < g1; i += offsetY) {
				fastData[_i(i, -1, k)] = value;
			}
		}
	}
	else if (side == DOWN) {
		for (int k = tX; k < g3; k += offsetX) {
			for (int i = tY; i < g1; i += offsetY) {
				fastData[_i(i, g2, k)] = value;
			}
		}
	}
	else if (side == FRONT) {
		for (int j = tX; j < g2; j += offsetX) {
			for (int i = tY; i < g1; i += offsetY) {
				fastData[_i(i, j, -1)] = value;
			}
		}
	}
	else {
		for (int j = tX; j < g2; j += offsetX) {
			for (int i = tY; i < g1; i += offsetY) {
				fastData[_i(i, j, g3)] = value;
			}
		}
	}
}



__global__ void cuda_main_function(double* fastData, double* fastNext, int g1,
 int g2, int g3, double hx, double hy, double hz) {
	int tX = blockIdx.x * blockDim.x + threadIdx.x;
	int tY = blockIdx.y * blockDim.y + threadIdx.y;
	int tZ = blockIdx.z * blockDim.z + threadIdx.z;
	int offsetX = gridDim.x * blockDim.x;
	int offsetY = gridDim.y * blockDim.y;
	int offsetZ = gridDim.z * blockDim.z;

	for (int k = tZ; k < g3; k += offsetZ) {
		for (int j = tY; j < g2; j += offsetY) {
			for (int i = tX; i < g1; i += offsetX) {
				fastNext[_i(i, j, k)] = 0.5 * ((fastData[_i(i + 1, j, k)] + 
					fastData[_i(i - 1, j, k)]) / (hx * hx) + (fastData[_i(i, j + 1, k)]
					+ fastData[_i(i, j - 1, k)]) / (hy * hy) + (fastData[_i(i, j, k + 1)]
					+ fastData[_i(i, j, k - 1)]) / (hz * hz)) /
					(1.0 / (hx * hx) + 1.0 / (hy * hy) + 1.0 / (hz * hz));
			}
		}
	}
}

__global__ void cuda_error_function(double* fastData, double* fastNext, int g1, int g2, int g3) {
	int tX = blockIdx.x * blockDim.x + threadIdx.x;
	int tY = blockIdx.y * blockDim.y + threadIdx.y;
	int tZ = blockIdx.z * blockDim.z + threadIdx.z;
	int offsetX = gridDim.x * blockDim.x;
	int offsetY = gridDim.y * blockDim.y;
	int offsetZ = gridDim.z * blockDim.z;

	for (int k = tZ - 1; k < g3 + 1; k += offsetZ) {
		for (int j = tY - 1; j < g2 + 1; j += offsetY) {
			for (int i = tX - 1; i < g1 + 1; i += offsetX) {
				bool lolkekval = (i != -1 && j != -1 && k != -1) *
					(i != g1 && j != g2 && k != g3);
				fastData[_i(i, j, k)] = fabs(fastNext[_i(i, j, k)] - fastData[_i(i, j, k)]) * lolkekval;
			}
		}
	}
}


int main(int argc, char** argv) {
	std::ios::sync_with_stdio(false);
	string outFile;
	int fileNameL = 16;

	int deviceCount;
	cudaGetDeviceCount(&deviceCount);


	int id;
	int ib, jb, kb;
	int i, j, k, iter;
	int numproc, proc_name_len;
	int L = 16;

	char proc_name[MPI_MAX_PROCESSOR_NAME];

	double eps;
	double lx, ly, lz;
	double hx, hy, hz;
	double Udown, Uup, Uleft, Uright, Ufront, Uback;
	double U0;
	double * temp;

	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	MPI_Get_processor_name(proc_name, &proc_name_len);


	cudaSetDevice(id % deviceCount);

	if (id == 0) {
		cerr << "Found " << deviceCount << " devices\n";
	}


	MPI_Barrier(MPI_COMM_WORLD);

	string nullString = "";
	const char* outFileC = nullString.c_str();



	//input data for 0 process
	if (id == 0) {

		cin >> p1 >> p2 >> p3;
		cin >> g1 >> g2 >> g3;
		cin >> outFile;
		cin >> eps;
		cin >> lx >> ly >> lz;
		cin >> Ufront >> Uback >> Uleft >> Uright >> Uup >> Udown;
		cin >> U0;

		outFileC = outFile.c_str();
		fileNameL = strlen(outFileC) + 1;
	}


	MPI_Barrier(MPI_COMM_WORLD);

	//send data to all processes
	MPI_Bcast(&p1, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&p2, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&p3, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast(&g1, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&g2, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&g3, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast(&eps, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Bcast(&lx, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&ly, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&lz, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Bcast(&Udown, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Uup, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Uleft, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Uright, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Ufront, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Uback, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&U0, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&fileNameL, 1, MPI_INT, 0, MPI_COMM_WORLD);





	if (p1 * p2 * p3 != numproc) {
		MPI_Finalize();
		if (id == 0) {
			cout << "ERROR: proc.grid != processes\n";
		}

		return -1;
	}



	//block id by coordinates
	ib = _ibx(id);
	jb = _iby(id);
	kb = _ibz(id);

	iter = 0;

	//find hs
	hx = lx / ((double)p1 * (double)g1);
	hy = ly / ((double)p2 * (double)g2);
	hz = lz / ((double)p3 * (double)g3);

	char* buff = (char*)malloc(sizeof(char) * (g1 * g2 * g3 * p1 * p2 * p3 * L));


	double* fastData, * fastNext;

	double* fastLeftIn, * fastRightIn, * fastUPIn, * fastDownIn, * fastFrontIn, * fastBackIn;
	double* fastLeftOut, * fastRightOut, * fastUPOut, * fastDownOut, * fastFrontOut, * fastBackOut;

	double* LeftIn, * LeftOut, * RightIn, * RightOut;
	double* UPIn, * UPOut, * DownIn, * DownOut;
	double* FrontIn, * FrontOut, * BackIn, * BackOut;

	LeftIn = (double*)malloc(sizeof(double) * g2 * g3); LeftOut = (double*)malloc(sizeof(double) * g2 * g3);
	RightIn = (double*)malloc(sizeof(double) * g2 * g3); RightOut = (double*)malloc(sizeof(double) * g2 * g3);

	UPIn = (double*)malloc(sizeof(double) * g1 * g3); UPOut = (double*)malloc(sizeof(double) * g1 * g3);
	DownIn = (double*)malloc(sizeof(double) * g1 * g3); DownOut = (double*)malloc(sizeof(double) * g1 * g3);

	FrontIn = (double*)malloc(sizeof(double) * g1 * g2); FrontOut = (double*)malloc(sizeof(double) * g1 * g2);
	BackIn = (double*)malloc(sizeof(double) * g1 * g2); BackOut = (double*)malloc(sizeof(double) * g1 * g2);


	//Init GPU Memory
	CSC(cudaMalloc((void**)&fastData, sizeof(double)* (g1 + 2)* (g2 + 2)* (g3 + 2)));
	CSC(cudaMalloc((void**)&fastNext, sizeof(double)* (g1 + 2)* (g2 + 2)* (g3 + 2)));

	CSC(cudaMalloc((void**)& fastLeftIn, sizeof(double)* g2 * g3));
	CSC(cudaMalloc((void**)& fastRightIn, sizeof(double)* g2* g3));

	CSC(cudaMalloc((void**)& fastUPIn, sizeof(double)* g1* g3));
	CSC(cudaMalloc((void**)& fastDownIn, sizeof(double)* g1* g3));

	CSC(cudaMalloc((void**)& fastFrontIn, sizeof(double)* g1* g2));
	CSC(cudaMalloc((void**)& fastBackIn, sizeof(double)* g1* g2));

	CSC(cudaMalloc((void**)& fastLeftOut, sizeof(double)* g2* g3));
	CSC(cudaMalloc((void**)& fastRightOut, sizeof(double)* g2* g3));

	CSC(cudaMalloc((void**)& fastUPOut, sizeof(double)* g1* g3));
	CSC(cudaMalloc((void**)& fastDownOut, sizeof(double)* g1* g3));

	CSC(cudaMalloc((void**)& fastFrontOut, sizeof(double)* g1* g2));
	CSC(cudaMalloc((void**)& fastBackOut, sizeof(double)* g1* g2));

	int SIZE = 4;

	dim3 gridSz(SIZE, SIZE, SIZE);
	dim3 blockSz(SIZE, SIZE, SIZE);

	ArrInits << < gridSz, blockSz >> > (fastData, fastNext, g1, g2, g3, U0);

	//make buffer

	int buffer_size;

	MPI_Pack_size((g1 + 2) * (g2 + 2) * (g3 + 2), MPI_DOUBLE, MPI_COMM_WORLD, &buffer_size);

	buffer_size = 2 * (buffer_size + MPI_BSEND_OVERHEAD); //6 edges


	double* buffer = (double*)malloc(buffer_size);

	MPI_Buffer_attach(buffer, buffer_size);

	//block init

	//requests

	MPI_Request send_request1_1, recv_request1_1;
	MPI_Request send_request2_1, recv_request2_1;
	MPI_Request send_request3_1, recv_request3_1;

	MPI_Request send_request1_2, recv_request1_2;
	MPI_Request send_request2_2, recv_request2_2;
	MPI_Request send_request3_2, recv_request3_2;


	double* errors;
	errors = (double*)malloc(numproc * sizeof(double));

	//string debug_name = "process_debug" + to_string(id) + ".txt";

	int size2 = 8;

	dim3 blocks(size2, size2);
	dim3 threads(size2, size2);

	double maxErr = 0;
	do {
		//send and get data
		MPI_Barrier(MPI_COMM_WORLD);


		if (ib > 0) { //only left side
			cuda_get_side << < blocks, threads >> > (fastData, fastLeftIn, LEFT, g1, g2, g3);
			CSC(cudaGetLastError());
			CSC(cudaMemcpy(LeftIn, fastLeftIn, sizeof(double)* g2* g3, cudaMemcpyDeviceToHost));
			MPI_Isend(LeftIn, g2* g3, MPI_DOUBLE, _ib(ib - 1, jb, kb), 0, MPI_COMM_WORLD, &send_request1_1);
		}

		if (jb > 0) { //only up side
			cuda_get_side << < blocks, threads >> > (fastData, fastUPIn, UP, g1, g2, g3);
			CSC(cudaGetLastError());
			CSC(cudaMemcpy(UPIn, fastUPIn, sizeof(double)* g1* g3, cudaMemcpyDeviceToHost));
			MPI_Isend(UPIn, g1* g3, MPI_DOUBLE, _ib(ib, jb - 1, kb), 0, MPI_COMM_WORLD, &send_request2_2);
		}

		if (kb > 0) { //only front side
			cuda_get_side << < blocks, threads >> > (fastData, fastFrontIn, FRONT, g1, g2, g3);
			CSC(cudaGetLastError());
			CSC(cudaMemcpy(FrontIn, fastFrontIn, sizeof(double)* g1* g2, cudaMemcpyDeviceToHost));

			MPI_Isend(FrontIn, g1* g2, MPI_DOUBLE, _ib(ib, jb, kb - 1), 0, MPI_COMM_WORLD, &send_request3_2);
		}


		//wait for data

		if (ib > 0) { //only left side
			MPI_Wait(&send_request1_1, &status);
		}

		if (jb > 0) { //only up side
			MPI_Wait(&send_request2_2, &status);
		}

		if (kb > 0) { //only front side
			MPI_Wait(&send_request3_2, &status);
		}



		//set new data

		if (ib + 1 < p1) { //get right side
			MPI_Irecv(RightOut, g2 * g3, MPI_DOUBLE, _ib(ib + 1, jb, kb), 0, MPI_COMM_WORLD, &recv_request1_2);
			MPI_Wait(&recv_request1_2, &status);

			CSC(cudaMemcpy(fastRightOut, RightOut, sizeof(double)* g2* g3, cudaMemcpyHostToDevice));
			cuda_set_side << < blocks, threads >> > (fastData, fastRightOut, RIGHT, g1, g2, g3);
			CSC(cudaGetLastError());
		}
		else {
			cuda_side_edge_values << < blocks, threads >> > (fastData, RIGHT, g1, g2, g3, Uright);
			CSC(cudaGetLastError());
		}

		if (jb + 1 < p2) { //get down side
			MPI_Irecv(DownOut, g1* g3, MPI_DOUBLE, _ib(ib, jb + 1, kb), 0, MPI_COMM_WORLD, &recv_request2_1);
			MPI_Wait(&recv_request2_1, &status);

			CSC(cudaMemcpy(fastDownOut, DownOut, sizeof(double)* g1* g3, cudaMemcpyHostToDevice));
			cuda_set_side << < blocks, threads >> > (fastData, fastDownOut, DOWN, g1, g2, g3);
			CSC(cudaGetLastError());
		}
		else {
			cuda_side_edge_values << < blocks, threads >> > (fastData, DOWN, g1, g2, g3, Udown);
			CSC(cudaGetLastError());
		}


		if (kb + 1 < p3) { //get back side
			MPI_Irecv(BackOut, g1* g2, MPI_DOUBLE, _ib(ib, jb, kb + 1), 0, MPI_COMM_WORLD, &recv_request3_1);
			MPI_Wait(&recv_request3_1, &status);

			CSC(cudaMemcpy(fastBackOut, BackOut, sizeof(double)* g1* g2, cudaMemcpyHostToDevice));
			cuda_set_side << < blocks, threads >> > (fastData, fastBackOut, BACK, g1, g2, g3);
			CSC(cudaGetLastError());
		}
		else {
			cuda_side_edge_values << < blocks, threads >> > (fastData, BACK, g1, g2, g3, Uback);
			CSC(cudaGetLastError());
		}





		if (ib + 1 < p1) { //only right side
			cuda_get_side << < blocks, threads >> > (fastData, fastRightIn, RIGHT, g1, g2, g3);
			CSC(cudaGetLastError());
			CSC(cudaMemcpy(RightIn, fastRightIn, sizeof(double) * g2 * g3, cudaMemcpyDeviceToHost));
			MPI_Isend(RightIn, g2 * g3, MPI_DOUBLE, _ib(ib + 1, jb, kb), 0, MPI_COMM_WORLD, &send_request1_2);
		}

		if (jb + 1 < p2) { //only down side
			cuda_get_side << < blocks, threads >> > (fastData, fastDownIn, DOWN, g1, g2, g3);
			CSC(cudaGetLastError());
			CSC(cudaMemcpy(DownIn, fastDownIn, sizeof(double) * g1 * g3, cudaMemcpyDeviceToHost));
			MPI_Isend(DownIn, g1 * g3, MPI_DOUBLE, _ib(ib, jb + 1, kb), 0, MPI_COMM_WORLD, &send_request2_1);
		}

		if (kb + 1 < p3) { //only back side
			cuda_get_side << < blocks, threads >> > (fastData, fastBackIn, BACK, g1, g2, g3);
			CSC(cudaGetLastError());
			CSC(cudaMemcpy(BackIn, fastBackIn, sizeof(double) * g1 * g2, cudaMemcpyDeviceToHost));

			MPI_Isend(BackIn, g1 * g2, MPI_DOUBLE, _ib(ib, jb, kb + 1), 0, MPI_COMM_WORLD, &send_request3_1);
		}

		if (ib + 1 < p1) { //only right side
			MPI_Wait(&send_request1_2, &status);
		}
		if (jb + 1 < p2) { //only down side
			MPI_Wait(&send_request2_1, &status);
		}
		if (kb + 1 < p3) { //only back side
			MPI_Wait(&send_request3_1, &status);
		}



		if (ib > 0) { //get left side
			MPI_Irecv(LeftOut, g2* g3, MPI_DOUBLE, _ib(ib - 1, jb, kb), 0, MPI_COMM_WORLD, &recv_request1_1);
			MPI_Wait(&recv_request1_1, &status);

			CSC(cudaMemcpy(fastLeftOut, LeftOut, sizeof(double)* g2* g3, cudaMemcpyHostToDevice));
			cuda_set_side << < blocks, threads >> > (fastData, fastLeftOut, LEFT, g1, g2, g3);
			CSC(cudaGetLastError());
		}
		else {
			cuda_side_edge_values << < blocks, threads >> > (fastData, LEFT, g1, g2, g3, Uleft);
			CSC(cudaGetLastError());
		}


		if (jb > 0) { //get up side
			MPI_Irecv(UPOut, g1* g3, MPI_DOUBLE, _ib(ib, jb - 1, kb), 0, MPI_COMM_WORLD, &recv_request2_2);
			MPI_Wait(&recv_request2_2, &status);

			CSC(cudaMemcpy(fastUPOut, UPOut, sizeof(double)* g1* g3, cudaMemcpyHostToDevice));
			cuda_set_side << < blocks, threads >> > (fastData, fastUPOut, UP, g1, g2, g3);
			CSC(cudaGetLastError());
		}
		else {
			cuda_side_edge_values << < blocks, threads >> > (fastData, UP, g1, g2, g3, Uup);
			CSC(cudaGetLastError());
		}


		if (kb > 0) { //get front side
			MPI_Irecv(FrontOut, g1* g2, MPI_DOUBLE, _ib(ib, jb, kb - 1), 0, MPI_COMM_WORLD, &recv_request3_2);
			MPI_Wait(&recv_request3_2, &status);

			CSC(cudaMemcpy(fastFrontOut, FrontOut, sizeof(double)* g1* g2, cudaMemcpyHostToDevice));
			cuda_set_side << < blocks, threads >> > (fastData, fastFrontOut, FRONT, g1, g2, g3);
			CSC(cudaGetLastError());
		}
		else {
			cuda_side_edge_values << < blocks, threads >> > (fastData, FRONT, g1, g2, g3, Ufront);
			CSC(cudaGetLastError());
		}

		MPI_Barrier(MPI_COMM_WORLD);
		
		cuda_main_function << < gridSz, blockSz >> > (fastData, fastNext, g1, g2, g3, hx, hy, hz);

		cuda_error_function << < gridSz, blockSz >> > (fastData, fastNext, g1, g2, g3);

		thrust::device_ptr<double> d_ptr = thrust::device_pointer_cast(fastData);
		
		double epsTemp[1];

		epsTemp[0] = *(thrust::max_element(d_ptr, d_ptr + (g1 + 2)*(g2 + 2)*(g3+2)));


		MPI_Barrier(MPI_COMM_WORLD);


		MPI_Allgather(epsTemp, 1, MPI_DOUBLE, errors, 1, MPI_DOUBLE, MPI_COMM_WORLD);
		epsTemp[0] = 0;
		for (i = 0; i < numproc; ++i) {
			epsTemp[0] = max(epsTemp[0], errors[i]);
		}

		temp = fastNext;
		fastNext = fastData;
		fastData = temp;

		maxErr = epsTemp[0];

		iter += 1;

	} while (maxErr >= eps);


	//cout << iter << "\n";

	CSC(cudaFree(fastNext));

	double* data = (double*)malloc(sizeof(double) * (g1+2)*(g2+2)*(g3+2));

	CSC(cudaMemcpy(data, fastData, sizeof(double)*(g1+2)*(g2+2)*(g3+2), cudaMemcpyDeviceToHost));


	CSC(cudaFree(fastData));

	MPI_Barrier(MPI_COMM_WORLD);

	memset(buff, ' ', L * g1 * g2 * g3 * sizeof(char));

	for (k = 0; k < g3; ++k) {
		for (j = 0; j < g2; j++) {
			for (i = 0; i < g1; i++) {
				if ((i == 0 && j == 0 && (k != 0 || _iby(id) != 0 || _ibz(id) != 0)) && _ibx(id) == 0 && (_iby(id) > 0 || _ibz(id) > 0 || k > 0)) {
					sprintf(buff + (k * (g1 * g2) + j * g1 + i) * L, "\n\n%.6e ", data[_i(i, j, k)]);
				}
				else if (i == 0 && _ibx(id) == 0 && (j > 0 || k > 0)) {
					sprintf(buff + (k * (g1 * g2) + j * g1 + i) * L, "\n%.6e ", data[_i(i, j, k)]);
				}
				else {
					sprintf(buff + (k * (g1 * g2) + j * g1 + i) * L, "%.6e ", data[_i(i, j, k)]);
				}
			}
		}
	}
	for (i = 0; i < g1 * g2 * g3 * L; ++i) {
		if (buff[i] == '\0')
			buff[i] = ' ';
		//cout << buff[i];
	}



	MPI_Datatype fileCube;

	MPI_Datatype fileType;

	int count = g1 * g2 * g3;


	int* lens = new int[count];

	for (int i = 0; i < count; ++i) lens[i] = L;

	MPI_Aint* adrToFile = new MPI_Aint[count];

	MPI_Aint* adrInFile = new MPI_Aint[count];

	for (int k = 0; k < g3; ++k) {
		for (int j = 0; j < g2; ++j) {
			for (int i = 0; i < g1; ++i) {
				int CZ = g1 * g2 * g3;
				adrToFile[k * g2 * g1 + j * g1 + i] = (((_ibz(id) * p1 * p2 * CZ) + (k * p1 * g1 * p2 * g2)) +
					((_iby(id) * p1 * g1 * g2) + (j * p1 * g1)) + (_ibx(id) * g1 + i)) * sizeof(char) * L;
			}
		}
	}

	for (int k = 0; k < g3; ++k) {
		for (int j = 0; j < g2; ++j) {
			for (int i = 0; i < g1; ++i) {
				int t = k * g2 * g1 + j * g1 + i;
				adrInFile[t] = t * sizeof(char) * L;
			}
		}
	}


	MPI_Type_create_hindexed(count, lens, adrToFile, MPI_CHAR, &fileCube);
	MPI_Type_create_hindexed(count, lens, adrInFile, MPI_CHAR, &fileType);

	MPI_Type_commit(&fileCube);
	MPI_Type_commit(&fileType);


	if (id == 0) {
		cerr << "Process GRID: " << p1 << "x" << p2 << "x" << p3 << "\n";
		cerr << "Num GRID: " << g1 << "x" << g2 << "x" << g3 << "\n";
		cerr << "File name " << outFile << "\n";
		cerr << "Eps: " << eps << "\n";
		cerr << "lx: " << lx << " ly: " << ly << " lz: " << lz << "\n";
		cerr << "Us: " << Ufront << " , " << Uback << " , " << Uleft << " , " << Uright << " , " << Uup << " , " << Udown << "\n";
		cerr << "U0: " << U0 << "\n";
		cerr << "Iterations: " << iter << "\n";
	}

	MPI_File fp;

	MPI_Request* send_request = new MPI_Request[numproc - 1];
	MPI_Request* recv_request = new MPI_Request[numproc - 1];


	char* buffName = new char[fileNameL];

	if (id == 0) {
		for (int t = 1; t < numproc; ++t) {
			MPI_Isend(outFileC, fileNameL * sizeof(char), MPI_CHAR, t, 0, MPI_COMM_WORLD, &send_request[t - 1]);
			MPI_Wait(&send_request[t - 1], &status);
		}
		for (int x = 0; x < fileNameL; ++x) {
			buffName[x] = outFileC[x];;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (id != 0) {
		MPI_Irecv(buffName, fileNameL * sizeof(char), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &recv_request[id - 1]);
		MPI_Wait(&recv_request[id - 1], &status);
	}

	MPI_Barrier(MPI_COMM_WORLD);



	MPI_File_open(MPI_COMM_WORLD, buffName, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fp);

	MPI_File_set_view(fp, 0, MPI_CHAR, fileCube, "native", MPI_INFO_NULL);

	MPI_File_write(fp, buff, 1, fileType, &status);

	MPI_File_close(&fp);


	MPI_Barrier(MPI_COMM_WORLD);


	MPI_Finalize();


	free(data);
	free(buffer);

	return 0;
}