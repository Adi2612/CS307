#include<bits/stdc++.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <chrono>
#include <fstream>

using namespace std;

#define N 5
#define FORK_PICKED_UP "Fork Picked Up"
#define EATING "Eating"
#define THINKING "THINKING"
#define RUNTIME 60 //runtime = 30 minutes = 1800 seconds


sem_t forks[N];
sem_t state_lock;
pthread_t thread_id[N];
vector<string> phil_state;
int philosopher_id[N] = {0,1,2,3,4}; 
vector<int> eating_count(N, 0);
string output_file;


void init_semaphores_and_state() {
  for(int i = 0 ; i < N; i++) {
    sem_init(&forks[i], 0, 1);
    phil_state.push_back("Philosopher " + to_string(i) + " state : " + THINKING);
  }
    
  sem_init(&state_lock, 0, 1);
}

void change_state(int id, string state) {
  sem_wait(&state_lock);
  phil_state.push_back("Philosopher" + to_string(id) + "state : " + state);
  sem_post(&state_lock); 
}

void dine(int id) {
  cout<<id<<endl;
  sem_wait(id != N-1 ? &forks[id] : &forks[(id+1)%N]);
  change_state(id, FORK_PICKED_UP);
  sem_wait(id == N-1 ? &forks[id] : &forks[(id+1)%N]);
  change_state(id, EATING);
  eating_count[id]++;
  sleep(1); // eat for 1 seconds;
  sem_wait(&state_lock);
  sem_post(&forks[id]);
  sem_post(&forks[(id + 1)%N]);
  phil_state.push_back("Philosopher" + to_string(id) + "state : " + THINKING);
  sem_post(&state_lock);
  sleep(rand()%5 + 1);
}


void* subroutine(void* id) {
  auto start_time = chrono::steady_clock::now();
  while(chrono::steady_clock::now() - start_time < chrono::seconds(RUNTIME)) {
    dine(*(int*)id);
  }
}

void start_threads() {
  for(int i = 0 ; i < N ; i++)
    pthread_create(&thread_id[i], NULL, subroutine , &philosopher_id[i]);
  
  for(int i = 0 ; i < N; i++)
    pthread_join(thread_id[i], NULL);
}

void print_state_and_count() {
  ofstream out;
  out.open(output_file, ios::app);
  for(auto it : phil_state)
    out<<it<<endl;
  for(int i = 0; i < N ; i++)
    out<<"Philosopher "<<i<<" count : "<<eating_count[i]<<endl;
  out.close();
}

int main(int argc, char const *argv[]) {
  if(argc == 2)
    output_file = argv[1];
  else {
    cout<<"Please provide output file to write to"<<endl;
    return 0;
  }

  init_semaphores_and_state();
  start_threads();
  print_state_and_count();

  return 0;
}
