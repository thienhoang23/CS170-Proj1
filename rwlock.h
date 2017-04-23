//#include<stdio.h>
//#include<pthread.h>
#include <semaphore.h>

#ifndef _RWLOCK_H_
#define _RWLOCK_H_

class RWLock{
private:
#ifdef RWLOCK
	pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
#else 
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
#endif
 
public:
    	~RWLock();
    //Reader
    	void startRead();
    	void doneRead();
    // Writer
    	void startWrite();
    	void doneWrite();
};

#endif