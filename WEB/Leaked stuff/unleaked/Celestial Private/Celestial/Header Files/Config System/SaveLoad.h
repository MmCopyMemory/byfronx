#pragma once
/*
*****************************************************
*												 	*
*	Configparser by Saeurenmensch				 	*
*												 	*
*	pls credit me if you're going to use this ;-)	*
*												 	*
*****************************************************
*/

#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>
#include <fstream>
#include <vector>


enum confParams //modi für configfiles
{
	CONFIG_READ, //nur lesen
	CONFIG_WRITE, //nur schreiben
	CONFIG_READWRITE //beides
};

class cConfig
{
public:
	cConfig(); // leerer konstruktor
	cConfig(std::string configpath, confParams configMode); // konstruktor mit öffnen der streams
	~cConfig(); // destruktor der die streams schliesst


	std::string getPath(); //wrapper für strConfigPath


	template<class write_T> // template für Write um in die config zu schreiben
	inline bool Write(std::string strOptName, write_T tValue); // funktion um in datei zu schreiben

	template<typename read_T> // teamplate für read um aus der config zu lesen
	inline read_T Read(std::string strOptName);

	template<class read_T> // teamplate für read um aus der config zu lesen
	inline read_T class_Read(std::string strOptName);




	void setParam(confParams Params);

	void FORCEINLINE closeOutputStream();
	void FORCEINLINE closeInputStream();

	void FORCEINLINE openOutputStream();
	void FORCEINLINE openInputStream();


private:
	confParams e_confparams;
	std::string	strConfigPath = ""; // Pfad für Config
	std::ofstream outputFileStream; //outputfilestream der überall verwendet werden soll also sollte. zünd mich an wenns nicht so ist.
	std::ifstream inputFileStream; //inputFileStream der überall verwendet werden soll also sollte. zünd mich an wenns nicht so ist.

};


template<class write_T> //template damit man nicht tausende überladungen hat
inline bool cConfig::Write(std::string strOptName, write_T tValue) // schreibt einen wert in eine datei
{
	if (!(this->e_confparams == CONFIG_WRITE) && !(this->e_confparams == CONFIG_READWRITE))
		return false;

	if (!this->outputFileStream.good()) //gibt false zurück wenn etwas mit der datei nicht stimmt.
		return false;

	this->outputFileStream << strOptName << " = \"" << tValue << "\";\n"; // schreibt in die datei

	return true; // gibt true zurück wenn alles glattlief

}


template<typename T>
std::vector<T>
splitstring(const T& str, const T& delimiters) // funktion zum teilen eines strings
{
	std::vector<T> returnVec;

	T::size_type tStart = 0;

	auto aPosition = str.find_first_of(delimiters, tStart);
	while (aPosition != T::npos) {
		if (aPosition != tStart)
			returnVec.emplace_back(str, tStart, aPosition - tStart);

		tStart = aPosition + 1;

		aPosition = str.find_first_of(delimiters, tStart);

	}
	if (tStart < str.length())
		returnVec.emplace_back(str, tStart, str.length() - tStart);


	return returnVec;
}

std::string FORCEINLINE strGetValueFromLine(std::string strLine) //gibt den string der value der line züruck
{
	return splitstring<std::string>(splitstring<std::string>(strLine, " = \"").at(1), "\";").at(0);
}
std::string FORCEINLINE strGetOptionNameFromLine(std::string strLine) //gibt den string des optionsnamen von der line zurück
{
	return splitstring<std::string>(strLine, " = \"").at(0);
}


template<typename read_T>
inline read_T cConfig::Read(std::string strOptName)
{

	if (!(this->e_confparams == CONFIG_READ) && !(this->e_confparams == CONFIG_READWRITE)) // nur lesen 
		return -1;

	this->closeInputStream(); //filestream closen damit wir wieder am anfang sind.
	this->openInputStream(); //filestream dann wieder äffnen um lesen zu können


	if (!this->inputFileStream.good()) // ist die datei vorhanden?
		return -1;


	char szLine[256]; // cstring ._.

	while (this->inputFileStream.getline(szLine, 256)) //solange die datei valid ist wird gelesen
	{
		std::string strLine = szLine; // die aktuelle linie
		std::string strValue = strGetValueFromLine(strLine);
		std::string strOptionName = strGetOptionNameFromLine(strLine);
		if (strOptName != strOptionName)
			continue;

		return atof(strValue.c_str());


	}

	return read_T();
}

template<class read_T>
inline read_T cConfig::class_Read(std::string strOptName)
{
	if (!(this->e_confparams == CONFIG_READ) && !(this->e_confparams == CONFIG_READWRITE)) // nur lesen 
		return -1;

	this->closeInputStream(); //filestream closen damit wir wieder am anfang sind.
	this->openInputStream(); //filestream dann wieder äffnen um lesen zu können


	if (!this->inputFileStream.good()) // ist die datei vorhanden?
		return -1;


	char szLine[256]; // cstring ._.

	while (this->inputFileStream.getline(szLine, 256)) //solange die datei valid ist wird gelesen
	{
		std::string strLine = szLine; // die aktuelle linie
		std::string strValue = strGetValueFromLine(strLine);
		std::string strOptionName = strGetOptionNameFromLine(strLine);
		if (strOptName != strOptionName)
			continue;

		return strValue;


	}

	return read_T();
}


/*
*****************************************************
*												 	*
*	Configparser by Saeurenmensch				 	*
*												 	*
*	pls credit me if you're going to use this ;-)	*
*												 	*
*****************************************************
*/


cConfig::cConfig()
{
	this->strConfigPath = ""; // Configpath leermachen da nichts übergeben wurde
}



cConfig::cConfig(std::string configpath, confParams configMode)
{
	//modus setzen
	this->e_confparams = configMode;

	this->strConfigPath = configpath; // strConfigPath auf den übergebenen path setzen

	// filestreams öffnen

	switch (this->e_confparams) // switch für die parameter
	{
	case CONFIG_READ: // nur lesen
		this->openInputStream();
		break;
	case CONFIG_WRITE: // nur schrieben
		this->openOutputStream();
		break;
	case CONFIG_READWRITE: //lesen und schreiben
		this->openInputStream();
		this->openOutputStream();
		break;
	default: // default wenn davon nix ist.
		break;
	}




}

cConfig::~cConfig() // schliest die streams
{
	//filestreams schließen
	this->closeInputStream();
	this->closeOutputStream();
}

std::string cConfig::getPath() // gibt den path der config zurück
{
	return this->strConfigPath; // returnt den strConfigPath
}

void cConfig::setParam(confParams Params)
{
	this->e_confparams = Params;
}

void FORCEINLINE cConfig::closeOutputStream()
{
	if (this->e_confparams == CONFIG_WRITE || this->e_confparams == CONFIG_READWRITE)
	{
		this->outputFileStream.close();
	}
}


void FORCEINLINE cConfig::closeInputStream()
{
	if (this->e_confparams == CONFIG_READ || this->e_confparams == CONFIG_READWRITE)
	{
		this->inputFileStream.close();
	}
}

void FORCEINLINE cConfig::openOutputStream()
{
	if (this->e_confparams == CONFIG_WRITE || this->e_confparams == CONFIG_READWRITE)
	{
		this->outputFileStream = std::ofstream(this->strConfigPath, std::ios::out); // outputstream öffnen
	}
}

void FORCEINLINE cConfig::openInputStream()
{
	if (this->e_confparams == CONFIG_READ || this->e_confparams == CONFIG_READWRITE)
	{
		this->inputFileStream = std::ifstream(this->strConfigPath, std::ios::in); // inputstream öffnen
	}
}
