#include <Geode/Bindings.hpp>
#include <Geode/modify/DemonFilterSelectLayer.hpp>
#include "ListManager.h"

class $modify(GrdDemonFilterSelectLayer, DemonFilterSelectLayer) {
    struct Fields {
        CCSprite* m_demon0Spr = nullptr;
        CCSprite* m_demon1Spr = nullptr;
        CCSprite* m_demon2Spr = nullptr;
        CCSprite* m_demon3Spr = nullptr;
        CCSprite* m_demon4Spr = nullptr;
        CCSprite* m_demon5Spr = nullptr;
    };

    static void onModify(auto& self) {
        self.setHookPriority("DemonFilterSelectLayer::init", -1);
    }

    bool init() {
        if (!DemonFilterSelectLayer::init()) return false;

        if (ListManager::demonIDList.empty()) return true;

        CCLayer* layer = nullptr;

        for (auto child : this->getChildren()) {
            if (auto newObj = dynamic_cast<CCLayer*>(child)) {
                layer = newObj;
                break;
            }
        }

        if (!layer) return false;

        handleTouchPriority(this); // Ensure touch priority is handled after getting `layer`

        CCScale9Sprite* s9spr = nullptr;
        CCLabelBMFont* label = nullptr;
        CCMenu* menu = nullptr;

        for (auto child : layer->getChildren()) {
            if (auto newObj = dynamic_cast<CCScale9Sprite*>(child)) {
                s9spr = newObj;
            } else if (auto newObj2 = dynamic_cast<CCLabelBMFont*>(child)) {
                label = newObj2;
            } else if (auto newObj3 = dynamic_cast<CCMenu*>(child)) {
                menu = newObj3;
            }
        }

        if (!s9spr || !label || !menu) return false;

        // Find OK Button
        CCMenuItemSpriteExtra* okButton = nullptr;
        for (auto child : menu->getChildren()) {
            if (auto newObj = dynamic_cast<CCMenuItemSpriteExtra*>(child)) {
                if (newObj->getPositionY() < 0) {
                    okButton = newObj;
                    break;
                }
            }
        }

        if (!okButton) return false;

        auto csize = s9spr->getContentSize();
        s9spr->setContentSize({csize.width, csize.height * 1.5f});
        auto newcsize = s9spr->getContentSize();
        label->setPositionY(newcsize.height);
        label->setScale(0.8f);
        menu->setPositionY(newcsize.height * 0.82f);
        okButton->setPositionY(-160.f);

        // Create new menu
        auto newMenu = CCMenu::create();
        layer->addChild(newMenu);
        newMenu->setPosition({layer->getContentSize().width / 2 - 3, layer->getContentSize().height * 0.38f});
        newMenu->setLayout(RowLayout::create()
            ->setGap(-1.f)
            ->setAxisAlignment(AxisAlignment::Center));

        auto instantSearch = CCLabelBMFont::create("Instant Search", "bigFont.fnt");
        instantSearch->setPosition({label->getPositionX(), layer->getContentSize().height * 0.52f});
        instantSearch->setScale(0.8f);
        layer->addChild(instantSearch);

        // Create buttons and sprites
        createDemonButton(newMenu, &this->m_fields->m_demon0Spr, "GrD_demon0_text.png", menu_selector(GrdDemonFilterSelectLayer::onButton0));
        createDemonButton(newMenu, &this->m_fields->m_demon1Spr, "GrD_demon1_text.png", menu_selector(GrdDemonFilterSelectLayer::onButton1));
        createDemonButton(newMenu, &this->m_fields->m_demon2Spr, "GrD_demon2_text.png", menu_selector(GrdDemonFilterSelectLayer::onButton2));
        createDemonButton(newMenu, &this->m_fields->m_demon3Spr, "GrD_demon3_text.png", menu_selector(GrdDemonFilterSelectLayer::onButton3));
        createDemonButton(newMenu, &this->m_fields->m_demon4Spr, "GrD_demon4_text.png", menu_selector(GrdDemonFilterSelectLayer::onButton4));

        if (!(Mod::get()->getSettingValue<bool>("grandpa-demon-disable"))) {
            createDemonButton(newMenu, &this->m_fields->m_demon5Spr, "GrD_demon5_text.png", menu_selector(GrdDemonFilterSelectLayer::onButton5));
        }

        newMenu->updateLayout();

        return true;
    }

    void createDemonButton(CCMenu* menu, CCSprite** sprite, const char* textureName, SEL_MenuHandler handler) {
        *sprite = CCSprite::createWithSpriteFrameName(textureName);
        if (*sprite) {
            auto button = CCMenuItemSpriteExtra::create(*sprite, this, handler);
            menu->addChild(button);
        }
    }

    void onButton0(CCObject* sender) { handleButtonClick(0, 349, 249); } // should go to 499
    void onButton1(CCObject* sender) { handleButtonClick(1, 249, 149); }
    void onButton2(CCObject* sender) { handleButtonClick(2, 149, 74); }
    void onButton3(CCObject* sender) { handleButtonClick(3, 74, 24); }
    void onButton4(CCObject* sender) { handleButtonClick(4, 24, 0); }
    void onButton5(CCObject* sender) { handleButtonClick(5, 0, 0); }

    void handleButtonClick(int filterType, int upper, int lower) {
        if (ListManager::demonIDList.empty()) return;
        if (upper >= ListManager::demonIDList.size()) upper = ListManager::demonIDList.size() - 1;
        if (lower < 0) lower = 0;
        if (lower > upper) return;

        ListManager::filterType = filterType;
        ListManager::isSupremeSearching = (filterType == 0);
        auto browserLayer = LevelBrowserLayer::create(ListManager::getSearchObject(upper, lower));
        geode::cocos::switchToScene(browserLayer);
    }
};
