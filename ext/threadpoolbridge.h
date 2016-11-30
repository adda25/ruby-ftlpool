#ifndef _THREAD_POOL_C_BRIDGE_H_
#define _THREAD_POOL_C_BRIDGE_H_
#ifdef __cplusplus

#include "ThreadPool/threadpool.hpp"

#endif // __cplusplus
/**
*     ____   ___       _             __                
*    / ___| |_ _|_ __ | |_ ___ _ __ / _| __ _  ___ ___ 
*   | |      | || '_ \| __/ _ \ '__| |_ / _` |/ __/ _ \
*   | |___   | || | | | ||  __/ |  |  _| (_| | (_|  __/
*    \____| |___|_| |_|\__\___|_|  |_|  \__,_|\___\___|
*                                                      
*/

/**
*   HOW TO CALL THE FOLLOWING METHODS
*   c_thread_pool_ptr ctp = c_thread_pool_init(4);
*   int pool_size = c_thread_pool_size(ctp);
*   c_thread_pool_deinit(ctp);
*/

typedef void *c_thread_pool_ptr;
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>

#define THREAD_POOL_CLASS(o) reinterpret_cast<astp::ThreadPool *>(o)

/** 
*   Init
*/
c_thread_pool_ptr 
c_thread_pool_init(int size) {
    return reinterpret_cast<void *>(new astp::ThreadPool(size));
}

/** 
*   Deinit
*/
void 
c_thread_pool_deinit(c_thread_pool_ptr rl) {
    delete THREAD_POOL_CLASS(rl);
};

/** 
*   Test method, return the size of the 
*   pool.
*/
int 
c_thread_pool_size(c_thread_pool_ptr rl) {
    return THREAD_POOL_CLASS(rl)->pool_size();
};


#ifdef __cplusplus
} // extern "C" {
#endif



#endif // _THREAD_POOL_C_BRIDGE_H_
