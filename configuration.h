#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include "misc.h"
#include <set>
#include "tv_types.h"

class Configuration
{
	public:
		Configuration();
		~Configuration();

		void initDefaults();
		void loadConfigurationFile();
		void saveToFile() const;
//		void visitHelpChapter(const unsigned int chapter);

// get/set commands:

		const float getFieldOfView() const;
		const float getFarplane() const;
		const cCONST_TV_TEXTUREFILTER getTextureFilter() const;
		float getLODSwitchDistance() const;
		const cCONST_TV_MULTISAMPLE_TYPE getAntiAliasing() const;
		bool getIsShowFPS() const;
		const cCONST_TV_LANDSCAPE_PRECISION getLandscapePrecision() const;
		const float getStartDistance() const;
		const bool getIsProgressiveLOD() const;
		const bool getIsUseSkybox() const;
		const bool getIsUseSun() const;
		const bool getIsUseLens() const;
		const bool getIsUseClouds() const;
		const bool getIsUseGlow() const;
		const cCONST_TV_KEY getMovementForwardKey() const;
		const cCONST_TV_KEY getMovementBackKey() const;
		const cCONST_TV_KEY getMovementLeftKey() const;
		const cCONST_TV_KEY getMovementRightKey() const;
		const cCONST_TV_KEY getMovementRunKey() const;
		const cCONST_TV_KEY getMovementBackwardsKey() const;
		const cCONST_TV_KEY getCharacterScreenKey() const;
		const cCONST_TV_DEPTHBUFFERFORMAT getBufferFormat() const;
		const float getGamma() const;
		const bool getIsFullScreen() const;
		const int getResolutionX() const;
		const int getResolutionY() const;
		const int getBitDepth() const;

		const bool setIsFullScreen(const bool fullscreen);
		const bool setResolutionX(const int res_x);
		const bool setResolutionY(const int res_y);
		const bool setBitDepth(const int depth);
		const bool setBufferFormat(const int format);
		const bool setGamma(const float gamma);
		const bool setFieldOfView(const float fov);
		const bool setFarplane(const float farplane);
		const bool setTextureFilter(const int filter);
		const bool setLODSwitchDistance(const float distance);
		const bool setAntiAliasing(const int antialiasing);
		const bool setIsShowFPS(const bool show_fps);
		const bool setLandscapePrecision(const int precision);
		const bool setStartDistance(const float startdistance);
		const bool setIsProgressiveLOD(const bool lod);
		const bool setIsUseSkybox(const bool skybox);
		const bool setIsUseSun(const bool sun);
		const bool setIsUseLens(const bool lens);
		const bool setIsUseClouds(const bool clouds);
		const bool setIsUseGlow(const bool glow);
		const bool setMovementForwardKey(const int key);
		const bool setMovementBackKey(const int key);
		const bool setMovementLeftKey(const int key);
		const bool setMovementRightKey(const int key);
		const bool setMovementRunKey(const int key);
		const bool setMovementBackwardsKey(const int key);
		const bool setCharacterScreenKey(const int key);


//		const bool getIsFirstStart() const;
//		const eLanguage getLanguage() const;
/*		const unsigned int getMusicVolume() const;
		const unsigned int getSoundVolume() const;
		const unsigned int getChannels() const;
		const bool isMusic() const;
		const bool isSound() const;*/
//		const unsigned int getDesiredFramerate() const;
//		const bool isTransparency() const;
//		const bool isUnloadGraphics() const;
		

		
/*
//		const bool setLanguage(const eLanguage current_language);
		const bool setMusicVolume(const unsigned int music_volume);
		const bool setSoundVolume(const unsigned int sound_volume);
		const bool setChannels(const unsigned int channel_num);
		const bool setMusic(const bool use_music);
		const bool setSound(const bool use_sound);
*/		
//		const bool setTransparency(const bool trans_parency);
//		const bool setUnloadGraphics(const bool unload_graphics);
		
//		const bool setFirstStart(const bool first_start);

		void setConfigurationFile(const std::string& configuration_file);
//		const bool isVisitedHelpChapter(const unsigned int chapter) const;
		void parseParameters(std::list<std::string>& arguments);
	
//		static const unsigned int MIN_DESIRED_FRAMERATE = 1;
//		static const unsigned int MAX_DESIRED_FRAMERATE = 1000;
	private:
		int resolutionX;
		int resolutionY;
		bool isUseFullscreen;
		float FOV;
		float farplane;
		cCONST_TV_TEXTUREFILTER textureFilter;
		float LODSwitchDistance;
		cCONST_TV_MULTISAMPLE_TYPE antiAliasing;
		bool isShowFPS;
		cCONST_TV_LANDSCAPE_PRECISION landscapePrecision;
		float startDistance;
		bool isUseProgressiveLOD;
		bool isUseSkybox;
		bool isUseSun;
		bool isUseLens;
		bool isUseClouds;
		bool isUseGlow;

		cCONST_TV_KEY movementForwardKey;
		cCONST_TV_KEY movementBackKey;
		cCONST_TV_KEY movementLeftKey;
		cCONST_TV_KEY movementRightKey;
		cCONST_TV_KEY movementRunKey;
		cCONST_TV_KEY movementBackwardsKey;
		cCONST_TV_KEY characterScreenKey;
		cCONST_TV_DEPTHBUFFERFORMAT bufferFormat;

		float gamma;
		int bitDepth;

//	int maxfps;
//		eLanguage language;
/*
		unsigned int musicVolume;
		unsigned int soundVolume;

		unsigned int channels;
	
		bool useMusic;
		bool useSound;
		
		unsigned int desiredFramerate; // X Frames per generation
		unsigned int desiredCPU; // hold X FPS
		unsigned int currentFramerate;
		unsigned int currentFramesPerGeneration;
*/
//		bool transparency;
//		bool unloadGraphics;
//		bool firstStart;
		
		std::string configurationFile;
//		std::set<unsigned int> visitedHelpChapters;
};

extern Configuration configuration;

inline void Configuration::setConfigurationFile(const std::string& configuration_file) {
	configurationFile = configuration_file;
}

/*inline const bool Configuration::isFirstStart() const {
	return(firstStart);
}*/

inline const float Configuration::getFieldOfView() const {
	return FOV;
}
inline const float Configuration::getFarplane() const {
	return farplane;
}
inline const cCONST_TV_TEXTUREFILTER Configuration::getTextureFilter() const {
	return textureFilter;
}
inline float Configuration::getLODSwitchDistance() const {
	return LODSwitchDistance;
}
inline const cCONST_TV_MULTISAMPLE_TYPE Configuration::getAntiAliasing() const {
	return antiAliasing;
}
inline bool Configuration::getIsShowFPS() const {
	return isShowFPS;
}
inline const cCONST_TV_LANDSCAPE_PRECISION Configuration::getLandscapePrecision() const {
	return landscapePrecision;
}
inline const float Configuration::getStartDistance() const {
	return startDistance;
}
inline const bool Configuration::getIsProgressiveLOD() const {
	return isUseProgressiveLOD;
}
inline const bool Configuration::getIsUseSkybox() const {
	return isUseSkybox;
}
inline const bool Configuration::getIsUseSun() const {
	return isUseSun;
}
inline const bool Configuration::getIsUseLens() const {
	return isUseLens;
}
inline const bool Configuration::getIsUseClouds() const {
	return isUseClouds;
}
inline const bool Configuration::getIsUseGlow() const {
	return isUseGlow;
}
inline const cCONST_TV_KEY Configuration::getMovementForwardKey() const {
	return movementForwardKey;
}
inline const cCONST_TV_KEY Configuration::getMovementBackKey() const {
	return movementBackKey;
}
inline const cCONST_TV_KEY Configuration::getMovementLeftKey() const {
	return movementLeftKey;
}
inline const cCONST_TV_KEY Configuration::getMovementRightKey() const {
	return movementRightKey;
}
inline const cCONST_TV_KEY Configuration::getMovementRunKey() const {
	return movementRunKey;
}
inline const cCONST_TV_KEY Configuration::getMovementBackwardsKey() const {
	return movementBackwardsKey;
}
inline const cCONST_TV_KEY Configuration::getCharacterScreenKey() const {
	return characterScreenKey;
}
inline const cCONST_TV_DEPTHBUFFERFORMAT Configuration::getBufferFormat() const {
	return bufferFormat;
}
inline const float Configuration::getGamma() const {
	return gamma;
}
inline const bool Configuration::getIsFullScreen() const {
	return isUseFullscreen;
}
inline const int Configuration::getResolutionX() const {
	return resolutionX;
}
inline const int Configuration::getResolutionY() const {
	return resolutionY;
}
inline const int Configuration::getBitDepth() const {
	return bitDepth;
}

#ifdef _SW_DEBUG
	if((<)||(>)) {
		toErrorLog("WARNING: (Configuration::): Value out of range.");return(MIN_);
	}
#endif

#endif // _CONFIGURATION_HPP



