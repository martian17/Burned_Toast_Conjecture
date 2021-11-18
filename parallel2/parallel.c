#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>


bool isIrrational(uint16_t* arr, uint16_t len){
    for(uint16_t i = 0; i < len; i++){
        uint16_t v = arr[i];
        if(v&65536 || v != i){//first bit is one => flipped
            return true;
        }
    }
    return false;
}


uint64_t toast(uint16_t x){
    /*uint16_t* arr = malloc(x*sizeof(uint16_t));
    for(uint16_t i = 0; i < x; i++){
        arr[i] = i;
        //printf("%d\n",i);
    }
    uint16_t* buff = malloc(x*sizeof(uint16_t));
    uint64_t cnt = 0;
    do{
        cnt += (uint64_t)x;
        uint16_t i = 0;
        for(i; i < (x+1)/2; i++){
            buff[i] = ~arr[x-(i*2)-1];
        }
        for(i; i < x; i++){
            buff[i] = arr[-x+(i*2)];
        }
        //swap the buffer
        uint16_t* temp = arr;
        arr = buff;
        buff = temp;
    }while(isIrrational(arr,x));
    free(arr);
    free(buff);
    return cnt;*/
    return (uint64_t)x;
}



typedef struct{
    volatile pthread_t thread;
    volatile uint16_t x;
    volatile uint64_t result;
    volatile bool kill;
    volatile bool active;
}thread_interface;

void* thread_func(void* arg){
    thread_interface* interface = (thread_interface*)arg;
    while(1){
        //fprintf(stderr,"kill signal: %s\n",interface->kill?"true":"false");
        if(interface->kill){
            fprintf(stderr,"kill signal recieved\n");
            break;
        }else if(interface->active){
            uint16_t x = interface->x;
            interface->result = toast(x);
            interface->active = false;
        }
    }
    fprintf(stderr,"terminating a thread\n");
}

int main(int argv, char** argc){
    int threads = 8;
    thread_interface* interfaces = malloc(threads*sizeof(thread_interface));
    for(int i = 0; i < threads; i++){
        pthread_t thread;
        interfaces[i].x = 0;
        interfaces[i].result = 0;
        interfaces[i].kill = false;
        interfaces[i].active = false;
        int state = pthread_create( &thread, NULL, thread_func, (void*) &interfaces[i]);
        fprintf(stderr,"Thread %d created with the state: %d\n",i,state);
        //fprintf(stderr,"Thread %zu\n",thread);
        interfaces[i].thread = thread;
    }
    
    //fprintf(stderr,"num: %u\n",interfaces[1].num);
    uint16_t i0 = 2;
    uint16_t i1 = 2;//frontier
    uint16_t max = 100;//32760;
    uint64_t* result = malloc(max*sizeof(uint16_t));
    for(int i = 0; i < max; i++){
        result[i] = 0;//initializing
    }
    char* fname = argc[1];
    fprintf(stderr,"%s\n",fname);
    FILE* destination = fopen(fname, "w+");
    result[0] = 0;
    result[1] = 2;
    fprintf(destination,"0 0\n");
    fprintf(destination,"1 2\n");
    fflush(destination);
    bool continueFlag = true;
    while(continueFlag){
        for(int i = 0; i < threads; i++){
            if(!interfaces[i].active){// store the result
                if(interfaces[i].result){
                    result[interfaces[i].x] = interfaces[i].result;
                    interfaces[i].result = 0;
                    //print the result
                    while(result[i0]){
                        fprintf(destination,"%d %zu\n",i0,result[i0]);
                        fflush(destination);
                        fprintf(stderr,"%d %zu\n",i0,result[i0]);
                        i0++;
                    }
                    if(i0 == max){
                        fprintf(stderr,"===========breaking!========\n");
                        continueFlag = false;
                    }
                }
                if(!interfaces[i].active && i1 < max){// if inactive assign a new job
                    interfaces[i].x = i1;
                    i1++;
                    interfaces[i].active = true;
                }
            }
        }
    }
    fprintf(stderr,"loop exited!!!!!!!!!!!!!!!!!!!!!!\n");
    
    
    for(int i = 0; i < threads; i++){
        fprintf(stderr,"Killing thread %d\n",i);
        interfaces[i].kill = true;
        pthread_join(interfaces[i].thread, NULL);
    }
    fprintf(stderr,"Threads joined\n");
    free(interfaces);
    free(result);
    fclose(destination);
}