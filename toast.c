#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


bool isIrrational(uint16_t* arr, uint16_t len){
    for(uint16_t i = 0; i < len; i++){
        uint16_t v = arr[i];
        if(v&65536 || v != i){//first bit is one => flipped
            return true;
        }
    }
    return false;
}

/*
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
        if(n == x+1)n = 1;
    }while(isIrrational(arr,x));
    free(arr);
    return cnt;
}
*/

uint64_t toast(uint16_t x){
    uint16_t* arr = malloc(x*sizeof(uint16_t));
    for(uint16_t i = 0; i < x; i++){
        arr[i] = i;
        //printf("%d\n",i);
    }
    uint16_t* buff = malloc(x*sizeof(uint16_t));
    uint64_t cnt = 0;
    do{
        cnt += x;
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
    return cnt;
}

int main(){
    for(uint16_t i = 2; i < 10000; i++){
        uint64_t result = toast(i);
        //printf("%u\t%zu\t%zu\t%zu\n",i,result,result/i,result%i);
        printf("%u %zu\n",i,result);
    }
}
