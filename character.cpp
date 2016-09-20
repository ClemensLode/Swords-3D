#include "character.h"
#include "misc.h"
#include <fstream>
#include <iostream>

Character::Character(std::string name, int* characteristics, bool* virtuesflaws):
	name(name) 
{
	for(int i = 0; i < MAX_CHARACTERISTICS; i++)
		characteristic[i] = characteristics[i];

	for(int i = 0; i < MAX_VIRTUESFLAWS; i++)
		virtueflaw[i] = virtuesflaws[i];	
}

void Character::initStatic() {
	characteristicIdentifier[INTELLIGENCE] = "INTELLIGENCE";
	characteristicIdentifier[PERCEPTION] = "PERCEPTION";
	characteristicIdentifier[STRENGTH] = "STRENGTH";
	characteristicIdentifier[STAMINA] = "STAMINA";
	characteristicIdentifier[PRESENCE] = "PRESENCE";
	characteristicIdentifier[COMMUNICATION] = "COMMUNICATION";
	characteristicIdentifier[DEXTERITY] = "DEXTERITY";
	characteristicIdentifier[QUICKNESS] = "QUICKNESS";

	characteristicTooltip[INTELLIGENCE] = "Intelligence represents both memory and the capacity \n\
to learn and think. It is important for Abilities that require \n\
thought power and is paramount for the Hermetic Arts. A \n\
character with low Intelligence is not necessarily stupid-\n\
common sense, street savvy, and wisdom are not described\n\
by Intelligence.";

	characteristicTooltip[PERCEPTION] = "Perception quantifies the ability to notice things, as \n\
well as powers of intuition. While sometimes conscious, \n\
Perception often works intuitively—your character simply \n\
notices something. The storyguide may also let you roll \n\
Perception when checking various Knowledges, to see if you \n\
have insight into a certain fact or concept. Perception is \n\
important for Abilities such as Awareness, Hunt, and Folk \n\
Ken.";

	characteristicTooltip[STRENGTH] = "Strength measures physical power: lifting, pushing, \n\
shoving, heaving and moving. Strength is important when \n\
hefting a melee weapon and when using brute, physical \n\
force against something or someone. People with high \n\
Strength are often bigger than those with low Strength, \n\
assuming they have the same Size.";

	characteristicTooltip[STAMINA] = "Stamina represents the limits of a character’s exertion, \n\
as well as how much physical punishment he can sustain. It \n\
is staying power, both mental and physical, and one of its \n\
most important components is simply the will to live. \n\
Stamina also measures someone’s ability to carry a load for \n\
an extended period, ignore fatigue, and withstand wounds.";

	characteristicTooltip[PRESENCE] = "Presence describes a character’s appearance, demeanor \n\
and charisma. It is important for making a good impression, \n\
as well as for leading people. Presence also measures how \n\
imposing or intimidating a person is. Even if not physically \n\
attractive, a person with high Presence might still have a \n\
demeanor that commands respect.";

	characteristicTooltip[COMMUNICATION] = "Communication represents the aptitude for self-expression. \n\
It is important when attempting to influence or communicate \n\
with someone, verbally or otherwise. A positive \n\
Communication score suggests a character who is comfortable \n\
with or confident in his relationships with other people.";

	characteristicTooltip[DEXTERITY] = "Dexterity means being able to move with agility and to \n\
manipulate objects accurately and skillfully. It includes \n\
hand-eye coordination, fine motor control, and bodily \n\
grace. Dexterity also helps determine how well your character \n\
swings a sword or throws a knife.";

	characteristicTooltip[QUICKNESS] = "Quickness indicates reaction speed and reflexes, and \n\
helps determine who acts first when two people are trying to \n\
do something with great haste. It also describes how well \n\
your character does something in a hurry. Your effective \n\
Quickness is usually modified by your Encumbrance.";

	Character::characteristicValueDescription[0] = "-6 (zero)";
	Character::characteristicValueDescription[1] = "-5 (awful)";
	Character::characteristicValueDescription[2] = "-4 (very bad)";
	Character::characteristicValueDescription[3] = "-3 (bad)";
	Character::characteristicValueDescription[4] = "-2 (disadvantaged)";
	Character::characteristicValueDescription[5] = "-1 (flawed)";
	Character::characteristicValueDescription[6] = "0 (average)";
	Character::characteristicValueDescription[7] = "1 (ok)";
	Character::characteristicValueDescription[8] = "2 (good)";
	Character::characteristicValueDescription[9] = "3 (very good)";
	Character::characteristicValueDescription[10] = "4 (great)";
	Character::characteristicValueDescription[11] = "5 (heroic)";
	Character::characteristicValueDescription[12] = "6 (superb)";


	for(int i = 0; i < MAX_CHARACTERISTICS; i++) {
		for(int j = 0; j < 13; j++) {
			Character::characteristicValueTooltip[i][j] = "test";// + i + "/" + j;
		}
	}

	for(int i = 0; i < MAX_VIRTUESFLAWS; i++)
	{
		Character::virtueflawLabel[i] = "error";
	}

	Character::virtueflawIdentifier[WANDERER] = "WANDERER";
	Character::virtueflawIdentifier[COVENFOLK] = "COVENFOLK";
	Character::virtueflawIdentifier[FREEMAN] = "FREEMAN";
	Character::virtueflawIdentifier[FAILED_APPRENTICE] = "FAILED_APPRENTICE";
	Character::virtueflawIdentifier[MENDICANT] = "MENDICANT";
	Character::virtueflawIdentifier[PETTY_MERCHANT] = "PETTY_MERCHANT";
	Character::virtueflawIdentifier[WISE_ONE] = "WISE_ONE";
	Character::virtueflawIdentifier[PROSPEROUS_PEASANT] = "PROSPEROUS_PEASANT";
	Character::virtueflawIdentifier[KNIGHT_ERRANT] = "KNIGHT_ERRANT";
	Character::virtueflawIdentifier[LESSER_NOBLE] = "LESSER_NOBLE";
	Character::virtueflawIdentifier[MERCENARY] = "MERCENARY";
	Character::virtueflawIdentifier[MAGISTER_IN_ARTIBUS] = "MAGISTER_IN_ARTIBUS";
	Character::virtueflawIdentifier[PRIEST] = "PRIEST";
	Character::virtueflawIdentifier[WEALTHY_MERCHANT] = "WEALTHY_MERCHANT";
	Character::virtueflawIdentifier[LANDED_KNIGHT] = "LANDED_KNIGHT";
	Character::virtueflawIdentifier[BRANDED_CRIMINAL] = "BRANDED_CRIMINAL";
	Character::virtueflawIdentifier[OUTCAST] = "OUTCAST";
	Character::virtueflawIdentifier[OUTLAW_LEADER] = "OUTLAW_LEADER";
	Character::virtueflawIdentifier[INFAMOUS_OUTLAW] = "INFAMOUS_OUTLAW";
		
	Character::virtueflawIdentifier[ENDURING_CONSTITUTION] = "ENDURING_CONSTITUTION";
	Character::virtueflawIdentifier[LARGE] = "LARGE";
	Character::virtueflawIdentifier[LONG_WINDED] = "LONG_WINDED";
	Character::virtueflawIdentifier[RAPID_CONVALESCENCE] = "RAPID_CONVALESCENCE";
	Character::virtueflawIdentifier[TOUGH] = "TOUGH";
	Character::virtueflawIdentifier[GIANT_BLOOD] = "GIANT_BLOOD";
	Character::virtueflawIdentifier[DISFIGURED] = "DISFIGURED";
	Character::virtueflawIdentifier[EVIL_EYE] = "EVIL_EYE";
	Character::virtueflawIdentifier[FRAGILE_CONSTITUTION] = "FRAGILE_CONSTITUTION";
	Character::virtueflawIdentifier[MISSING_EAR] = "MISSING_EAR";
	Character::virtueflawIdentifier[MISSING_EYE] = "MISSING_EYE";
	Character::virtueflawIdentifier[OBESE] = "OBESE";
	Character::virtueflawIdentifier[CLUMSY] = "CLUMSY";
	Character::virtueflawIdentifier[DECREPIT] = "DECREPIT";
	Character::virtueflawIdentifier[HUNCHBACK] = "HUNCHBACK";
	Character::virtueflawIdentifier[LAME] = "LAME";
	Character::virtueflawIdentifier[MISSING_HAND] = "MISSING_HAND";
	Character::virtueflawIdentifier[SMALL_FRAME] = "SMALL_FRAME";
	Character::virtueflawIdentifier[ARTHRITIS] = "ARTHRITIS";
	Character::virtueflawIdentifier[MUTE] = "MUTE";
	Character::virtueflawIdentifier[DWARF] = "DWARF";
	Character::virtueflawIdentifier[AGE_QUICKLY] = "AGE_QUICKLY";
	Character::virtueflawIdentifier[BLIND] = "BLIND";
	Character::virtueflawIdentifier[ENFEEBLED] = "ENFEEBLED";

	Character::virtueflawIdentifier[AMBIDEXTROUS] = "AMBIDEXTROUS";
	Character::virtueflawIdentifier[CAREFREE] = "CAREFREE";
	Character::virtueflawIdentifier[CLEAR_THINKER] = "CLEAR_THINKER";
	Character::virtueflawIdentifier[FREE_EXPRESSION] = "FREE_EXPRESSION";
	Character::virtueflawIdentifier[INSPIRATIONAL] = "INSPIRATIONAL";
	Character::virtueflawIdentifier[KEEN_VISION] = "KEEN_VISION";
	Character::virtueflawIdentifier[LIGHT_SLEEPER] = "LIGHT_SLEEPER";
	Character::virtueflawIdentifier[MAGIC_RESISTANCE] = "MAGIC_RESISTANCE";
	Character::virtueflawIdentifier[PERFECT_BALANCE] = "PERFECT_BALANCE";
	Character::virtueflawIdentifier[PIERCING_GAZE] = "PIERCING_GAZE";
	Character::virtueflawIdentifier[SHARP_EARS] = "SHARP_EARS";
	Character::virtueflawIdentifier[STRONG_WILLED] = "STRONG_WILLED";
	Character::virtueflawIdentifier[TROUPE_UPBRINGING] = "TROUPE_UPBRINGING";
	Character::virtueflawIdentifier[VENUS_BLESSING] = "VENUS_BLESSING";
	Character::virtueflawIdentifier[WELL_TRAVELED] = "WELL_TRAVELED";
	Character::virtueflawIdentifier[WITHSTANDING_MAGIC] = "WITHSTANDING_MAGIC";
	Character::virtueflawIdentifier[LIGHTNING_REFLEXES] = "LIGHTNING_REFLEXES";
	Character::virtueflawIdentifier[LIGHT_TOUCH] = "LIGHT_TOUCH";
	Character::virtueflawIdentifier[WAYS_OF_THE_LAND] = "WAYS_OF_THE_LAND";
	Character::virtueflawIdentifier[DEEP_SLEEPER] = "DEEP_SLEEPER";
	Character::virtueflawIdentifier[MAGICAL_AIR] = "MAGICAL_AIR";
	Character::virtueflawIdentifier[MAGIC_SUSPECTIBILITY] = "MAGIC_SUSPECTIBILITY";
	Character::virtueflawIdentifier[POOR_EYESIGHT] = "POOR_EYESIGHT";
	Character::virtueflawIdentifier[POOR_HEARING] = "POOR_HEARING";
	Character::virtueflawIdentifier[WEAK_WILLED] = "WEAK_WILLED";
	Character::virtueflawIdentifier[NONCOMBATANT] = "NONCOMBATANT";

	Character::virtueflawIdentifier[BOOK_LEARNER] = "BOOK_LEARNER";
	Character::virtueflawIdentifier[EDUCATED] = "EDUCATED";
	Character::virtueflawIdentifier[FURTHER_EDUCATION] = "FURTHER_EDUCATION";
	Character::virtueflawIdentifier[LOW_SELFESTEEM] = "LOW_SELFESTEEM";
	Character::virtueflawIdentifier[SELF_CONFIDENT] = "SELF_CONFIDENT";
	Character::virtueflawIdentifier[VETERAN] = "VETERAN";
	Character::virtueflawIdentifier[GREAT_CHARACTERISTIC_INTELLIGENCE] = "GREAT_CHARACTERISTIC_INTELLIGENCE";
	Character::virtueflawIdentifier[GREAT_CHARACTERISTIC_PERCEPTION] = "GREAT_CHARACTERISTIC_PERCEPTION";
	Character::virtueflawIdentifier[GREAT_CHARACTERISTIC_STRENGTH] = "GREAT_CHARACTERISTIC_STRENGTH";
	Character::virtueflawIdentifier[GREAT_CHARACTERISTIC_STAMINA] = "GREAT_CHARACTERISTIC_STAMINA";
	Character::virtueflawIdentifier[GREAT_CHARACTERISTIC_PRESENCE] = "GREAT_CHARACTERISTIC_PRESENCE";
	Character::virtueflawIdentifier[GREAT_CHARACTERISTIC_COMMUNICATION] = "GREAT_CHARACTERISTIC_COMMUNICATION";
	Character::virtueflawIdentifier[GREAT_CHARACTERISTIC_DEXTERITY] = "GREAT_CHARACTERISTIC_DEXTERITY";
	Character::virtueflawIdentifier[GREAT_CHARACTERISTIC_QUICKNESS] = "GREAT_CHARACTERISTIC_QUICKNESS";
	Character::virtueflawIdentifier[JACK_OF_ALL_TRADES] = "JACK_OF_ALL_TRADES";
	Character::virtueflawIdentifier[TRAINING] = "TRAINING";
	Character::virtueflawIdentifier[FAST_LEARNER] = "FAST_LEARNER";
	Character::virtueflawIdentifier[HIGHLY_TRAINED] = "HIGHLY_TRAINED";
	Character::virtueflawIdentifier[INCREDIBLE_CHARACTERISTIC_INTELLIGENCE] = "INCREDIBLE_CHARACTERISTIC_INTELLIGENCE";
	Character::virtueflawIdentifier[INCREDIBLE_CHARACTERISTIC_PERCEPTION] = "INCREDIBLE_CHARACTERISTIC_PERCEPTION";
	Character::virtueflawIdentifier[INCREDIBLE_CHARACTERISTIC_STRENGTH] = "INCREDIBLE_CHARACTERISTIC_STRENGTH";
	Character::virtueflawIdentifier[INCREDIBLE_CHARACTERISTIC_STAMINA] = "INCREDIBLE_CHARACTERISTIC_STAMINA";
	Character::virtueflawIdentifier[INCREDIBLE_CHARACTERISTIC_PRESENCE] = "INCREDIBLE_CHARACTERISTIC_PRESENCE";
	Character::virtueflawIdentifier[INCREDIBLE_CHARACTERISTIC_COMMUNICATION] = "INCREDIBLE_CHARACTERISTIC_COMMUNICATION";
	Character::virtueflawIdentifier[INCREDIBLE_CHARACTERISTIC_DEXTERITY] = "INCREDIBLE_CHARACTERISTIC_DEXTERITY";
	Character::virtueflawIdentifier[INCREDIBLE_CHARACTERISTIC_QUICKNESS] = "INCREDIBLE_CHARACTERISTIC_QUICKNESS";
	Character::virtueflawIdentifier[MYTHIC_CHARACTERISTIC_INTELLIGENCE] = "MYTHIC_CHARACTERISTIC_INTELLIGENCE";
	Character::virtueflawIdentifier[MYTHIC_CHARACTERISTIC_PERCEPTION] = "MYTHIC_CHARACTERISTIC_PERCEPTION";
	Character::virtueflawIdentifier[MYTHIC_CHARACTERISTIC_STRENGTH] = "MYTHIC_CHARACTERISTIC_STRENGTH";
	Character::virtueflawIdentifier[MYTHIC_CHARACTERISTIC_STAMINA] = "MYTHIC_CHARACTERISTIC_STAMINA";
	Character::virtueflawIdentifier[MYTHIC_CHARACTERISTIC_PRESENCE] = "MYTHIC_CHARACTERISTIC_PRESENCE";
	Character::virtueflawIdentifier[MYTHIC_CHARACTERISTIC_COMMUNICATION] = "MYTHIC_CHARACTERISTIC_COMMUNICATION";
	Character::virtueflawIdentifier[MYTHIC_CHARACTERISTIC_DEXTERITY] = "MYTHIC_CHARACTERISTIC_DEXTERITY";
	Character::virtueflawIdentifier[MYTHIC_CHARACTERISTIC_QUICKNESS] = "MYTHIC_CHARACTERISTIC_QUICKNESS";
	Character::virtueflawIdentifier[SHORT_ATTENTION_SPAN] = "SHORT_ATTENTION_SPAN";
	Character::virtueflawIdentifier[POOR_CHARACTERISTIC_INTELLIGENCE] = "POOR_CHARACTERISTIC_INTELLIGENCE";
	Character::virtueflawIdentifier[POOR_CHARACTERISTIC_PERCEPTION] = "POOR_CHARACTERISTIC_PERCEPTION";
	Character::virtueflawIdentifier[POOR_CHARACTERISTIC_STRENGTH] = "POOR_CHARACTERISTIC_STRENGTH";
	Character::virtueflawIdentifier[POOR_CHARACTERISTIC_STAMINA] = "POOR_CHARACTERISTIC_STAMINA";
	Character::virtueflawIdentifier[POOR_CHARACTERISTIC_PRESENCE] = "POOR_CHARACTERISTIC_PRESENCE";
	Character::virtueflawIdentifier[POOR_CHARACTERISTIC_COMMUNICATION] = "POOR_CHARACTERISTIC_COMMUNICATION";
	Character::virtueflawIdentifier[POOR_CHARACTERISTIC_DEXTERITY] = "POOR_CHARACTERISTIC_DEXTERITY";
	Character::virtueflawIdentifier[POOR_CHARACTERISTIC_QUICKNESS] = "POOR_CHARACTERISTIC_QUICKNESS";
	Character::virtueflawIdentifier[FEEBLE_CHARACTERISTIC_INTELLIGENCE] = "FEEBLE_CHARACTERISTIC_INTELLIGENCE";
	Character::virtueflawIdentifier[FEEBLE_CHARACTERISTIC_PERCEPTION] = "FEEBLE_CHARACTERISTIC_PERCEPTION";
	Character::virtueflawIdentifier[FEEBLE_CHARACTERISTIC_STRENGTH] = "FEEBLE_CHARACTERISTIC_STRENGTH";
	Character::virtueflawIdentifier[FEEBLE_CHARACTERISTIC_STAMINA] = "FEEBLE_CHARACTERISTIC_STAMINA";
	Character::virtueflawIdentifier[FEEBLE_CHARACTERISTIC_PRESENCE] = "FEEBLE_CHARACTERISTIC_PRESENCE";
	Character::virtueflawIdentifier[FEEBLE_CHARACTERISTIC_COMMUNCATION] = "FEEBLE_CHARACTERISTIC_COMMUNCATION";
	Character::virtueflawIdentifier[FEEBLE_CHARACTERISTIC_DEXTERITY] = "FEEBLE_CHARACTERISTIC_DEXTERITY";
	Character::virtueflawIdentifier[FEEBLE_CHARACTERISTIC_QUICKNESS] = "FEEBLE_CHARACTERISTIC_QUICKNESS";
	Character::virtueflawIdentifier[PATHETIC_CHARACTERISTIC_INTELLIGENCE] = "PATHETIC_CHARACTERISTIC_INTELLIGENCE";
	Character::virtueflawIdentifier[PATHETIC_CHARACTERISTIC_PERCEPTION] = "PATHETIC_CHARACTERISTIC_PERCEPTION";
	Character::virtueflawIdentifier[PATHETIC_CHARACTERISTIC_STRENGTH] = "PATHETIC_CHARACTERISTIC_STRENGTH";
	Character::virtueflawIdentifier[PATHETIC_CHARACTERISTIC_STAMINA] = "PATHETIC_CHARACTERISTIC_STAMINA";
	Character::virtueflawIdentifier[PATHETIC_CHARACTERISTIC_PRESENCE] = "PATHETIC_CHARACTERISTIC_PRESENCE";
	Character::virtueflawIdentifier[PATHETIC_CHARACTERISTIC_COMMUNICATION] = "PATHETIC_CHARACTERISTIC_COMMUNICATION";
	Character::virtueflawIdentifier[PATHETIC_CHARACTERISTIC_DEXTERITY] = "PATHETIC_CHARACTERISTIC_DEXTERITY";
	Character::virtueflawIdentifier[PATHETIC_CHARACTERISTIC_QUICKNESS] = "PATHETIC_CHARACTERISTIC_QUICKNESS";
}
void Character::loadStringFiles() {
	loadStringFile("characteristics_english.dat", Character::characteristicIdentifier, Character::characteristicLabel, MAX_CHARACTERISTICS);
	loadStringFile("virtueflaws_english.dat", Character::virtueflawIdentifier, Character::virtueflawLabel, MAX_VIRTUESFLAWS);
}

eVirtueFlaw Character::getVirtueflawId(const std::string virtueflaw_name) {
	for(int i = 0; i < MAX_VIRTUESFLAWS; i++)
	{
		if(virtueflaw_name == virtueflawLabel[i])
			return (eVirtueFlaw)i;
	}
	return MAX_VIRTUESFLAWS; // TODO
}

eCharacteristic Character::getCharacteristicId(const std::string characteristic_name) {
	for(int i = 0; i < MAX_CHARACTERISTICS; i++)
	{
		if(characteristic_name == characteristicLabel[i])
			return (eCharacteristic)i;
	}
	return MAX_CHARACTERISTICS; // TODO
}

Character::~Character() {
}


const bool Character::loadVirtuesFlawsFile()
{
	std::string virtues_flaws_filename = "virtueflaws.dat";
	std::ifstream pFile(virtues_flaws_filename.c_str());
	if(!checkStreamIsOpen(pFile, "Character::loadVirtuesFlawsFile", virtues_flaws_filename)) {
		toErrorLog("ERROR (Character::loadVirtuesFlawsFile()): Error opening " + virtues_flaws_filename);return false;}
	char line[1024];
	bool in_block = false;
	std::fstream::pos_type old_pos = pFile.tellg();
	while(pFile.getline(line, 1024)) {
		if(!checkStreamForFailure(pFile, "Character::loadVirtuesFlawsFile", virtues_flaws_filename)) {
			toErrorLog("ERROR (Character::loadVirtuesFlawsFile()): Error reading " + virtues_flaws_filename);return false;}
		std::string text = line;
		size_t start = text.find_first_not_of("\t ");
		if((start == std::string::npos) || (text[0] == '#') || (text[0] == '\0'))
			continue; // ignore line
		size_t stop = text.find_first_of("\t ", start);
		if(stop == std::string::npos) 
			stop = text.size();
		std::string index = text.substr(start, stop);

		if(!in_block) {
			in_block = (index == "@VIRTUESFLAWS");
		} else 
		{
			if(index == "@END")
				return true;
			pFile.seekg(old_pos);
			std::map<std::string, std::list<std::string> > block;
			if(!parse_block_map(pFile, block)) {
				toErrorLog("WARNING (Character::loadVirtuesFlawsFile()): No concluding @END for @ block was found in file " + virtues_flaws_filename + " => trying to parse what we have so far.");
			}
			for(int j = 0; j < MAX_VIRTUESFLAWS; j++)
			{
				std::map<std::string, std::list<std::string> >::iterator i;
				if((i = block.find(Character::virtueflawIdentifier[j])) != block.end())
                {
					i->second.pop_front(); // Identifier loeschen
					if(!i->second.size()) {
						toErrorLog("WARNING (Character::loadVirtuesFlawsFile()): Parameter for virtue/flaw value missing for entry " + Character::virtueflawIdentifier[j] + " in file " + virtues_flaws_filename + " => ignoring line.");
						continue;
					} else
					{
						int flaw_value = atoi(i->second.front().c_str());
						Character::virtueData[j].value = flaw_value;
						i->second.pop_front();
					}
					if(!i->second.size()) {
						toErrorLog("WARNING (Character::loadVirtuesFlawsFile()): Parameter for category value missing for entry " + Character::virtueflawIdentifier[j] + " in file " + virtues_flaws_filename + " => ignoring line.");
						 continue;
					} else
					{
						eVirtueCategory category = (eVirtueCategory)atoi(i->second.front().c_str());
						Character::virtueData[j].category = category;
						i->second.pop_front();
					}
                    block.erase(i);
				} else {
					toErrorLog("ERROR (Character::loadVirtuesFlawsFile()): Entry " + Character::virtueflawIdentifier[j] + " missing in file " + virtues_flaws_filename);
				}
			}
			if(block.size() > 0)
			{
				toErrorLog("WARNING (Character::loadVirtuesFlawsFile()): Unknown entries in file " + virtues_flaws_filename);
			}
		}
	}
	return true;
}


std::string Character::characteristicLabel[MAX_CHARACTERISTICS];
std::string Character::characteristicTooltip[MAX_CHARACTERISTICS];

std::string Character::characteristicValueDescription[13];
std::string Character::characteristicValueTooltip[MAX_CHARACTERISTICS][13];

std::string Character::virtueflawLabel[MAX_VIRTUESFLAWS];
//std::string Character::virtueflawTooltip[MAX_VIRTUESFLAWS];
Virtue Character::virtueData[MAX_VIRTUESFLAWS];

std::string Character::characteristicIdentifier[MAX_CHARACTERISTICS];
std::string Character::virtueflawIdentifier[MAX_VIRTUESFLAWS];
