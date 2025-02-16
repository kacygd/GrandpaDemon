#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include "ListManager.h"

class $modify(GrDLevelBrowserLayer, LevelBrowserLayer) {

    struct Fields {
        int m_currentPage = 0;
        int m_furthestLoadedPage = 0;
        int m_lowIdx = 0;
    };

    bool init(GJSearchObject* p0) {
        if (!ListManager::isSupremeSearching) {
            return LevelBrowserLayer::init(p0);
        }

        if (!p0 || p0->m_searchType != SearchType::Type19) {
            return LevelBrowserLayer::init(p0);
        }

        this->m_fields->m_currentPage = 0;
        this->m_fields->m_lowIdx = 0;

        auto searchObj = ListManager::getSearchObject(499, 489);
        if (!searchObj) return false;

        return LevelBrowserLayer::init(searchObj);
    }

    void loadLevelsFinished(cocos2d::CCArray* p0, char const* p1, int p2) {
        LevelBrowserLayer::loadLevelsFinished(p0, p1, p2);
        if (!ListManager::isSupremeSearching || !this->m_searchObject) {
            return;
        }
        if (this->m_searchObject->m_searchType != SearchType::Type19) {
            return;
        }

        auto prevBtn = this->m_leftArrow;
        auto nextBtn = this->m_rightArrow;

        hideStuff();

        prevBtn->setVisible(true);
        nextBtn->setVisible(true);

        if (this->m_fields->m_currentPage <= 0) {
            prevBtn->setVisible(false);
        } else if (this->m_fields->m_currentPage >= 24) {
            nextBtn->setVisible(false);
        }
    }

    void onNextPage(CCObject* sender) {
        LevelBrowserLayer::onNextPage(sender);
        if (!ListManager::isSupremeSearching || !this->m_searchObject) {
            return;
        }
        if (this->m_searchObject->m_searchType != SearchType::Type19) {
            return;
        }

        if (this->m_fields->m_currentPage < 24) {
            this->m_fields->m_currentPage += 1;
        }
        nextBtnActions();
    }

    void onPrevPage(CCObject* sender) {
        LevelBrowserLayer::onPrevPage(sender);
        if (!ListManager::isSupremeSearching || !this->m_searchObject) {
            return;
        }
        if (this->m_searchObject->m_searchType != SearchType::Type19) {
            return;
        }

        if (this->m_fields->m_currentPage > 0) {
            this->m_fields->m_currentPage -= 1;
        }
        nextBtnActions();
    }

    void nextBtnActions() {
        hideStuff();
        auto searchObj = ListManager::getSearchObject(499 - this->m_fields->m_currentPage * 10, 489 - this->m_fields->m_currentPage * 10);
        if (searchObj) {
            LevelBrowserLayer::loadPage(searchObj);
        }
    }

    void hideStuff() {
        if (this->m_pageBtn) this->m_pageBtn->setVisible(false);
        if (this->m_countText) {
            this->m_countText->setString(fmt::format("{} to {} of 250", 
                this->m_fields->m_currentPage * 10 + 1, 
                this->m_fields->m_currentPage * 10 + 10).c_str()
            );
        }
    }
};
