#include"task.h"
#include<cstdio>
#include<memory>
int add(int a,int b){
    std::printf("add(%d,%d)=%d\n",a,b,a+b);
    return a+b;
}
int main(){
    Task t1=make_task(add,3,4);
    t1();
    Task t2=make_task([](int x){
        std::printf("x=%d\n",x);
    },9);
    t2();
    Task t3=make_task([](std::unique_ptr<int> p){
        std::printf("p=%d\n",*p);
    },std::make_unique<int>(5));
    t3();
    std::puts("OK");
}