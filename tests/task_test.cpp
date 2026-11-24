#include "task.h"

#include <cassert>
#include <cstdio>
#include <functional>
#include <memory>
int main(){
    Task t=[]{
        std::puts("hello task");
    };
    assert(t);
    t();
    int x=7;
    Task t2=[x]{
        std::printf("x=%d\n",x);
    };
    t2();
    Task empty;
    assert(!empty);
    bool caught=false;
    try{
        empty();
    }catch(const std::bad_function_call&){
        caught=true;
    }
    assert(caught);
    Task t3=std::move(t2);
    t3();
    assert(t3);
    assert(!t2);
    auto p=std::make_unique<int>(42);
    Task t4=[p=std::move(p)]{
        std::printf("p=%d\n",*p);
    };
    t4();
    std::puts("task OK");
}