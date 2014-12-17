#include <omp.h>
#include <common.h>
#include <iostream>
#include <sstream>      

#define NUM_THREADS 4

using namespace std;

void on_ompt_event_runtime_shutdown(uint64_t command,  uint64_t modifier)
{
    exit(CORRECT);
}

void 
init_test(ompt_function_lookup_t lookup)
{
    global_error_code = NOT_IMPLEMENTED;
    if (!register_callback(ompt_event_runtime_shutdown, (ompt_callback_t) on_ompt_event_runtime_shutdown)) {
        cout << "Failed to register ompt_event_runtime_shutdown" << endl;
    }
}

void work()
{
    #pragma parallel num_threads(NUM_THREADS)
    {
        #pragma parallel num_threads(NUM_THREADS) 
        {
          serialwork(1);
        }
    }
}

int
main(int argc, char** argv)
{
    warmup();
    work();
    usleep(3000000); /* sleep 3s */
    return global_error_code;
}
