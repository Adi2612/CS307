#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include <fstream>


int **mat1;
int **mat2;
int **mat3;
int size=0;

void appendtofile(double a, double b)
{
 ofstream outfile;

  outfile.open("output_without_threads.txt", std::ios_base::app); // append instead of overwrite
  outfile <<size <<' '<< a <<' '<<b<<'\n'; 
}

//Matrix multiplication without using the threads.
void matrix_mul_without_threads(int **A, int **B)
{
	int ** result = mat3;
	for(int i =0; i< size; i++)
	{
		for(int j =0; j<size; j++)
		{
			for(int k=0; k<size; k++)
			{
				result[i][j]+=A[i][k]*B[k][j];
			}
		}
	}
	// print_matrix(result, size);	
}

int main(int argc, char const *argv[])
{


	//Converting string to integer
	size = atoi(argv[1]);
	cout<<size<<"without threads" <<endl;
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
	matrix_mul_without_threads(mat1, mat2);
	delete mat1, mat2, mat3;

}