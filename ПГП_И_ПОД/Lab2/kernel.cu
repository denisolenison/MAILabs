
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
typedef unsigned char bytes;

struct image {
	int width;
	int height;
	pixels* pixs;
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

texture<pixels, 2, cudaReadModeElementType> tex;

cudaArray* c_arr;


void makeTexture(image* img) {
	int w = img->width;
	int h = img->height;

	cudaChannelFormatDesc ch = cudaCreateChannelDesc<pixels>();

	CSC(cudaMallocArray(&c_arr, &ch, w, h));
	CSC(cudaMemcpyToArray(c_arr, 0, 0, img->pixs, sizeof(pixels) * w * h, cudaMemcpyHostToDevice));

	tex.addressMode[0] = cudaAddressModeClamp;
	tex.addressMode[1] = cudaAddressModeClamp;
	tex.channelDesc = ch;
	tex.filterMode = cudaFilterModePoint;
	tex.normalized = false;
	CSC(cudaBindTextureToArray(tex, c_arr, tex.channelDesc));
}

__global__ void filterSSAA(pixels* pixelsOut, int w, int h, int psx, int psy)
{
	int tX = blockIdx.x * blockDim.x + threadIdx.x;
	int tY = blockIdx.y * blockDim.y + threadIdx.y;
	int offsetX = gridDim.x * blockDim.x;
	int offsetY = gridDim.y * blockDim.y;
	int imW = w * psx;

	for (int i = tY; i < h; i += offsetY)
	{
		for (int j = tX; j < w; j += offsetX)
		{
			pixels thisPixel;
			double thisRed = 0.0;
			double thisGreen = 0.0;
			double thisBlue = 0.0;

			for (int Y = psy * i; Y < psy * i + psy; ++Y) {
				for (int X = psx * j; X < psx * j + psx; ++X) {
					thisPixel = tex2D(tex, X, Y);
					thisRed += thisPixel.x;
					thisGreen += thisPixel.y;
					thisBlue += thisPixel.z;
				}
			}

			thisRed /= (psx * psy);
			thisGreen /= (psx * psy);
			thisBlue /= (psx * psy);


			bytes nRed = (bytes)min((int)thisRed, (int)0xFF);
			bytes nGreen = (bytes)min((int)thisGreen, (int)0xFF);
			bytes nBlue = (bytes)min((int)thisBlue, (int)0xFF);


			pixelsOut[j + i * w].x = nRed;
			pixelsOut[j + i * w].y = nGreen;
			pixelsOut[j + i * w].z = nBlue;
			pixelsOut[j + i * w].w = 0;
		}
	}
}

void begin(image* image1, image* image2, int psX, int psY) {

	pixels* newPixels;

	makeTexture(image1);

	int size2 = sizeof(pixels) * image2->width * image2->height;

	CSC(cudaMalloc((void**)& newPixels, size2));

	dim3 gridSz(32, 32);
	dim3 blockSz(32, 32);

	filterSSAA << < gridSz, blockSz >> > (newPixels, image2->width, image2->height, psX, psY);

	CSC(cudaUnbindTexture(tex));

	CSC(cudaFreeArray(c_arr));

	CSC(cudaMemcpy(image2->pixs, newPixels, size2, cudaMemcpyDeviceToHost));
	CSC(cudaFree(newPixels));

}

int main()
{
	string input;
	string output;

	int wNew, hNew;

	std::cin >> input >> output;
	std::cin >> wNew >> hNew;

	image myImage = newImage(input);

	int PS_x = 0;
	int PS_y = 0;

	if (myImage.width % wNew != 0 || myImage.height % hNew) {
		cout << "ERROR: Not prorortional!\n";
		return 0;
	}
	else {
		PS_x = myImage.width / wNew;
		PS_y = myImage.height / hNew;
	}

	image newIM = newImage(myImage.width / PS_x, myImage.height / PS_y);

	begin(&myImage, &newIM, PS_x, PS_y);

	writeToFile(newIM, output);

	return 0;
}