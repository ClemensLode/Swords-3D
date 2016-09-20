#include "configuration.h"
#include <sstream>
#include <fstream>

Configuration::Configuration(): 
	configurationFile("gui.cfg")
{
	loadConfigurationFile();
}

Configuration::~Configuration()
{ }

void Configuration::initDefaults()
{
	setIsFullScreen(false);
	setResolutionX(800);
	setResolutionY(600);
	setBitDepth(32);
	setBufferFormat(cTV_DEPTHBUFFER_BESTBUFFER);
	setGamma(1.0f);
	setFieldOfView(90.0f);
	setFarplane(10000.0f);
	setTextureFilter(cTV_FILTER_BILINEAR);
	setLODSwitchDistance(512.0f);
	setAntiAliasing(cTV_MULTISAMPLE_2_SAMPLES);
	setIsShowFPS(true);
	setLandscapePrecision(cTV_PRECISION_HIGH);
	setStartDistance(256.0f);
	setIsProgressiveLOD(true);
	setIsUseSkybox(true);
	setIsUseSun(true);
	setIsUseLens(true);
	setIsUseClouds(true);
	setIsUseGlow(true);
	setMovementForwardKey(cTV_KEY_W);
	setMovementBackKey(cTV_KEY_S);
	setMovementLeftKey(cTV_KEY_A);
	setMovementRightKey(cTV_KEY_D);
	setMovementRunKey(cTV_KEY_LEFTSHIFT);
	setMovementBackwardsKey(cTV_KEY_ALT_LEFT);
	setCharacterScreenKey(cTV_KEY_C);

	/*
	setMusicVolume(75);
	setSoundVolume(75);
	setChannels(16);
	setMusic(true);
	setSound(true);*/
	configurationFile = "gui.cfg";
}

void Configuration::parseParameters(std::list<std::string>& arguments)
{
/*	for(std::list<std::string>::iterator i = arguments.begin(); i != arguments.end(); )
	{
		bool not_found = false;
		if((*i) == "-nosound")
		{
			setSound(false);
			setMusic(false);
		} 
		else if((*i) == "-640")
		{
			setResolution(RESOLUTION_640x480);
		} else if((*i) == "-800")
		{
			setResolution(RESOLUTION_800x600);
		} else if((*i) == "-1024")
		{
			setResolution(RESOLUTION_1024x768);
		} else if((*i) == "-1280")
		{
			setResolution(RESOLUTION_1280x1024);
		} else if((*i) == "-8bit")
		{
			setBitDepth(DEPTH_8BIT);
		} else if((*i) == "-16bit")
		{
			setBitDepth(DEPTH_16BIT);
		} else if((*i) == "-24bit")
		{
			setBitDepth(DEPTH_24BIT);
		} else if((*i) == "-32bit")
		{
			setBitDepth(DEPTH_32BIT);
		} else if((*i) == "-fs")
		{
			setFullScreen(true);
		} else if((*i) == "-window")
		{
			setFullScreen(false);
		} else 
			not_found = true;
		if(not_found)
			i++;
		else i = arguments.erase(i);
	}*/

}
/*
void Configuration::visitHelpChapter(const unsigned int chapter)
{
	visitedHelpChapters.insert(chapter);
}*/

void Configuration::saveToFile() const
{
	std::ofstream pFile(configurationFile.c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!pFile.is_open())
	{
		toErrorLog("ERROR: (Configuration::saveToFile): File could not be opened.");
		return;
	}
	pFile << "@SETTINGS" << std::endl;
	pFile << "# 1 = full screen, 0 = window mode" << std::endl;
	pFile << "    \"Full Screen\" = \"" << (int)getIsFullScreen() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Resolution X\" = \"" << getResolutionX() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Resolution Y\" = \"" << getResolutionY() << "\"" << std::endl;
	pFile << "# 8, 16, 24, 32" << std::endl;
	pFile << "    \"Bit Depth\" = \"" << getBitDepth() << "\"" << std::endl;
	pFile << "# 0.0f to 10.0f" << std::endl;
	pFile << "    \"Gamma\" = \"" << getGamma() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Buffer Format\" = \"" << (int)getBufferFormat() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Field Of View\" = \"" << getFieldOfView() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Farplane\" = \"" << getFarplane() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Texture Filter\" = \"" << (int)getTextureFilter() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"LOD Switch Distance\" = \"" << getLODSwitchDistance() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Anti-Aliasing\" = \"" << (int)getAntiAliasing() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Show FPS\" = \"" << (int)getIsShowFPS() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Landscape Precision\" = \"" << (int)getLandscapePrecision() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Start Distance\" = \"" << getStartDistance() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Use Progressive LOD\" = \"" << (int)getIsProgressiveLOD() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Use Skybox\" = \"" << (int)getIsUseSkybox() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Use Sun\" = \"" << (int)getIsUseSun() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Use Lensflare\" = \"" << (int)getIsUseLens() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Use Clouds\" = \"" << (int)getIsUseClouds() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Use Glow\" = \"" << (int)getIsUseGlow() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Movement Forward Key\" = \"" << (int)getMovementForwardKey() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Movement Back Key\" = \"" << (int)getMovementBackKey() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Movement Left Key\" = \"" << (int)getMovementLeftKey() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Movement Right Key\" = \"" << (int)getMovementRightKey() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Movement Run Key\" = \"" << (int)getMovementRunKey() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Movement Backwards Key\" = \"" << (int)getMovementBackwardsKey() << "\"" << std::endl;
	pFile << "# " << std::endl;
	pFile << "    \"Character Screen Key\" = \"" << (int)getCharacterScreenKey() << "\"" << std::endl;


	/*pFile << "# Is this the first start? Then display a language menu at the beginning... 1 = yes, 0 = no" << std::endl;
	pFile << "    \"First start\" = \"" << (int)isFirstStart() << "\"" << std::endl;
	pFile << "# 1 = english, 2 = german" << std::endl;
	pFile << "    \"Language\" = \"" << (int)getLanguage() << "\"" << std::endl;
	
	pFile << "# use music (1: on, 0: off)" << std::endl;
	pFile << "    \"Music\" = \"" << (int)isMusic() << "\"" << std::endl;
	pFile << "# use sound (1: on, 0: off)" << std::endl;
	pFile << "    \"Sound effects\" = \"" << (int)isSound() << "\"" << std::endl;

	pFile << "# music volume (0% - 100%)" << std::endl;
	pFile << "    \"Music volume\" = \"" << (int)getMusicVolume() << "\"" << std::endl;
	pFile << "# sound volume (0% - 100%)" << std::endl;
	pFile << "    \"Sound volume\" = \"" << (int)getSoundVolume() << "\"" << std::endl;
	pFile << "# max number of simultaneously played sounds" << std::endl;
	pFile << "    \"Channels\" = \"" << (int)getChannels() << "\"" << std::endl;
	
	pFile << "# internal" << std::endl;
	pFile << "    \"Visited help chapters\" =";
	for(std::set<unsigned int>::const_iterator i = visitedHelpChapters.begin(); i != visitedHelpChapters.end(); ++i)
		pFile << " " << *i;*/
	pFile << std::endl;
	pFile << "@END" << std::endl;
}

void Configuration::loadConfigurationFile()
{
	std::ifstream pFile(configurationFile.c_str());
	if(!checkStreamIsOpen(pFile, "Configuration::loadConfigurationFile()", configurationFile))
	{
//		toErrorLog(lookUpString(START_CREATE_NEW_DEFAULT_FILE_STRING));
//		problem: kein Zugriff auf theme!
                initDefaults();
                saveToFile();
                return;
	}
	
	std::fstream::pos_type old_pos = pFile.tellg();
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "Configuration::loadConfigurationFile()", configurationFile))
			return;
		
		std::string text = line;
		size_t start=text.find_first_not_of("\t ");
		if((start==std::string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		size_t stop=text.find_first_of("\t ", start);
		if(stop==std::string::npos) stop=text.size();
		std::string index=text.substr(start, stop);
		std::string value;
		std::map<std::string, std::list<std::string> >::iterator i;
		if(index=="@SETTINGS")
		{
			std::map<std::string, std::list<std::string> > block;
			pFile.seekg(old_pos);
			parse_block_map(pFile, block);

			if((i=block.find("Full Screen"))!=block.end()){
				i->second.pop_front();
			   	setIsFullScreen(atoi(i->second.front().c_str()) == 1);
			}
			if((i=block.find("Resolution X"))!=block.end()){
				i->second.pop_front();
			   	setResolutionX(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Resolution Y"))!=block.end()){
				i->second.pop_front();
			   	setResolutionY(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Bit Depth"))!=block.end()){
				i->second.pop_front();
			   	setBitDepth(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Gamma"))!=block.end()){
				i->second.pop_front();
			   	setGamma(atof(i->second.front().c_str()));
			}
			if((i=block.find("Buffer Format"))!=block.end()){
				i->second.pop_front();
			   	setBufferFormat(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Field Of View"))!=block.end()){
				i->second.pop_front();
			   	setFieldOfView(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Farplane"))!=block.end()){
				i->second.pop_front();
			   	setFarplane(atof(i->second.front().c_str()));
			}
			if((i=block.find("Texture Filter"))!=block.end()){
				i->second.pop_front();
			   	setTextureFilter(atoi(i->second.front().c_str()));
			}
			if((i=block.find("LOD Switch Distance"))!=block.end()){
				i->second.pop_front();
			   	setLODSwitchDistance(atof(i->second.front().c_str()));
			}
			if((i=block.find("Anti-Aliasing"))!=block.end()){
				i->second.pop_front();
			   	setAntiAliasing(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Show FPS"))!=block.end()){
				i->second.pop_front();
			   	setIsShowFPS(atoi(i->second.front().c_str()) == 1);
			}
			if((i=block.find("Landscape Precision"))!=block.end()){
				i->second.pop_front();
			   	setLandscapePrecision(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Start Distance"))!=block.end()){
				i->second.pop_front();
			   	setStartDistance(atof(i->second.front().c_str()));
			}
			if((i=block.find("Use Progressive LOD"))!=block.end()){
				i->second.pop_front();
			   	setIsProgressiveLOD(atoi(i->second.front().c_str()) == 1);
			}
			if((i=block.find("Use Skybox"))!=block.end()){
				i->second.pop_front();
			   	setIsUseSkybox(atoi(i->second.front().c_str()) == 1);
			}
			if((i=block.find("Use Sun"))!=block.end()){
				i->second.pop_front();
			   	setIsUseSun(atoi(i->second.front().c_str()) == 1);
			}
			if((i=block.find("Use Lensflare"))!=block.end()){
				i->second.pop_front();
			   	setIsUseLens(atoi(i->second.front().c_str()) == 1);
			}
			if((i=block.find("Use Clouds"))!=block.end()){
				i->second.pop_front();
			   	setIsUseClouds(atoi(i->second.front().c_str()) == 1);
			}
			if((i=block.find("Use Glow"))!=block.end()){
				i->second.pop_front();
			   	setIsUseGlow(atoi(i->second.front().c_str()) == 1);
			}
			if((i=block.find("Movement Forward Key"))!=block.end()){
				i->second.pop_front();
			   	setMovementForwardKey(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Movement Back Key"))!=block.end()){
				i->second.pop_front();
			   	setMovementBackKey(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Movement Left Key"))!=block.end()){
				i->second.pop_front();
			   	setMovementLeftKey(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Movement Right Key"))!=block.end()){
				i->second.pop_front();
			   	setMovementRightKey(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Movement Run Key"))!=block.end()){
				i->second.pop_front();
			   	setMovementRunKey(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Movement Backwards Key"))!=block.end()){
				i->second.pop_front();
			   	setMovementBackwardsKey(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Character Screen Key"))!=block.end()){
				i->second.pop_front();
			   	setCharacterScreenKey(atoi(i->second.front().c_str()));
			}
/*			if((i=block.find("Music"))!=block.end()){
				i->second.pop_front();
			   	setMusic(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Sound effects"))!=block.end()){
				i->second.pop_front();
			   	setSound(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Music volume"))!=block.end()){
				i->second.pop_front();
			   	setMusicVolume(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Sound volume"))!=block.end()){
				i->second.pop_front();
			   	setSoundVolume(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Channels"))!=block.end()){
				i->second.pop_front();
			   	setChannels(atoi(i->second.front().c_str()));
			}*/
			/*
			if((i=block.find("Visited help chapters"))!=block.end()){
				i->second.pop_front();
				visitedHelpChapters.clear();
				for(std::list<std::string>::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
					visitedHelpChapters.insert(atoi(j->c_str()));
			}*/
		}
		old_pos = pFile.tellg();
	}// END while
} // schoen :)



const bool Configuration::setFieldOfView(const float fov) {
	if(fov == FOV)
		return false;
	FOV = fov;
	return true;
}
const bool Configuration::setFarplane(const float farplane) {
	if(this->farplane == farplane)
		return false;
	this->farplane = farplane;
	return true;
}

const bool Configuration::setTextureFilter(const int filter) {
	if(textureFilter == (cCONST_TV_TEXTUREFILTER)filter)
		return false;
	textureFilter = (cCONST_TV_TEXTUREFILTER)filter;
	return true;
}
const bool Configuration::setLODSwitchDistance(const float distance) {
	if(LODSwitchDistance == distance)
		return false;
	LODSwitchDistance = distance;
	return true;
}

const bool Configuration::setAntiAliasing(const int antialiasing) {
	// TODO RANGE CHECK
	if(antiAliasing == (cCONST_TV_MULTISAMPLE_TYPE)antialiasing)
		return false;
	antiAliasing = (cCONST_TV_MULTISAMPLE_TYPE)antialiasing;
	return true;
}
const bool Configuration::setIsShowFPS(const bool show_fps) {
	if(isShowFPS == show_fps)
		return false;
	isShowFPS = show_fps;
	return true;
}
const bool Configuration::setLandscapePrecision(const int precision) {
	if(landscapePrecision == (cCONST_TV_LANDSCAPE_PRECISION)precision)
		return false;
	landscapePrecision = (cCONST_TV_LANDSCAPE_PRECISION)precision;
	return true;
}
const bool Configuration::setStartDistance(const float startDistance) {
	if(this->startDistance == startDistance)
		return false;
	this->startDistance = startDistance;
	return true;
}
const bool Configuration::setIsProgressiveLOD(const bool lod) {
	if(isUseProgressiveLOD == lod)
		return false;
	isUseProgressiveLOD = lod;
	return true;
}
const bool Configuration::setIsUseSkybox(const bool skybox) {
	if(isUseSkybox == skybox)
		return false;
	isUseSkybox = skybox;
	return true;
}
const bool Configuration::setIsUseSun(const bool sun) {
	if(isUseSun == sun)
		return false;
	isUseSun = sun;
	return true;
}
const bool Configuration::setIsUseLens(const bool lens) {
	if(isUseLens == lens)
		return false;
	isUseLens = lens;
	return true;
}
const bool Configuration::setIsUseClouds(const bool clouds) {
	if(isUseClouds == clouds)
		return false;
	isUseClouds = clouds;
	return true;
}
const bool Configuration::setIsUseGlow(const bool glow) {
	if(isUseGlow == glow)
		return false;
	isUseGlow = glow;
	return true;
}
const bool Configuration::setMovementForwardKey(const int key) {
	if(movementForwardKey == (cCONST_TV_KEY)key)
		return false;
	movementForwardKey = (cCONST_TV_KEY)key;
	return true;
}
const bool Configuration::setMovementBackKey(const int key) {
	if(movementBackKey == (cCONST_TV_KEY)key)
		return false;
	movementBackKey = (cCONST_TV_KEY)key;
	return true;
}
const bool Configuration::setMovementLeftKey(const int key) {
	if(movementLeftKey == (cCONST_TV_KEY)key)
		return false;
	movementLeftKey = (cCONST_TV_KEY)key;
	return true;
}
const bool Configuration::setMovementRightKey(const int key) {
	if(movementRightKey == (cCONST_TV_KEY)key)
		return false;
	movementRightKey = (cCONST_TV_KEY)key;
	return true;
}
const bool Configuration::setMovementRunKey(const int key) {
	if(movementRunKey == (cCONST_TV_KEY)key)
		return false;
	movementRunKey = (cCONST_TV_KEY)key;
	return true;
}
const bool Configuration::setMovementBackwardsKey(const int key) {
	if(movementBackwardsKey == (cCONST_TV_KEY)key)
		return false;
	movementBackwardsKey = (cCONST_TV_KEY)key;
	return true;
}
const bool Configuration::setCharacterScreenKey(const int key) {
	if(characterScreenKey == (cCONST_TV_KEY)key)
		return false;
	characterScreenKey = (cCONST_TV_KEY)key;
	return true;
}
const bool Configuration::setBufferFormat(const int format) {
	if(bufferFormat == (cCONST_TV_DEPTHBUFFERFORMAT)format)
		return false;
	bufferFormat = (cCONST_TV_DEPTHBUFFERFORMAT)format;
	return true;
}
const bool Configuration::setGamma(const float gamma) {
	if(this->gamma == gamma)
		return false;
	this->gamma = gamma;
	return true;
}
const bool Configuration::setIsFullScreen(const bool fullscreen) {
	if(isUseFullscreen == fullscreen)
		return false;
	isUseFullscreen = fullscreen;
	return true;
}
const bool Configuration::setResolutionX(const int res_x) {
	if(resolutionX == res_x)
		return false;
	resolutionX = res_x;
	return true;
}
const bool Configuration::setResolutionY(const int res_y) {
	if(resolutionY == res_y)
		return false;
	resolutionY = res_y;
	return true;
}
const bool Configuration::setBitDepth(const int depth) {
	if(bitDepth == depth)
		return false;
	bitDepth = depth;
	return true;
}

/*const bool Configuration::setFirstStart(const bool first_start) 
{
	if(firstStart == first_start)
		return(false);
	firstStart = first_start;
	if(!firstStart)
		saveToFile();
	return(true);
}*/


Configuration configuration;

