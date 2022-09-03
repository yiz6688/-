// LibTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<iostream>
#include<streambuf>
#include<fstream>
#include <iostream>
#include <string>


using namespace std;
int main()
{
    char ch;
    std::streambuf* pbuf;

    ifstream  fs;
    fs.open(R"(C:\\111\test.txt)");

    pbuf = fs.rdbuf();
    istream &stream = fs;

    pbuf = fs.rdbuf();

    cout << pbuf->in_avail() << endl;

    char arr[1024];
    string sss;



    ch = stream.get();

    cout << ch << endl;

    ch = stream.get();

    cout << ch << endl;

    streampos pos = stream.tellg();

    cout << pos << endl;


    streampos curpos = pbuf->pubseekoff(0, ios_base::cur);


    pos = pbuf->pubseekoff(0, ios_base::end);

    cout << pos << endl;



    pos = pbuf->pubseekpos(curpos);

    cout << pos << endl;

    pos = stream.tellg();

    cout << pos << endl;

    ch = stream.get();

    cout << ch << endl;
   

}
