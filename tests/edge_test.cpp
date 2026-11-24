#include "task.h"
#include "task_queue.h"

#include <cassert>
#include <cstdio>
#include <functional>
#include <stdexcept>
#include <utility>
int main(){
    Task empty;
    bool bad_call=false;
    try{
        empty();
    }catch(const std::bad_function_call&){
        bad_call=true;
    }
    assert(bad_call);
    int x=0;
    Task t=[&x]{++x;};
    Task t2=std::move(t);
    assert(!t);
    assert(t2);
    t2();
    assert(x==1);
    TaskQueue q;
    q.stop();
    Task out;
    bool push_threw=false;
    try{
        q.push([]{});
    }catch(const std::runtime_error&){
        push_threw=true;
    }
    assert(push_threw);
    q.stop();
    TaskQueue q2;
    q2.push([]{});
    q2.stop();
    Task out2;
    assert(q2.pop(out2));
    assert(q.size()==0);
    std::puts("edge OK");
}