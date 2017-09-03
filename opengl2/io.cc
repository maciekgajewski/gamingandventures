#include "io.hh"

#include <fstream>
#include <ios>

namespace OT {

std::string readFile(const std::string& path)
{
	std::ifstream f;

	f.open(path);
	if (f.fail())
		throw std::runtime_error("Error opening '" + path + "'");

	std::string result;

	const int BUFSIZE = 4096;
	char buf[BUFSIZE];

	while(!f.eof())
	{
		auto count = f.readsome(buf, BUFSIZE);
		if (count == 0)
			break;
		result.append(buf, count);
	}

	return result;
}

}
