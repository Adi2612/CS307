#include <bits/stdc++.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <chrono>
#include <fstream>

using namespace std;

#define N 5 // 5 philosophers
#define FORK_PICKED_UP "Fork picked up"
#define EATING "Eating"
#define THINKING "Thinking"
#define RUNTIME 1800 //runtime = 30 minutes = 1800 seconds


sem_t forks[N]; //semaphore array used for forks
sem_t state_lock; //semaphore used to lock philosopher_state before changing it
pthread_t thread_id[N]; //used in pthread_create and p_thread_join
vector<string> phil_state; //array to store current state of philosophers
int philosopher_id[N] = {0,1,2,3,4}; // assigning an integer ID to each philosopher
vector<int> eating_count(N, 0); //counts number of times each philosopher eats
string output_file;


void init_semaphores_and_state() {
  for(int i = 0 ; i < N; i++) {
    sem_init(&forks[i], 0, 1); // initialize all semaphores to 1, arg 2 is 0 because these are shared over threads, would be 1 if shared over processes
    phil_state.push_back("Philosopher " + to_string(i) + " state : " + THINKING);
  }
    
  sem_init(&state_lock, 0, 1);
}

void change_state(int id, string state) {
  sem_wait(&state_lock);
  phil_state.push_back("Philosopher " + to_string(id) + " state : " + state);
  sem_post(&state_lock); 
}

void dine(int id) {
  sem_wait(id != N-1 ? &forks[id] : &forks[(id+1)%N]);
  change_state(id, FORK_PICKED_UP);
  sem_wait(id == N-1 ? &forks[id] : &forks[(id+1)%N]);
  change_state(id, EATING);
  eating_count[id]++;
  sleep(1); // eat for 1 seconds;
  sem_wait(&state_lock);
  sem_post(&forks[id]);
  sem_post(&forks[(id + 1)%N]);
  phil_state.push_back("Philosopher " + to_string(id) + " state : " + THINKING);
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
    pthread_create(&thread_id[i], NULL, subroutine , &philosopher_id[i]); //creates thread and runs subroutine
  
  for(int i = 0 ; i < N; i++)
    pthread_join(thread_id[i], NULL); //joins threads with supplied ID
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
