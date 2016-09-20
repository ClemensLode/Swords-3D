#include "gamestrings.h"

GameStrings::GameStrings() {
	lang["Test"] = "Test";
}

GameStrings::~GameStrings() {
}

std::map<std::string, std::string> GameStrings::lang;