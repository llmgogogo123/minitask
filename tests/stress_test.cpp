#include"task_queue.h"
#include<atomic>
#include<cassert>
#include<chrono>
#include<cstdio>
#include<thread>
#include<vector>
int main(){
    TaskQueue q;
    constexpr int kProducers=4;
    constexpr int kConsumers=4;
    constexpr int kPerProducer=25000;
    constexpr int KTotal=kProducers*kPerProducer;
    std::atomic<int> executed{0};
    std::atomic<int> consumed{0};
    auto start=std::chrono::steady_clock::now();
    std::vector<std::thread> producers;
    for(int p=0;p<kProducers;p++){
        producers.emplace_back([&]{
            for(int i=0;i<kPerProducer;i++){
                q.push([&executed]{
                    executed.fetch_add(1,std::memory_order_relaxed);
                });
            }
        });
    }
    std::vector<std::thread> consumers;
    for(int c=0;c<kConsumers;c++){
        consumers.emplace_back([&]{
            Task t;
            while(q.pop(t)){
                t();
                consumed.fetch_add(1,std::memory_order_relaxed);
            }
        });
    }
    for(auto& t : producers) t.join();
        q.stop();
    for(auto& t : consumers) t.join();
    auto end=std::chrono::steady_clock::now();
    double ms=std::chrono::duration<double,std::milli>(end-start).count();
    std::printf("executed=%d consumed=%d size=%zu time=%.1f ms\n",executed.load(),consumed.load(),q.size(),ms );
    assert(executed.load()==KTotal);
    assert(consumed.load()==KTotal);
    assert(q.size()==0);
    std::puts("stress OK : 100000");
}