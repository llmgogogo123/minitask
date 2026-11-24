#pragma once

#include "task.h"

#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>
#include <stdexcept>
#include <utility>
class TaskQueue{
    private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::deque<Task> tasks_;
    bool stopped_=false;
    public:
    TaskQueue()=default;
    ~TaskQueue()=default;
    TaskQueue(const TaskQueue&)=delete;
    TaskQueue& operator=(const TaskQueue&)=delete;
    void push(Task task){
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if(stopped_){
                throw std::runtime_error("TaskQueue is stopped");
            }
            tasks_.push_back(std::move(task));
        }
        cv_.notify_one();
    }
    bool pop(Task& out){
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock,[this]{
            return stopped_||!tasks_.empty();
        });
        if(tasks_.empty()){
            return false;
        }
        out=std::move(tasks_.front());
        tasks_.pop_front();
        return true;
    }
    void stop(){
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stopped_=true;
        }
        cv_.notify_all();
    }
    std::size_t size() const{
        std::lock_guard<std::mutex> lock(mutex_);
        return tasks_.size();
    }
};