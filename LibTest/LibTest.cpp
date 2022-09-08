// LibTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<iostream>
#include<streambuf>
#include<fstream>
#include <iostream>
#include <string>


using namespace std;

union aaa
{
    double data;
    int data1[2];
    char data2[8];
};




void decode1(string s1,string s2)
{
    char key[64];
    for (int i = 0; i < 10; i++)
    {
        key[i] = '0' + i;
    }
    for (int i = 0; i < 26; i++)
    {
        key[i + 10] = 'A' + i;
        key[i + 36] = 'a' + i;
    }
    key[62] = '?';
    key[63] = '!';


    auto fx = [&key](char ch)->int{
        for (int i = 0; i < 64; i++)
        {
            if (ch == key[i])
            {
                return i;
            }
        }

        return 0;
    };


    aaa a;
    int temp;
    if (s1 == "")
    {
        a.data1[0] = 0;
    }
    else
    {
        temp = 0;
        for (char ch : s1)
        {
            temp = temp * 16 + fx(ch);
        }
        a.data1[0] = temp;
    }

    if (s2 == "")
    {
        a.data1[1] = 0;
    }
    else
    {
        temp = 0;
        for (char ch : s1)
        {
            temp = temp * 16 + fx(ch);
        }
        a.data1[1] = temp;
    }

    for (int i = 0; i < 8; i++)
    {
        cout << a.data2[i];
    }


}





int main()
{


    decode1("XHpQV1", "izsTm1");



    return 0;


    ifstream  ffs;
  //  ffs.open(R"(C:\\111\test.txt)");
   


    istream& iis = ffs;


    cout << typeid(iis).name() << endl;

    if (iis.eof())
    {
        cout << "aa" << endl;
    }

    char buffer[512];

    cout  << endl;

    if (iis.read(buffer, 512))
    {
        cout << "读取成功" << endl;
    }


    return 0;
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
