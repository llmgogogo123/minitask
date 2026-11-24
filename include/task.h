#pragma once
#include<memory>
#include<functional>
#include<type_traits>
#include<utility>
#include<tuple>
class Task{
    private:
    struct Concept{
        virtual ~Concept()=default;
        virtual void call()=0;
    };
    template<typename F>
    struct Model final : Concept{
        template<typename G>
        explicit Model(G&& g) : f(std::forward<G>(g)){}
        void call() override {f();}
        F f;
    };
    public:
    Task() noexcept = default;
    template<typename F,typename = std::enable_if_t<
    !std::is_same_v<std::decay_t<F>,Task>&&
    std::is_invocable_v<std::decay_t<F>&>>>
    Task(F&& f)
    : impl_(std::make_unique<Model<std::decay_t<F>>>(std::forward<F>(f))) {}
    Task(Task&&) noexcept=default;
    Task& operator=(Task&&) noexcept=default;
    ~Task()=default;
    Task(const Task&)=delete;
    Task& operator=(const Task&)=delete;
    void operator()(){
        if(!impl_)
        throw std::bad_function_call();
        impl_->call();
    }
    explicit operator bool() const noexcept{
        return static_cast<bool>(impl_);
    }
    private:
    std::unique_ptr<Concept> impl_;
};
template<typename F,typename...Args>
Task make_task(F&& f,Args&&...args){
    return Task(
        [f=std::forward<F>(f),
        tuple=std::make_tuple(std::forward<Args>(args)...)]() mutable{
            std::apply(std::move(f),std::move(tuple));
        }
    );
}
    
    
        
    
    
