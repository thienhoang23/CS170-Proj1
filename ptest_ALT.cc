/**
 *Test program for concurrent hashtable
 **/

#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> 
#include <unistd.h>
//#include <cstdlib>
//#include <string>
//#include <cstdio>
using namespace std;


#ifdef HASHCHAIN
#include "hashchain.h"
#else
#include "rwlock.h"
#include "phash.h"
#endif

#include <sys/time.h>

struct ReportArray
{
	int get_counts;
	int put_counts;
	int remove_counts;
	ReportArray(){
		get_counts = 0;
		put_counts = 0;
		remove_counts = 0;
	}
	int sum() {
		return get_counts + put_counts + remove_counts;
	}
};

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
       return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

ReportArray test1(HashMap *htable, int n, int k, int w){
	int i, errorflag=0;
	ReportArray report;
	for (i=0; i<n; i++){
		(*htable).get(i);
		++report.get_counts;
	}
	for (i=k-w; i<k+w; i++){
		if(i<n && i>=0){ 
			(*htable).put(i,i);
			++report.put_counts;
		}
	}
	if((*htable).get(k)==-1){
		errorflag=1;		
	}
	++report.get_counts;
	(*htable).remove(k);
	++report.remove_counts;
	for (i=0; i<n; i++){
		(*htable).get(i);
		++report.get_counts;
	}
	(*htable).put(k,k);
	++report.put_counts;
	for (i=0; i<n; i++){
		(*htable).get(i);
		++report.get_counts;
	}
	if((*htable).get(k)==-1){
		errorflag=1;
	}
	++report.get_counts;	
	if(errorflag==1)
		cout << "Failed to find  some elements"<<endl;
	return report;
}

HashMap hash;
int NUMKEYS = 1000;
int NumberofThread;
pthread_t* thr;

void *tfunc(void *arg){
	long k= (long) arg; //thread ID
	int b=NUMKEYS/NumberofThread;
	int i;
	ReportArray* report = new ReportArray();
	ReportArray part_report;
	int result[3];
	if( NUMKEYS % NumberofThread!=0)
		b=b+1;
	// cout << "Thread " <<i;
	for (i= b *k; i< b*k+b && i<NUMKEYS; i++){
		 //cout << "Thread " <<k <<" does test1 "<<i<<endl;
		// printf(" Thread %d does test1 %d\n",  k, i);
		part_report = test1(&hash, NUMKEYS, i, 2);
		report->get_counts += part_report.get_counts;
		report->put_counts += part_report.put_counts;
		report->remove_counts += part_report.remove_counts; 
	}
	return (void*) report;
}	
	

void test(){
	long i, errorflag=0;
	ReportArray* report = new ReportArray();
	ReportArray* part_report;
	//  Start Timers
	double wall0 = get_wall_time();
	double cpu0  = get_cpu_time();
    /*
	for (i=0; i<NUMKEYS; i++)
		hash.put(i,i); */

	double spawn_time = 0.0;
	double join_time = 0.0;

	if(NumberofThread>1){
		double spawn_start_time = get_wall_time();
    	for(i=0;i<NumberofThread;i++) 
      		pthread_create(&thr[i], NULL, tfunc, (void *)  i);
      	double spawn_end_time = get_wall_time();
      	spawn_time = spawn_end_time - spawn_start_time;
    	double join_start_time = get_wall_time();
    	for(i=0;i<NumberofThread;i++) {
        	pthread_join(thr[i], (void **) &part_report);
        	report->get_counts += part_report -> get_counts;
        	report->put_counts += part_report -> put_counts;
        	report->remove_counts += part_report -> remove_counts;
        	delete part_report;
        }
        double join_end_time = get_wall_time();
        join_time = join_end_time - join_start_time;
	} else {
		i=0;
		delete report;
		report = (ReportArray*) tfunc ((void *)  i);
    }
	//  Stop timers
	double wall1 = get_wall_time();
	double cpu1  = get_cpu_time();

	cout << "Total Operation Counts: " << report->sum() << endl;
	cout << "Total Get Counts: " << report->get_counts << endl;
	cout << "Total Put Counts: " << report->put_counts << endl;
	cout << "Total Remove Counts: " << report->remove_counts << endl;

	cout << "Average Get Operations per Seconds: " << report->get_counts/(wall1 - wall0) << "/s" << endl;
	cout << "Average Put Operations per Seconds: " << report->put_counts/(wall1 - wall0) << "/s" << endl;
	cout << "Average Remove Operations per Seconds: " << report->remove_counts/(wall1 - wall0) << "/s" << endl;	

	cout << "Wall Time = " << wall1 - wall0 << "s"<<endl;
	cout << "CPU Time  = " << cpu1  - cpu0  << "s"<<endl;
	cout << "Throughput= " << (4*NUMKEYS+8)/(wall1 - wall0)  << "/s"<<endl;
	
	cout << "Average Overhead for Spawning = " << spawn_time/NumberofThread << "s" << endl; 
	cout << "Average Overhead for Joining = " << join_time/NumberofThread << "s" << endl; 

	delete report;
}


int main(int argc, char* argv[])
{
    if(argc != 2){
        cerr << "Argument Count Error!" << endl;
        cout << "USAGE: ./ptest [NUMBER_OF_THREADS]" <<endl;
        return 1;
    }
    NumberofThread = atoi(argv[1]);
    thr = new pthread_t[NumberofThread];
    int key, value;
    int choice;
    bool success;
    test();
    delete[] thr;
 //    while (1)
 //    {
 //        cout<<"1. Insert element into the table"<<endl;
 //        cout<<"2. Search element from the key"<<endl;
 //        cout<<"3. Delete element at a key"<<endl;
 //        cout<<"4. Repeat performance test"<<endl;
 //        cout<<"5. Exit"<<endl;
 //        cout<<"Enter your choice: ";
 //        cin>>choice;
	// if(cin.fail()){
	// 	cin.clear();
 //        	cout<<"Invalid choice "<<endl;
	// 	 cin.ignore(80, '\n');
	// } else {
 //        switch(choice)
 //        {
 //        case 1:
 //            cout<<"Enter element to be inserted: ";
 //            cin>>value;
 //            cout<<"Enter key at which element to be inserted: ";
 //            cin>>key;
 //            hash.put(key, value);
 //            break;
 //        case 2:
 //            cout<<"Enter key of the element to be searched: ";
 //            cin>>key;
 //            cout<<"Element at key "<<key<<" : ";
 //            value= hash.get(key); 
 //            if (value == -1)
 //            {
	//         cout<<"No element found at key "<<key<<endl;
	//     } else
	//         cout<<value<<endl;
 //            break;
 //        case 3:
 //            cout<<"Enter key of the element to be deleted: ";
 //            cin>>key;
 //            success = hash.remove(key);
 //            if(success)
 //                cout << "DELETE SUCCESS" << endl;
 //            else
 //                cout << "DELETE FAIL" << endl;
 //            break;
 //        case 4:
 //    	    test();
 //            break;
 //        case 5:
 //            //exit(1);
 //            return 0;
 //        default:
 //           cout<<"\nEnter correct option\n";
 //       }}
 //    }
    return 0;
}

