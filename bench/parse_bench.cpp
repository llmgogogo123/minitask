#include"parse.h"
#include<chrono>
#include<cstdio>
#include<string>
#include<string_view>
#include<cctype>
struct KeyValueString{
    std::string key;
    std::string value;
};
std::string trim_string(std::string s){
    std::size_t b=0;
    std::size_t e=s.size();
    while(b<e&&std::isspace(static_cast<unsigned char>(s[b]))){
        b++;
    }
    while(e>b&&std::isspace(static_cast<unsigned char>(s[e-1]))){
        e--;
    }
    s.erase(0,b);
    s.erase(e-b);
    return s;
}
KeyValueString parse_key_value_string(const std::string line){
    auto eq=line.find('=');
    if(eq==std::string::npos){
        return{trim_string(line),{}};
    }
    return{
        trim_string(line.substr(0,eq)),trim_string(line.substr(eq+1))
    };
}
int main(){
    const int N=100000;
    std::string data;
    data.reserve(N*32);
    for(int i=0;i<N;i++){
        data+="key";
        data+=std::to_string(i);
        data+=" = value";
        data+=std::to_string(i);
        data+="  \n";
    }
    auto t0=std::chrono::steady_clock::now();
    std::size_t total_key =0;
    std::size_t total_value =0;
    std::size_t lines =0;
    std::string_view view(data);
    std::size_t pos=0;
    while(pos<view.size()){
        auto end=view.find('\n',pos);
        if(end==std::string_view::npos){
            end=view.size();
        }
        auto line=view.substr(pos,end-pos);
        auto[key,value]=parse_key_value(line);
        total_key+=key.size();
        total_value+=value.size();
        ++lines;
        pos=end+1;
    }
    auto t1=std::chrono::steady_clock::now();
    double ms=std::chrono::duration<double,std::milli>(t1-t0).count();

    auto t2=std::chrono::steady_clock::now();
    std::size_t total_key_str = 0;
    std::size_t total_value_str = 0;
    std::size_t lines_str = 0;
    pos = 0;
    while(pos<view.size()){
        auto end=view.find('\n',pos);
        if(end==std::string_view::npos){
            end=view.size();
        }
        auto line_view=view.substr(pos,end-pos);
        std::string line(line_view.data(),line_view.size());
        auto [key,value]=parse_key_value_string(line);
        
        total_key_str += key.size();
        total_value_str += value.size();
        ++lines_str;

        pos = end + 1;
    }
        auto t3 = std::chrono::steady_clock::now();
    double ms_str = std::chrono::duration<double, std::milli>(t3 - t2).count();

    std::printf("lines=%zu keys=%zu value=%zu\n",lines,total_key,total_value);
    std::printf("string_view parse: %.3f ms\n",ms);
       std::printf("std::string: lines=%zu keys=%zu value=%zu\n",
                lines_str, total_key_str, total_value_str);
    std::printf("std::string parse: %.3f ms\n", ms_str);

}