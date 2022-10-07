#pragma once
#include<string>
#include<stdexcept>
using namespace std;

class ChunkIdentifier
{

public:
	static int ChunkIdentifierToInt32(string s);



};
/// <summary>
/// ת��Ϊ���Σ�����Ƚ�
/// </summary>
/// <param name="s"></param>
/// <returns></returns>
int ChunkIdentifier::ChunkIdentifierToInt32(string s)
{
	if (s.length() != 4)
	{
		throw std::invalid_argument("Must be a four character string");
	}

	int nret = *reinterpret_cast<const int*>(s.c_str());

	return nret;
}
