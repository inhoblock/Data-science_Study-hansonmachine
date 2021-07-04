#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int** convolution(int** image, int** filter, int padding_size, int stride_size);
void print_array(int** arr, int size);
void free_array(int** arr, int size);

int main()
{
	int F, P, S;
	int** filter;
	int*** image;
	int*** convolution_image;
	int** convolution_R;
	int** convolution_G;
	int** convolution_B;

	// [W][W][3] image ¹Þ±â

	// Input F, P, S
	printf("Enter the size of Filter size F: ");
	scanf("%d", &F);
	printf("Enter the size of zero padding P: ");
	scanf("%d", &P);
	printf("Enter the size of stride S: ");
	scanf("%d", &S);
	

	filter = (int**)malloc(F * sizeof(int*));
	for (int i = 0; i < F; i++)
		filter[i] = (int*)malloc(F * sizeof(int));


	printf("-- Filter --\n");
	printf("Please enter %d values of Filter components.\n", F * F);
	for (int i = 0; i < F; i++) {
		for (int j = 0; j < F; j++) {
			scanf("%d", &filter[i][j]);
		}
	}

	convolution_R = convolution(image[0], filter, S, P);
	convolution_G = convolution(image[1], filter, S, P);
	convolution_B = convolution(image[2], filter, S, P);

	//make convolution_image matrix
	int X = _msize(convolution_R[0]) / sizeof(int);
	convolution_image = (int***)malloc(X * sizeof(int**));
	for (int i = 0; i < X; i++)
	{
		convolution_image[i] = (int**)malloc(X * sizeof(int*));

		for (int j = 0; j < X; j++)
		{
			convolution_image[i][j] = (int*)malloc(3 * sizeof(int));
		}
	}

	for (int k = 0; k < 3; k++)
	{
		for (int i = 0; i < X; i++)
		{
			for (int j = 0; j < X; j++)
			{
				if (k == 0)
					convolution_image[i][j][k] = convolution_R[i][j];

				else if (k == 1)
					convolution_image[i][j][k] = convolution_G[i][j];
				
				else
					convolution_image[i][j][k] = convolution_B[i][j];
			}
		}
	}

	free_array(filter, F);
	free_array(convolution_R, X);
	free_array(convolution_G, X);
	free_array(convolution_B, X);
	
	for (int i = 0; i < X; i++)
	{
		for (int j = 0; j < X; j++)
		{
			free(convolution_image[i][j]);
		}
		free(convolution_image[i]);
	}
	free(convolution_image);



	return 0;
}

int** convolution(int** image, int** filter, int padding_size, int stride_size)
{
	int W, F;
	W = _msize(image[0]) / sizeof(int);
	F = _msize(filter[0]) / sizeof(int);

	if ((W - F + 2 * padding_size) % stride_size != 0)
	{
		printf("convolution_size error");
		exit(1);
	}

	// make convolution_matrix
	int X = ((W - F + 2 * padding_size) / stride_size) + 1; // X : convolution matrix size
	int** convolution_matrix;

	convolution_matrix = (int**)malloc(X * sizeof(int*));
	for (int i = 0; i < X; i++)
		convolution_matrix[i] = (int*)malloc(X * sizeof(int));

	// make padding_image
	int** padding_image;

	padding_image = (int**)malloc((W + 2 * padding_size) * sizeof(int*));
	for (int i = 0; i < (W + 2 * padding_size); i++)
		padding_image[i] = (int*)malloc((W + 2 * padding_size) * sizeof(int));

	for (int i = 0; i < (W + 2 * padding_size); i++)
	{
		if (i < padding_size)
			for (int j = 0; j < (W + 2 * padding_size); j++)
				padding_image[i][j] = 0;

		else if (i < (W + 2 * padding_size) - padding_size)
		{
			for (int j = 0; j < padding_size; j++)
				padding_image[i][j] = 0;

			for (int j = padding_size; j < (W + 2 * padding_size) - padding_size; j++)
				padding_image[i][j] = image[i-padding_size][j-padding_size];

			for (int j = (W + 2 * padding_size) - padding_size; j < (W + 2 * padding_size); j++)
				padding_image[i][j] = 0;
		}

		else
			for (int j = 0; j < (W + 2 * padding_size); j++)
				padding_image[i][j] = 0;

	}

	// computation convolution
	int sum = 0;

	for (int i = 0; i < X; i++)
	{
		for (int j = 0; j < X; j++)
		{
			sum = 0;

			for (int p = 0; p < F; p++)
			{
				for (int q = 0; q < F; q++)
				{
					sum += filter[p][q] * padding_image[p + (i * stride_size)][q + (j * stride_size)];
				}
			}
			
			convolution_matrix[i][j] = sum;
		}
	}

	// free padding image
	free_array(padding_image, W + 2 * padding_size);


	return convolution_matrix;
}

void print_array(int** arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
			printf("%3d", arr[i][j]);

		printf("\n");
	}
}

void free_array(int** arr, int size)
{
	for (int i = 0; i < size; i++)
		free(arr[i]);

	free(arr);
}