#ifndef _CHARACTER_CREATION_H
#define _CHARACTER_CREATION_H

#include "character.h"

#define STARTING_CHARACTERISTICS_POINTS 7
#define STARTING_FLAW_POINTS 10
#define STARTING_VIRTUE_POINTS 10
#define MAX_CHARACTERISTIC_VALUE 3
#define EXTREME_CHARACTERISTIC_VALUE 6

class CharacterCreation {
	public:
		CharacterCreation();
		~CharacterCreation();
// character statistics
		bool increase(const eCharacteristic characteristic);
		bool decrease(const eCharacteristic characteristic);
		const int getValue(const eCharacteristic characteristic) const;

		const bool chooseVirtueFlaw(const eVirtueFlaw virtue_flaw);

		const int* getCurrentStats() const;
		const bool* getCurrentIncreasePossibilities() const;
		const bool* getCurrentDecreasePossibilities() const;

		const bool allowIncrease(const eCharacteristic characteristic) const;
		const bool allowDecrease(const eCharacteristic characteristic) const;

		const int increaseCosts(const eCharacteristic characteristic) const;
		const int decreaseCosts(const eCharacteristic characteristic) const;

		const std::string getIncreaseCostsString(const eCharacteristic characteristic) const;
		const std::string getDecreaseCostsString(const eCharacteristic characteristic) const;

		const bool characteristicsDone() const;
		const bool virtuesflawsDone() const;

		const int getRemainingCharacteristicPoints() const;
		const unsigned int getRemainingVirtuePoints() const;
		const unsigned int getRemainingFlawPoints() const;
// character miscellaneous
		void setName(std::string name);
		const std::string getName() const;
// factory
		Character* createCharacter() const;
	private:
		int* characteristic;
		bool* virtuesflaws;
		int remainingCharacteristicPoints;

		int remainingVirtuePoints;
		int remainingFlawPoints;

		std::string name;
};

#endif