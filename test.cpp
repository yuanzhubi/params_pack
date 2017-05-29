
#include <string>
#include <iostream>
#include <initializer_list>

#include "params_pack.h"

using namespace std;

template <typename ...T>
struct test_struct{

    static void output(const T&...args){
        std::initializer_list<int>{(cout << args, 0)...};
        cout << endl;
    }
};


template <typename T>
struct test_struct_1{

    static void output(const T& args){
        cout << args;
        cout << endl;
    }
};

int main() {
	cout << params_pack<int, string>::size << endl;

    params_pack<int, string>::at<1>::type a1 = "hello ";
    params_pack<int, string>::at<0>::type a0 = 0;

    params_pack<int, string>::first_type b0 = 1;
    params_pack<int, string>::last_type  b1 = "world!";
    params_pack<int, string>::at<1>::type  b2 = b1;

    (params_pack<>::first_type)a1;
    (params_pack<>::last_type)a1;
    (params_pack<>::at<0>::type)a1;
    (params_pack<>::at<1>::type)a1;
    (params_pack<char>::at<1>::type)a1;

    cout << a0 <<  a1 << b0 << b1 << b2 << endl;

    params_pack<string, int, string, int>::pop_front<test_struct>::type::output( 2, "Hello!", 3);
    params_pack<string, int, string, int>::pop_back<test_struct>::type::output("Hi", 2, "Hello!");

    params_pack<string, int, string, int>::range<test_struct, 0, 1>::type::output("Hello!");
    params_pack<string, int, string, int>::range<test_struct, 1, 3>::type::output(2, "Hello!");
    params_pack<string, int, string, int>::range<test_struct, 1, 4>::type::output(2, "Hello!", 3);
    params_pack<string, char, string, char>::range<test_struct>::type::output( "Hello ", ' ', "world", '!');

   params_pack<string, char, string, char>::erase<test_struct, 3>::type::output( "Hello ", ' ', "world");

    params_pack<string, char, string, char, int>::swap<test_struct, 1, 3>::type::output( "Hello ", ' ', "world", '!', 0);
    params_pack<string, char, string, char, int>::swap<test_struct, 1, 2>::type::output( "Hello ", "world", ' ', '!', 0);
    params_pack<string, char, string, char, int>::reverse<test_struct>::type::output( 0, ' ',"Hello ", ' ',"world!");

    params_pack<string, char, string>::
        erase<eraser, 1>::type::
        erase<test_struct_1, 1>::type::output("Hello world!");

    params_pack<string, char, string>::
        erase<test_struct_1, 1, 3>::type::output("Hello world!");
	return 0;
}
