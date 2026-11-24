#include "task_queue.h"

#include <atomic>
#include <cassert>
#include <cstdio>
#include <stdexcept>
#include <thread>
#include <vector>
int main(){
    {
    TaskQueue q;
    bool ran1=false;
    bool ran2=false;
    q.push([&]{ran1=true;});
    q.push([&]{ran2=true;});
    Task t;
    assert(q.pop(t));
    t();
    assert(q.pop(t));
    t();
    assert(ran1);
    assert(ran2);
    q.stop();
    assert(!q.pop(t));
    std::puts("single OK");
    }
    {
        TaskQueue q;
        int ran=0;
        for(int i=0;i<3;i++){
            q.push([&]{ran++;});
        }
        q.stop();
        Task t;
        while(q.pop(t)){
            t();
        }
        assert(ran==3);
        bool threw=false;
        try{
            q.push([]{});
        }catch(const std::runtime_error&){
            threw=true;
        }
        assert(threw);
        std::puts("drain OK");
    }
    {
        TaskQueue q;
        std::atomic<int> done{0};
        const int producers=4;
        const int per_producer=1000;
        std::vector<std::thread> ps;
        for(int p=0;p<producers;p++){
            ps.emplace_back([&]{
                for(int i=0;i<per_producer;i++){
                    q.push([&done]{done++;});
                }
            });
        }
        std::vector<std::thread> cs;
        for(int c=0;c<4;c++){
            cs.emplace_back([&]{
                Task t;
                while(q.pop(t)){
                    t();
                }
            });
        }
        for(auto& t : ps) t.join();
        q.stop();
        for(auto& t : cs) t.join();
        assert(done==producers*per_producer);
        std::printf("stress OK: %d\n",done.load());
    }
    

}