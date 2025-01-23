#include "ISerializable.h"

bool ISerializable::WriteTo(std::ofstream& outFile) const
{
	return false;
}

bool ISerializable::ReadFrom(std::ifstream& inFile)
{
	return false;
}