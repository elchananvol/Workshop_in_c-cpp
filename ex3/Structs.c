#include "Structs.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define LESS -1
#define EQUAL 0
#define GREATER 1


/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
	char *c = (char *) a;
	char *d = (char *) b;
	return strcmp(c, d);

}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
	char *p = (char *) pConcatenated;
	char *w = (char *) word;
	if (p != NULL && w != NULL)
	{

		strcat(p, w);
		strcat(p, "\n");
		return 1;
	}
	else
	{ return 0; }
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
	free(s);
}


/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
	Vector *c = (Vector *) a;
	Vector *d = (Vector *) b;
	int i = 0;


	while (1)
	{
		if (c->vector[i] == d->vector[i])
		{
			if (c->vector[i] == '\0')
			{
				return EQUAL;
			}
			else
			{ i++; }
		}
		else if (c->vector[i] == '\0')  //todo
		{
			return LESS;
		}
		else if (c->vector[i] == '\0')
		{
			return GREATER;
		}
		else if (c->vector[i] >= d->vector[i])
		{
			return GREATER;

		}
		else if (c->vector[i] <= d->vector[i])
		{
			return LESS;

		}

	}


}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
	if (pVector == NULL)
	{
		return;
	}
	Vector *p = (Vector *) pVector;
	if (p->vector != NULL)   //todo
	{
		free(p->vector);
	}
	free(pVector);
}

double findNorm(Vector *pVector);

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
	Vector *v = (Vector *) pVector;
	Vector *m = (Vector *) pMaxVector;
	if (v != NULL && m != NULL)
	{
		if (findNorm(v) > findNorm(m) || m->vector == NULL)
		{
			double *x = (double *) realloc(m->vector, v->len * sizeof(double));
			if (x == NULL)
			{
				return false;
			}
			for (int i = 0; i <= v->len; i++)
			{
				x[i] = v->vector[i];
			}
//			memcpy(x, v->vector, v->len * sizeof(double));
			m->vector = x;
			m->len = v->len;
			return true;
		}
		else
		{ return false; }
	}
	else
	{ return false; }
}

/**
 * calculate the norm^2 of vector
 * @param pVector vector
 * @return norm^2 or 0 if vector empty.
 */
double findNorm(Vector *pVector)
{
	double sum = 0;
	if (pVector->vector != NULL)
	{


		for (int i = 0; i < pVector->len; i++)
		{
			sum += (pVector->vector[i] * pVector->vector[i]);
		}
	}
	return sum;
}


/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
	Vector *v = (Vector *) malloc(sizeof(Vector));
	if (v == NULL)
	{
		return NULL;
	}
	v->vector = NULL;
	forEachRBTree(tree, copyIfNormIsLarger, v);
	return v;
}