#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include <fstream>

int **mat1;
int **mat2;
int **mat3;
int size=0;
int step=0;
#define thread_num 8



//Matrix multiplication using the threads.
void* matrix_mul_with_threads(void *arg)
{
	int core = step++;
	for(int i = core * size/thread_num; i < (core+1) * size/thread_num; i++)
	{
		for(int j=0; j <size; j++)
		{
			// cout<<i<<" "<<j<<endl;
			for (int k=0; k< size; k++)
			{
				
				mat3[i][j]= mat3[i][j]+mat1[i][k]*mat2[k][j];
			}
		}
	}
}





//Calling the main function
int main(int argc, char const *argv[])
{



	//Converting string to integer
	size = atoi(argv[1]);
	cout<<size<<"with threads" <<endl;
	// cout<<size<<endl;

	//Dynamic allocation to create the arrays
	mat1 = new int*[size];
	for(int i=0; i<size; i++)
	{
		mat1[i]= new int[size];
	}
	// cout<<"check"<<endl;
	//Dynamic allocation to create the arrays
	mat2 = new int*[size];
	for(int i=0; i<size; i++)
	{
		mat2[i]= new int[size];
	}
	// cout<<"check"<<endl;

	//filling up the arrays
	for(int i=0; i < size; i++)
	{
		for (int j=0; j <size; j++)
		{
			mat1[i][j] = rand()%10;
			mat2[i][j] = rand()%10;

		}
	}

	mat3 = new int*[size];
	for(int i=0; i<size; i++)
	{
		mat3[i]= new int[size];
	}

	// for(int i=0; i<size; i++)
	// {
	// 	for(int j=0; j<size; j++)
	// 	{
	// 		mat3[i][j]=0;
	// 	}
	// }
	

	pthread_t threads[thread_num];
	
	for(int i =0; i< thread_num; i++)
	{
		int *p;
		pthread_create(&threads[i], NULL, matrix_mul_with_threads, (void *)(p));
	
	}
	
	for(int i=0; i< thread_num; i++)
	{
		pthread_join(threads[i], NULL);
	
	}
	
	delete mat1, mat2, mat3;
}
