#include "Geode/binding/GJUserScore.hpp"
#include "Geode/cocos/base_nodes/Layout.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/EditLevelLayer.hpp>

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
		CCNode* menus[] = {
			this->getChildByID("creator-buttons-menu"),
			this->getChildByID("exit-menu"),
			this->getChildByID("bottom-left-menu"),
			this->getChildByID("top-right-menu"),
			this->getChildByID("bottom-right-menu")
		};

		menus[0]->setContentWidth(500);
		
		
		for(auto menu : menus) {
			shuffle(menu);
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

		CCNode* menus[] = {
			this->getChildByID("center-button-menu"),
			this->getChildByID("left-button-menu"),
			this->getChildByID("right-button-menu"),
			this->getChildByID("bottom-button-menu")
		};
		auto centerButtonMenuLayout = RowLayout::create();
		centerButtonMenuLayout->setGap(15.0f);
		menus[0]->setLayout(centerButtonMenuLayout);
		menus[0]->setContentWidth(500.0f);

		for(auto menu : menus) {
			shuffle(menu);
		}
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