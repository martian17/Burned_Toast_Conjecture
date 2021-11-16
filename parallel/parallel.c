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
    uint16_t* arr = malloc(x*sizeof(uint16_t));
    for(uint16_t i = 0; i < x; i++){
        arr[i] = i;
    }
    uint64_t cnt = 0;
    uint16_t n = 1;
    do{
        cnt++;
        for(uint16_t i = 0; i < n>>1; i++){
            uint16_t temp = arr[i];
            arr[i] = ~arr[n-i-1];
            arr[n-i-1] = ~temp;
        }
        if(n&1){//if odd, do the middle
            int idx = n>>1;//divide by 2 faster!
            arr[idx] = ~arr[idx];
        }
        //n = (n+1)%x;
        //faster than modulo
        n++;
        if(n == x)n = 0;
    }while(isIrrational(arr,x));
    free(arr);
    return cnt;
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
        //printf("kill signal: %s\n",interface->kill?"true":"false");
        if(interface->kill){
            printf("kill signal recieved\n");
            break;
        }else if(interface->active){
            int x = interface->x;
            interface->result = toast(x);
            interface->active = false;
        }
    }
    printf("terminating a thread\n");
}

int main(){
    int threads = 8;
    thread_interface* interfaces = malloc(threads*sizeof(thread_interface));
    for(int i = 0; i < threads; i++){
        pthread_t thread;
        interfaces[i].x = 0;
        interfaces[i].result = 0;
        interfaces[i].kill = false;
        interfaces[i].active = false;
        int state = pthread_create( &thread, NULL, thread_func, (void*) &interfaces[i]);
        printf("Thread %d created with the state: %d\n",i,state);
        //printf("Thread %zu\n",thread);
        interfaces[i].thread = thread;
    }
    
    //printf("num: %u\n",interfaces[1].num);
    uint16_t i0 = 2;
    uint16_t i1 = 2;//frontier
    uint16_t max = 1000;
    uint64_t* result = malloc(max*sizeof(uint16_t));
    result[0] = 0;
    result[1] = 2;
    printf("0 0\n");
    printf("1 2\n");
    bool continueFlag = true;
    while(continueFlag){
        for(int i = 0; i < threads; i++){
            if(!interfaces[i].active){// store the result
                if(interfaces[i].result){
                    result[interfaces[i].x] = interfaces[i].result;
                    interfaces[i].result = 0;
                    //print the result
                    while(result[i0]){
                        printf("%d %zu\n",i0,result[i0]);
                        i0++;
                    }
                    if(i0 == max){
                        printf("===========breaking!========\n");
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
    printf("loop exited!!!!!!!!!!!!!!!!!!!!!!\n");
    
    
    for(int i = 0; i < threads; i++){
        printf("Killing thread %d\n",i);
        interfaces[i].kill = true;
        pthread_join(interfaces[i].thread, NULL);
    }
    printf("Threads joined\n");
    free(interfaces);
    free(result);
}