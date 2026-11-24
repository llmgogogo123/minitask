#pragma once
#include<cctype>
#include<iostream>
#include<string>
#include<string_view>
struct KeyValue{
    std::string_view key;
    std::string_view value;
};
inline std::string_view trim(std::string_view s){
    while(!s.empty()&&std::isspace(static_cast<unsigned char>(s.front()))){
        s.remove_prefix(1);
    }
    while(!s.empty()&&std::isspace(static_cast<unsigned char>(s.back()))){
        s.remove_suffix(1);
    }
    return s;
}
inline KeyValue parse_key_value(std::string_view line){
    auto eq=line.find('=');
    if(eq==std::string_view::npos){
        return {trim(line),{}};
    }
    return {
        trim(line.substr(0,eq)),trim(line.substr(eq+1))
    };
}
