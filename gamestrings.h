#ifndef _GAMESTRINGS_H
#define _GAMESTRINGS_H

#include <map>
#include <string>

class GameStrings {
	public:
		GameStrings();
		~GameStrings();

		static std::map<std::string, std::string> lang;
};



#endif