#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    
    //initialize status
    
    thread_func_args->thread_complete_success=false;
    
    //preparing
    int rc = usleep(thread_func_args->starting_mutex);
    if(rc!=0){
      ERROR_LOG("failed on waiting");
      return thread_param;
      
    }
    
    //locking
    rc=pthread_mutex_lock(thread_func_args->mutex);
    if(rc!=0){
      ERROR_LOG("failed on locking mutex");
      return thread_param;
    }
    
    //waiting for release
    rc=usleep(thread_func_args->releasing_mutex);
    if(rc!=0){
      ERROR_LOG("failed on waiting");
      return thread_param;
    }
    
    //unlock
    rc=pthread_mutex_unlock(thread_func_args->mutex);
    if(rc!=0){
      ERROR_LOG("failed to unlock");
      return thread_param;
    }
      
    thread_func_args->thread_complete_success=true;
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
     
     //allocating memory
     struct thread_data* data=(struct thread_data*)malloc(sizeof(struct thread_data));
     if(data==NULL){
       ERROR_LOG("fail to allocate memory");
       return false;
     }
     else{
       DEBUG_LOG("sucess to allocate memory");
     }
    
    //mutex
    data->mutex=mutex;
    data->starting_mutex=wait_to_obtain_ms;
    data->releasing_mutex=wait_to_release_ms;
    
    
    int rc= pthread_create(thread,NULL,threadfunc,(void*)data);
    if(rc!=0){
      ERROR_LOG("fail to create thread");
      return false;
    }
    return true;
}

