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

	// Base test function for XML purposes (TODO)
	std::map<std::string, int> LoadXML(std::string &&filename, int dif_) {
		int dif = dif_;
		std::map<std::string, int> loadLvl;
		rapidxml::file<> xmlFile(RESOURCE_FILE(filename));
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		rapidxml::xml_node<> *root_node = doc.first_node("MEDIUM");
		switch (dif)
		{
			case 0:
				root_node = doc.first_node("EASY");
				break;
			case 1:
				root_node = doc.first_node("MEDIUM");
				break;
			case 2:
				root_node = doc.first_node("HARD");
				break;
			default:
				break;	
		}	

		//En lugar de printar atributos, los guardamos para asociarlos a levelData
		for (rapidxml::xml_node<> * node = root_node->first_node("node"); node; node = node->next_sibling()) {
			loadLvl["vida"] = int(node->first_attribute("vida")->value());
			loadLvl["cEnemigo"] = int(node->first_attribute("cEnemigo")->value());
			loadLvl["vEnemigo"] = int(node->first_attribute("vEnemigo")->value());
			loadLvl["iEnemigo"] = int(node->first_attribute("iEnemigo")->value());
		}
		return loadLvl;
	}

	// Loader function that takes level info for a grid
	std::vector<std::vector<ObjectID>> LoadLevel(std::string &&filename, int &rows, int &cols) {
		std::ifstream fileData(RESOURCE_FILE(filename));
		ASSERT(fileData.good());
		std::vector<std::vector<ObjectID>> lvlData;
		std::string line;
		while (std::getline(fileData, line)) {
			std::istringstream strData(std::move(line));
			lvlData.emplace_back(std::istream_iterator<ObjectID>(std::move(strData)), std::istream_iterator<ObjectID>());
		}
		rows = int(lvlData.size()); cols = int(lvlData[0].size());
		fileData.close();
		#pragma region DEBUG_DATA
			Println("Level: ", filename);
			for (auto &r : lvlData) {
				for (auto &c : r) Print(c, ' ');
				Endln();
			}
		#pragma endregion
		return std::move(lvlData);
	}
	struct PlayerData {
		std::string name;
		int score;
	};



	int LoadBinary() {
		// READ //
		std::ofstream savefile("score.bin", std::ios::binary);
		if (savefile.good()) {
			PlayerData playerData = { "Tona", 246 };
			savefile.write(playerData.name.c_str(), playerData.name.size()); // Write string to binary file
			savefile.write("\0", sizeof(char)); // Add null end string for easier reading
			savefile.write(reinterpret_cast<char*>(&playerData.score), sizeof(playerData.score)); // Write int to binary file
			savefile.close();
		}
		// WRITE //
		std::ifstream loadfile("score.bin", std::ios::binary);
		if (loadfile.good()) {
			PlayerData playerData;
			std::getline(loadfile, playerData.name, '\0'); // Get player name (only if null ternimated in binary)
			loadfile.read(reinterpret_cast<char*>(&playerData.score), sizeof(playerData.score)); // Read int bytes
			std::cout << "Player: { " << playerData.name << ", " << playerData.score << " }" << std::endl;
			loadfile.close();
		}
		return 0;
	}

}