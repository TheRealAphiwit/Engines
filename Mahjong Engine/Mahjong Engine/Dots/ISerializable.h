#pragma once
#include <fstream>

class ISerializable
{
public:
	virtual ~ISerializable() = default;

	virtual bool WriteTo(std::ofstream& outFile) const = 0;
	virtual bool ReadFrom(std::ifstream& inFile) = 0;
};