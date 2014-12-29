//*****************************************************************************
// system include files 
//*****************************************************************************

#include <stdio.h>
#include <signal.h>

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <omp.h>



//*****************************************************************************
// local include files 
//*****************************************************************************

#include "common.h"



//*****************************************************************************
// global variables
//*****************************************************************************

#define DEFINE_OMPT_FN_PTR( lookup, fn ) \
    fn = ( fn ## _t )lookup( #fn ); \
    if ( !fn ) \
    { \
        fprintf( stderr, "Could not lookup function %s!\n", #fn); \
	fflush(stderr); \
        assert(0); \
    }



//*****************************************************************************
// global variables
//*****************************************************************************
    
#define macro( fn ) fn ## _t fn;
FOREACH_OMPT_FN( macro )
#undef macro



//*****************************************************************************
// interface functions 
//*****************************************************************************

void
warmup() 
{
    int a = omp_get_num_threads();
    #pragma omp atomic
    a += 1;
}


void
serialwork(int workload)
{
    int i = 0;
    for (i = 0; i < workload; i++) {
        usleep(500000);
    }
    for (i = 0; i < 10000; i++) {
        void * p = (void *) malloc(10);
        free(p);
    }
}



int
register_callback(ompt_event_t e, ompt_callback_t c) {
    int code = ompt_set_callback(e, c);
    if (code != ompt_set_result_event_may_occur_callback_always) {
        return FALSE;
    }
    return TRUE;
}


extern "C" {

int
ompt_initialize( ompt_function_lookup_t lookup,
                 const char*            runtime_version,
                 unsigned int           ompt_version )
{
    // lookup functions
    #define macro( fn ) DEFINE_OMPT_FN_PTR( lookup, fn )
    FOREACH_OMPT_FN( macro )
    #undef macro
    pthread_mutex_init(&thread_mutex, NULL);
    pthread_mutex_init(&assert_mutex, NULL);
    init_test(lookup);
    return 1;
}

};
