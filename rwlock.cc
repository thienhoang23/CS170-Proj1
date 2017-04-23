#include <semaphore.h>
#include <pthread.h>
#include "rwlock.h"

RWLock::~RWLock(){
	#ifdef RWLOCK
		pthread_rwlock_destroy(&rwlock);
	#else
		pthread_mutex_destroy(&lock);
	#endif
}

void RWLock::startRead(){
	#ifdef RWLOCK
		pthread_rwlock_rdlock(&rwlock);
	#else
		pthread_mutex_lock(&lock);
	#endif
}

void RWLock::doneRead(){
	#ifdef RWLOCK
		pthread_rwlock_unlock(&rwlock);
	#else
		pthread_mutex_unlock(&lock);
	#endif
}

void RWLock::startWrite(){
	#ifdef RWLOCK
		pthread_rwlock_wrlock(&rwlock);
	#else
		pthread_mutex_lock(&lock);
	#endif
}

void RWLock::doneWrite(){
	#ifdef RWLOCK
		pthread_rwlock_unlock(&rwlock);
	#else
		pthread_mutex_unlock(&lock);
	#endif
}