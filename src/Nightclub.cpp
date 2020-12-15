#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <atomic>
#include "Nightclub.h"
#include "Semaphore.h"
using namespace std;

//TODO use a semaphore to ensure only MAX_PEOPLE threads
//can be in function inside(int id) concurrently
//all other threads must wait until there
//are fewer than MAX_PEOPLE threads in inside(int id)
//You MUST NOT use condition variables
//You MUST use semaphores

//TODO please also make any non-threadsafe APIs threadsafe

const int NUMB_THREADS =20;
const int MAX_PEOPLE =2;
atomic<int> inClub(0);
std::vector<thread> thds;

mutex printMut;
Semaphore sem(MAX_PEOPLE);

void log(string s){
	lock_guard<mutex> printGuard(printMut);
	cout<<s<<endl;
}

void inside(int id){
	//bask in noisy ambiance
	log(string("Thread "+to_string(id)+" is inside" + (inClub>MAX_PEOPLE?"\nMAX OCCUPANCY EXCEEDED. " + to_string(inClub) + " threads inside": "")));
//	log(string(inClub + " threads inside"));
	std::this_thread::sleep_for (std::chrono::seconds(1));
}

void nc(int id){
	log(string("Thread "+to_string(id)+" waiting to get in"));
	sem.wait();
	inClub++;
	inside(id);
	log(string("Thread "+to_string(id)+" has left"));
	inClub--;
	sem.signal();
}

//PLEASE DO NOT CHANGE THIS FUNCTION
void Nightclub(){
	for (int j=0;j<NUMB_THREADS;j++){
			thds.push_back( thread(nc,j));
		}

	for (auto& thd:thds)
		thd.join();
}


