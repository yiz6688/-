#include<iostream>
#include<string>
#include<string_view>
#include<sstream>
#include"WaveFormatEncoding.h"
#include"BinaryReader.h"
#include"WaveFileReader.h"
#include<fstream>
#include<functional>
#include"AsioDriver.h"
using namespace std;




typedef  int (*func)(int a, int b);



struct aa
{
	func f;
	int a;
};



class tester2
{

public:
	int dec(int a, int b)
	{
		return 0;
	}


};



class tester
{

public:
	tester(int p)
	{
		a.a = p;
		//function<int(int, int)> addFunc = bind(&tester::add, this, std::placeholders::_1, std::placeholders::_2);

		//a.f = [&addFunc](int a, int b)-> int {return addFunc(a, b); };

		tester2 t2;

	}

public:

	void bb()
	{
		
		int ret = a.f(a.a, 100);
		cout << ret << endl;
	}
private:
	int add(int a, int b)
	{
		return a + b;
	}

private:
	aa a;



};


struct info
{
	int inx;
	char* arr[2];
};



int main()
{
	//WaveFileReader wr("C:\\111\\waveFiles\\48k@16bit_mono_5s.wav");

	//WaveFileReader wr("C:\\111\\waveFiles\\trustwave.wav");

	/*ifstream file("C:\\111\\waveFiles\\48k@16bit_mono_5s.wav");

	cout << file.tellg() << endl;

	char ch;
	file >> ch;


	cout << file.tellg() << endl;*/

	AsioDriver::Init();
	AsioDriver::OpenDriver();

	return 0;


}