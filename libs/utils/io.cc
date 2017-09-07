#include "io.hh"

#include <fstream>
#include <ios>
#include <iterator>

namespace Utils {

std::string readFile(const std::string& path)
{
	std::ifstream f;

	f.open(path, std::ios_base::in | std::ios_base::binary);
	if (f.fail())
		throw std::runtime_error("Error opening '" + path + "'");

	std::istreambuf_iterator<char> b(f);
	std::istreambuf_iterator<char> e;
	std::string result(b, e);
	return result;
}

}
