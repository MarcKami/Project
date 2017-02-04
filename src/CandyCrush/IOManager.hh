#pragma once
#include "ID.hh"
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include "Logger.hh"
#include "Assert.hh"
#include "Resource.hh"
#include <XML/rapidxml_utils.hpp>
#include <map>
using namespace Logger;

namespace IOManager {

	std::vector<LevelData> LoadXML(std::string &&filename);


	void WriteBinary(std::string &&filename, std::multimap<int, char*> rank);


	std::multimap<int, char*> ReadBinary(std::string &&filename);

}