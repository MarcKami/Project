#pragma once
//Standard library dependencies.
#include <ostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
//Utilities dependencies.
#include "Assert.hh"
#include "Resources.hh"

/*!
* @struct UserHighscore
* @brief An structure that holds a name and an score.
*/
struct UserHighscore {
	std::string m_name;
	int m_score;
	UserHighscore(std::string name = "...", int score = 0) : m_name(name), m_score(score) { }
	std::string to_str(void) { return m_name + ": " + std::to_string(m_score); }
	std::ostream &operator << (std::ostream &out) { return out << m_name << ": " << m_score; }
	bool operator < (const UserHighscore &rhs) { return m_score < rhs.m_score; }
};

/*!
* @def HM
* @brief Defines a shortcut to the HighscoreManager Singleton instance.
*/
#define HM HighscoreManager::Instance()

class HighscoreManager {
	/*!
	* @brief Explicit default constructor.
	*/
	HighscoreManager(void) = default;
	/*!
	* @brief Explicit default destructor.
	*/
	~HighscoreManager(void) = default;
	/*!
	* @brief Deleted copy constructor.
	*/
	HighscoreManager(const HighscoreManager &rhs) = delete;
	/*!
	* @brief Deleted copy assignment operator.
	*/
	HighscoreManager &operator = (const HighscoreManager &rhs) = delete;

public:
	static constexpr int HIGHSCORES{ 10 }, MAX_USERNAME_LENGTH{ 16 };
	/*!
	* @brief Creates a Singleton instance of the HighscoreManager class.
	* @return A reference to the static instance.
	*/
	inline static HighscoreManager &Instance(void) {
		static HighscoreManager inputManager;
		return inputManager;
	}
	/*!
	* @brief Set the current user score for the Singleton instance to store it between scenes.
	* @param score The score for updating @m_currentUserHighscore.
	*/
	inline void SetUserScore(int score) { m_currentUserHighscore.m_score = score; }
	/*!
	* @brief Set the current user name for the Singleton instance to store it and use it.
	* @param name The name for updating @m_currentUserHighscore.
	*/
	inline void SetUserName(const std::string &name) { m_currentUserHighscore.m_name = name; }
	/*!
	* @brief Pushes the current user highscore triggering internal events such as reading or writing into a binary file and sorting the UserHighscore data.
	* Use it along SetUserScore() and SetUserName() previously.
	*/
	void Push() {
		ReadBinaryFile(); //Read the binary file which updates @m_userHighscoreData, returning with a @HIGHSCORES size.
		m_userHighscoreData.emplace_back(m_currentUserHighscore); //Push @m_currentUserHighscore (oversizes @m_userHighscoreData).
																  //Note: The line of code resumenes: sort(data, data.begin(), data.end()), reverse(data, data.begin(), data.end()); reducing it's complexity but more importantly highly optimizing the result we're aiming. 
		std::sort(m_userHighscoreData.rbegin(), m_userHighscoreData.rend()); //Using reverse_iterators, sort @m_userHighscoreData to store data from bigger to lower score. 
		m_userHighscoreData.pop_back(); //Delete the lower UserHighscore score so that @m_userHighscoreData size remains appropiate.
		WriteBinaryFile(); //Write the updated fixed-size @m_userHighscoreData data back into the binary file.
	}
	/*!
	* @brief Retrieves user highscores data.
	* @return A reference to the private member @m_userHighscoreData previously sorted from biggest to lowest score.
	*/
	inline const std::vector<UserHighscore> &Retrieve(void) {
		ReadBinaryFile();
		return m_userHighscoreData;
	}

private:
	/*!
	* @brief Reads some UserHighscore data from a binary file.
	*/
	void ReadBinaryFile(void) {
		m_userHighscoreData.resize(0);	//First clear @m_userHighscoreData in order to fill it again.
		std::ifstream infile(RESOURCE_FILE(m_filePath), std::ios::in | std::ios::binary); //Open the input file.
		if (infile.is_open()) {
			size_t name_len{ NULL }; //Determines the number of characters that correspond to the the incoming UserHighscore name length to be read.
			while (infile.read(reinterpret_cast<char*>(&name_len), sizeof(size_t))) { //Condition that determine wether we should start/keep reading or not. Also sets the @name_len.
				std::string name; //Represents the name that is going to be read.
				name.resize(name_len); //Allocate enough memory to write into the string.
				infile.read(reinterpret_cast<char*>(&*name.begin()), name_len); //Read the name.
				int score{ NULL }; //Represents the score that is going to be read.
				infile.read(reinterpret_cast<char*>(&score), sizeof(int)); //Read the score.
				m_userHighscoreData.emplace_back(UserHighscore(name, score)); //Emplace read values into @m_userHighscoreData.
			}
			infile.close(); //Close the file after operating on it.
		}
		while (m_userHighscoreData.size() < HIGHSCORES) //Fill the data until a fixed @HIGHSCORES amount before returning.
			m_userHighscoreData.emplace_back(UserHighscore());
	}
	/*!
	* @brief Writes a fixed amount of UserHighscore data into a binary file.
	*/
	void WriteBinaryFile(void) {
		ASSERT(m_userHighscoreData.size() <= HIGHSCORES); //Assert the data to write isn't bigger than intended.
		std::ofstream outfile(RESOURCE_FILE(m_filePath), std::ios::out | std::ios::binary); //Open the output file.
		for (auto &userHighscore : m_userHighscoreData) {
			size_t name_len{ userHighscore.m_name.size() }; //Determines the number of characters that correspond to the the incoming UserHighscore name length to be written.
			outfile.write(reinterpret_cast<char*>(&name_len), sizeof(size_t)); //Write the number of characters of the name.
			outfile.write(userHighscore.m_name.data(), name_len); //Write the name.
			outfile.write(reinterpret_cast<char*>(&userHighscore.m_score), sizeof(int)); //Write the score.
		}
		outfile.close(); //Close the file after operating on it.
	}

private:
	const std::string m_filePath{ "data/highscores.dat" };
	UserHighscore m_currentUserHighscore;					//!< The current user highscore structure holding a name and an score.
	std::vector<UserHighscore> m_userHighscoreData;			//!< Vector used for retrieving, writing and sorting UserHighscores.
};