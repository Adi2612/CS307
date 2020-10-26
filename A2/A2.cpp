#include <iostream>
#include <bits/stdc++.h>
#include <fstream>

using namespace std;

int size = 0;
int **m1;
int **m2;
int **m3;

int **alloc_memory() {
  int **temp;
  temp = new int*[size];
  for(int i = 0 ; i < size; i++)
    temp[i] = new int[size];
  return temp; 
}

void randomize() {
  for(int i = 0 ; i < size ; i++) {
    for(int j = 0 ; j < size ; j++) {
      m1[i][j] = rand()%100;
      m2[i][j] = rand()%100;
    }
  }
}

void multiply_matrix() {
  for(int i = 0; i < size ; i++)
    for(int j = 0 ; j < size ; j++)
      for(int k = 0 ; k < size ; k++)
        m3[i][j] += m1[i][k]*m2[k][j];
}


int main(int argc, char const *argv[]) {
  
  if(argc == 2) {
    size = atoi(argv[1]);
  } else {
    cout<<"Enter Size in CLI"<<endl;
    return 0;
  }
  m1 = alloc_memory();
  m2 = alloc_memory();
  m3 = alloc_memory();
  randomize();
  multiply_matrix();
  delete m1, m2, m3;
  return 0;
}
