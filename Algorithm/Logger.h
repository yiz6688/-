#pragma once
#include<fstream>
#include<string>
#include<stdexcept>

using std::string;
using std::ofstream;

class Logger {

public:
	Logger(const string filepath, bool overwrite)
	{
		if (overwrite)
		{
			log.open(filepath, std::ios_base::trunc);
		}
		else
		{
			log.open(filepath, std::ios_base::app);
		}
		
		if (log.is_open() == false)
		{
			throw std::current_exception();
		}
	}

	~Logger()
	{
		log.close();
	}
public:
	void Debug(string Msg)
	{
		Msg += "\n";
		log.write(Msg.c_str(),Msg.length());
	}


private:
	ofstream log;


};