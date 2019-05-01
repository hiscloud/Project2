// Ref: https://stackoverflow.com/questions/6119664/how-to-use-pthread-mutex-and-its-functions-within-a-class
#include <iostream>
#include <time.h>
#include "windows.h"  // for sleep - windows
//#include <unistd.h> // for sleep - unix/linux/mac
using namespace std;
#include <pthread.h>
#include <stdio.h>
#include <cstdlib> // exit :: may be needed on CodeBlocks;

pthread_mutex_t toon_signal_mutex;
struct MUTEX_SHARED{
int COUNTING;
int AGENT;
};

class CParallel
{      public:
		CParallel(){};
		~CParallel(){};

		int thread_id_obj;
		pthread_t thread_id;
		pthread_mutex_t *toon_signal_mutex = NULL;
		MUTEX_SHARED *SHARED=NULL;
		int stop;

		void init(int value, MUTEX_SHARED *temp_SHARED,
            pthread_mutex_t *temp_toon_signal_mutex){
		    thread_id_obj = value;
		    SHARED = (MUTEX_SHARED*)temp_SHARED;
		    toon_signal_mutex = (pthread_mutex_t*)temp_toon_signal_mutex;
		    stop = 0;
		}

		static void *parallel_API(void *context)
		{	((CParallel *)context)->run_process();
		}

    private:
        void *run_process(void){
            while(!stop){
                toon_signal();
                cout << "thread_id_obj = " << thread_id_obj << endl;
                Sleep(1500);
            }
            pthread_exit(NULL);
			return 0;
		}

        void toon_signal(void)
		{   pthread_mutex_lock(toon_signal_mutex);
                if(SHARED->COUNTING<2 && thread_id_obj == 0){
                    // RUN 1st THREAD
                    SHARED->AGENT = thread_id_obj;
                    cout << "SHARED->COUNTING = " << SHARED->COUNTING
                            << " SHARED->AGENT = " << SHARED->AGENT << endl;
                    SHARED->COUNTING++;
                    if(SHARED->COUNTING==2){
                        stop=1;
                    }
                }else if(SHARED->COUNTING<4 && thread_id_obj == 1){
                    // RUN 2nd THREAD
                    SHARED->AGENT = thread_id_obj;
                    cout << "SHARED->COUNTING = " << SHARED->COUNTING
                            << " SHARED->AGENT = " << SHARED->AGENT << endl;
                    SHARED->COUNTING++;
                    if(SHARED->COUNTING==4){
                        stop=1;
                    }
				}else if(SHARED->COUNTING<6 && thread_id_obj == 2){
					
					 // RUN 3rd THREAD
                    SHARED->AGENT = thread_id_obj;
                    cout << "SHARED->COUNTING = " << SHARED->COUNTING
                            << " SHARED->AGENT = " << SHARED->AGENT << endl;
                    SHARED->COUNTING++;
                    if(SHARED->COUNTING==6){
                        stop=1;
                    }
				}else if(SHARED->COUNTING<8 && thread_id_obj == 3){
					
					 // RUN 4th THREAD
                    SHARED->AGENT = thread_id_obj;
                    cout << "SHARED->COUNTING = " << SHARED->COUNTING
                            << " SHARED->AGENT = " << SHARED->AGENT << endl;
                    SHARED->COUNTING++;
                    if(SHARED->COUNTING==8){
                        stop=1;
                    }
				}else{
					
					 // RUN 5th THREAD
                    SHARED->AGENT = thread_id_obj;
                    cout << "SHARED->COUNTING = " << SHARED->COUNTING
                            << " SHARED->AGENT = " << SHARED->AGENT << endl;
                    SHARED->COUNTING++;
                    if(SHARED->COUNTING==10){
                        stop=1;
                    }
				}
            pthread_mutex_unlock(toon_signal_mutex);
        }
};

int main (int argc, char *argv[])
{   int total_threads = 5;
    CParallel obj[5];
    MUTEX_SHARED SHARED;
    SHARED.COUNTING=0;

    pthread_mutex_init(&toon_signal_mutex, NULL);

    for(int i=0;i<total_threads;i++){
        obj[i].init(i,&SHARED,&toon_signal_mutex);
        pthread_create(&obj[i].thread_id, NULL,
                       &CParallel::parallel_API, &obj[i]);
        //sleep(1);  // for linux/unix/mac
        Sleep(1); // for windows
        pthread_join(obj[i].thread_id, NULL);
    }

    pthread_mutex_destroy(&toon_signal_mutex);
	
    Sleep(800);
    cout << "program finished!"<< endl;

    return 0;
}
