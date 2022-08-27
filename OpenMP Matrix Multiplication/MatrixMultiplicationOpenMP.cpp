// Sequential code for matrix multiplication

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <time.h>
#include <fstream>
#include <omp.h>

using namespace std;
using namespace std::chrono;

#define SIZE 100
#define NUM_THREADS 20

int **m_A, **m_B, **m_C;

void randMatrix(int** matrix)
{
	omp_set_dynamic(0);					// Disable dynamic teams. This forces the OpenMP to use a user-specified number of threads
	omp_set_num_threads(NUM_THREADS);	// Set number of threads

	// Each thread will work on a row of the matrix each
	#pragma omp parallel for num_threads(NUM_THREADS) collapse(2)
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			matrix[i][j] = rand() % 10;
		}
	}
}

void multiplyMatrix(int** a, int** b, int** c)
{
	omp_set_dynamic(0);					// Disable dynamic teams. This forces the OpenMP to use a user-specified number of threads
	omp_set_num_threads(NUM_THREADS);	// Set number of threads

	#pragma omp parallel for num_threads(NUM_THREADS) collapse(2)
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			c[i][j] = 0;

			// Find dot product
			for (int k = 0; k < SIZE; k++)
			{
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

int main()
{
	// Set number of threads
	omp_set_num_threads(NUM_THREADS);


	// Allocate memory
	m_A = new int*[SIZE];
	m_B = new int*[SIZE];
	m_C = new int*[SIZE];

	for (int i = 0; i < SIZE; i++)
	{
		m_A[i] = new int[SIZE];
		m_B[i] = new int[SIZE];
		m_C[i] = new int[SIZE];
	}


	// Record timer
	auto start = high_resolution_clock::now();
	randMatrix(m_A);
	randMatrix(m_B);
	multiplyMatrix(m_A, m_B, m_C);
	auto stop = high_resolution_clock::now();


	// Print results
	ofstream fileWriter;
	fileWriter.open("openmp_program_output.txt");
	
	printf("Matrix A:\n");
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			printf("%d ", m_A[i][j]);
		}
		printf("\n");
	}

	printf("\n");

	printf("Matrix B:\n");
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			printf("%d ", m_B[i][j]);
		}
		printf("\n");
	}

	printf("\n");

	printf("Matrix C:\n");
	fileWriter << "Result:" << endl;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			printf("%d ", m_C[i][j]);
			fileWriter << m_C[i][j] << " ";
		}
		printf("\n");
		fileWriter << endl;
	}

	printf("\n");


	// Performance Summary
	auto duration = duration_cast<microseconds>(stop - start);
	printf("Calculation time: %lld microseconds\n", duration.count());
	fileWriter << "Calculation time: " << duration.count() << " microseconds" << endl;
	printf("Using %d threads\n", NUM_THREADS);
	fileWriter << "Using " << NUM_THREADS << " threads" << endl;
	
	fileWriter.close();

	return 0;
}