#include <stdio.h>
#include <string.h>

#define FIELD_LENGTH  42
#define STUDENT_LIST_LENGTH 5500
#define LINE_LENGTH 62
#define TRUE  1
#define FALSE 0

/**
 *@param id: the ID of student
 *@param grade: the grade of student
 *@param age: the age of student
 *@param name: string of the name of student
 *@param county: string of the country of student
 *@param city: string of the city of student
 */
typedef struct student
{

	long int ID;
	int grade;
	int age;
	char Name[FIELD_LENGTH];
	char country[FIELD_LENGTH];
	char city[FIELD_LENGTH];
} student;

int main(int argc, char *argv[]);

int mode(char args[]);

int separator(char *line, int lineNum);

int checkId(char *id);

int checkStr(char str[], int field);

int checkGrade(char grade[]);

int checkAge(char age[]);

void merge(struct student *arr, int start, int mid, int end);

void mergeSort(struct student *arr, int start, int end);

int checkInt(char arr[]);

void printStudents();

int partition(student *arr, int start, int end);

void swap(student *arr, int i, int j);

void quickSort(student *arr, int start, int end);

int stuNum = 0;
student gAllStudent[STUDENT_LIST_LENGTH];

int mode(char args[])
/**
 *this function follow the orders for the arguments the plan starting
 * @param args array that present user argument
 * @return false if process fail, and true if success
 */
{
	if (strcmp(args, "best") == 0)
	{
		double best = 0;
		int bestStu = 0;
		int i;
		double achievement;
		if (stuNum > 0)
		{
			for (i = 0; i <= stuNum; i++)
			{
				achievement = (double) gAllStudent[i].grade / gAllStudent[i].age;
				if (achievement > best)
				{
					best = achievement;
					bestStu = i;
				}

			}


			printf(
					"best student info is: %ld,%s,%d,%d,%s,%s\n", gAllStudent[bestStu].ID,
					gAllStudent[bestStu].Name,
					gAllStudent[bestStu].grade, gAllStudent[bestStu].age,
					gAllStudent[bestStu].country,
					gAllStudent[bestStu].city);

		}
	}

	else if (strcmp(args, "merge") == 0)
	{
		mergeSort(gAllStudent, 0, stuNum - 1);
		printStudents();


	}
	else if (strcmp(args, "quick") == 0)
	{
		quickSort(gAllStudent, 0, stuNum - 1);
		printStudents();

	}
	else
	{

		return FALSE;
	}
	return TRUE;
}

void printStudents()
/**
 * the function print all the students by order
 */
{
	for (int k = 0; k < stuNum; k++)
	{
		printf("%ld,%s,%d,%d,%s,%s\n", gAllStudent[k].ID, gAllStudent[k].Name,
			   gAllStudent[k].grade, gAllStudent[k].age, gAllStudent[k].country,
			   gAllStudent[k].city);
	}
}

int separator(char *line, int lineNum)
/**
 * the function spared the line from user by ',' end send every field to anther functions to
 * check if the field right
 * @param line: the input from user
 * @param lineNum: the line's number
 * @return false if line not correct, and true if success
 */
{

	int j = 0;
	int p = 0;
	char newStr[FIELD_LENGTH];
	for (long unsigned int i = 0; i <= strlen(line); i++)
	{


		if (line[i] == ',')
		{
			newStr[p] = '\0';
			j++;
			if (j == 1)
			{
				if (!(checkId(newStr)))
				{
					printf("ERROR: The ID number should be 10 digits, and no 0 digit at first\n");
					printf("in line %d\n", lineNum);
					return 0;
				}
			}

			if (j == 2)
			{
				if (!(checkStr(newStr, 2)))
				{
					printf("ERROR: The name field can only contain Latin letters, space, or '-' sign \n");
					printf("in line %d\n", lineNum);
					return 0;

				}
			}
			if (j == 3)
			{
				if (!(checkGrade(newStr)))
				{
					printf("ERROR: The grade field must be a number between 0 and 100\n");
					printf("in line %d\n", lineNum);
					return 0;

				}
			}
			if (j == 4)
			{
				if (!(checkAge(newStr)))
				{
					printf("ERROR: The age field must be a number between 18 and 120\n");
					printf("in line %d\n", lineNum);
					return 0;
				}
			}
			if (j == 5)
			{
				if (!(checkStr(newStr, 5)))
				{
					printf("ERROR: The country field can only contain Latin letters, or '-' sign\n");
					printf("in line %d\n", lineNum);
					return 0;

				}
			}
			if (j == 6)
			{
				if (!(checkStr(newStr, 6)))
				{
					printf("ERROR: The city field can only contain Latin letters, or '-' sign\n");
					printf("in line %d\n", lineNum);
					return 0;

				}
			}
			p = 0;
		}
		else
		{
			newStr[p] = line[i];
			p++;
		}

	}
	return 1;

}

int checkId(char *id)
/**
 *
 * @param id: the id field
 * @return false if ID not correct, and true if success
 */
{
	int x;


	x = sscanf(id, "%ld", &gAllStudent[stuNum].ID);
	if (id[0] == '0' || strlen(id) != 10 || x == FALSE || checkInt(id) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}


int checkStr(char *str, int field)
/**
 *check if the field follow the orders of exercise
 * @param str: the str from user
 * @param field: the kind of str. 2 for name, 5 for country and 6 for city.
 * @return false if the string not correct or not follow the rules, and true if success
 */
{
	char x;
	if (strlen(str) == FALSE)
	{
		return FALSE;
	}
	for (long unsigned int i = 0; i < strlen(str); i++)
	{

		x = str[i];
		if ((!(x <= 'Z' && 'A' <= x)) && (!(x <= 'z' && 'a' <= x)) && x != '-' && x != ' ')
		{
			return FALSE;
		}
		if (field != 2 && x == ' ')
		{
			return FALSE;
		}
	}
	if (field == 2)
	{
		strcpy(gAllStudent[stuNum].Name, str);
	}
	if (field == 5)
	{
		strcpy(gAllStudent[stuNum].country, str);
	}
	if (field == 6)
	{
		strcpy(gAllStudent[stuNum].city, str);
	}

	return TRUE;
}

int checkGrade(char grade[])
/**
 *
 * @param grade: the grade number that user inputted
 * @return false if the grade not correct or not follow the rules, and true if success
 */
{
	int x;
	x = sscanf(grade, "%d", &gAllStudent[stuNum].grade);
	if (gAllStudent[stuNum].grade > 100 || gAllStudent[stuNum].grade < 0 || x == FALSE ||
		checkInt(grade) == FALSE)
	{

		return FALSE;
	}
	return TRUE;
}

int checkAge(char age[])
/**
 *
 * @param age: the age number that user inputted
 * @return false if the age not correct or not follow the rules, and true if success
 */
{
	int x;
	x = sscanf(age, "%d", &gAllStudent[stuNum].age);
	if (gAllStudent[stuNum].age > 120 || gAllStudent[stuNum].age < 18 || x == FALSE ||
		checkInt(age) == FALSE)
	{

		return FALSE;
	}
	return TRUE;
}

int checkInt(char arr[])
/**
 * check if ell cells in array are ints
 * @param arr: array
 * @return false if the string not contain just nums, and true if success
 */
{
	for (long unsigned int i = 0; i < strlen(arr); i++)
	{
		if (!(arr[i] >= '0' && arr[i] <= '9'))
		{
			return FALSE;
		}
	}
	return TRUE;


}

void mergeSort(student *arr, int start, int end)
/**
 * this function make the first step of marge sort algorithm
 * and split the array to 2 parts
 * @param arr: array of student struct
 * @param start: index of starting array
 * @param end: index of ending array
 */
{
	int mid = 0;

	if (start < end)
	{
		mid = (start + end) / 2;
		mergeSort(arr, start, mid);
		mergeSort(arr, mid + 1, end);
		merge(arr, start, mid, end);
	}
}

void merge(student *arr, int start, int mid, int end)
/**
 * the function pass every cell between tow arrays, compere, and make them into one new
 * @param arr array of student struct
 * @param start
 * @param mid
 * @param end
 */
{
	int i = 0;
	int m = 0;
	int k = 0;
	int l = 0;
	struct student new[STUDENT_LIST_LENGTH];

	l = start;
	i = start;
	m = mid + 1;

	while ((l <= mid) && (m <= end))
	{
		if (arr[l].grade <= arr[m].grade)
		{
			new[i] = arr[l];
			l++;
		}
		else
		{
			new[i] = arr[m];
			m++;
		}
		i++;
	}

	if (l > mid)
	{
		for (k = m + 1; k <= end; k++)
		{
			new[i] = arr[k];
			i++;
		}
	}
	else
	{
		for (k = l; k <= mid; k++)
		{
			new[i] = arr[k];
			i++;
		}
	}

	for (k = start; k <= end; k++)
	{
		arr[k] = new[k];
	}
}

int partition(student *arr, int start, int end)
/**
 * make the last of array pivot, and put after him
 * @param arr: array of student struct
 * @param start: index of starting array
 * @param end: index of ending array
 * @return the next pivot
 */

{

	char pivot[FIELD_LENGTH];
	strcpy(pivot, arr[end].Name);
	int i = (start - 1);  // Index of smaller element
	int s;


	for (int j = start; j <= end - 1; j++)
	{
		s = strncmp(arr[j].Name, pivot, FIELD_LENGTH);
		if (s < 0)  // If current element is smaller than the pivot
		{
			i++;
			swap(arr, i, j);
		}
	}
	swap(arr, i + 1, end);
	return (i + 1);
}

void quickSort(student *arr, int start, int end)
/**
 * The main function that implements QuickSort
 * @param arr: array of student struct
 * @param start: index of starting array
 * @param end: index of ending array
 */
{
	if (start < end)
	{
		/* pi is partitioning index, arr[p] is now
		   at right place */
		int pi = partition(arr, start, end);

		// Separately sort elements before
		// partition and after partition
		quickSort(arr, start, pi - 1);
		quickSort(arr, pi + 1, end);
	}
}


void swap(student *arr, int i, int j)
/**
 * this func replace the content of student between index i and j
 * @param arr array of student struct
 * @param i: i index
 * @param j: j index
 */
{
	student saver;
	saver = arr[i];
	arr[i] = arr[j];
	arr[j] = saver;
}


int main(int argc, char **argv)
/**
 *
 * @param argc num of args that get from user
 * @param argv the users arguments
 * @return return 0 if processes succeed, and 1 if not
 */
{
	if (argc != 2 || mode(argv[1]) == 0)
	{
		printf("USAGE: u should write one parameter. write 'best' to get the best student or "
			   "'merge' to "
			   "sort by grade or 'quick' to sort by name\n");
		return 1;
	}

	int line = 0;
	int checking;
	char stud[LINE_LENGTH];
	long unsigned int p;

	while (TRUE)
	{
		printf("Enter student info. To exit press q, then enter\n");
		if (fgets(stud, LINE_LENGTH, stdin) == NULL)
		{
			printf("ERROR: could not read info\nin line %d\n", line);
			return 1;
		}
		if ((stud[0] == 'q' && strlen(stud) == 2) || stud[0] == '\0')
		{
			break;
		}
		p = strlen(stud);
		stud[p - 1] = ',';
		stud[p] = '\0';
		checking = separator(stud, line);
		line++;
		if (checking)
		{

			stuNum++;

		}
	}
	mode(argv[1]);
	return 0;
}

