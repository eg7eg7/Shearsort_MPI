/*
EDEN DUPONT 204808596
DANIIL ROLNIK 334018009
*/


#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG //prints throughout file
//#define DEBUG_CART //prints new ranks of processes and their coordinates
//#define DEBUG_TOK //reading file debug
#define PRINT_MATRICES


struct Pixel
{
	int id;
	int x;
	int y;
	float rgb[3];
};

void setPixel(struct Pixel *pix, char *s)
{
	char * _word;
	_word = strtok(s, " ");
#ifdef DEBUG_TOK
	printf("token = %s\n", _word);
#endif // DEBUG_TOK
	(*pix).id = atoi(_word);
	_word = strtok(NULL, " ");
#ifdef DEBUG_TOK
	printf("token = %s\n", _word);
#endif // DEBUG_TOK
	(*pix).x = atoi(_word);
	_word = strtok(NULL, " ");
#ifdef DEBUG_TOK
	printf("token = %s\n", _word);
#endif // DEBUG_TOK
	(*pix).y = atoi(_word);
	_word = strtok(NULL, " ");
#ifdef DEBUG_TOK
	printf("token = %s\n", _word);
#endif // DEBUG_TOK
	(*pix).rgb[0] = (float) atof(_word);
	_word = strtok(NULL, " ");
#ifdef DEBUG_TOK
	printf("token = %s\n", _word);
#endif // DEBUG_TOK
	(*pix).rgb[1] = (float) atof(_word);
	_word = strtok(NULL, " ");
#ifdef DEBUG_TOK
	printf("token = %s\n", _word);
#endif // DEBUG_TOK
	(*pix).rgb[2] = (float) atof(_word);
}
void readPixelFromFile(struct Pixel **pixels, int *num_elements)
{
	const int line_size = 500;
	char* line = (char*)malloc(line_size);
	int row = 0;
	int col = 0;
	int num_elements_file = 0;
	int num_in_line;
	FILE *file;
	file = fopen("C:\\pixel.txt", "r");
	if (file == NULL)
	{
		printf("Failed to open file.");
		exit(EXIT_FAILURE);
	}

	if (fgets(line, line_size, file) != NULL)
	{
		num_elements_file = atoi(line);
#ifdef DEBUG_TOK
		printf("num of elements = %d\n", num_elements_file);
#endif // DEBUG_TOK
		*num_elements = num_elements_file;
		num_in_line = (int)sqrt(num_elements_file);
#ifdef DEBUG_TOK
		printf("num_in_line = %d\n", num_in_line);
#endif // DEBUG_TOK
		*pixels = (struct Pixel*) malloc(sizeof(struct Pixel)*num_elements_file);
	}

	for (int i = 0; i < num_elements_file && fgets(line, line_size, file) != NULL; i++)
	{
		row = (int)i / num_in_line;
		col = i % num_in_line;
#ifdef DEBUG_TOK
		printf("row = %d col = %d, parsing = %s\n", row, col, line);
#endif // DEBUG_TOK
		setPixel(&((*pixels)[i]), line);
	}
	fclose(file);
	free(line);
}


//If both pixels are black (rgb[] = {0.0, 0.0, 0.0}) – compare distances from the pixel to the coordinate origin (0, 0). 
//Otherwise compare their intensity - sum of elements of arrays rgb[] of the pixels.
int compareAsc(struct Pixel p1, struct Pixel p2)
{
	float color1_1 = p1.rgb[0];
	float color1_2 = p1.rgb[1];
	float color1_3 = p1.rgb[2];
	float color2_1 = p2.rgb[0];
	float color2_2 = p2.rgb[1];
	float color2_3 = p2.rgb[2];

	float color1 = sqrtf(color1_1*color1_1 + color1_2 * color1_2 + color1_3 * color1_3);
	float color2 = sqrtf(color2_1*color2_1 + color2_2 * color2_2 + color2_3 * color2_3);

	float intensity1 = color1_1 + color1_2 + color1_3;
	float intensity2 = color2_1 + color2_2 + color2_3;

	if (color1 > color2)
		return -1;
	else if (color1 < color2)
		return 1;

	double p1_length = sqrt(p1.x*p1.x + p1.y * p1.y);
	double p2_length = sqrt(p2.x*p2.x + p2.y * p2.y);

	if (p1_length > p2_length)
		return -1;
	else if (p1_length < p2_length)
		return 1;

	if (intensity1 > intensity2)
		return -1;
	else if (intensity1 < intensity2)
		return 1;

	return 0;
}

//If both pixels are black (rgb[] = {0.0, 0.0, 0.0}) – compare distances from the pixel to the coordinate origin (0, 0). 
//Otherwise compare their intensity - sum of elements of arrays rgb[] of the pixels.
int compareDesc(struct Pixel p1, struct Pixel p2)
{
	return (-1)*compareAsc(p1, p2);
}


void printPixel(struct Pixel *pix)
{
	printf("pixel id=%4d\trgb = %.2f,%.2f,%.2f\tx = %6d\ty = %6d\n", pix->id, pix->rgb[0], pix->rgb[1], pix->rgb[2], pix->x, pix->y);
}

//order = 1 print forward
//order = 0 print backwards
void printPixelsRow(struct Pixel * pixels, int size, int order) {
	int i;
	if (order)
		for (i = 0; i < size; i++)
			printPixel(&pixels[i]);
	else
		for (i = size - 1; i >= 0; i--)
			printPixel(&pixels[i]);
}

//if no neighbour process, send -1
//location = rank of process
//right/left = rank of neighbour processes, send -1 if none.
void oddEvenSort(int location, int left, int right, MPI_Datatype MPIType, MPI_Comm comm, struct Pixel *myPixel, int(*compar1)(struct Pixel, struct Pixel), int num_row)
{
	MPI_Status status;
	struct Pixel otherPixel;
	int(*compare)(struct Pixel, struct Pixel);
	compare = compar1;

	for (int i = 0; i < num_row; i++)
	{
		if (i % 2 == 0)
		{
			if (location % 2 == 0 && right > -1)
			{
				//receive from odd
				MPI_Sendrecv(myPixel, 1, MPIType, right, 0, &otherPixel, 1, MPIType, right, 0, comm, &status);
				if (compare(*myPixel, otherPixel) == -1)
				{
					*myPixel = otherPixel;
				}
			}
			else if (location % 2 == 1 && left > -1)
			{
				//receive from even
				MPI_Sendrecv(myPixel, 1, MPIType, left, 0, &otherPixel, 1, MPIType, left, 0, comm, &status);
				if (compare(*myPixel, otherPixel) == 1)
				{
					*myPixel = otherPixel;
				}
			}
		}
		else
		{
			if (location % 2 == 1 && location > 0 && right > -1)
			{
				//receive from even
				MPI_Sendrecv(myPixel, 1, MPIType, right, 0, &otherPixel, 1, MPIType, right, 0, comm, &status);
				if (compare(*myPixel, otherPixel) == -1)
				{
					*myPixel = otherPixel;
				}
			}
			else if (location % 2 == 0 && left > -1)
			{
				//receive from odd
				MPI_Sendrecv(myPixel, 1, MPIType, left, 0, &otherPixel, 1, MPIType, left, 0, comm, &status);
				if (compare(*myPixel, otherPixel) == 1)
				{
					*myPixel = otherPixel;
				}
			}
		}
	}
}

void printMatrixAsSnake(struct Pixel * pixels, int size)
{
	int order = 1;
	printf("Printing matrix..\n");
	for (int i = 0; i < size; i++)
	{
		printf("Printing row %d\n\n", i);
		printPixelsRow(&(pixels[i*size]), size, order);
		order = !order;
	}
}

void Shearsort(struct Pixel *myPixel, int myRank, int matrix_size, MPI_Comm comm_orig_world, MPI_Datatype MPIType,int orderDesc) {
	MPI_Comm comm;
	int dim[2]; //setting the cartesian dimensions
	int period[] = { 0,0 };
	int coord[2];
	int row_length = (int)sqrt(matrix_size);
	dim[0] = row_length;
	dim[1] = row_length;
	int newRank = -1;
	int reorder = 1;
	int right, left, up, down;

	MPI_Cart_create(comm_orig_world, 2, dim, period, reorder, &comm);
	MPI_Cart_shift(comm, 1, 1, &left, &right);
	MPI_Cart_shift(comm, 0, 1, &up, &down);
	MPI_Cart_coords(comm, myRank, 2, coord);
	MPI_Comm_rank(comm, &newRank);
#ifdef DEBUG_CART
	printf("rank %d coord:(%d,%d)\tleft=%d right=%d,up=%d,down=%d\n", newRank, coord[0], coord[1], left, right, up, down);
#endif //  DEBUG_CART

	int(*compare1)(struct Pixel, struct Pixel) = compareAsc;
	int(*compare2)(struct Pixel, struct Pixel) = compareDesc;
	if (orderDesc)
	{
		compare1 = compareDesc;
		compare2 = compareAsc;
	}
	int num_iterations = (int)(2 * log(row_length) + 1);
	for (int i = 0; i < num_iterations; i++)
	{
		if (i % 2 == 0)
		{
			//sort rows
			if (coord[0] % 2 == 0)
			{
				//even row - sort 
				oddEvenSort(newRank, left, right, MPIType, comm, myPixel, compare1, row_length);
			}
			else
			{
				//odd row - sort in opposite way
				oddEvenSort(newRank, left, right, MPIType, comm, myPixel, compare2, row_length);
			}
		}
		else
		{
			//sort columns - same order as even rows
			oddEvenSort(newRank, up, down, MPIType, comm, myPixel, compare1, row_length);
		}
	}
}

void MPI_Init_with_datatype(int argc, char *argv[], MPI_Datatype *PixelMPIType) {
	struct Pixel pixel;
	
	MPI_Datatype type[4] = { MPI_INT, MPI_INT, MPI_INT, MPI_FLOAT };
	int blocklen[4] = { 1, 1, 1, 3 };
	MPI_Aint disp[4];

	MPI_Init(&argc, &argv);


	// Create MPI user data type for partical
	disp[0] = (char *)&pixel.id - (char *)&pixel;
	disp[1] = (char *)&pixel.x - (char *)&pixel;
	disp[2] = (char *)&pixel.y - (char *)&pixel;
	disp[3] = (char *)pixel.rgb - (char *)&pixel;
	MPI_Type_create_struct(4, blocklen, disp, type, PixelMPIType);
	MPI_Type_commit(PixelMPIType);
}

void main(int argc, char *argv[])
{
	MPI_Datatype PixelMPIType;
	MPI_Init_with_datatype(argc, argv, &PixelMPIType);
	int myrank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	struct Pixel* pixels;
	struct Pixel myPixel;
	int num_elements_in_matrix = 0;
	int num_elements_in_row;
	int abort = 0;

	if (myrank == 0)
	{
		readPixelFromFile(&pixels, &num_elements_in_matrix);
		//check if number in file matches processes
		num_elements_in_row = (int)sqrt(num_elements_in_matrix);
		if (size != num_elements_in_matrix || num_elements_in_row * num_elements_in_row != num_elements_in_matrix) {
			printf("Text file pixel.txt not matching.\nPlease run with %d processes.\nMake sure number of elements can be squared.\n", num_elements_in_matrix); fflush(stdout);
			abort = 1;
		}
	}
	MPI_Bcast(&abort, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (abort)
	{
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	MPI_Bcast(&num_elements_in_matrix, 1, MPI_INT, 0, MPI_COMM_WORLD);
	num_elements_in_row = (int)sqrt(num_elements_in_matrix);


#ifdef PRINT_MATRICES
	if (myrank == 0)
	{
		printf("********************************************\n");
		printf("Before sorting:\n");
		printMatrixAsSnake(pixels, num_elements_in_row);
		printf("********************************************\n");
	}
#endif // PRINT_MATRICES

	MPI_Scatter(pixels, 1, PixelMPIType, &myPixel, 1, PixelMPIType, 0, MPI_COMM_WORLD);
	int orderDescending = 1;
	Shearsort(&myPixel, myrank, num_elements_in_matrix, MPI_COMM_WORLD, PixelMPIType, orderDescending);

#ifdef DEBUG
	printf("mat size broadcasted = %d, myrank=%d , myPixel = %d\n", mat_size, myrank, myPixel.id);
	printPixel(&myPixel);
#endif // DEBUG

	MPI_Gather(&myPixel, 1, PixelMPIType, pixels, 1, PixelMPIType, 0, MPI_COMM_WORLD);

	if (myrank == 0)
	{
#ifdef PRINT_MATRICES
		printf("********************************************\n");
		printf("After sorting:\n");
		printMatrixAsSnake(pixels, num_elements_in_row);
		printf("********************************************\n");
#endif // PRINT_MATRICES
		free(pixels);
	}
	MPI_Finalize();
}