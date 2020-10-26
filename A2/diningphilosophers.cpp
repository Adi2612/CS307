#include <iostream>
#include <string>
#include <unistd.h>
#include <semaphore.h>
#include <chrono>
#include <fstream>
#include <stdlib.h>

using namespace std;

#define N 5 // 5 philosophers
#define RUNTIME 1800 //runtime = 30 minutes = 1800 seconds

void* thread_Subroutine(void* p_ID);
void change_State(int i, string state);

pthread_t thread_ID[N]; //used in pthread_create and p_thread_join
int philosopher_ID[N] = {0,1,2,3,4}; // assigning an integer ID to each philosopher

string philosopher_state[N] = {"Thinking", "Thinking", "Thinking", "Thinking", "Thinking"}; //array to store current state of philosophers
sem_t state_lock; //semaphore used to lock philosopher_state before changing it
sem_t forks[N]; //semaphore array used for forks
string output_file;

int eat_count[N] = {0,0,0,0,0}; //counts number of times each philosopher eats

void init_Semaphores() // initialize all semaphores to 1
{
    for (int i = 0; i < N; i++)
    {
        sem_init(&forks[i], 0, 1); // arg 2 is 0 because these are shared over threads, would be 1 if shared over processes
    }
    
    sem_init(&state_lock, 0, 1);
}

void create_Threads()
{
    for (int i = 0; i < N; i++)
    {
        pthread_create(&thread_ID[i], NULL, thread_Subroutine, &philosopher_ID[i]); //creates thread and runs subroutine
    }
    
}

void join_Threads()
{
    for (int i = 0; i < N; i++)
    {
        pthread_join(thread_ID[i], NULL); //joins threads with supplied ID
    }
    
}

void print_State() //call function from inside thread only if state is locked, prints state of all philosophers to output file
{
    ofstream out;
    out.open(output_file, ios::app);

    for (int i = 0; i < N; i++)
    {
        out<<"Philosopher "<<i<<" state : "<<philosopher_state[i]<<endl;
    }
    
    out.close();
}

void print_Count()
{
    ofstream out;
    out.open(output_file, ios::app);

    for (int i = 0; i < N; i++)
    {
        out<<"Philosopher "<<i<<" count : "<<eat_count[i]<<endl;
    }
    
    out.close();
}

void change_State(int i, string state)
{
    sem_wait(&state_lock); 
    philosopher_state[i] = state;
    print_State();
    sem_post(&state_lock);
}

void dine(int p_ID)
{
    if (p_ID != N-1)
    {
        sem_wait(&forks[p_ID]);

        change_State(p_ID, "Fork picked up");

        sem_wait(&forks[(p_ID + 1)%N]);
    }
    else if (p_ID == N-1)
    {
        sem_wait(&forks[(p_ID + 1)%N]);

        change_State(p_ID, "Fork picked up");

        sem_wait(&forks[p_ID]);
    }
    
    change_State(p_ID, "Eating");
    sleep(1); //eat for 1 seconds
    eat_count[p_ID]++; //increment count
    sem_post(&forks[p_ID]);
    sem_post(&forks[(p_ID + 1)%N]);
}

void* thread_Subroutine(void* p_ID) //supplied to pthread_create
{
    auto start_time = chrono::steady_clock::now();

    while (chrono::steady_clock::now() - start_time < chrono::seconds(RUNTIME)) //run for RUNTIME
    {
        dine(*(int *)p_ID); //eat
        change_State(*(int *)p_ID, "Thinking"); //think
        sleep(rand()%5 + 1); //think for random time
    }
}

int main(int argc, char const *argv[]) 
{
    if (argc == 2)
    {
        output_file = argv[1];
    }
    else 
    {
        cout<<"Please provide output file to write to"<<endl;
        return 0;
    }

    print_State();
    
    init_Semaphores();
    create_Threads();
    join_Threads();

    print_Count();

    return 0;
}