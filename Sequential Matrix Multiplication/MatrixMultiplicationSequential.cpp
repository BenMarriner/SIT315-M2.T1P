// Sequential code for matrix multiplication

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <time.h>
#include <fstream>

using namespace std;
using namespace std::chrono;

#define SIZE 10

int **m_A, **m_B, **m_C;

void randMatrix(int** matrix)
{
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
	m_A = new int*[SIZE];
	m_B = new int*[SIZE];
	m_C = new int*[SIZE];

	for (int i = 0; i < SIZE; i++)
	{
		m_A[i] = new int[SIZE];
		m_B[i] = new int[SIZE];
		m_C[i] = new int[SIZE];
	}

	auto start = high_resolution_clock::now();
	randMatrix(m_A);
	randMatrix(m_B);
	multiplyMatrix(m_A, m_B, m_C);
	auto stop = high_resolution_clock::now();

	ofstream fileWriter;
	fileWriter.open("sequential_program_output.txt");
	
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


	auto duration = duration_cast<microseconds>(stop - start);
	printf("Calculation time: %lld microseconds", duration.count());
	fileWriter << "Calculation time: " << duration.count() << " microseconds" << endl;
	printf("Using 1 thread");
	fileWriter << "Using 1 thread" << endl;

	fileWriter.close();

	return 0;
}