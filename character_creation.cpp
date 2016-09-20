#include "character_creation.h"
#include <sstream>

CharacterCreation::CharacterCreation():
	remainingCharacteristicPoints(STARTING_CHARACTERISTICS_POINTS),
	remainingFlawPoints(STARTING_FLAW_POINTS),
	remainingVirtuePoints(STARTING_VIRTUE_POINTS) {
		characteristic = new int[MAX_CHARACTERISTICS];
		for(int i = 0; i < MAX_CHARACTERISTICS; i++)
			characteristic[i] = 0;
		virtuesflaws = new bool[MAX_VIRTUESFLAWS];
		for(int i = 0; i < MAX_VIRTUESFLAWS; i++)
			virtuesflaws[i] = false;
		virtuesflaws[WANDERER] = true;
}

CharacterCreation::~CharacterCreation() {
	delete characteristic;
}

const bool CharacterCreation::characteristicsDone() const {
	return remainingCharacteristicPoints == 0;
}

const bool CharacterCreation::virtuesflawsDone() const {
	return remainingFlawPoints == remainingVirtuePoints;
}

const bool CharacterCreation::chooseVirtueFlaw(const eVirtueFlaw virtue_flaw) {
// already chosen
	if(virtuesflaws[virtue_flaw])
		return false;

	int value = Character::virtueData[virtue_flaw].value;
	if(value > 0) {
		if(remainingVirtuePoints < value)
			return false;
		remainingVirtuePoints -= value;
	} else if(value < 0) {
		if(remainingFlawPoints < (-value))
			return false;
		remainingFlawPoints -= (-value);
	}
	virtuesflaws[virtue_flaw] = true;
	return true;
// TODO Zusammenhaenge zwischen den Virtues, z.B. sozialer Status
}

void CharacterCreation::setName(std::string name) {
	this->name = name;
}

const int CharacterCreation::getValue(const eCharacteristic characteristic) const {
	return this->characteristic[characteristic];
}

const std::string CharacterCreation::getName() const {
	return name;
}

Character* CharacterCreation::createCharacter() const {
	return new Character(name, characteristic, virtuesflaws);
}

bool CharacterCreation::decrease(eCharacteristic characteristic) {
	// todo zusaetzliche Faktoren aus Rasse, Geschlecht, Herkunft, Spezialeigenschaften etc.
	int decrease_cost = decreaseCosts(characteristic);
	if(this->characteristic[characteristic] > -3)
	{
		this->characteristic[characteristic]--;
	} else return false;
	remainingCharacteristicPoints += decrease_cost;
	return true;
}

bool CharacterCreation::increase(eCharacteristic characteristic) {
	// todo zusaetzliche Faktoren aus Rasse, Geschlecht, Herkunft, Spezialeigenschaften etc.
	int increase_cost = increaseCosts(characteristic);
	if(increase_cost > remainingCharacteristicPoints)
		return false;
	if(this->characteristic[characteristic] < 3)
	{
		this->characteristic[characteristic]++;
	} else return false;
	remainingCharacteristicPoints -= increase_cost;
	return true;
}

const int* CharacterCreation::getCurrentStats() const {
	return characteristic;
}

const bool* CharacterCreation::getCurrentIncreasePossibilities() const {
	bool* poss = new bool[MAX_CHARACTERISTICS];
	for(int i = 0; i < MAX_CHARACTERISTICS; i++) {
		poss[i] = allowIncrease((eCharacteristic)i);
	}
	return poss;
}

const bool* CharacterCreation::getCurrentDecreasePossibilities() const {
	bool* poss = new bool[MAX_CHARACTERISTICS];
	for(int i = 0; i < MAX_CHARACTERISTICS; i++) {
		poss[i] = allowDecrease((eCharacteristic)i);
	}
	return poss;
}

const bool CharacterCreation::allowIncrease(eCharacteristic i) const {
	if((characteristic[i] == MAX_CHARACTERISTIC_VALUE) || (increaseCosts(i) > remainingCharacteristicPoints))
		return false;
	else return true;
}

const bool CharacterCreation::allowDecrease(eCharacteristic i) const {
	if(characteristic[i] == -MAX_CHARACTERISTIC_VALUE)
		return false;
	else return true;
}


const int CharacterCreation::increaseCosts(const eCharacteristic characteristic) const {
	switch(this->characteristic[characteristic]) {
		case 2:
		case -3:return 6;
		
		case 1:
		case -2:return 3;

		case -1:
		case 0:return 1;
		default:return 0;
	}
}

const int CharacterCreation::decreaseCosts(const eCharacteristic characteristic) const {
	switch(this->characteristic[characteristic]) {
		case 3:
		case -2:return 6;
		
		case 2:
		case -1:return 3;

		case 1:
		case 0:return 1;
		default:return 0;
	}
}

const int CharacterCreation::getRemainingCharacteristicPoints() const {
	return remainingCharacteristicPoints;
}
const unsigned int CharacterCreation::getRemainingVirtuePoints() const {
	return remainingVirtuePoints;
}

const unsigned int CharacterCreation::getRemainingFlawPoints() const {
	return remainingFlawPoints;
}

const std::string CharacterCreation::getIncreaseCostsString(const eCharacteristic characteristic) const
{
	std::ostringstream os;
	os << "Increase to " << Character::characteristicValueDescription[EXTREME_CHARACTERISTIC_VALUE + 1 + this->characteristic[characteristic]] << 
					"\n[Costs " << increaseCosts(characteristic) << "]\n" <<
					Character::characteristicValueTooltip[characteristic][EXTREME_CHARACTERISTIC_VALUE + 1 + this->characteristic[characteristic]];
	return os.str();
}

const std::string CharacterCreation::getDecreaseCostsString(const eCharacteristic characteristic) const
{
	std::ostringstream os;
	os << "Decrease to " << Character::characteristicValueDescription[EXTREME_CHARACTERISTIC_VALUE - 1 + this->characteristic[characteristic]] << 
					"\n[Gain " << decreaseCosts(characteristic) << "]\n" <<
					Character::characteristicValueTooltip[characteristic][EXTREME_CHARACTERISTIC_VALUE - 1 + this->characteristic[characteristic]];
	return os.str();
}
