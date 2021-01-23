
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <algorithm>
#include <string>
#include <cuda.h>


using namespace std;

#define CSC(call)  													\
do {																\
	cudaError_t res = call;											\
	if (res != cudaSuccess) {										\
		fprintf(stderr, "ERROR in %s:%d. Message: %s\n",			\
				__FILE__, __LINE__, cudaGetErrorString(res));		\
		exit(0);													\
	}															    \
} while (0)		                                                    \
											                        \

typedef uchar4 pixels;
typedef double pixFloat[3];
typedef pixFloat matrix3[3];


__constant__ pixFloat dev_cAvg[32];
__constant__ matrix3 dev_cMatrInv[32];

typedef unsigned char bytes;

struct image {
	int width;
	int height;
	pixels* pixs;
};

struct pixel {
	int x;
	int y;
};

image newImage(int w, int h) {
	image nIMG;
	nIMG.width = w;
	nIMG.height = h;
	nIMG.pixs = new pixels[w * h];
	return nIMG;
}

image newImage(string filename) {
	FILE* file;
	image thisImg;
	if ((file = fopen(filename.c_str(), "rb")) == NULL) {
		std::cout << "Can't load image from file" << std::endl;
		exit(1);
	}

	fread(&thisImg.width, sizeof(thisImg.width), 1, file);
	fread(&thisImg.height, sizeof(thisImg.height), 1, file);

	thisImg.pixs = new pixels[thisImg.width * thisImg.height];
	fread(thisImg.pixs, sizeof(pixels), thisImg.width * thisImg.height, file);

	fclose(file);
	return thisImg;
}

void writeToFile(image img, string filename) {
	FILE* file = fopen(filename.c_str(), "wb");

	fwrite(&img.width, sizeof(img.width), 1, file);
	fwrite(&img.height, sizeof(img.height), 1, file);
	fwrite(img.pixs, sizeof(pixels), img.width * img.height, file);
	fclose(file);
}

string imgToString(image img) {
	std::stringstream stream;
	stream << img.width << " " << img.height << "\n";
	for (int i = 0; i < img.height; i++) {
		for (int j = 0; j < img.width; j++) {
			int k = i * img.width + j;
			stream << hex << setfill('0') << setw(2) << (int)img.pixs[k].x << setfill('0') << setw(2) << (int)img.pixs[k].y << setfill('0') << setw(2) << (int)img.pixs[k].z << setfill('0') << setw(2) << (int)img.pixs[k].w << " ";
		}
		stream << "\n";
	}

	return stream.str();
}

__global__ void Mahalanobisse(pixels* pixelsOut, int w, int h, int classes)
{
	int tX = blockIdx.x * blockDim.x + threadIdx.x;
	int tY = blockIdx.y * blockDim.y + threadIdx.y;
	int offsetX = gridDim.x * blockDim.x;
	int offsetY = gridDim.y * blockDim.y;

	for (int i = tY; i < h; i += offsetY)
	{
		for (int j = tX; j < w; j += offsetX)
		{
			pixels thisPixel = pixelsOut[j + i * w];
			double thisRed = (double)thisPixel.x;
			double thisGreen = (double)thisPixel.y;
			double thisBlue = (double)thisPixel.z;


			double maxAm = 0;
			int argMax = -1;
			for (int c = 0; c < classes; ++c) {
				double ans = 0;
				pixFloat vec1;
				pixFloat vec2;
				pixFloat vec3;

				vec1[0] = -(thisRed - dev_cAvg[c][0]);
				vec2[0] = thisRed - dev_cAvg[c][0];

				vec1[1] = -(thisGreen - dev_cAvg[c][1]);
				vec2[1] = thisGreen - dev_cAvg[c][1];

				vec1[2] = -(thisBlue - dev_cAvg[c][2]);
				vec2[2] = thisBlue - dev_cAvg[c][2];


				vec3[0] = vec1[0] * dev_cMatrInv[c][0][0] + vec1[1] * dev_cMatrInv[c][1][0] + vec1[2] * dev_cMatrInv[c][2][0];
				vec3[1] = vec1[0] * dev_cMatrInv[c][0][1] + vec1[1] * dev_cMatrInv[c][1][1] + vec1[2] * dev_cMatrInv[c][2][1];
				vec3[2] = vec1[0] * dev_cMatrInv[c][0][2] + vec1[1] * dev_cMatrInv[c][1][2] + vec1[2] * dev_cMatrInv[c][2][2];

				ans = vec3[0] * vec2[0] + vec3[1] * vec2[1] + vec3[2] * vec2[2];

				if (ans > maxAm || argMax == -1) {
					maxAm = ans;
					argMax = c;
				}

			}

			pixelsOut[j + i * w].w = argMax;

			//coloring 3 for report
			//if (argMax == 0) {
			//	pixelsOut[j + i * w].x = 255;
			//	pixelsOut[j + i * w].y = 0;
			//	pixelsOut[j + i * w].z = 0;
			//}
			//else if (argMax == 1) {
			//	pixelsOut[j + i * w].x = 0;
			//	pixelsOut[j + i * w].y = 255;
			//	pixelsOut[j + i * w].z = 0;
			//}
			//else if (argMax == 2) {
			//	pixelsOut[j + i * w].x = 0;
			//	pixelsOut[j + i * w].y = 0;
			//	pixelsOut[j + i * w].z = 255;
			//}

		}
	}
}

void begin(image* image1, int classes) {

	pixels* oldPixels;

	int size1 = sizeof(pixels) * image1->width * image1->height;

	CSC(cudaMalloc((void**)& oldPixels, size1));

	int SIZE = 32;

	dim3 gridSz(SIZE, SIZE);
	dim3 blockSz(SIZE, SIZE);

	CSC(cudaMemcpy(oldPixels, image1->pixs, size1, cudaMemcpyHostToDevice));

	cudaEvent_t start, end;
	CSC(cudaEventCreate(&start));
	CSC(cudaEventCreate(&end));
	CSC(cudaEventRecord(start));

	Mahalanobisse << < gridSz, blockSz >> > (oldPixels, image1->width, image1->height, classes);

	CSC(cudaGetLastError()); //просмотр ошибок

	CSC(cudaEventRecord(end));
	CSC(cudaEventSynchronize(end));
	float t;
	CSC(cudaEventElapsedTime(&t, start, end));
	CSC(cudaEventDestroy(start));
	CSC(cudaEventDestroy(end));
	//конец записи
	printf("time = %f\n", t);


	CSC(cudaMemcpy(image1->pixs, oldPixels, size1, cudaMemcpyDeviceToHost));
	CSC(cudaFree(oldPixels));

}





int main()
{
	string input;
	string output;

	int w;

	cin >> input >> output;

	image myImage = newImage(input);
	w = myImage.width;

	int classes;
	cin >> classes;

	double curRed = 0;
	double curGreen = 0;
	double curBlue = 0;

	pixFloat* cAvg = new pixFloat[classes];
	matrix3* cMatr = new matrix3[classes];
	matrix3* cMatrInv = new matrix3[classes];

	//генерация средних векторов и ков. матриц
	for (int i = 0; i < classes; ++i) {
		long long pixs_am = 0;

		curRed = 0;
		curGreen = 0;
		curBlue = 0;

		cin >> pixs_am;
		pixel* pixPairs = new pixel[pixs_am];
		for (long long j = 0; j < pixs_am; ++j) {
			int X, Y;
			cin >> X >> Y;
			curRed += (double)myImage.pixs[X + w * Y].x;
			curGreen += (double)myImage.pixs[X + w * Y].y;
			curBlue += (double)myImage.pixs[X + w * Y].z;
			pixPairs[j].x = X;
			pixPairs[j].y = Y;
		}
		curRed /= pixs_am;
		curGreen /= pixs_am;
		curBlue /= pixs_am;

		cAvg[i][0] = curRed;
		cAvg[i][1] = curGreen;
		cAvg[i][2] = curBlue;


		matrix3 totalMatrix;

		for (int Ti = 0; Ti < 3; ++Ti) {
			for (int Tj = 0; Tj < 3; ++Tj) {
				totalMatrix[Ti][Tj] = 0;
			}
		}

		for (int j = 0; j < pixs_am; ++j) {
			pixFloat vec;
			vec[0] = (double)myImage.pixs[pixPairs[j].x + w * pixPairs[j].y].x - cAvg[i][0];
			vec[1] = (double)myImage.pixs[pixPairs[j].x + w * pixPairs[j].y].y - cAvg[i][1];
			vec[2] = (double)myImage.pixs[pixPairs[j].x + w * pixPairs[j].y].z - cAvg[i][2];

			for (int Ti = 0; Ti < 3; ++Ti) {
				for (int Tj = 0; Tj < 3; ++Tj) {
					totalMatrix[Ti][Tj] += vec[Ti] * vec[Tj];
				}
			}
		}

		for (int Ti = 0; Ti < 3; ++Ti) {
			for (int Tj = 0; Tj < 3; ++Tj) {
				totalMatrix[Ti][Tj] /= max(0.000001, (double)pixs_am - 1);
				cMatr[i][Ti][Tj] = totalMatrix[Ti][Tj];
			}
		}

		delete[] pixPairs;

	}

	for (int i = 0; i < classes; ++i) {
		double det = 0;
		det = cMatr[i][0][0] * cMatr[i][1][1] * cMatr[i][2][2] + cMatr[i][0][2] * cMatr[i][1][0] * cMatr[i][2][1] +
			cMatr[i][0][1] * cMatr[i][1][2] * cMatr[i][2][0] - cMatr[i][2][0] * cMatr[i][1][1] * cMatr[i][0][2] -
			cMatr[i][0][1] * cMatr[i][1][0] * cMatr[i][2][2] - cMatr[i][0][0] * cMatr[i][1][2] * cMatr[i][2][1];
		if (det == 0) det = 0.0000001; //чтобы программа не вылетала
		matrix3 transp;
		for (int x = 0; x < 3; ++x) {
			for (int y = 0; y < 3; ++y) {
				transp[x][y] = cMatr[i][y][x];
			}
		}


		double dop1 = transp[1][1] * transp[2][2] - transp[1][2] * transp[2][1];
		double dop4 = transp[1][2] * transp[2][0] - transp[1][0] * transp[2][2];
		double dop7 = transp[1][0] * transp[2][1] - transp[1][1] * transp[2][0];

		double dop2 = transp[0][2] * transp[2][1] - transp[0][1] * transp[2][2];
		double dop5 = transp[0][0] * transp[2][2] - transp[0][2] * transp[2][0];
		double dop8 = transp[0][1] * transp[2][0] - transp[0][0] * transp[2][1];

		double dop3 = transp[0][1] * transp[1][2] - transp[0][2] * transp[1][1];
		double dop6 = transp[0][2] * transp[1][0] - transp[0][0] * transp[1][2];
		double dop9 = transp[0][0] * transp[1][1] - transp[0][1] * transp[1][0];

		cMatrInv[i][0][0] = dop1 / det;
		cMatrInv[i][0][1] = dop2 / det;
		cMatrInv[i][0][2] = dop3 / det;

		cMatrInv[i][1][0] = dop4 / det;
		cMatrInv[i][1][1] = dop5 / det;
		cMatrInv[i][1][2] = dop6 / det;

		cMatrInv[i][2][0] = dop7 / det;
		cMatrInv[i][2][1] = dop8 / det;
		cMatrInv[i][2][2] = dop9 / det;
	}

	cudaMemcpyToSymbol(dev_cAvg, cAvg, sizeof(pixFloat) * classes, 0, cudaMemcpyHostToDevice);
	cudaMemcpyToSymbol(dev_cMatrInv, cMatrInv, sizeof(matrix3) * classes, 0, cudaMemcpyHostToDevice);

	delete[] cAvg;
	delete[] cMatr;
	delete[] cMatrInv;


	begin(&myImage, classes);

	writeToFile(myImage, output);

	return 0;
}