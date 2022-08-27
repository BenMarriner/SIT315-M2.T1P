// Parallel code for matrix multiplication
//
// Roadmap to parallising the code:
// 1. Define number of threads (maximum threads = number of cores x 2)
// 2. Define thread arrays
// 3. Define partition size for each thread
// 4. randMatrix task
// 5. multiplyMatrix task

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <time.h>
#include <fstream>
#include <thread>

using namespace std;
using namespace std::chrono;

#define SIZE 10
#define NUM_THREADS 12
#define RAND_SEED 0

int** m_A, ** m_B, ** m_C;
std::thread randThreads[NUM_THREADS];
std::thread multiplyThreads[NUM_THREADS];
int partition_size = SIZE / NUM_THREADS;

struct RandTask
{
	int* m;
	int seed;
};

struct MultiplyTask
{
	int** a, ** b, ** c;
	int row;
};

void randMatrix(void* args)
{
	RandTask* task = (RandTask*)args;

	srand(task->seed);

	for (int i = 0; i < SIZE; i++)
	{
		task->m[i] = rand() % 10;
	}
}

void multiplyMatrix(void* args)
{
	MultiplyTask* task = (MultiplyTask*)args;

	// Find dot product
	for (int i = 0; i < SIZE; i++)
	{
		task->c[task->row][i] = 0;
		for (int j = 0; j < SIZE; j++)
		{
			task->c[task->row][i] += task->a[task->row][j] * task->b[j][i];
		}
	}
}

int main()
{
	m_A = (int**)malloc(sizeof(int*) * SIZE);
	m_B = (int**)malloc(sizeof(int*) * SIZE);
	m_C = (int**)malloc(sizeof(int*) * SIZE);

	for (int i = 0; i < SIZE; i++)
	{
		m_A[i] = (int*)malloc(sizeof(int) * SIZE);
		m_B[i] = (int*)malloc(sizeof(int) * SIZE);
		m_C[i] = (int*)malloc(sizeof(int) * SIZE);
	}

	auto start = high_resolution_clock::now();
	
	// Generate matrix A
	for (int i = 0; i < SIZE; i++)
	{
		RandTask* rand_task = (RandTask*)malloc(sizeof(RandTask));
		rand_task->m = m_A[i];
		rand_task->seed = i;

		randThreads[i] = thread(randMatrix, rand_task);
	}
	
	for (int i = 0; i < SIZE; i++)
	{
		randThreads[i].join();
	}

	// Generate matrix B
	for (int i = 0; i < SIZE; i++)
	{
		RandTask* rand_task = (RandTask*)malloc(sizeof(RandTask));
		rand_task->m = m_B[i];
		rand_task->seed = i * 2;

		randThreads[i] = thread(randMatrix, rand_task);
	}

	for (int i = 0; i < SIZE; i++)
	{
		randThreads[i].join();
	}

	for (int i = 0; i < SIZE; i++)
	{
		MultiplyTask* multiply_task = (MultiplyTask*)malloc(sizeof(MultiplyTask));
		multiply_task->a = m_A;
		multiply_task->b = m_B;
		multiply_task->c = m_C;
		multiply_task->row = i;

		multiplyThreads[i] = std::thread(multiplyMatrix, multiply_task);
	}

	for (int i = 0; i < SIZE; i++)
	{
		multiplyThreads[i].join();
	}

	auto stop = high_resolution_clock::now();

	ofstream fileWriter;
	fileWriter.open("parallel_program_output.txt");

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
	printf("Using %d threads", NUM_THREADS);
	fileWriter << "Using " << NUM_THREADS << "threads" << endl;

	fileWriter.close();

	return 0;
}