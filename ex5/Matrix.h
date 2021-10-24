// Matrix.h
#define WRONG_ACTION "Invalid matrix dimensions.\n"
#define WRONG_RANGE "Index out of range.\n"
#define WRONG_INPUT "Error loading from input stream.\n"
#define ZERO_DIVISION "Division by zero.\n"
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cstring>

/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
	int rows, cols;
} MatrixDims;

/**
 * the class represent matrix
 * @param _matrix hold tha float nums of mat
 * @param _rows the num of rows
 * @param _cols the num of columns
 * @param _size its cols*rows
 */
class Matrix
{
private:
	float *_matrix;
	int _rows;
	int _cols;
	int _size;
public:
	/**
 * init the matrix with size row*col and fill with 0's
 * @param rows: nums of matrix rows
 * @param cols nums of matrix cols
 */
	Matrix(int rows, int cols);

	/** default constructor. fill the mat 1*1 with 0 */
	Matrix();

	/**
 *init matrix from matrix. copy constructor
 * @param m matrix
 */
	Matrix(const Matrix &m);

/** destructor. free the allocation*/
	~Matrix();

/** const funt that return the row's num of mat.*/
	int getRows() const;

	/** const funt that return the row's num of mat.*/
	int getCols() const;

	/**
 * vectorize the mat and make the size of mat to 1*(rows*cols)
 * @return self
 */
	Matrix &vectorize();

	/** print the mat according the orders*/
	void print() const;

	/**
 * copy mat m to our mat.
 * @param m matrix to copy from
 * @return self
 */

	Matrix &operator=(const Matrix &m);

/**
 * multiplication matrices. self*m
 * @return new matrix.
 */
	Matrix operator*(const Matrix &m) const;

/**
 * multiplication matrix in scalar
 * @param c scalar. float
 * @return new mat
 */
	Matrix operator*(const float &c) const;

	/**
 * multiplication matrix. self = self*m
 * @return self.
 */

	Matrix &operator*=(const Matrix &m);

	/**
 * multiplication matrix in scalar
 * @param c scalar - float
 * @return self.
 */

	Matrix &operator*=(const float &c);

	/**
 * multiplication division in scalar
 * @param c scalar - float
 * @return conclusion matrix
 */

	Matrix operator/(const float &c) const;

	/**
 * multiplication division in scalar
 * @param c scalar - float
 * @return self.
 */

	Matrix &operator/=(const float &c);

	/**
 * add to this matrix float in each cell
 * @param c- float number
 * @return this
 */

	Matrix &operator+=(const float &c);

	/**
 * bool function to check if two matrix are not the same
 * @param m the another matrix
 * @return false if they are same, true otherwise
 */

	bool operator!=(const Matrix &m) const;

	/**
 * bool function to check if two matrix are the same
 * @param m the another matrix
 * @return true if they are same, false otherwise
 */

	bool operator==(const Matrix &m) const;

	/**
 * addition two matrices. this+m = new matrix
 * @return the new matrix
 */

	Matrix operator+(const Matrix &m) const;

	/**
 * add to this matrix another matrix
 * @param m - matrix
 * @return this
 */

	Matrix &operator+=(const Matrix &m);

	/**
 * find matrix(i,j)
 * @return reference to value
 */

	float &operator()(int i, int j);

	/**
 * find matrix(i,j) for const matrix
 * @return the value  in (i,j)
 */

	float operator()(int i, int j) const;

	/**
 * const method for find matrix(k) like the matrix vectorised.
 * matrix (i,j)=matrix(i*rows size + j) = matrix (k)
 * @return the value
 */

	float operator[](int i) const;

	/**
 * find matrix(i) like the matrix vectorised.
 * matrix (i,j)=matrix(i*rows size + j) = matrix (k)
 * @return reference to value
 */

	float &operator[](int i);

	/**
* friend of class, multiplication matrix in scalar too
* @param c scalar
* @param m matrix
* @return the new mat.
*/
	friend Matrix operator*(const float &c, Matrix &m);

	/**
 * friend of class. get istream and fill the matrix accordingly
 * @param is stream
 * @param m matrix
 * @return the stream
 */
	friend std::istream &operator>>(std::istream &s, Matrix &m);

	/**
 * get a stream and print the mat to it
 * @return the stream.
 */

	friend std::ostream &operator<<(std::ostream &s, const Matrix &m);
};

#endif //MATRIX_H