#include "gui.h"


#include <CEGUI.h>
#include <CEGUIImageset.h>
#include <CEGUISystem.h>
#include <CEGUILogger.h>
#include <CEGUISchemeManager.h>
#include <CEGUIWindowManager.h>
#include <CEGUIFactoryModule.h>
#include <CEGUIWindow.h>
#include <CEGUIDefaultResourceProvider.h>
#include <CEGUIConfig.h>
#include <directx9GUIRenderer/d3d9renderer.h>

#include "character.h"
#include "character_creation.h"

#ifdef CEGUI_WITH_XERCES
#   include "CEGUIXercesParser.h"
#endif

// ----------- CEGUI -----------
CEGUI::Renderer* mGUIRenderer;
CEGUI::System* mGUISystem;
CEGUI::Window* wnd[2];//MAX_VIRTUE_CATEGORIES + 1];
int page = 0;
// ----------- END CEGUI -----------


GUI::GUI(LPDIRECT3DDEVICE9 d3Device, CharacterCreation* character_creation) {
// ---------- CEGUI INITIALIZATION ----------
	this->d3Device = d3Device;
	this->character_creation = character_creation;
	Character::loadVirtuesFlawsFile();
	Character::loadStringFiles();
	mGUIRenderer = new CEGUI::DirectX9Renderer(this->d3Device,0);
	mGUISystem = new CEGUI::System(mGUIRenderer);
	CEGUI::Logger::getSingleton().setLogFilename("..\\cegui.log", false);
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

	// initialise the required dirs for the DefaultResourceProvider
	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());

	rp->setResourceGroupDirectory("schemes", "../datafiles/schemes/");
	rp->setResourceGroupDirectory("imagesets", "../datafiles/imagesets/");
	rp->setResourceGroupDirectory("fonts", "../datafiles/fonts/");
	rp->setResourceGroupDirectory("layouts", "../datafiles/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "../datafiles/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "../datafiles/lua_scripts/");
#if defined(CEGUI_WITH_XERCES) && (CEGUI_DEFAULT_XMLPARSER == XercesParser)
	rp->setResourceGroupDirectory("schemas", "../../XMLRefSchema/");
#endif

	// set the default resource groups to be used
	CEGUI::Imageset::setDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
#ifdef CEGUI_WITH_XERCES
	CEGUI::XercesParser::setSchemaDefaultResourceGroup("schemas");
#endif

	CEGUI::Imageset* taharezImages = CEGUI::ImagesetManager::getSingleton().createImageset("TaharezLook.imageset");
	CEGUI::System::getSingleton().setDefaultMouseCursor(&taharezImages->getImage("MouseArrow"));
	CEGUI::FontManager::getSingleton().createFont("fkp-16.font");

	CEGUI::WidgetLookManager::getSingleton().parseLookNFeelSpecification("TaharezLook.looknfeel");
	CEGUI::SchemeManager::getSingleton().loadScheme("/TaharezLookWidgets.scheme");

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::DefaultWindow* root = (CEGUI::DefaultWindow*)winMgr.createWindow("DefaultWindow", "Root");
	CEGUI::System::getSingleton().setGUISheet(root);
// ---------- END CEGUI INITIALIZATION ----------


// ---------- CEGUI OBJECT DEFINITIONS ----------
	wnd[0] = winMgr.loadWindowLayout("CharacterCreationSheet.layout");
	wnd[1] = winMgr.loadWindowLayout("CharacterCreationSheet_VirtuesFlaws.layout");

	root->addChildWindow(wnd[0]);
	wnd[0]->setVisible(false);
	root->addChildWindow(wnd[1]);
	wnd[1]->setVisible(false);

	for(int i = 0; i < 2; i++)
	{
		std::string prefix;
		if(i == 0)
			prefix = "CharacterCreationSheet/";
		else if(i == 1)
			prefix = "CharacterCreationSheet_VirtuesFlaws/";
		CEGUI::Window* navigation = winMgr.loadWindowLayout("Navigation.layout", prefix);
		if(i == 1)
			navigation->setProperty("UnifiedAreaRect", "{{0,20},{0,500},{0,400},{0,520}}");
		else if(i == 0)
			navigation->setProperty("UnifiedAreaRect", "{{0,20},{0,460},{0,400},{0,500}}");

		wnd[i]->addChildWindow(navigation);
	}

	for(int i = 0; i < MAX_CHARACTERISTICS; i++) {
		CEGUI::Window* characteristic = winMgr.loadWindowLayout("CharacterCreationSheet_CharacteristicEntry.layout", "CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/");
		std::ostringstream os;
		os << "{{0,20},{0," << (100 + i * 30) << "},{0,400},{0," << (120 + i*30) << "}}";
		characteristic->setProperty("UnifiedAreaRect", os.str());
		wnd[0]->addChildWindow(characteristic);
		CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/Entry/Label")->setProperty("Text", Character::characteristicLabel[i] + " :");
	}

	CEGUI::Window* points = winMgr.loadWindowLayout("VirtuesFlawsPoints.layout", "CharacterCreationSheet_VirtuesFlaws/");
	points->setProperty("UnifiedAreaRect", "{{0,20},{0,400},{0,400},{0,450}}");
	wnd[1]->addChildWindow(points);

	CEGUI::Window* list_box = winMgr.loadWindowLayout("CharacterCreationSheet_VF_Listbox.layout", "CharacterCreationSheet_VirtuesFlaws/");
	list_box->setProperty("UnifiedAreaRect", "{{0,20},{0,50},{0,400},{0,350}}");
	for(int i = MAX_VIRTUESFLAWS; i--;)
	{
		std::ostringstream os;
		os << Character::virtueflawLabel[i] << " (" << Character::virtueData[i].value << ")";
		CEGUI::ListboxItem* item = new CEGUI::ListboxTextItem(os.str());
		item->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
		((CEGUI::Listbox*)list_box)->insertItem(item, NULL);

	}
	wnd[1]->addChildWindow(list_box);

/*
	STATUS_VIRTUE_CATEGORY,
	PHYSICAL_VIRTUE_CATEGORY,
	TRAITS_VIRTUE_CATEGORY,
	STATS_VIRTUE_CATEGORY,
	SOCIAL_VIRTUE_CATEGORY,
*/
//	for(int i = STATUS_VIRTUE_CATEGORY; i < MAX_VIRTUE_CATEGORIES; i++)
//	for(int i = 1; i < MAX_VIRTUE_CATEGORIES; i++) {
/*	wnd[STATUS_VIRTUE_CATEGORY]->setProperty("Text", "Character creation (status)");
	wnd[PHYSICAL_VIRTUE_CATEGORY]->setProperty("Text", "Character creation (physical)");
	wnd[TRAITS_VIRTUE_CATEGORY]->setProperty("Text", "Character creation (traits)");
	wnd[STATS_VIRTUE_CATEGORY]->setProperty("Text", "Character creation (stats)");
	wnd[SOCIAL_VIRTUE_CATEGORY]->setProperty("Text", "Character creation (social)");*/

//    text = winMgr.createWindow("TaharezLook/StaticText", "Speed");
//    wnd->addChildWindow(text);
/*    text->setProperty("FrameEnabled", "false");
    text->setProperty("BackgroundEnabled", "false");
    text->setText("Current Speed:");
*/

	
//	tooltip = new CEGUI::Tooltip("TaharezLook/Tooltip", "default_tooltip");
	CEGUI::System::getSingleton().setDefaultTooltip("TaharezLook/Tooltip");
	CEGUI::System::getSingleton().getDefaultTooltip()->setDisplayTime(0.0f);
	CEGUI::System::getSingleton().getDefaultTooltip()->setFadeTime(0.4f);
	CEGUI::System::getSingleton().getDefaultTooltip()->setAlwaysOnTop(true);
	CEGUI::System::getSingleton().getDefaultTooltip()->setVisible(true);
	for(int i = 0; i < MAX_CHARACTERISTICS; i++)
	{
//		static std::string characteristicValueDescription[13];
		CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/Entry/Label")->setTooltipText(Character::characteristicTooltip[i]);
		CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/Entry/Value")->setTooltipText(Character::characteristicValueTooltip[i][0]);
		CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/Entry/AddButton")->setTooltipText(character_creation->getIncreaseCostsString((eCharacteristic)i));
		CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/Entry/SubtractButton")->setTooltipText(character_creation->getDecreaseCostsString((eCharacteristic)i));

	    CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/Entry/AddButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::handleSubmit, this));
		CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/Entry/SubtractButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::handleSubmit, this));
	}
	CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/Name/Label")->setTooltipText("Enter the name of your character");
	CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/Name/Value")->setTooltipText("Enter the name of your character");

	CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/Navigation/NextPage")->
			subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::handleSubmit, this));
	CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/Navigation/PreviousPage")->
			subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::handleSubmit, this));
	CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet_VirtuesFlaws/Navigation/NextPage")->
			subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::handleSubmit, this));
	CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet_VirtuesFlaws/Navigation/PreviousPage")->
			subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::handleSubmit, this));

	CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet_VirtuesFlaws/Listbox")->
			subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::handleSubmit, this));


//	stamina_bar = winMgr.createWindow("", "Stamina");
// ---------- END CEGUI OBJECT DEFINITION ----------
}

GUI::~GUI() {
}

void GUI::injectTimePulse(float time_elapsed) {
	CEGUI::System::getSingleton().injectTimePulse(time_elapsed);
}

void GUI::injectMouseMove(int x, int y) {
	CEGUI::System::getSingleton().injectMouseMove(x, y);
}

void GUI::injectChar(int key_code) {
//	GUI::System::getSingleton().injectChar(my_char);
// show / hide
}

const bool GUI::isCharacterMenuShown() const {
	return (wnd[0]->isVisible() || wnd[1]->isVisible());
}

void GUI::injectMouseButton(int retButton1, int retButton2, int retButton3, int oldRetButton1, int oldRetButton2, int oldRetButton3) {
	if(retButton1!=0 && oldRetButton1==0) CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
	if(retButton2!=0 && oldRetButton2==0) CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton::MiddleButton);
	if(retButton3!=0 && oldRetButton3==0) CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton::RightButton);
	if(retButton1==0 && oldRetButton1!=0) CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
	if(retButton2==0 && oldRetButton2!=0) CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MouseButton::MiddleButton);
	if(retButton3==0 && oldRetButton3!=0) CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MouseButton::RightButton);
}

void GUI::renderGUI() {
	CEGUI::System::getSingleton().renderGUI();
	d3Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}


bool GUI::handleSubmit(const CEGUI::EventArgs& event) {
    const CEGUI::WindowEventArgs* evt = static_cast<const CEGUI::WindowEventArgs*>(&event);
    CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(evt->window);
	std::string t = button->getName().c_str();
	int item = 0;
	bool found = false;
	for(int i = 0; i < MAX_CHARACTERISTICS; i++) {
		if(t == "CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/Entry/AddButton") {
			character_creation->increase((eCharacteristic)i);
			item = i;
			found = true;
			break;
		}
		else
		if(t == "CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/Entry/SubtractButton") {
			character_creation->decrease((eCharacteristic)i);
			item = i;
			found = true;
			break;
		}
	}
	if(found) {
		CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[item] + "/Entry/Value")->setText(Character::characteristicValueDescription[EXTREME_CHARACTERISTIC_VALUE + character_creation->getValue((eCharacteristic)item)]);
		std::ostringstream os;
		os << character_creation->getRemainingCharacteristicPoints();
		CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/Points/Value")->setText(os.str());
		CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[item] + "/Entry/AddButton")->setTooltipText(character_creation->getIncreaseCostsString((eCharacteristic)item));
		CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[item] + "/Entry/SubtractButton")->setTooltipText(character_creation->getDecreaseCostsString((eCharacteristic)item));

		updateCharacteristicSheet();
		return true;
	}

	if( t == "CharacterCreationSheet/Navigation/NextPage" ) {
		wnd[0]->hide();
		wnd[1]->show();
	} else
	if( t == "CharacterCreationSheet/Navigation/PreviousPage" ) {
		wnd[0]->hide();
		wnd[1]->show();
	} else
	if( t == "CharacterCreationSheet_VirtuesFlaws/Navigation/NextPage" ) {
		wnd[1]->hide();
		wnd[0]->show();
	} else
	if( t == "CharacterCreationSheet_VirtuesFlaws/Navigation/PreviousPage" ) {
		wnd[1]->hide();
		wnd[0]->show();
	}  
	return false;
}

void GUI::updateCharacteristicSheet() {
	const bool* possinc = character_creation->getCurrentIncreasePossibilities();
	const bool* possdec = character_creation->getCurrentDecreasePossibilities();
	for(int i = 0; i < MAX_CHARACTERISTICS; i++) {
		if(!possinc[i]) {
			CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/Entry/AddButton")->setEnabled(false);
		} else {
			CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/Entry/AddButton")->setEnabled(true);
		}
		if(!possdec[i]) {
			CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/Entry/SubtractButton")->setEnabled(false);
		} else {
			CEGUI::WindowManager::getSingleton().getWindow("CharacterCreationSheet/" + Character::characteristicIdentifier[i] + "/Entry/SubtractButton")->setEnabled(true);
		}
	}
}

bool GUI::show() {
	if(wnd[0]->isVisible() || wnd[1]->isVisible()) {
		wnd[0]->hide();
		wnd[1]->hide();
		return false;
	}
	else 
	{
		wnd[0]->show();
		return true;
	}
}