#include "Geode/binding/EditorUI.hpp"
#include "Geode/binding/GJUserScore.hpp"
#include "Geode/binding/LevelEditorLayer.hpp"
// #include "Geode/ui/Layout.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

void shuffle(CCNode* menu) {
	int childrenCount = menu->getChildrenCount();
	if (childrenCount <= 1) return;
	log::debug("shuffling menu: {}", menu->getID());
	CCNode* shuffledChildrenBuffer[childrenCount];

	for(int i=0; childrenCount > 0; i++, childrenCount--) {
		auto child = static_cast<CCNode*>(menu->getChildren()->objectAtIndex(rand()%childrenCount));
		// log::debug("Children: {}",childrenCount);
		shuffledChildrenBuffer[i] = child;
		menu->removeChild(child);
	}
	
	for(auto child : shuffledChildrenBuffer) {
		menu->addChild(child,0);
	}
	
	menu->updateLayout();
}

void shuffleRaw(CCNode* menu) {
	int childrenCount = menu->getChildrenCount();
	if (childrenCount <= 1) return;
	log::debug("rawshuffling menu: {}", menu->getID());
	auto children = menu->getChildren();

	for(int i=0; i<childrenCount; i++) {
		auto child1 = static_cast<CCNode*>(children->objectAtIndex(i));
		auto child2 = static_cast<CCNode*>(children->objectAtIndex(rand()%(childrenCount-i)+i));
		auto temp = child2->getPosition();
		child2->setPosition(child1->getPosition());
		child1->setPosition(temp);
	}
}

class $modify(HookedMenuLayer, MenuLayer) {
	static void onModify(auto& self) {
		if (!self.setHookPriority("MenuLayer::init", -100)) {
			log::warn("Failed to set hook priority.");
		}
	}

	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}
		CCNode* menus[] = {
			this->getChildByID("bottom-menu"),
			this->getChildByID("main-menu"),
			this->getChildByID("side-menu"),
			this->getChildByID("right-side-menu"),
			this->getChildByID("top-right-menu"),
			this->getChildByID("profile-menu"),
			this->getChildByID("more-games-menu"),
			this->getChildByID("close-menu")
		};
		
		for(auto menu : menus) {
			shuffle(menu);
		}
		return true;
	}
};

class $modify(HookedCreatorLayer, CreatorLayer) {
	static void onModify(auto& self) {
		if (!self.setHookPriority("CreatorLayer::init", -100)) {
			log::warn("Failed to set hook priority.");
		}
	}

	bool init() {
		if (!CreatorLayer::init()) {
			return false;
		}
		auto creatorButtons = this->getChildByID("creator-buttons-menu");

		CCNode* menus[] = {
			this->getChildByID("exit-menu"),
			this->getChildByID("bottom-left-menu"),
			this->getChildByID("top-right-menu"),
			this->getChildByID("bottom-right-menu")
		};

		// menus[0]->setContentWidth(500);
		
		
		for(auto menu : menus) {
			shuffle(menu);
		}
		if(Loader::get()->isModLoaded("alphalaneous.vanilla_pages")) {
			shuffle(creatorButtons);
		}
		else {
			shuffleRaw(creatorButtons);
		}
		return true;
	}
};

class $modify(HookedGJGarageLayer, GJGarageLayer) {
	static void onModify(auto& self) {
		if (!self.setHookPriority("GJGarageLayer::init", -100)) {
			log::warn("Failed to set hook priority.");
		}
	}

	bool init() {
		if (!GJGarageLayer::init()) {
			return false;
		}
		CCNode* menus[] = {
			this->getChildByID("back-menu"),
			this->getChildByID("shards-menu"),
			this->getChildByID("prev-page-menu"),
			this->getChildByID("next-page-menu"),
			this->getChildByID("category-menu")
		};
		
		for(auto menu : menus) {
			shuffle(menu);
		}
		return true;
	}
};

class $modify(HookedPauseLayer, PauseLayer) {
	static void onModify(auto& self) {
		if (!self.setHookPriority("PauseLayer::customSetup", -100)) {
			log::warn("Failed to set hook priority.");
		}
	}

	virtual void customSetup() {
		PauseLayer::customSetup();
		
		CCNode* menusRaw[] = {
			this->getChildByID("center-button-menu"),
		};

		CCNode* menus[] = {
			this->getChildByID("left-button-menu"),
			this->getChildByID("right-button-menu"),
			this->getChildByID("bottom-button-menu")
		};

		for(auto menu : menusRaw) shuffleRaw(menu);
		for(auto menu : menus) shuffle(menu);
	}
};

class $modify(HookedProfilePage, ProfilePage) {
	static void onModify(auto& self) {
		if (!self.setHookPriority("ProfilePage::getUserInfoFinished", -100)) {
			log::warn("Failed to set hook priority.");
		}
	}

	virtual void getUserInfoFinished(GJUserScore* p0) {
		ProfilePage::getUserInfoFinished(p0);
		auto layer=static_cast<CCNode*>(this->getChildren()->objectAtIndex(0));
		CCNode* menus[] = {
			layer->getChildByID("left-menu"),
			layer->getChildByID("socials-menu"),
			layer->getChildByID("player-menu"),
			layer->getChildByID("bottom-menu"),
			layer->getChildByID("username-menu")
		};
		
		log::debug("Shuffling ProfilePage");
		for(auto menu : menus) {
			shuffle(menu);
		}
	}
};

class $modify(HookedEditLevelLayer, EditLevelLayer) {
	static void onModify(auto& self) {
		if (!self.setHookPriority("EditLevelLayer::init", -100)) {
			log::warn("Failed to set hook priority.");
		}
	}

	bool init(GJGameLevel* p0) {
		if (!EditLevelLayer::init(p0)) {
			return false;
		}
		CCNode* menus[] = {
			this->getChildByID("level-edit-menu"),
			this->getChildByID("description-menu"),
			this->getChildByID("folder-menu"),
			this->getChildByID("back-menu"),
			this->getChildByID("level-actions-menu")
		};
		
		for(auto menu : menus) {
			shuffle(menu);
		}
		return true;
	}
};

class $modify(HookedLevelSearchLayer, LevelSearchLayer) {
	static void onModify(auto& self) {
		if (!self.setHookPriority("LevelSearchLayer::init", -100)) {
			log::warn("Failed to set hook priority.");
		}
	}

	bool init(int p0) {
		if (!LevelSearchLayer::init(p0)) return false;
	
		CCNode* menusRaw[] = {
			this->getChildByID("quick-search-menu"),
			this->getChildByID("exit-menu"),
			this->getChildByID("difficulty-filter-menu"),
			this->getChildByID("length-filter-menu"),
			this->getChildByID("search-button-menu")
		};
		auto protectedChild = menusRaw[2]->getChildByID("demon-type-filter-button");
		menusRaw[2]->removeChildByID("demon-type-filter-button");

		CCNode* menus[] = {
			this->getChildByID("other-filter-menu"),
			this->getChildByID("bottom-left-menu")
		};

		for(auto menu : menusRaw) shuffleRaw(menu);
		for(auto menu : menus) shuffle(menu);

		menusRaw[2]->addChild(protectedChild);

		return true;
	}
};

class $modify(HookedEditorUI, EditorUI) {
	static void onModify(auto& self) {
		if (!self.setHookPriority("EditorUI::init", -100)) {
			log::warn("Failed to set hook priority.");
		}
	}

	bool init(LevelEditorLayer* editorLayer) {
		if (!EditorUI::init(editorLayer)) return false;

		auto deleteMenus = this->getChildByID("delete-category-menu");

		CCNode* menus[] = {
			this->getChildByID("toolbar-categories-menu"),
			deleteMenus->getChildByID("delete-button-menu"),
			deleteMenus->getChildByID("delete-filter-menu"),
			this->getChildByID("toolbar-toggles-menu"),
			this->getChildByID("undo-menu"),
			this->getChildByID("playback-menu"),
			this->getChildByID("playtest-menu"),
			this->getChildByID("zoom-menu"),
			this->getChildByID("link-menu"),
			this->getChildByID("settings-menu"),
			this->getChildByID("editor-buttons-menu"),
			this->getChildByID("layer-menu"),
			this->getChildByID("build-tabs-menu"),
		};

		for(auto menu : menus) shuffle(menu);

		return true;
	}
};

// TODO: shuffle coin order
class $modify(HookedLevelInfoLayer, LevelInfoLayer) {
	static void onModify(auto& self) {
		if (!self.setHookPriority("LevelInfoLayer::init", -100)) {
			log::warn("Failed to set hook priority.");
		}
	}

	bool init(GJGameLevel* level, bool challenge) {
		if (!LevelInfoLayer::init(level, challenge)) return false;

		CCNode* menusRaw[] = {
			this->getChildByID("other-menu"),
			this->getChildByID("settings-menu"),
			this->getChildByID("play-menu"),
			this->getChildByID("garage-menu")
		};

		CCNode* menus[] = {
			this->getChildByID("right-side-menu"),
			this->getChildByID("creator-info-menu"),
			this->getChildByID("left-side-menu"),
		};
		
		for(auto menu : menusRaw) shuffleRaw(menu);
		for(auto menu : menus) shuffle(menu);

		return true;
	}
};