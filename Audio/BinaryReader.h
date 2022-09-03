#pragma once
#include<iostream>
#include<istream>
#include<ostream>
#include<string>
#include<fstream>
using namespace std;

class BinaryReader 
{
public:
	BinaryReader(std::streambuf *buf)
	{
		this->buf = buf;
	}
	virtual ~BinaryReader() { }

	 virtual int Read() { return 0; }
	 virtual int Read(char* buffer, int index, int count) 
	 {
		 
		 streamsize size = buf->sgetn(buffer, count);
		 return 0; 
	 }

	 virtual short ReadInt16()
	 {
		 char buffer[2];
		 streamsize size = buf->sgetn(buffer, 2);
		 return *(short*)buffer;
	 }

	 virtual int ReadInt32()
	 {
		 char buffer[4];
		 streamsize size = buf->sgetn(buffer, 4);
		 return *(int*)buffer;
	 }





private:

	std::streambuf *buf;





};

