#include<cctype>
#include<iostream>
#include<string>
#include<string_view>
struct KeyValue{
    std::string_view key;
    std::string_view value;
};
std::string_view trim(std::string_view s){
    while(!s.empty()&&std::isspace(static_cast<unsigned char>(s.front()))){
        s.remove_prefix(1);
    }
    while(!s.empty()&&std::isspace(static_cast<unsigned char>(s.back()))){
        s.remove_suffix(1);
    }
    return s;
}
KeyValue parse_key_value(std::string_view line){
    auto eq=line.find('=');
    if(eq==std::string_view::npos){
        return {trim(line),{}};
    }
    return {
        trim(line.substr(0,eq)),trim(line.substr(eq+1))
    };
}
void check(std::string_view input,std::string_view expected_key,std::string_view expeced_value){
    auto [key,value]=parse_key_value(input);
    if(key!=expected_key||value!=expeced_value){
        std::cerr<<"FAIL input=["<<input<<"]"
        <<"key=["<<key<<"]"
        <<"value=["<<value<<"]\n";
        std::abort();
    }
    std::cout<<"OK key=["<<key<<"] value=["<<value<<"]\n";
}
int main(){
    std::string line="  name = alice";
    auto [key,value]=parse_key_value(line);
    std::cout<<"key=["<<key<<"] value=["<<value<<"]\n";
    auto [k2,v2]=parse_key_value("a=b=c");
    std::cout<<"key=["<<k2<<"] value=["<<v2<<"]\n";
    auto [k3,v3]=parse_key_value("no_equal");
    std::cout<<"key=["<<k3<<"] value="<<v3<<"]\n";
    check("  name = alice  ", "name", "alice");
    check("a=b=c", "a", "b=c");
    check("no_equal", "no_equal", "");
    check("=alice", "", "alice");
    check("name=", "name", "");
    check("   ", "", "");
    check("\tname\t=\talice\t", "name", "alice");
}