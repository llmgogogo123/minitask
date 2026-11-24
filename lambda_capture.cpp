#include<cstdio>
int main(){
    int base=10;
    auto by_value=[=] mutable{
        base+=1;
        return base;
    };
    auto by_ref=[&](){
        base+=1;
        return base;
    };
    
    std::printf("by_value() = %d\n", by_value());  // 11
    std::printf("by_value() = %d\n", by_value());  // 12
    std::printf("outer base = %d\n", base);         // 10

    std::printf("by_ref() = %d\n", by_ref());       // 11
    std::printf("outer base = %d\n", base);         // 11
}