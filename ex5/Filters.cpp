//
// Created by Eli on 04/09/2020.
//
#define IMAGE_SIZE 256
#define MULTI_FOR_BLUR 0.0625
#define MULTI_FOR_SOBEL 0.125
#define MATRIX_CONVOLATION_SIZE 3

#include <cmath>
#include "Matrix.h"


/**
 * check for all value of matrix if they are more then 255 or less then 0, and fix them
 * @param reference to matrix to check
 * @return reference to fixed mat
 */
Matrix &fixValue(Matrix &matrix)
{
	for (int i = 0; i < matrix.getRows() * matrix.getCols(); i++)
	{
		if (matrix[i] < 0)
		{
			matrix[i] = 0;
		}
		if (matrix[i] > IMAGE_SIZE - 1)
		{
			matrix[i] = (float) IMAGE_SIZE - 1;
		}
	}
	return matrix;
}

/**
 *  for the Convolution process we need to take matrix in 3*3 around some cell (i,j)
 *  and if there isn't 9 cell around (like in the edges) we fill the missing cell with 0)
 * @param i i's row
 * @param j j's column
 * @param m original matrix to work on
 * @return 3*3 matrix with the value of original matrix around (i,j) (or 0 if the mat missing)
 */
Matrix makeLittleMatrix(int i, int j, const Matrix &m)
{
	int a = 0;
	Matrix newM(MATRIX_CONVOLATION_SIZE, MATRIX_CONVOLATION_SIZE);
	for (int k = -1; k < MATRIX_CONVOLATION_SIZE-1; k++)
	{
		for (int l = -1; l < MATRIX_CONVOLATION_SIZE-1; l++, a++)
		{
			if (!(i + k < 0 || j + l < 0 || i + k >= m.getCols() ||
			    j + l >= m.getRows()))
			{
				newM[a] = m(i + k, j + l);
			}
		}
	}
	return newM;

}

/**
 * This function calculates the result of the Convolution process for a specific cell
 * get tow matrices and multiple a[i]*b[i] and sum
 * @param littleMatrix as returned from func makeLittleMat 3*3
 * @param k the Convolution mat 3*3
 * @return Rounded number float of the sum
 */
float moltipicationForConvolution(const Matrix &littleMatrix, const Matrix &k)
{
	float sum = 0;
	for (int i = 0; i < 9; i++)
	{
		sum += littleMatrix[i] * k[i];
	}
	return rintf(sum);
}

/**
 * main func that make convolution
 * @param m main matrix
 * @param k the 3*3 matrix for the convolution
 * @return the new matrix after the process.
 */
Matrix convolution(const Matrix &m, const Matrix &k)
{
	Matrix newM(m.getRows(), m.getCols());

	for (int i = 0; i < m.getRows(); i++)
	{
		for (int j = 0; j < m.getCols(); j++)
		{
			newM(i, j) = moltipicationForConvolution(makeLittleMatrix(i, j, m), k);
		}
	}
	return newM;

}

/**
 * do the quantization process by divide the range of colors into "level" range
 * and put new level of color to pixel in matrix
 * @param image original matrix
 * @param level to which level divide the colors
 * @return new image after process
 */
Matrix quantization(const Matrix &image, int level)
{
	Matrix newM(image);
	fixValue(newM);
	for (int j = 0; j < image.getRows() * image.getCols(); j++)
	{
		for (int i = 0; i < level; i++)
		{

			if ((float) (IMAGE_SIZE * i / level) <= newM[j] &&
				newM[j] < (float) (IMAGE_SIZE * (i + 1) / level))
			{
				newM[j] = floorf((float) ((IMAGE_SIZE * i / level) - 1 + (IMAGE_SIZE * (i + 1) /
																		  level)) / 2);
			}
		}
	}
	return newM;
}

/**
 * do the BLUR process by convolation with the appropriate matrix
 * @param image original matrix
 * @return new image after process
 */
Matrix blur(const Matrix &image)
{
	Matrix matrixForConvo(MATRIX_CONVOLATION_SIZE, MATRIX_CONVOLATION_SIZE);
	matrixForConvo[0] = 1;
	matrixForConvo[2] = 1;
	matrixForConvo[6] = 1;
	matrixForConvo[8] = 1;
	matrixForConvo[1] = 2;
	matrixForConvo[3] = 2;
	matrixForConvo[5] = 2;
	matrixForConvo[7] = 2;
	matrixForConvo[4] = 4;
	matrixForConvo *= MULTI_FOR_BLUR;
	Matrix newImage = convolution(image, matrixForConvo);

	return fixValue(newImage);

}

/**
 * do the sobel process by convolation with the appropriate matrix
 * @param image original matrix
 * @return new image after process
 */
Matrix sobel(const Matrix &image)
{
	Matrix matForConvo_x(MATRIX_CONVOLATION_SIZE, MATRIX_CONVOLATION_SIZE);
	Matrix matForConvo_y(MATRIX_CONVOLATION_SIZE, MATRIX_CONVOLATION_SIZE);
	matForConvo_x[0] = 1;
	matForConvo_x[6] = 1;
	matForConvo_y[0] = 1;
	matForConvo_y[2] = 1;
	matForConvo_y[1] = 2;
	matForConvo_x[3] = 2;
	matForConvo_x[2] = -1;
	matForConvo_x[8] = -1;
	matForConvo_y[6] = -1;
	matForConvo_y[8] = -1;
	matForConvo_y[7] = -2;
	matForConvo_x[5] = -2;
	matForConvo_x *= MULTI_FOR_SOBEL;
	matForConvo_y *= MULTI_FOR_SOBEL;
	Matrix newImage = convolution(image, matForConvo_x) + convolution(image, matForConvo_y);
	return fixValue(newImage);
}


