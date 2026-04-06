
#include "./fixedpoint.h"

int fixed_convert(int n ){
    return n * (1 << 14);
}

int fixed_to_int(int x){
    return x / (1 << 14);
}

int fixed_to_int_near(int x){
    if (x >= 0){
        return (x + (1<<14 / 2)) / (1<<14);
    } else {
        return (x - (1<<14 / 2)) / (1<<14);
    }
}

int fixed_add(int x,int y){
    return x+y;
}

int fixed_subtract(int x, int y){
    return x-y;
}

//result is in fixed format
int fixed_fix_plus_int(int x,int n){
    return x + n * (1<<14);
}

//result is in fixed format
int fixed_fix_minus_int(int x,int n){
    return x - n * (1<<14);
}

int fixed_multiply(int x,int y){
    return ((int64_t)x) * y / (1<<14);
}

int fixed_int_multiply(int x,int n){
    return x*n;
}

//DIVIDE FIXED BY FIXED
int fixed_divide(int x,int y){
    return ((int64_t)x) * (1<<14) / y;
}
//DIVIDE FIXED BY INTEGER
int fixed_int_divide(int x,int n){
    return x/n;
}

