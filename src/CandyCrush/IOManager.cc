#include "IOManager.hh"



std::vector<LevelData> IOManager::LoadXML(std::string &&filename) {
	std::vector<LevelData> loadLvl(3);
	rapidxml::file<> xmlFile(RESOURCE_FILE(filename));
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	rapidxml::xml_node<> *root_node = doc.first_node("EASY");
	//En lugar de printar atributos, los guardamos para asociarlos a levelData
	for (rapidxml::xml_node<> * node = root_node->first_node("stats"); node; node = node->next_sibling()) {
		loadLvl[0].life = atoi(node->first_attribute("vida")->value());
		loadLvl[0].cEn = atoi(node->first_attribute("cEnemigo")->value());
		loadLvl[0].vEn = atoi(node->first_attribute("vEnemigo")->value());
		loadLvl[0].iEn = atoi(node->first_attribute("iEnemigo")->value());
	}
	root_node = doc.first_node("MEDIUM");
	for (rapidxml::xml_node<> * node = root_node->first_node("stats"); node; node = node->next_sibling()) {
		loadLvl[1].life = atoi(node->first_attribute("vida")->value());
		loadLvl[1].cEn = atoi(node->first_attribute("cEnemigo")->value());
		loadLvl[1].vEn = atoi(node->first_attribute("vEnemigo")->value());
		loadLvl[1].iEn = atoi(node->first_attribute("iEnemigo")->value());
	}
	root_node = doc.first_node("HARD");
	for (rapidxml::xml_node<> * node = root_node->first_node("stats"); node; node = node->next_sibling()) {
		loadLvl[2].life = atoi(node->first_attribute("vida")->value());
		loadLvl[2].cEn = atoi(node->first_attribute("cEnemigo")->value());
		loadLvl[2].vEn = atoi(node->first_attribute("vEnemigo")->value());
		loadLvl[2].iEn = atoi(node->first_attribute("iEnemigo")->value());
	}
	return loadLvl;
}



void IOManager::WriteBinary(std::string &&filename, std::multimap<int, char*> rank) {
	std::multimap<int, char*> playerData = rank;
	std::string name;
	int score;
	// WRITE //
	std::ofstream savefile(RESOURCE_FILE(filename), std::ios::binary);
	if (savefile.good()) {
		for (auto it = playerData.begin(); it != playerData.end(); ++it) {
			name = it->second;
			savefile.write(name.c_str(), name.size()); // Write string to binary file
			savefile.write("\0", sizeof(char)); // Add null end string for easier reading
			score = it->first;
			savefile.write(reinterpret_cast<char*>(&score), sizeof(score)); // Write int to binary file
		}
		savefile.close();
	}
}



std::multimap<int, char*> IOManager::ReadBinary(std::string &&filename) {
	std::multimap<int, char*> playerData;
	std::string name;
	char* r_name;
	int score;
	// READ //
	std::ifstream loadfile(RESOURCE_FILE(filename), std::ios::binary);
	if (loadfile.good()) {
		for (int i = 0; i < 10; i++) {
			std::getline(loadfile, name, '\0'); // Get player name (only if null ternimated in binary)
			r_name = _strdup(name.c_str());
			loadfile.read(reinterpret_cast<char*>(&score), sizeof(score)); // Read int bytes
			playerData.insert(std::pair<int, char*>(score, r_name));
			Logger::Println(name, " || ", -score);
		}
		loadfile.close();
	}
	return playerData;
}