//Labau Cristea Andrei Liviu 314CB

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "bmp_header.h"

/* Aloca memorie pentru un sir de caractere
	@name = numele pentru care se aloca memorie
*/
char* allocName(char* name) {
	char* outputImageName = (char*)malloc(strlen(name) * sizeof(char));
	if(outputImageName == NULL) {
		fprintf(stderr, "Name alloc error\n");
	}
	else {
		return outputImageName;
	}
}
/* Calculeaza paddingul unei imagini
	@cols = latimea imaginii in pixeli
*/
int getPadding(int cols) {
	int padding = 0; 			// in octeti
	int size = cols * 3; 		// dimensiunea unei linii in bytes fara padding
	while((size + padding) % 4 != 0) { 
		padding ++; 			// se completeaza pana la multiplu de 4
	}
	return padding;
}
/* Aloca memorie pentru un bitmap cu tot cu padding
	@rows = inaltimea unei imagini
	@cols = latimea unei imagini
	@padding = paddingul unei imagini
*/
TPixel** allocPixelMatrix(int rows, int cols, int padding) {
	int i;
	TPixel** pixels = (TPixel**)malloc(rows * sizeof(TPixel*));
	if(pixels == NULL) {
		fprintf(stderr, "pixels alloc error\n");
	}
	for(i = 0; i < rows; i++) {
		pixels[i] = (TPixel*)malloc((cols + padding) * sizeof(TPixel));
		if(pixels[i] == NULL) {
			fprintf(stderr, "pixel alloc error\n");
		}
	}
return pixels;
/* Aloca memorie pentru o matrice patratica
	@size = dimensiunea matricii
*/
}
int** allocMatrix(int size) {
	int i;
	int** matrix = (int**)malloc(size * sizeof(int*));
	if(matrix == NULL) {
		fprintf(stderr, "matrix alloc error\n");
	}
	for(i = 0; i < size; i++) {
		matrix[i] = (int*)malloc(size * sizeof(int));
		if(matrix[i] == NULL) {
			fprintf(stderr, "matrix element alloc error\n");
		}
	}
	return matrix;
}
/* Creaza numele fisierului de scriere
	@name - numele imaginii initiale
	@extensionName - extensia ce va fi adaugata numelui imaginii initiale
*/
char* getOutputImageName(char* name, char* extensionName) {
	int i;
	char* copyImageName = allocName(name); 		 //copie a numelui 
	for(i = 0; i < strlen(name); i++) {
		copyImageName[i] = name[i];
	}
	char* outImageName = allocName(name);
	char* token1 = strtok(copyImageName, ".");	  //desparte in tokeni pana la .
	outImageName = strcat(token1, extensionName); //concatenare
	return outImageName;
}
/* Face un pixel alb sau negru*/
TPixel goWhite(TPixel pixel, char color) {
	if(color == 'w') {
		pixel.r = 255;
		pixel.g = 255;
		pixel.b = 255;
	}
	else if (color == 'b') {
		pixel.r = 0;
		pixel.g = 0;
		pixel.b = 0;
	}
	return pixel;
}
/* Pune 0 intre header si bitmap
	@outputImage = fisierul in care se scrie
	@imageDataOffset = offsetul de unde se incepe scrierea
*/
void writeZero(FILE* outputImage, int imageDataOffset) {
	unsigned char zero = '\0';
	while(ftell(outputImage) < imageDataOffset) { 
		fwrite(&zero, sizeof(unsigned char), 1, outputImage);
	}
	return;
}
/*Transforma imaginea din color in black&white
	@rows = inaltimea unei imagini
	@cols = latimea unei imagini
	@pixels = imaginea asupra careia se vor aplica transformarile
*/
TPixel** goBlackWhite(int rows, int cols, TPixel** pixels) {
	int i, j;
	for(i = 0; i < rows; i++) {
		for(j = 0; j < cols; j++) {
			int suma = pixels[i][j].b + pixels[i][j].g + pixels[i][j].r;
			pixels[i][j].b = suma / 3;
			pixels[i][j].g = suma / 3;
			pixels[i][j].r = suma / 3;
		}
	}
	return pixels;
}
/* Face o copie a unei matrice de pixeli
	@pixels = matricea initiala
	@rows = nr de linii
	@cols = nr de coloane
*/
TPixel** copyPixels(TPixel** pixels, int rows, int cols) {
	int i, j;
	TPixel** auxPixels = allocPixelMatrix(rows, cols, getPadding(rows));
	for(i = 0; i < rows; i++) {
		for(j = 0; j < cols; j++) {
			auxPixels[i][j] = pixels[i][j];
		}
	}
	return auxPixels;
}
/* Elibereaza memoria alocata unei imagini
	@rows = inaltimea unei imagini
	@pixels = imaginea
*/
void freeBitmap(int rows, TPixel** pixels) {
	int i;
	for(i = 0; i < rows; i++) {
		free(pixels[i]);
	}
	free(pixels);
}
/* Inchide fisierele ce contin imaginile de output si input
*/
void closeInOut(FILE* inputImage, FILE* outputImage) {
	fflush(inputImage);
	fflush(outputImage);
	fclose(inputImage);
	fclose(outputImage);
}

int main () {
	int i = 0, j = 0;
	unsigned char zero = '\0';
	//deschid fisierul input.txt pt citire
	char* inputFileName = allocName("input.txt");
	inputFileName = "input.txt";
	FILE* inputFile = fopen(inputFileName, "rt");
	if(inputFile == NULL) {
		fprintf(stderr, "Can't open the file %s\n", inputFileName);
		return -1;
	}
	//aloc memorie pentru numele/caile diferitelor resurse
	char* imageName = allocName("testx.bmp");
	char* filterName = allocName("./input/filters/filterx.txt");
	char* poolName = allocName("./input/pooling/poolingx.txt");
	char* clusterName = allocName("./input/clustering/clusterx.txt");
	//citesc numele/caile diferitelor resurse
	fscanf(inputFile, "%s", imageName); 
	fscanf(inputFile, "%s", filterName);
	fscanf(inputFile, "%s", poolName); 
	fscanf(inputFile, "%s", clusterName);
	//deschid fisierul binar ce contine imaginea pentru citire pentru task 1
	FILE* inputImage = fopen(imageName, "rb");
	if(inputImage == NULL) {
		fprintf(stderr, "Can't open the file %s\n", imageName);
		return -1;
	}
	//deschid fisierul binar in care urmeaza sa pun imaginea black&white
	char* bwImageName = getOutputImageName(imageName,"_black_white.bmp");
	FILE* outputImage = fopen(bwImageName, "wb");
	if(outputImage == NULL) {
		fprintf(stderr, "Can't open the file %s\n", bwImageName);
		return -1;
	}
	//Citesc informatiile din headere
	bmp_fileheader fileH;
	fread(&fileH, sizeof(bmp_fileheader), 1, inputImage);
	bmp_infoheader infoH;
	fread(&infoH, sizeof(bmp_infoheader), 1, inputImage);
	TPixel** pixels = allocPixelMatrix(infoH.height, infoH.width,
	getPadding(infoH.width));
	//citesc imaginea pixel by pixel eliminand paddingul
	for(i = 0; i < infoH.height; i++) {
		for(j = 0; j < infoH.width; j++) {
			fread(&pixels[i][j], sizeof(TPixel), 1, inputImage);
		}
		fseek(inputImage, getPadding(infoH.width), SEEK_CUR);
	}
	//fac pixelii alb-negru
	TPixel** blackPixels = allocPixelMatrix(infoH.height, infoH.width,
	getPadding(infoH.width));
	blackPixels = goBlackWhite(infoH.height, infoH.width, pixels);
	//pun pixelii alb-negru in noua imagine
	fwrite(&fileH, sizeof(bmp_fileheader), 1, outputImage);
	fwrite(&infoH, sizeof(bmp_infoheader), 1, outputImage);
	writeZero(outputImage, fileH.imageDataOffset);     // completez cu 0
	for(i = 0; i < infoH.height; i++) {
		for(j = 0; j < infoH.width; j++) { 			   // pun pixels
			fwrite(&blackPixels[i][j], sizeof(TPixel), 1, outputImage);
		}
		for(j = 0; j < getPadding(infoH.width); j++) { // completez cu padding
			fwrite(&zero, sizeof(unsigned char), 1, outputImage);
		}
	}
	freeBitmap(infoH.height, pixels); 
	closeInOut(inputImage, outputImage);

	//deschid fisierul din care citesc imaginea pentru task 2
	inputImage = fopen(imageName, "rb");
	if(inputImage == NULL) {
		fprintf(stderr, "Can't open the file %s\n", imageName);
		return -1;
	}
	//deschid fisierul binar in care urmeaza sa pun imaginea crop
	char* cropImageName = getOutputImageName(imageName,"_nocrop.bmp");
	outputImage = fopen(cropImageName, "wb");
	if(outputImage == NULL) {
		fprintf(stderr, "Can't open the file %s\n", cropImageName);
		return -1;
	}
	
	TPixel white = goWhite(white,'w'); 						//creez un pixel alb
	fwrite(&fileH, sizeof(bmp_fileheader), 1, outputImage); //fileheader
	if(infoH.height > infoH.width) { 
		fseek(inputImage, fileH.imageDataOffset, SEEK_SET);
		pixels = allocPixelMatrix(infoH.height, infoH.height,
		getPadding(infoH.height));
		for(i = 0; i < infoH.height; i++) { //citesc imaginea
			for(j = 0; j < infoH.width; j++) {
				fread(&pixels[i][j], sizeof(TPixel), 1, inputImage);
			}
			fseek(inputImage, getPadding(infoH.width), SEEK_CUR);
		} 
		int diff = infoH.height - infoH.width;
		int aux = infoH.width; 		//retin val. latimii
		infoH.width = infoH.height;	//actualizez latimea
		fwrite(&infoH, sizeof(bmp_infoheader), 1, outputImage);//infoheader
		infoH.width = aux;			//resetez val. latimii
		writeZero(outputImage, fileH.imageDataOffset);
		for(i = 0; i < infoH.height; i++) {
			for(j = 0; j < diff / 2; j++) { //completez cu pixeli alb jos
				fwrite(&white, sizeof(TPixel), 1, outputImage);
			}
			for(j = 0; j < infoH.width; j++) {//completez cu pixeli la mijloc
				fwrite(&pixels[i][j], sizeof(TPixel), 1, outputImage);
			}
			for(j = 0; j < diff / 2; j++) { //completez cu pixeli albi sus
				fwrite(&white, sizeof(TPixel), 1, outputImage);
			}
			for(j = 0; j < getPadding(infoH.height); j++) { //padding
				fwrite(&zero, sizeof(unsigned char), 1, outputImage);
			}
		}
	}
	else if(infoH.height < infoH.width) { 
		fseek(inputImage, fileH.imageDataOffset, SEEK_SET);
		pixels = allocPixelMatrix(infoH.width, infoH.width,
		getPadding(infoH.width));
		for(i = 0; i < infoH.height; i++) {
			for(j = 0; j < infoH.width; j++) {
				fread(&pixels[i][j], sizeof(TPixel), 1, inputImage);
			}
			fseek(inputImage, getPadding(infoH.width), SEEK_CUR);
		}
		int diff = infoH.width - infoH.height;
		int aux = infoH.height; 	//retin valoarea inaltimii 
		infoH.height = infoH.width; //actualizez inaltimea
		fwrite(&infoH, sizeof(bmp_infoheader), 1, outputImage);//pun infoheader
		infoH.height = aux; 		//resetez val. inaltimii 
		writeZero(outputImage, fileH.imageDataOffset); 
		// completez partea de jos cu alb
		if(diff  % 2 == 0) { //diferenta este para
			for(i = 0; i < diff / 2; i++) {
				for(j = 0; j < infoH.width; j++) {
					fwrite(&white, sizeof(TPixel), 1, outputImage);
				}
				for(j = 0; j < getPadding(infoH.width); j++) {
					fwrite(&zero, sizeof(unsigned char), 1, outputImage);
				}
			}
		}
		else { //diferenta este impara
			for(i = 0; i < (diff / 2) + 1; i++) {
				for(j = 0; j < infoH.width; j++) {
					fwrite(&white, sizeof(TPixel), 1, outputImage);
				}
				for(j = 0; j < getPadding(infoH.width); j++) {
					fwrite(&zero, sizeof(unsigned char), 1, outputImage);
				}
			}
		}
		//scriu imaginea
		for(i = 0; i < infoH.height; i++) {
			for(j = 0; j < infoH.width; j++) {
				fwrite(&pixels[i][j], sizeof(TPixel), 1, outputImage);
			}
			for(j = 0; j < getPadding(infoH.width); j++) {
				fwrite(&zero, sizeof(unsigned char), 1, outputImage);
			}
		}
		//completez partea de sus cu alb
		for(i = 0; i < diff / 2; i++) {
			for(j = 0; j < infoH.width; j++) {
				fwrite(&white, sizeof(TPixel), 1, outputImage);
			}
			for(j = 0; j < getPadding(infoH.width); j++) {
				fwrite(&zero, sizeof(unsigned char), 1, outputImage);
			}
		}
	}		
	else {
		fwrite(&infoH, sizeof(bmp_infoheader), 1, outputImage);// infoheader
		fseek(inputImage, fileH.imageDataOffset, SEEK_SET);
		pixels = allocPixelMatrix(infoH.height, infoH.width, 
		getPadding(infoH.width));
		for(i = 0; i < infoH.height; i++) {
			for(j = 0; j < infoH.width; j++) {
				fread(&pixels[i][j], sizeof(TPixel), 1, inputImage);
			}
			fseek(inputImage, getPadding(infoH.width), SEEK_CUR);
		} 
		writeZero(outputImage, fileH.imageDataOffset);
		for(i = 0; i < infoH.height; i++) {
			for(j = 0; j < infoH.width; j++) { 				// pixeli
				fwrite(&pixels[i][j], sizeof(TPixel), 1, outputImage);
			}
			for(j = 0; j < getPadding(infoH.width); j++) { // padding
				fwrite(&zero, sizeof(unsigned char), 1, outputImage);
			}
		}
	}
	freeBitmap(infoH.height, pixels);
	closeInOut(inputImage, outputImage);

	//deschid fisierul binar ce contine imaginea pentru citire pentru task 3
	inputImage = fopen(imageName, "rb");
	if(inputImage == NULL) {
		fprintf(stderr, "Can't open the file %s\n", imageName);
		return -1;
	}
	//deschid fisierul binar in care urmeaza sa pun imaginea filtrata
	char* filterImageName = getOutputImageName(imageName,"_filter.bmp");
	outputImage = fopen(filterImageName, "wb");
	if(outputImage == NULL) {
		fprintf(stderr, "Can't open the file %s\n", filterImageName);
		return -1;
	}
	//deschid fisierul text din care citesc filtrul
	FILE* filterFile = fopen(filterName, "rt");
	if(filterFile == NULL) {
		fprintf(stderr, "Can't open the file %s\n", filterName);
		return -1;
	}
	int fSize;                         // dimensiune filtru
	fscanf(filterFile, "%d", &fSize);
	int** filter = allocMatrix(fSize); 
	for(i = 0; i < fSize; i++) { 	  
		for(j = 0; j < fSize; j++) {
			fscanf(filterFile, "%d", &filter[i][j]);
		}
	}
	//citesc matricea de pixeli initiala rasturnata
	pixels = allocPixelMatrix(infoH.height, infoH.width,
	getPadding(infoH.width));
	TPixel** pixelsaux = allocPixelMatrix(infoH.height, infoH.width,
	getPadding(infoH.width));
	fseek(inputImage, fileH.imageDataOffset, SEEK_SET);
	for(i = infoH.height; i > 0; i--) {
		for(j = 0; j < infoH.width; j++) {
			fread(&pixels[i-1][j], sizeof(TPixel), 1, inputImage);
		}
		fseek(inputImage, getPadding(infoH.width), SEEK_CUR);
	}
	//aplic filtrul
	int m, n;
	//parcurg matricea de pixeli
	for(i  = 0; i < infoH.height; i++) {
		for(j = 0; j < infoH.width; j++) {
			long long sum1 = 0, sum2 = 0, sum3 = 0;
			//parcurg minimatricea centrata in pixelul curent
			for(m = i - fSize/2; m <= i+fSize/2; m++) {
				for(n = j - fSize/2; n <= j+fSize/2; n++) {
					// elimin vecinii din afara imaginii
					if(m >= 0 && m < infoH.height && n >=0 && n < infoH.width) {
						sum1 += pixels[m][n].r * filter[m-i+fSize/2][n-j+fSize/2]; 
						sum2 += pixels[m][n].g * filter[m-i+fSize/2][n-j+fSize/2];
						sum3 += pixels[m][n].b * filter[m-i+fSize/2][n-j+fSize/2];
					}
				}
			}
			if(sum1 >= 255) { 			// daca valoarea este in afara limitelor
				pixelsaux[i][j].r = 255; // actualizeaza
			}
			else if (sum1 <= 0) {
				pixelsaux[i][j].r = 0;
			} else {
				pixelsaux[i][j].r = sum1;
			}
			if(sum2 >= 255) {
				pixelsaux[i][j].g = 255;
			}
			else if(sum2 <= 0) {
				pixelsaux[i][j].g = 0;
			}
			else {
				pixelsaux[i][j].g = sum2;
			}
			if(sum3 >= 255) {
				pixelsaux[i][j].b = 255;
			}
			else if(sum3 <= 0) {
				pixelsaux[i][j].b = 0;
			}
			else {
				pixelsaux[i][j].b = sum3;
			}
		}
	}
	//scriu imaginea
	fwrite(&fileH, sizeof(bmp_fileheader), 1, outputImage);
	fwrite(&infoH, sizeof(bmp_infoheader), 1, outputImage);
	writeZero(outputImage, fileH.imageDataOffset);     
	for(i = infoH.height; i > 0; i--) {
		for(j = 0; j < infoH.width; j++) { 			   // pixeli
			fwrite(&pixelsaux[i-1][j], sizeof(TPixel), 1, outputImage);
		}
		for(j = 0; j < getPadding(infoH.width); j++) { // padding
			fwrite(&zero, sizeof(unsigned char), 1, outputImage);
		}
	}
	freeBitmap(infoH.height, pixels);
	freeBitmap(infoH.height, pixelsaux);
	closeInOut(inputImage, outputImage);

	return 0;
}


