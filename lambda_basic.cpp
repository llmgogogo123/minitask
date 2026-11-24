#include<cstdio>
int main(){
    auto hello=[] {
        std::printf("hello lambda\n");
    };
    hello();
    auto add=[](int a,int b){
        return a+b;
    };
    std::printf("add(2,3)=%d\n",add(2,3));
    int base=10;
    auto add_base=[base](int x){
        return x+base;
    };
    std::printf("add_base(5)=%d\n",add_base(5));
}