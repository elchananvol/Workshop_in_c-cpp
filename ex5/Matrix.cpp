#include <sstream>
#include "Matrix.h"

/** constructor. fill the mat with 0 */
Matrix::Matrix(int rows, int cols) :
		_matrix(nullptr)
{
	if (rows < 0 || 0 > cols)
	{
		std::exit(EXIT_FAILURE);
	}
	else
	{

		_rows = rows;
		_cols = cols;
		_matrix = new float[_rows * _cols]{0};
	}

}

/** default constructor. fill the mat 1*1 with 0 */
Matrix::Matrix() :
		_matrix(nullptr), _rows(1), _cols(1)
{
	_matrix = new float[1];
	_matrix[0] = 0;
}

/**
 *init matrix from matrix. copy constructor
 * @param m matrix
 */
Matrix::Matrix(const Matrix &m) :
		_matrix(nullptr), _rows(1), _cols(1)
{

	_rows = m._rows;
	_cols = m._cols;
	_matrix = new float[_rows * _cols];
	for (int i = 0; i < _rows * _cols; i++)
	{
		_matrix[i] = m[i];
	}
}

/** destructor. free the allocation*/
Matrix::~Matrix()
{
	delete[] _matrix;
}

/** const funt that return the row's num of mat.*/
int Matrix::getRows() const
{
	return _rows;
}

/** const funt that return the row's num of mat.*/
int Matrix::getCols() const
{
	return _cols;
}

/**
 * vectorize the mat and make the size of mat to 1*(rows*cols)
 * @return self
 */
Matrix &Matrix::vectorize()
{
	_rows *= _cols;
	_cols = 1;
	return *this;

}

/** print the mat according the orders*/
void Matrix::print() const
{
	std::cout << (*this);
}

/**
 * copy mat m to our mat.
 * @param m matrix to copy from
 * @return self
 */
Matrix &Matrix::operator=(const Matrix &m)
{
	if (this == &m)
	{
		return *this;
	}

	delete[] _matrix;
	_rows = m._rows;
	_cols = m._cols;
	_matrix = new float[_rows * _cols];
	for (int i = 0; i < _rows * _cols; i++)
	{
		_matrix[i] = m[i];
	}

	return *this;
}

/**
 * multiplication matrices. self*m
 * @return new matrix.
 */
Matrix Matrix::operator*(const Matrix &m) const
{
	if (_cols != m._rows)
	{
		std::cerr << WRONG_ACTION;
		std::exit(EXIT_FAILURE);
	}
	Matrix newM(_rows, m._cols);
	int i, j, k;
	for (i = 0; i < _rows; ++i)
	{
		for (j = 0; j < m._cols; ++j)
		{
			for (k = 0; k < _cols; ++k)
			{
				newM(i, j) += ((*this)(i, k) * m(k, j));
			}
		}
	}
	return newM;
}

/**
 * multiplication matrix in scalar
 * @param c scalar. float
 * @return new mat
 */
Matrix Matrix::operator*(const float &c) const
{
	Matrix newM(*this);
	for (int i = 0; i < _cols * _rows; i++)
	{ newM._matrix[i] *= c; }
	return newM;

}

/**
 * multiplication matrix. self = self*m
 * @return self.
 */
Matrix &Matrix::operator*=(const Matrix &m)
{
	*this = ((*this) * m);
	return *this;

}

/**
 * multiplication matrix in scalar
 * @param c scalar - float
 * @return self.
 */
Matrix &Matrix::operator*=(const float &c)
{

	for (int i = 0; i < _rows * _cols; i++)
	{ _matrix[i] *= c; }
	return *this;

}

/**
 * multiplication division in scalar
 * @param c scalar - float
 * @return conclusion matrix
 */
Matrix Matrix::operator/(const float &c) const
{
	if (c == 0)
	{
		{
			std::cerr << ZERO_DIVISION;
			std::exit(EXIT_FAILURE);
		}
	}
	Matrix newM(*this);
	for (int i = 0; i < _rows * _cols; i++)
	{ newM._matrix[i] /= c; }
	return newM;
}

/**
 * multiplication division in scalar
 * @param c scalar - float
 * @return self.
 */
Matrix &Matrix::operator/=(const float &c)
{
	*this = ((*this) / c);
	return (*this);
}


/**
 * friend of class, multiplication matrix in scalar too
 * @param c scalar
 * @param m matrix
 * @return the new mat.
 */
Matrix operator*(const float &c, Matrix &m)
{
	Matrix newM = (m * c);
	return newM;
}

/**
 * addition two matrices. this+m = new matrix
 * @return the new matrix
 */
Matrix Matrix::operator+(const Matrix &m) const
{
	Matrix newM(*this);
	newM += m;
	return newM;
}

/**
 * add to this matrix another matrix
 * @param m - matrix
 * @return this
 */
Matrix &Matrix::operator+=(const Matrix &m)
{
	if (_rows != m._rows || _cols != m._cols)
	{
		std::cerr << WRONG_ACTION;
		std::exit(EXIT_FAILURE);
	}
	for (int i = 0; i < _cols * _rows; i++)
	{ _matrix[i] += m._matrix[i]; }
	return *this;
}

/**
 * add to this matrix float in each cell
 * @param c- float number
 * @return this
 */
Matrix &Matrix::operator+=(const float &c)
{
	for (int i = 0; i < _rows * _cols; i++)
	{ _matrix[i] += c; }
	return *this;
}


/**
 * find matrix(i,j) for const matrix
 * @return the value  in (i,j)
 */
float Matrix::operator()(int i, int j) const
{
	if (i >= _rows || j >= _cols || j < 0 || i < 0)
	{
		std::cerr << WRONG_RANGE;
		std::exit(EXIT_FAILURE);
	}
	return _matrix[(i * _cols) + j];
}

/**
 * find matrix(i,j)
 * @return reference to value
 */
float &Matrix::operator()(int i, int j)
{
	if (i >= _rows || j >= _cols || j < 0 || i < 0)
	{
		std::cerr << WRONG_RANGE;
		std::exit(EXIT_FAILURE);
	}
	return _matrix[(i * _cols) + j];
}

/**
 * const method for find matrix(k) like the matrix vectorised.
 * matrix (i,j)=matrix(i*rows size + j) = matrix (k)
 * @return the value
 */
float Matrix::operator[](int i) const
{
	if (i >= _rows * _cols || i < 0)
	{
		std::cerr << WRONG_RANGE;
		std::exit(EXIT_FAILURE);
	}
	return _matrix[i];
}

/**
 * find matrix(i) like the matrix vectorised.
 * matrix (i,j)=matrix(i*rows size + j) = matrix (k)
 * @return reference to value
 */
float &Matrix::operator[](int i)
{
	if (i >= _rows * _cols || i < 0)
	{
		std::cerr << WRONG_RANGE;
		std::exit(EXIT_FAILURE);
	}
	return _matrix[i];
}

/**
 * bool function to check if two matrix are the same
 * @param m the another matrix
 * @return true if they are same, false otherwise
 */
bool Matrix::operator==(const Matrix &m) const
{
	if (this == &m)
	{
		return true;
	}
	if (_rows == m._rows && _cols == m._cols)
	{
		for (int i = 0; i < _rows * _cols; i++)
		{
			if (_matrix[i] != m[i])
			{ return false; }
		}
		return true;
	}
	return false;
}

/**
 * bool function to check if two matrix are not the same
 * @param m the another matrix
 * @return false if they are same, true otherwise
 */

bool Matrix::operator!=(const Matrix &m) const
{
	return !((*this) == m);
}


/**
 * friend of class. get istream and fill the matrix accordingly
 * @param is stream
 * @param m matrix
 * @return the stream
 */
std::istream &operator>>(std::istream &is, Matrix &m)
{
	std::string line;

	int k = 0;

	while (std::getline(is, line))
	{
		if (k == m.getRows())
		{
			k++;
			break;
		}

		float value;
		std::stringstream ss(line);
		int i = 0;

		for (; i < m.getCols(); i++)
		{
			ss >> value;
			m(k, i) = value;
		}
		k++;
	}
	if (is.bad() || k != m.getRows() || (is.peek() != EOF && !is.eof()))
	{
		std::cerr << WRONG_INPUT;
		std::exit(EXIT_FAILURE);
	}
	return is;
}


/**
 * get a stream and print the mat to it
 * @return the stream.
 */
std::ostream &operator<<(std::ostream &os, const Matrix &m)
{
	for (int i = 0; i < m.getRows(); i++)
	{
		for (int j = 0; j < m.getCols(); j++)
		{
			if (j == m.getCols() - 1)
			{
				if (i != m.getRows() - 1)
				{
					os << m(i, j) << "\n";
				}
				else
				{
					os << m(i, j);
				}

			}
			else
			{
				os << m(i, j) << " ";
			}
		}

	}
	return os;
}
