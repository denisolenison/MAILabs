#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <iostream>
#include <string>
#include <algorithm>

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
	double *bufferUPOut, *bufferRightOut, *bufferFrontOut;
	double* bufferUPIn, * bufferRightIn, * bufferFrontIn;
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


	data = (double*)malloc(sizeof(double) * (g1 + 2) * (g2 + 2) * (g3 + 2));
	next = (double*)malloc(sizeof(double) * (g1 + 2) * (g2 + 2) * (g3 + 2));

	bufferFrontOut = (double*)malloc(sizeof(double) * (g1 + 2) * (g2 + 2));  // wall1
	bufferRightOut = (double*)malloc(sizeof(double) * (g2 + 2) * (g3 + 2));  // wall2
	bufferUPOut = (double*)malloc(sizeof(double) * (g1 + 2) * (g3 + 2));  // wall3

	bufferFrontIn = (double*)malloc(sizeof(double) * (g1 + 2) * (g2 + 2));  // wall1
	bufferRightIn = (double*)malloc(sizeof(double) * (g2 + 2) * (g3 + 2));  // wall2
	bufferUPIn = (double*)malloc(sizeof(double) * (g1 + 2) * (g3 + 2));  // wall3

	bufferIString = (double*)malloc(sizeof(double) * g1);  // wall3

	//make buffer

	int buffer_size;

	MPI_Pack_size((g1 + 2) * (g2 + 2) * (g3 + 2), MPI_DOUBLE, MPI_COMM_WORLD, &buffer_size);

	buffer_size = 2 * (buffer_size + MPI_BSEND_OVERHEAD); //6 edges


	double* buffer = (double*)malloc(buffer_size);
	
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

	//string debug_name = "process_debug" + to_string(id) + ".txt";



	double maxErr = 0;
	do  {
		//send and get data
		MPI_Barrier(MPI_COMM_WORLD);


		if (ib > 0 && ib + 1 < p1) { //both left and right
			for (k = 0; k < g3; ++k) {
				for (j = 0; j < g2; ++j) {
					bufferRightIn[j + k * g2] = data[_i(0, j, k)];
					bufferRightOut[j + k * g2] = data[_i(g1 - 1, j, k)];
				}
			}
			MPI_Isend(bufferRightIn, g2* g3, MPI_DOUBLE, _ib(ib - 1, jb, kb), 0, MPI_COMM_WORLD, &send_request1_1);
			MPI_Isend(bufferRightOut, g2* g3, MPI_DOUBLE, _ib(ib + 1, jb, kb), 0, MPI_COMM_WORLD, &send_request1_2);
		}
		else if (ib > 0) { //only left side
			for (k = 0; k < g3; ++k) {
				for (j = 0; j < g2; ++j) {
					bufferRightIn[j + k * g2] = data[_i(0, j, k)];
				}
			}
			MPI_Isend(bufferRightIn, g2 * g3, MPI_DOUBLE, _ib(ib - 1, jb, kb), 0, MPI_COMM_WORLD, &send_request1_1);
		}
		else if (ib + 1 < p1) { //only right side
			for (k = 0; k < g3; ++k) {
				for (j = 0; j < g2; ++j) {
					bufferRightOut[j + k * g2] = data[_i(g1 - 1, j, k)];
				}
			}
			MPI_Isend(bufferRightOut, g2 * g3, MPI_DOUBLE, _ib(ib + 1, jb, kb), 0, MPI_COMM_WORLD, &send_request1_2);
		}


		if (jb + 1 < p2 && jb > 0) { //both down and up
			for (k = 0; k < g3; ++k) {
				for (i = 0; i < g1; ++i) {
					bufferUPIn[i + k * g1] = data[_i(i, 0, k)];
					bufferUPOut[i + k * g1] = data[_i(i, g2 - 1, k)];
				}
			}
			MPI_Isend(bufferUPIn, g1* g3, MPI_DOUBLE, _ib(ib, jb - 1, kb), 0, MPI_COMM_WORLD, &send_request2_2);
			MPI_Isend(bufferUPOut, g1* g3, MPI_DOUBLE, _ib(ib, jb + 1, kb), 0, MPI_COMM_WORLD, &send_request2_1);
		}
		else if (jb > 0) { //only up side
			for (k = 0; k < g3; ++k) {
				for (i = 0; i < g1; ++i) {
					bufferUPIn[i + k * g1] = data[_i(i, 0, k)];
				}
			}
			MPI_Isend(bufferUPIn, g1 * g3, MPI_DOUBLE, _ib(ib, jb - 1, kb), 0, MPI_COMM_WORLD, &send_request2_2);
		}
		else if (jb + 1 < p2) { //only down side
			for (k = 0; k < g3; ++k) {
				for (i = 0; i < g1; ++i) {
					bufferUPOut[i + k * g1] = data[_i(i, g2 - 1, k)];
				}
			}
			MPI_Isend(bufferUPOut, g1* g3, MPI_DOUBLE, _ib(ib, jb + 1, kb), 0, MPI_COMM_WORLD, &send_request2_1);
		}


		if (kb + 1 < p3 && kb > 0) { //both back and front
			for (j = 0; j < g2; ++j) {
				for (i = 0; i < g1; ++i) {
					bufferFrontIn[i + j * g1] = data[_i(i, j, 0)];
					bufferFrontOut[i + j * g1] = data[_i(i, j, g3 - 1)];
				}
			}
			MPI_Isend(bufferFrontIn, g1* g2, MPI_DOUBLE, _ib(ib, jb, kb - 1), 0, MPI_COMM_WORLD, &send_request3_2);
			MPI_Isend(bufferFrontOut, g1* g2, MPI_DOUBLE, _ib(ib, jb, kb + 1), 0, MPI_COMM_WORLD, &send_request3_1);
		}
		else if (kb > 0) { //only front side
			for (j = 0; j < g2; ++j) {
				for (i = 0; i < g1; ++i) {
					bufferFrontIn[i + j * g1] = data[_i(i, j, 0)];
				}
			}

			cerr << "ITERATION " << iter << "\n";
			for (int a = 0; a < g2; ++a) {
				for (int b = 0; b < g1; ++b) {
					cerr << bufferFrontIn[b + a * g1] << " ";
				}
				cerr << "\n";
			}


			MPI_Isend(bufferFrontIn, g1* g2, MPI_DOUBLE, _ib(ib, jb, kb - 1), 0, MPI_COMM_WORLD, &send_request3_2);
		}
		else if (kb + 1 < p3) { //only back side
			for (j = 0; j < g2; ++j) {
				for (i = 0; i < g1; ++i) {
					bufferFrontOut[i + j * g1] = data[_i(i, j, g3 - 1)];
				}
			}
			MPI_Isend(bufferFrontOut, g1 * g2, MPI_DOUBLE, _ib(ib, jb, kb + 1), 0, MPI_COMM_WORLD, &send_request3_1);
		}


		//while wait for data

		//iterational function
		double epsTemp[1];
		epsTemp[0] = 0;

		for (k = 1; k < g3 - 1; ++k) {
			for (j = 1; j < g2 - 1; ++j) {
				for (i = 1; i < g1 - 1; ++i) {
					next[_i(i, j, k)] = 0.5 * ((data[_i(i + 1, j, k)] + data[_i(i - 1, j, k)]) / (hx * hx) +
						(data[_i(i, j + 1, k)] + data[_i(i, j - 1, k)]) / (hy * hy) +
						(data[_i(i, j, k + 1)] + data[_i(i, j, k - 1)]) / (hz * hz)) /
						(1.0 / (hx * hx) + 1.0 / (hy * hy) + 1.0 / (hz * hz));
					epsTemp[0] = max(epsTemp[0], abs(next[_i(i, j, k)] - data[_i(i, j, k)]));
				}
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



		//set new data

		if (ib + 1 < p1) { //get right side
			MPI_Irecv(bufferRightIn, g2 * g3, MPI_DOUBLE, _ib(ib + 1, jb, kb), 0, MPI_COMM_WORLD, &recv_request1_2);
			MPI_Wait(&recv_request1_2, &status);
			for (k = 0; k < g3; ++k) {
				for (j = 0; j < g2; ++j) {
					data[_i(g1, j, k)] = bufferRightIn[j + k * g2];
				}
			}
		}
		else {
			for (k = 0; k < g3; ++k) {
				for (j = 0; j < g2; ++j) {
					data[_i(g1, j, k)] = Uright;
					next[_i(g1, j, k)] = Uright;
				}
			}
		}

		if (jb + 1 < p2) { //get down side
			MPI_Irecv(bufferUPIn, g1 * g3, MPI_DOUBLE, _ib(ib, jb + 1, kb), 0, MPI_COMM_WORLD, &recv_request2_1);
			MPI_Wait(&recv_request2_1, &status);
			for (k = 0; k < g3; ++k) {
				for (i = 0; i < g1; ++i) {
					data[_i(i, g2, k)] = bufferUPIn[i + k * g1];
				}
			}
		}
		else {
			for (k = 0; k < g3; ++k) {
				for (i = 0; i < g1; ++i) {
					data[_i(i, g2, k)] = Udown;
					next[_i(i, g2, k)] = Udown;
				}
			}
		}


		if (kb + 1 < p3) { //get back side
			MPI_Irecv(bufferFrontIn, g1 * g2, MPI_DOUBLE, _ib(ib, jb, kb + 1), 0, MPI_COMM_WORLD, &recv_request3_1);
			MPI_Wait(&recv_request3_1, &status);
			for (j = 0; j < g2; ++j) {
				for (i = 0; i < g1; ++i) {
					data[_i(i, j, g3)] = bufferFrontIn[i + j * g1];
				}
			}
		}
		else {
			for (j = 0; j < g2; ++j) {
				for (i = 0; i < g1; ++i) {
					data[_i(i, j, g3)] = Uback;
					next[_i(i, j, g3)] = Uback;
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



		if (ib > 0) { //get left side
			MPI_Irecv(bufferRightOut, g2* g3, MPI_DOUBLE, _ib(ib - 1, jb, kb), 0, MPI_COMM_WORLD, &recv_request1_1);
			MPI_Wait(&recv_request1_1, &status);
			for (k = 0; k < g3; ++k) {
				for (j = 0; j < g2; ++j) {
					data[_i(-1, j, k)] = bufferRightOut[j + k * g2];
				}
			}
		}
		else {
			for (k = 0; k < g3; ++k) {
				for (j = 0; j < g2; ++j) {
					data[_i(-1, j, k)] = Uleft;
					next[_i(-1, j, k)] = Uleft;
				}
			}
		}


		if (jb > 0) { //get up side
			MPI_Irecv(bufferUPOut, g1* g3, MPI_DOUBLE, _ib(ib, jb - 1, kb), 0, MPI_COMM_WORLD, &recv_request2_2);
			MPI_Wait(&recv_request2_2, &status);
			for (k = 0; k < g3; ++k) {
				for (i = 0; i < g1; ++i) {
					data[_i(i, -1, k)] = bufferUPOut[i + k * g1];
				}
			}
		}
		else {
			for (k = 0; k < g3; ++k) {
				for (i = 0; i < g1; ++i) {
					data[_i(i, -1, k)] = Uup;
					next[_i(i, -1, k)] = Uup;
				}
			}
		}


		if (kb > 0) { //get front side
			MPI_Irecv(bufferFrontOut, g1* g2, MPI_DOUBLE, _ib(ib, jb, kb - 1), 0, MPI_COMM_WORLD, &recv_request3_2);
			MPI_Wait(&recv_request3_2, &status);
			for (j = 0; j < g2; ++j) {
				for (i = 0; i < g1; ++i) {
					data[_i(i, j, -1)] = bufferFrontOut[i + j * g1];
				}
			}
		}
		else {
			for (j = 0; j < g2; ++j) {
				for (i = 0; i < g1; ++i) {
					data[_i(i, j, -1)] = Ufront;
					next[_i(i, j, -1)] = Ufront;
				}
			}
		}


		MPI_Barrier(MPI_COMM_WORLD);

		//for edges
		
		int* i_start, * j_start, * k_start;

		k_start = (int*)malloc(sizeof(int) * 2);
		k_start[0] = 0;
		k_start[1] = g3 - 1;

		j_start = (int*)malloc(sizeof(int) * 2);
		j_start[0] = 0;
		j_start[1] = g2 - 1;

		i_start = (int*)malloc(sizeof(int) * 2);
		i_start[0] = 0;
		i_start[1] = g1 - 1;

		//k-edges
		for (int k_s = 0; k_s < 2; ++k_s) {
			k = k_start[k_s];
			for (j = 0; j < g2; ++j) {
				for (i = 0; i < g1; ++i) {
					next[_i(i, j, k)] = 0.5 * ((data[_i(i + 1, j, k)] + data[_i(i - 1, j, k)]) / (hx * hx) +
						(data[_i(i, j + 1, k)] + data[_i(i, j - 1, k)]) / (hy * hy) +
						(data[_i(i, j, k + 1)] + data[_i(i, j, k - 1)]) / (hz * hz)) /
						(1.0 / (hx * hx) + 1.0 / (hy * hy) + 1.0 / (hz * hz));
					epsTemp[0] = max(epsTemp[0], abs(next[_i(i, j, k)] - data[_i(i, j, k)]));
				}
			}
		}



		//j-edges
		for (k = 0; k < g3; ++k) {
			for (int j_s = 0; j_s < 2; ++j_s) {
				j = j_start[j_s];
				for (i = 0; i < g1; ++i) {
					next[_i(i, j, k)] = 0.5 * ((data[_i(i + 1, j, k)] + data[_i(i - 1, j, k)]) / (hx * hx) +
						(data[_i(i, j + 1, k)] + data[_i(i, j - 1, k)]) / (hy * hy) +
						(data[_i(i, j, k + 1)] + data[_i(i, j, k - 1)]) / (hz * hz)) /
						(1.0 / (hx * hx) + 1.0 / (hy * hy) + 1.0 / (hz * hz));
					epsTemp[0] = max(epsTemp[0], abs(next[_i(i, j, k)] - data[_i(i, j, k)]));
				}
			}
		}


		//i-edges
		for (k = 0; k < g3; ++k) {
			for (j = 0; j < g2; ++j) {
				for (int i_s = 0; i_s < 2; ++i_s) {
					i = i_start[i_s];
					next[_i(i, j, k)] = 0.5 * ((data[_i(i + 1, j, k)] + data[_i(i - 1, j, k)]) / (hx * hx) +
						(data[_i(i, j + 1, k)] + data[_i(i, j - 1, k)]) / (hy * hy) +
						(data[_i(i, j, k + 1)] + data[_i(i, j, k - 1)]) / (hz * hz)) /
						(1.0 / (hx * hx) + 1.0 / (hy * hy) + 1.0 / (hz * hz));
					epsTemp[0] = max(epsTemp[0], abs(next[_i(i, j, k)] - data[_i(i, j, k)]));
				}
			}
		}


		MPI_Allgather(epsTemp, 1, MPI_DOUBLE, errors, 1, MPI_DOUBLE, MPI_COMM_WORLD);
		epsTemp[0] = 0;
		for (i = 0; i < numproc; ++i) {
			epsTemp[0] = max(epsTemp[0], errors[i]);
		}

		temp = next;
		next = data;
		data = temp;

		maxErr = epsTemp[0];
		
		iter += 1;

	}while (maxErr >= eps);


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

	free(bufferRightIn);
	free(bufferUPIn);
	free(bufferFrontIn);

	free(bufferRightOut);
	free(bufferUPOut);
	free(bufferFrontOut);

	free(data);
	free(next);
	free(buffer);

	return 0;
}