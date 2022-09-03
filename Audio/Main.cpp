#include<iostream>
#include<string>
#include<string_view>
#include<sstream>
#include"WaveFormatEncoding.h"
#include"BinaryReader.h"

using namespace std;

int main()
{
	
	//cout << sizeof(WaveFormatEncoding::Aptx) << endl;
	stringbuf sb;
	char arr[]{ 0x01,0x02,0x03,0x04,0x05,0x01 };
	sb.str(arr);
	BinaryReader br(&sb);
	int ret = br.ReadInt16();
	cout << ret << endl;
	ret = br.ReadInt32();
	cout << ret << endl;

	filebuf fb;
	
	
	

	streambuf* sbb = &sb;
	
	


	cout << typeid(fb).name() << endl;
	cout << typeid(filebuf).name() << endl;
	cout << (typeid(*sbb) == typeid(stringbuf)) << endl;
	cout << typeid(stringbuf).name() << endl;
}