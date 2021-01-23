#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <algorithm>

#include <omp.h>
#include <mpi.h>

using namespace std;

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


int main(int argc, char** argv) {
	std::ios::sync_with_stdio(false);
	string outFile;

	int id;
	int ib, jb , kb;
	int i, j, k, iter;
	int numproc, proc_name_len;
	char proc_name[MPI_MAX_PROCESSOR_NAME];

	double eps;
	double lx, ly, lz;
	double hx, hy, hz;
	double Udown, Uup, Uleft, Uright, Ufront, Uback;
	double U0;
	double *data, *temp, *next;
	double *bufferIString;

	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	MPI_Get_processor_name(proc_name, &proc_name_len);

	MPI_Barrier(MPI_COMM_WORLD);

	//input data for 0 process
	if (id == 0) {
		
		cin >> p1 >> p2 >> p3;
		cin >> g1 >> g2 >> g3;
		cin >> outFile;
		cin >> eps;
		cin >> lx >> ly >> lz;
		cin >> Ufront >> Uback >> Uleft >> Uright >> Uup >> Udown;
		cin >> U0;

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

	int max_threads = omp_get_max_threads(); //calculate max threads

	data = (double*)malloc(sizeof(double) * (g1 + 2) * (g2 + 2) * (g3 + 2));
	next = (double*)malloc(sizeof(double) * (g1 + 2) * (g2 + 2) * (g3 + 2));

	//datatypes (NEW)

	MPI_Datatype sendLeft, sendRight, recvLeft, recvRight;
	MPI_Datatype sendUP, sendDown, recvUP, recvDown;
	MPI_Datatype sendFront, sendBack, recvFront, recvBack;

	int countFB = g1 * g2;
	int countUD = g1 * g3;
	int countLR = g2 * g3;

	int* lenFB = new int[countFB];
	int* lenUD = new int[countUD];
	int* lenLR = new int[countLR];

	for (int i = 0; i < countFB; ++i) lenFB[i] = 1;
	for (int i = 0; i < countUD; ++i) lenUD[i] = 1;
	for (int i = 0; i < countLR; ++i) lenLR[i] = 1;

	MPI_Aint* adr_sLeft = new MPI_Aint[countLR];
	MPI_Aint* adr_rLeft = new MPI_Aint[countLR];
	MPI_Aint* adr_sRight = new MPI_Aint[countLR];
	MPI_Aint* adr_rRight = new MPI_Aint[countLR];

	MPI_Aint* adr_sUP = new MPI_Aint[countUD];
	MPI_Aint* adr_rUP = new MPI_Aint[countUD];
	MPI_Aint* adr_sDown = new MPI_Aint[countUD];
	MPI_Aint* adr_rDown = new MPI_Aint[countUD];

	MPI_Aint* adr_sFront = new MPI_Aint[countFB];
	MPI_Aint* adr_rFront = new MPI_Aint[countFB];
	MPI_Aint* adr_sBack = new MPI_Aint[countFB];
	MPI_Aint* adr_rBack = new MPI_Aint[countFB];

	for (int k = 0; k < g3; ++k) {
		for (int j = 0; j < g2; ++j) {
			adr_sLeft[k * g2 + j] = _i(0, j, k) * sizeof(double);
			adr_rLeft[k * g2 + j] = _i(-1, j, k) * sizeof(double);
			adr_sRight[k * g2 + j] = _i(g1 - 1, j, k) * sizeof(double);
			adr_rRight[k * g2 + j] = _i(g1, j, k) * sizeof(double);
		}
	}

	for (int k = 0; k < g3; ++k) {
		for (int i = 0; i < g1; ++i) {
			adr_sUP[k * g1 + i] = _i(i, 0, k) * sizeof(double);
			adr_rUP[k * g1 + i] = _i(i, -1, k) * sizeof(double);
			adr_sDown[k * g1 + i] = _i(i, g2 - 1, k) * sizeof(double);
			adr_rDown[k * g1 + i] = _i(i, g2, k) * sizeof(double);
		}
	}

	for (int j = 0; j < g2; ++j) {
		for (int i = 0; i < g1; ++i) {
			adr_sFront[j * g1 + i] = _i(i, j, 0) * sizeof(double);
			adr_rFront[j * g1 + i] = _i(i, j, -1) * sizeof(double);
			adr_sBack[j * g1 + i] = _i(i, j, g3 - 1) * sizeof(double);
			adr_rBack[j * g1 + i] = _i(i, j, g3) * sizeof(double);
		}
	}

	MPI_Type_create_hindexed(countFB, lenFB, adr_sFront, MPI_DOUBLE, &sendFront);
	MPI_Type_create_hindexed(countFB, lenFB, adr_rFront, MPI_DOUBLE, &recvFront);
	MPI_Type_create_hindexed(countFB, lenFB, adr_sBack, MPI_DOUBLE, &sendBack);
	MPI_Type_create_hindexed(countFB, lenFB, adr_rBack, MPI_DOUBLE, &recvBack);

	MPI_Type_create_hindexed(countUD, lenUD, adr_sUP, MPI_DOUBLE, &sendUP);
	MPI_Type_create_hindexed(countUD, lenUD, adr_rUP, MPI_DOUBLE, &recvUP);
	MPI_Type_create_hindexed(countUD, lenUD, adr_sDown, MPI_DOUBLE, &sendDown);
	MPI_Type_create_hindexed(countUD, lenUD, adr_rDown, MPI_DOUBLE, &recvDown);

	MPI_Type_create_hindexed(countLR, lenLR, adr_sLeft, MPI_DOUBLE, &sendLeft);
	MPI_Type_create_hindexed(countLR, lenLR, adr_rLeft, MPI_DOUBLE, &recvLeft);
	MPI_Type_create_hindexed(countLR, lenLR, adr_sRight, MPI_DOUBLE, &sendRight);
	MPI_Type_create_hindexed(countLR, lenLR, adr_rRight, MPI_DOUBLE, &recvRight);


	MPI_Type_commit(&sendFront); MPI_Type_commit(&recvFront); MPI_Type_commit(&sendBack); MPI_Type_commit(&recvBack);
	MPI_Type_commit(&sendUP); MPI_Type_commit(&recvUP); MPI_Type_commit(&sendDown); MPI_Type_commit(&recvDown);
	MPI_Type_commit(&sendLeft); MPI_Type_commit(&recvLeft); MPI_Type_commit(&sendRight); MPI_Type_commit(&recvRight);

	bufferIString = (double*)malloc(sizeof(double) * g1);  // wall3

	//make buffer

	int buffer_size;

	MPI_Pack_size((g1 + 2) * (g2 + 2) * (g3 + 2), MPI_DOUBLE, MPI_COMM_WORLD, &buffer_size);

	buffer_size = 2 * (buffer_size + MPI_BSEND_OVERHEAD); //6 edges


	double* buffer = (double*)malloc(buffer_size);

	double* epsTemp = new double[max_threads];
	
	MPI_Buffer_attach(buffer, buffer_size);

	//block init
	for (int a = 0; a < g1; ++a) {
		for (int b = 0; b < g2; ++b) {
			for (int c = 0; c < g3; ++c) {
				data[_i(a, b, c)] = U0;
			}
		}
	}
	//requests
	MPI_Request send_request1, recv_request1; //output

	MPI_Request send_request1_1, recv_request1_1;
	MPI_Request send_request2_1, recv_request2_1;
	MPI_Request send_request3_1, recv_request3_1;

	MPI_Request send_request1_2, recv_request1_2;
	MPI_Request send_request2_2, recv_request2_2;
	MPI_Request send_request3_2, recv_request3_2;


	double* errors;
	errors = (double*)malloc(numproc * sizeof(double));

	int* i_start, * j_start, * k_start;

	k_start = (int*)malloc(sizeof(int) * 2);
	j_start = (int*)malloc(sizeof(int) * 2);
	i_start = (int*)malloc(sizeof(int) * 2);
	//string debug_name = "process_debug" + to_string(id) + ".txt";


	omp_set_dynamic(0); // OMP static threads
	double maxErr = 0;


	do  {
		//send and get data
		MPI_Barrier(MPI_COMM_WORLD);

		for (int yy = 0; yy < max_threads; ++yy) {
			epsTemp[yy] = 0;
		}
		
		if (ib > 0 && ib + 1 < p1) { //both left and right
			MPI_Isend(data, 1, sendLeft, _ib(ib - 1, jb, kb), 0, MPI_COMM_WORLD, &send_request1_1);
			MPI_Isend(data, 1, sendRight, _ib(ib + 1, jb, kb), 0, MPI_COMM_WORLD, &send_request1_2);
		}
		else if (ib > 0) { //only left side
			MPI_Isend(data, 1, sendLeft, _ib(ib - 1, jb, kb), 0, MPI_COMM_WORLD, &send_request1_1);
		}
		else if (ib + 1 < p1) { //only right side
			MPI_Isend(data, 1, sendRight, _ib(ib + 1, jb, kb), 0, MPI_COMM_WORLD, &send_request1_2);
		}


		if (jb + 1 < p2 && jb > 0) { //both down and up
			MPI_Isend(data, 1, sendUP, _ib(ib, jb - 1, kb), 0, MPI_COMM_WORLD, &send_request2_2);
			MPI_Isend(data, 1, sendDown, _ib(ib, jb + 1, kb), 0, MPI_COMM_WORLD, &send_request2_1);
		}
		else if (jb > 0) { //only up side
			MPI_Isend(data, 1, sendUP, _ib(ib, jb - 1, kb), 0, MPI_COMM_WORLD, &send_request2_2);
		}
		else if (jb + 1 < p2) { //only down side
			MPI_Isend(data, 1, sendDown, _ib(ib, jb + 1, kb), 0, MPI_COMM_WORLD, &send_request2_1);
		}


		if (kb + 1 < p3 && kb > 0) { //both back and front
			MPI_Isend(data, 1, sendFront, _ib(ib, jb, kb - 1), 0, MPI_COMM_WORLD, &send_request3_2);
			MPI_Isend(data, 1, sendBack, _ib(ib, jb, kb + 1), 0, MPI_COMM_WORLD, &send_request3_1);
		}
		else if (kb > 0) { //only front side
			MPI_Isend(data, 1, sendFront, _ib(ib, jb, kb - 1), 0, MPI_COMM_WORLD, &send_request3_2);
		}
		else if (kb + 1 < p3) { //only back side
			MPI_Isend(data, 1, sendBack, _ib(ib, jb, kb + 1), 0, MPI_COMM_WORLD, &send_request3_1);
		}


		//while wait for data

		//iterational function (inside of blocks)
		#pragma omp parallel
		{
			int threads = omp_get_num_threads();
			int thread_id = omp_get_thread_num();

			//-2 because we don't check edges
			#define ijk_next(i, j, k, diff) { \
                i += diff; \
                while(i > g1 - 2){ \
                    i -= g1 - 2; \
                    ++j; \
                } \
                while(j > g2 - 2){ \
                    j -= g2 - 2; \
                    ++k; \
                } \
            } \

			int i_ = 1;
			int j_ = 1;
			int k_ = 1;
			ijk_next(i_, j_, k_, thread_id); //like in CUDA


			while (k_ < g3 - 1) {
				next[_i(i_, j_, k_)] = 0.5 * ((data[_i(i_ + 1, j_, k_)] + data[_i(i_ - 1, j_, k_)]) / (hx * hx) +
					(data[_i(i_, j_ + 1, k_)] + data[_i(i_, j_ - 1, k_)]) / (hy * hy) +
					(data[_i(i_, j_, k_ + 1)] + data[_i(i_, j_, k_ - 1)]) / (hz * hz)) /
					(1.0 / (hx * hx) + 1.0 / (hy * hy) + 1.0 / (hz * hz));

				epsTemp[thread_id] = max(epsTemp[thread_id], abs(next[_i(i_, j_, k_)] - data[_i(i_, j_, k_)]));
				ijk_next(i_, j_, k_, threads);
			}
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

		//getting 1

		if (ib + 1 < p1) { //get right side
			MPI_Irecv(data, 1, recvRight, _ib(ib + 1, jb, kb), 0, MPI_COMM_WORLD, &recv_request1_2);
			MPI_Wait(&recv_request1_2, &status);
		}

		if (jb + 1 < p2) { //get down side
			MPI_Irecv(data, 1, recvDown, _ib(ib, jb + 1, kb), 0, MPI_COMM_WORLD, &recv_request2_1);
			MPI_Wait(&recv_request2_1, &status);
		}

		if (kb + 1 < p3) { //get back side
			MPI_Irecv(data, 1, recvBack, _ib(ib, jb, kb + 1), 0, MPI_COMM_WORLD, &recv_request3_1);
			MPI_Wait(&recv_request3_1, &status);
		}

		//parallel receiving step 1
		#pragma omp parallel
		{
			int threads = omp_get_num_threads();
			int thread_id = omp_get_thread_num();

			#define ij_next(i, j, diff) { \
                i += diff; \
                while(i > g1 - 1){ \
                    i -= g1; \
                    ++j; \
                } \
            } \

			#define ik_next(i, k, diff) { \
                i += diff; \
                while(i > g1 - 1){ \
                    i -= g1; \
                    ++k; \
                } \
            } \

			#define jk_next(j, k, diff) { \
                j += diff; \
                while(j > g2 - 1){ \
                    j -= g2; \
                    ++k; \
                } \
            } \

			//set new data
			int i_ = 0;
			int j_ = 0; //int helps this variable be local for each thread
			int k_ = 0;

			jk_next(j_, k_, thread_id);

			if (ib + 1 < p1) { //get right side
			}
			else {
				while (k_ < g3) {
					data[_i(g1, j_, k_)] = Uright;
					next[_i(g1, j_, k_)] = Uright;
					jk_next(j_, k_, threads);
				}
			}
			
			i_ = 0;
			k_ = 0;
			ik_next(i_, k_, thread_id);

			if (jb + 1 < p2) { //get down side
			}
			else {
				while (k_ < g3) {
					data[_i(i_, g2, k_)] = Udown;
					next[_i(i_, g2, k_)] = Udown;
					ik_next(i_, k_, threads);
				}
			}

			i_ = 0;
			j_ = 0;
			ij_next(i_, j_, thread_id);

			if (kb + 1 < p3) { //get back side
			}
			else {
				while (j_ < g2) {
					data[_i(i_, j_, g3)] = Uback;
					next[_i(i_, j_, g3)] = Uback;
					ij_next(i_, j_, threads);
				}
			}
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

		//getting 2

		if (ib > 0) { //get left side
			MPI_Irecv(data, 1, recvLeft, _ib(ib - 1, jb, kb), 0, MPI_COMM_WORLD, &recv_request1_1);
			MPI_Wait(&recv_request1_1, &status);
		}


		if (jb > 0) { //get up side
			MPI_Irecv(data, 1, recvUP, _ib(ib, jb - 1, kb), 0, MPI_COMM_WORLD, &recv_request2_2);
			MPI_Wait(&recv_request2_2, &status);
		}


		if (kb > 0) { //get front side
			MPI_Irecv(data, 1, recvFront, _ib(ib, jb, kb - 1), 0, MPI_COMM_WORLD, &recv_request3_2);
			MPI_Wait(&recv_request3_2, &status);
		}



		//parallel receiving step 2
		#pragma omp parallel
		{
			int threads = omp_get_num_threads();
			int thread_id = omp_get_thread_num();

			#define ij_next(i, j, diff) { \
                i += diff; \
                while(i > g1 - 1){ \
                    i -= g1; \
                    ++j; \
                } \
            } \

			#define ik_next(i, k, diff) { \
                i += diff; \
                while(i > g1 - 1){ \
                    i -= g1; \
                    ++k; \
                } \
            } \

			#define jk_next(j, k, diff) { \
                j += diff; \
                while(j > g2 - 1){ \
                    j -= g2; \
                    ++k; \
                } \
            } \

			int i_ = 0;
			int j_ = 0;
			int k_ = 0;

			jk_next(j_, k_, thread_id);

			if (ib > 0) { //get left side
			}
			else {
				while (k_ < g3) {
					data[_i(-1, j_, k_)] = Uleft;
					next[_i(-1, j_, k_)] = Uleft;
					jk_next(j_, k_, threads);
				}
			}


			i_ = 0;
			k_ = 0;
			ik_next(i_, k_, thread_id);

			if (jb > 0) { //get up side
			}
			else {
				while (k_ < g3) {
					data[_i(i_, -1, k_)] = Uup;
					next[_i(i_, -1, k_)] = Uup;
					ik_next(i_, k_, threads);
				}
			}


			i_ = 0;
			j_ = 0;
			ij_next(i_, j_, thread_id);


			if (kb > 0) { //get front side
			}
			else {
				while (j_ < g2) {
					data[_i(i_, j_, -1)] = Ufront;
					next[_i(i_, j_, -1)] = Ufront;
					ij_next(i_, j_, threads);
				}
			}


		}
			
		//for edges
		


		k_start[0] = 0;
		k_start[1] = g3 - 1;

		j_start[0] = 0;
		j_start[1] = g2 - 1;

		i_start[0] = 0;
		i_start[1] = g1 - 1;
		

		
		
		#pragma omp parallel
		{
			//parallel calculate edges
			int threads = omp_get_num_threads();
			int thread_id = omp_get_thread_num();

			#define ij_next(i, j, diff) { \
				i += diff; \
				while(i > g1 - 1){ \
					i -= g1; \
					++j; \
				} \
			} \

			#define ik_next(i, k, diff) { \
				i += diff; \
				while(i > g1 - 1){ \
					i -= g1; \
					++k; \
				} \
			} \

			#define jk_next(j, k, diff) { \
				j += diff; \
				while(j > g2 - 1){ \
					j -= g2; \
					++k; \
				} \
			} \


			int i_ = 0;
			int j_ = 0;
			int k_ = 0;


			//k-edges
			for (int k_s = 0; k_s < 2; ++k_s) {
				k_ = k_start[k_s];
				i_ = 0;
				j_ = 0;
				ij_next(i_, j_, thread_id);

				while (j_ < g2) {
					next[_i(i_, j_, k_)] = 0.5 * ((data[_i(i_ + 1, j_, k_)] + data[_i(i_ - 1, j_, k_)]) / (hx * hx) +
						(data[_i(i_, j_ + 1, k_)] + data[_i(i_, j_ - 1, k_)]) / (hy * hy) +
						(data[_i(i_, j_, k_ + 1)] + data[_i(i_, j_, k_ - 1)]) / (hz * hz)) /
						(1.0 / (hx * hx) + 1.0 / (hy * hy) + 1.0 / (hz * hz));
					epsTemp[thread_id] = max(epsTemp[thread_id], abs(next[_i(i_, j_, k_)] - data[_i(i_, j_, k_)]));

					ij_next(i_, j_, threads);
				}

			}
			

			//j-edges
			for (int j_s = 0; j_s < 2; ++j_s) {
				j_ = j_start[j_s];
				i_ = 0;
				k_ = 0;
				ik_next(i_, k_, thread_id);

				while (k_ < g3) {
					next[_i(i_, j_, k_)] = 0.5 * ((data[_i(i_ + 1, j_, k_)] + data[_i(i_ - 1, j_, k_)]) / (hx * hx) +
						(data[_i(i_, j_ + 1, k_)] + data[_i(i_, j_ - 1, k_)]) / (hy * hy) +
						(data[_i(i_, j_, k_ + 1)] + data[_i(i_, j_, k_ - 1)]) / (hz * hz)) /
						(1.0 / (hx * hx) + 1.0 / (hy * hy) + 1.0 / (hz * hz));
					epsTemp[thread_id] = max(epsTemp[thread_id], abs(next[_i(i_, j_, k_)] - data[_i(i_, j_, k_)]));

					ik_next(i_, k_, threads);
				}
			}


			//i-edges
			for (int i_s = 0; i_s < 2; ++i_s) {
				i_ = i_start[i_s];
				j_ = 0;
				k_ = 0;
				jk_next(j_, k_, thread_id);

				while (k_ < g3) {
					next[_i(i_, j_, k_)] = 0.5 * ((data[_i(i_ + 1, j_, k_)] + data[_i(i_ - 1, j_, k_)]) / (hx * hx) +
						(data[_i(i_, j_ + 1, k_)] + data[_i(i_, j_ - 1, k_)]) / (hy * hy) +
						(data[_i(i_, j_, k_ + 1)] + data[_i(i_, j_, k_ - 1)]) / (hz * hz)) /
						(1.0 / (hx * hx) + 1.0 / (hy * hy) + 1.0 / (hz * hz));
					epsTemp[thread_id] = max(epsTemp[thread_id], abs(next[_i(i_, j_, k_)] - data[_i(i_, j_, k_)]));

					jk_next(j_, k_, threads);
				}
			}

		}




		double maxE[1];
		maxE[0] = 0;

		for (int t = 0; t < max_threads; ++t) {
			maxE[0] = max(maxE[0], epsTemp[t]);
		}


		MPI_Allgather(maxE, 1, MPI_DOUBLE, errors, 1, MPI_DOUBLE, MPI_COMM_WORLD);
		maxE[0] = 0;
		for (i = 0; i < numproc; ++i) {
			maxE[0] = max(maxE[0], errors[i]);
		}


		temp = next;
		next = data;
		data = temp;

		maxErr = maxE[0];
		
		iter += 1;

	}while (maxErr >= eps);

	//fclose(File);
	//cout << iter << "\n";

	MPI_Barrier(MPI_COMM_WORLD);
	
	if (id != 0) {
		for (k = 0 ; k < g3 ; ++k) {
			for (j = 0; j < g2; ++j) {
				for (i = 0; i < g1; ++i) {
					bufferIString[i] = data[_i(i, j, k)];
				}
				MPI_Isend(bufferIString, g1, MPI_DOUBLE, 0, id, MPI_COMM_WORLD, &send_request1);
				MPI_Wait(&send_request1, &status);
			}
		}
	}
	else {
		cerr << "Process GRID: " << p1 << "x" << p2 << "x" << p3 << "\n";
		cerr << "Num GRID: " << g1 << "x" << g2 << "x" << g3 << "\n";
		cerr << "Eps: " << eps << "\n";
		cerr << "lx: " << lx << " ly: " << ly << " lz: " << lz << "\n";
		cerr << "Us: " << Ufront << " , " << Uback << " , " << Uleft << " , " << Uright << " , " << Uup << " , " << Udown << "\n";
		cerr << "U0: " << U0 << "\n";
		cerr << "Iterations: " << iter << "\n";

		FILE* File = fopen(outFile.c_str(), "w+");

		for (kb = 0; kb < p3; ++kb) {
			for (k = 0; k < g3; ++k) {
				for (jb = 0; jb < p2; ++jb) {
					for (j = 0; j < g2; ++j) {
						for (ib = 0; ib < p1; ++ib) {
							if (_ib(ib, jb, kb) == 0) {
								for (i = 0; i < g1; ++i) {
									bufferIString[i] = data[_i(i, j, k)];
									printf("%.6e ", bufferIString[i]);
								}
								if (ib + 1 == p1) {
									printf("\n");
									if (j + 1 == g2) {
										printf("\n");
									}
								}
							}
							else {
								MPI_Irecv(bufferIString, g1, MPI_DOUBLE, _ib(ib, jb, kb), _ib(ib, jb, kb), MPI_COMM_WORLD, &recv_request1);
								MPI_Wait(&recv_request1, &status);
								for (i = 0; i < g1; ++i) {
									printf("%.6e ", bufferIString[i]);
								}
								if (ib + 1 == p1) {
									printf("\n");
									if (j + 1 == g2) {
										printf("\n");
									}
								}
							}
						}
					}
				}
			}
		}
		fclose(File);
	}
	
	MPI_Buffer_detach(buffer, &buffer_size);


	MPI_Finalize();

	free(lenFB);
	free(lenUD);
	free(lenLR);

	free(adr_sLeft);
	free(adr_rLeft);
	free(adr_sRight);
	free(adr_rRight);

	free(adr_sUP);
	free(adr_rUP);
	free(adr_sDown);
	free(adr_rDown);

	free(adr_sFront);
	free(adr_rFront);
	free(adr_sBack);
	free(adr_rBack);

	free(data);
	free(next);
	free(buffer);

	return 0;
}