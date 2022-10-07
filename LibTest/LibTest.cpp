// LibTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<iostream>
#include<streambuf>
#include<fstream>
#include <iostream>
#include <string>
#include<iomanip>
using namespace std;


#include "fftw3.h"


#pragma comment(lib,"fftw3.lib")

int fftwtest()

{
    

    fftw_complex* in, * out;

    fftw_plan p;

    int N = 8;

    in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);

    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);

    for (int i = 0; i < N; i++)

    {

        in[i][0] = 1.0;

        in[i][1] = 0.0;

        printf("%6.2f ", in[i][0]);

    }

    printf("\n");  // in 是输入的数据

    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(p); /* repeat as needed*/

    for (int j = 0; j < N; j++)

    {

        printf("%6.2f ", out[j][0]);

    }

    printf("\n");

    fftw_destroy_plan(p);

    fftw_free(in);

    fftw_free(out);

    return 0;

}




int main()
{

    fftwtest();

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
