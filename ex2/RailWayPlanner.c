#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>


#define TRUE 1
#define FALSE 0
#define EXIT_FAILURE 1
#define LINE_LENGHT 1025
#define MAX_CHARECTERS 256
/**
 * the struct fill the thing we nedd to now on every connector part
 * @param start: its char that represent the right side of part
 * @param end: its char that represent the left side of part
 * @param length ,price:present the length of part and its price
 * @param startIndex: it the index of the char "start" in the array of all character that valid
 * (and its need for the algo and to avoid duplicates to find it.
 */
typedef struct connector
{
	char start;
	char end;
	long int length;
	long int price;
	int startIndex;

} connector;

struct connector *readFile(char *fileName, char *characters);

int inputManager(char *str, char *characters, connector *listOfConnectors);

int connectionLines(char *str, char *characters, connector *listOfConnectors);

int charactersLine(const char *str, char *characters);

int printCenter(int param);

int findIndexInArr(char letter, char *arr);

int algo(char *characters, connector *listOfConnectors);

long int **createTable(char *characters, connector *listOfConnectors);

void fillTable(int n, long int **table, long int *line, const char *characters, connector *conList);

long checkInt(char *str);

int countLines(FILE *fileName);


int lineCounter = 0;
int indexInArrey = 0;
long railLength = 0;
long connectionsNum = 0;
long minPrice = INT_MAX;
char lineInFile[LINE_LENGHT];


int main(int argc, char **argv)
{
	char characters[MAX_CHARECTERS];
	if (argc != 2)
	{
		printCenter(1);
		return EXIT_FAILURE;
	}
	connector *ConnectorsList = readFile(argv[1], characters);
	if (ConnectorsList != NULL && algo(characters, ConnectorsList) == TRUE)
	{
		free(ConnectorsList);
	}
	else
	{
		free(ConnectorsList);
		return EXIT_FAILURE;
	}
}
connector *readFile(char *fileName, char *characters)
/**
*this func read the file lineCounter by lineCounter end send every kine to another func to check.
* @param fileName fileName to open
* @param characters empty array to put in it char from lineCounter 3.
* @return if everything ok the return value to array of struct that fill all the lines, else it
 * return struct NULL.
*/
{

	FILE *inputFile = fopen(fileName, "r");
	if (inputFile == NULL)
	{
		printCenter(2);
		return NULL;
	}
	connector *listOfConnectors = malloc(countLines(inputFile) * sizeof(connector));
	if (listOfConnectors == NULL)
	{
		return NULL;
	}
	while (fgets(lineInFile, LINE_LENGHT, (FILE *) inputFile) != NULL)
	{
		lineCounter++;
		if (!(inputManager(lineInFile, characters, listOfConnectors)))
		{
			printCenter(4);
			return NULL;
		}
	}
	if (lineCounter == 0)
	{
		printCenter(3);
		return NULL;
	}

	if (fclose(inputFile) == EOF)
	{
		return NULL;
	}
	return listOfConnectors;
}

int countLines(FILE *fileName)
/**
 * the func counting lines inside file by counting '\n'
 * @param fileName file name at string
 * @return the nums of lines
 */
{
	int c;
	int count = 0;
	for (c = getc(fileName); c != EOF; c = getc(fileName))
	{
		if (c == '\n')
		{
			count++;
		}
	}
	fseek(fileName, 0, SEEK_SET);
	return count;
}


int inputManager(char *str, char *characters, connector *listOfConnectors)
/**
 * this func manage the input according the orders
 * for each lineCounter it check or send to another func to check
 * @return TRUE if success FALSE if process failed
 */
{


	if (lineCounter == 1)
	{
		railLength = checkInt(str);
		if (railLength < 0)
		{
			return FALSE;
		}
	}
	if (lineCounter == 2)
	{
		connectionsNum = checkInt(str);
		if (connectionsNum <= 0)
		{
			return FALSE;
		}
	}
	if (lineCounter == 3)
	{
		if (!(charactersLine(str, characters)))
		{

			return FALSE;
		}
	}
	if (lineCounter > 3)
	{
		if (!(connectionLines(str, characters, listOfConnectors)))
		{
			return FALSE;
		}
	}
	return TRUE;
}

long checkInt(char *str)
/**
 * tiny func to chack if all char in string are nums
 * @param str string to check
 * @return return the num if process succeed, and -1 if not
 */
{
	char *jank;
	long num = strtol(str, &jank, 10);
	if (jank[0] != '\0' && jank[0] != '\n' && jank[0] != '\r')
	{
		return -1;
	}
	return num;
}

int charactersLine(const char *str, char *characters)
/**
 * the func will check each argument of chartres lineCounter if OK, and will store it
 * in array/
 * @param str the lineCounter from file
 * @param characters its the array that install in the main
 * @return TRUE if success FALSE if process failed
 */
{
	int counter = 0;
	for (int i = 0; i <= (int) strlen(str); i++)
	{
		if (str[i] == '\r' || str[i] == '\n')
		{
			return TRUE;
		}
		if (i % 2 == 0)
		{
			if (findIndexInArr(str[i], characters) == -1)
			{
				characters[counter] = str[i];
				counter++;
			}
			else
			{
				connectionsNum--;
			}
		}
		if (i % 2 != 0)
		{
			if (str[i] != ',')
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}


int connectionLines(char *str, char *characters, connector *listOfConnectors)
/**
 * this func get the lineCounter from file (@param str) and check if right and store it in struct array
 * (@param listOfConnectors)
 * @param str lineCounter from file
 * @param characters: the array of characters that valid
 * @param listOfConnectors: array of struct that will contain all part of the rail
 * @return TRUE if success FALSE if process failed
 */
{
	int counter = 0;
	if (str[0] == ',' || str[1] != ',' || str[3] != ',')
	{
		return FALSE;
	}
	for (int i = 0; i <= (int) strlen(str); i++)
	{
		if (str[i] == '\r' || str[i] == '\n' || str[i] == '\0')
		{
			break;
		}

		if (str[i] == ',')
		{
			counter++;
		}

		else if (counter == 0 || counter == 1)
		{
			if (findIndexInArr(str[i], characters) == -1)
			{
				return FALSE;
			}

		}
		else if (counter == 2 || counter == 3)
		{
			if (!(str[i] >= '0' && str[i] <= '9'))
			{
				return FALSE;
			}
		}
		else if (counter == 4)
		{
			return FALSE;
		}

	}
	sscanf(str, "%c,%c,%ld,%ld", &listOfConnectors[indexInArrey].start,
		   &listOfConnectors[indexInArrey].end,
		   &listOfConnectors[indexInArrey].length,
		   &listOfConnectors[indexInArrey].price);
	listOfConnectors[indexInArrey].startIndex = findIndexInArr(listOfConnectors[indexInArrey].start,
															   characters);

	if (0 >= listOfConnectors[indexInArrey].length || 0 >= listOfConnectors[indexInArrey].price)
	{
		return FALSE;
	}

	indexInArrey++;
	return TRUE;


}

int findIndexInArr(char letter, char *arr)
/**
 * the func find the first index that letter appear in string
 * @param letter any char
 * @param arr any string
 * @return first index thet letter appear, or -1 if doesnt appear
 */
{
	for (int i = 0; i < (int) strlen(arr); i++)
	{
		if (letter == arr[i])
		{
			return i;
		}

	}
	return -1;
}

int printCenter(int param)
/**
 * this punc will print to output file the requested print
 * param 1 for printing if args did not the standard
 * param 2 if file doesn't exist
 * param 3 if file is empty
 * param 4 if invalid file for a reason
 * param 5 to print the output of the algorithm
 */

{


	FILE *output;
	output = fopen("railway_planner_output.txt", "w");
	if (output == NULL)
	{
		return FALSE;
	}
	if (param == 1)
	{
		fprintf(output, "Usage: RailWayPlanner <InputFile>");
	}
	if (param == 2)
	{
		fputs("File doesn't exists.", output);
	}
	if (param == 3)
	{
		fputs("File is empty.", output);
	}

	if (param == 4)
	{
		fprintf(output, "Invalid input in line: %d.", lineCounter);
	}
	if (param == 5)
	{
		fprintf(output, "The minimal price is: %ld", minPrice);
	}
	if (fclose(output) == EOF)
	{
		return FALSE;
	}
	return TRUE;
}

int algo(char *characters, connector *listOfConnectors)
/**
 *the first func of algorithm. it send to bild array and fill, and than find the nin of
 * array[rail length].
 * @param characters: the array of characters that valid
 * @param listOfConnectors: array of struct that contain all part of the rail
 * @return TRUE if success FALSE if process failed
 */
{
	long int **table = createTable(characters, listOfConnectors);
	if (table != NULL)
	{
		minPrice = INT_MAX;
		for (int i = 0; i < connectionsNum; i++)
		{

			if (minPrice > table[railLength][i])
			{
				minPrice = table[railLength][i];
			}

		}
		if (minPrice >= INT_MAX)
		{
			minPrice = -1;
		}

		for (int i = 0; i <= railLength; i++)
		{
			free(table[i]);
		}
		free(table);
		printCenter(5);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

long int **createTable(char *characters, connector *listOfConnectors)
/**
 * its bild array (in size of  rail_length) of arrays (in size of num of
 * connectors). send to another func to fill the table
 * @param characters: the array of characters that valid
 * @param listOfConnectors: array of struct that contain all part of the rail
 * @return pointer to table
 */
{
	long int **table = (long int **) malloc((railLength + 1) * sizeof(long int *));
	if (table == NULL)
	{
		return NULL;
	}
	for (int i = 0; i <= railLength; i++)
	{
		long int *lineInTable = (long int *) malloc(connectionsNum * sizeof(long int));
		if (lineInTable == NULL)
		{
			return NULL;
		}
		fillTable(i, table, lineInTable, characters, listOfConnectors);
		table[i] = lineInTable;
	}
	return table;
}

void fillTable(int n, long int **table, long int *line, const char *characters, connector *conList)
/**
 * the func fill the table according the algorithm. it base that for every n the table until n are full
 * @param n: lineCounter num n to fill it
 * @param table two dimensional array
 * @param line: empty array of ints for one line in table
 * @param characters: the array of characters that valid
 * @param conList: array of struct that contain all part of the rail
 */
{

	for (int i = 0; i < connectionsNum; i++)
	{
		long int min = INT_MAX;
		line[i] = min;
		for (int j = 0; j < indexInArrey; j++)
		{
			if (conList[j].end == characters[i] && n - conList[j].length >= 0)
			{
				if ((conList[j].price + table[n - conList[j].length][conList[j].startIndex]) < min)
				{
					min = conList[j].price + table[n - conList[j].length][conList[j].startIndex];
				}

			}
		}
		line[i] = min;
		if (n == 0)
		{
			line[i] = 0;
		}

	}
}



