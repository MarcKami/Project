#pragma once
#include <iostream>

// Enum ID used to identify each Game Object in the Game
enum class ObjectID : int {
	BULLET, PLAYER, ROCK_L, ROCK_M1, ROCK_M2, ROCK_S1, ROCK_S2, ROCK_XS1, ROCK_XS2, UFO_L, UFO_S, // Candies IDs
	CELL_EMPTY, CANDY_EMPTY, CANDY_MAX, // Cell IDs
	BG_00, BG_01  // Background IDs
};
inline std::ostream &operator<<(std::ostream &os, const ObjectID &id) { return os << int(id); };
inline std::istream &operator>>(std::istream &is, ObjectID &id) { return is >> reinterpret_cast<int&>(id); };

// Enum ID used to store each true type font for text rendering
enum class FontID : int { 
	ARIAL,
	CANDY,
	FACTORY, 
	TITTLE,
	MAX 
};