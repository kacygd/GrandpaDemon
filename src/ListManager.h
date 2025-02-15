/// --- File: ListManager.h ---
#ifndef LISTMANAGER_H
#define LISTMANAGER_H
#include <vector>
#include <string>
#include <sstream>

using namespace geode::prelude;

class ListManager {
public:
    inline static std::vector<int> demonIDList;
    inline static bool firstTimeOpen;
    inline static int filterType;
    inline static bool isSupremeSearching;

    inline static void parseRequestString(std::string str) {
        size_t isFound = str.find("_id");
        while (isFound != std::string::npos) {
            str = str.substr(isFound + 5);
            size_t findBracket = str.find("}");
            if (findBracket == std::string::npos) break;
            try {
                int id = std::stoi(str.substr(0, findBracket));
                demonIDList.push_back(id);
            } catch (const std::exception&) {
                break;
            }
            isFound = str.find("_id");
        }
    }

    inline static int getPositionOfID(int id) {
        for (unsigned int i = 0; i < demonIDList.size(); i++) {
            if (demonIDList[i] == id) return i;
        }
        return -1;
    }

    inline static CCSprite* getSpriteFromPosition(int pos, bool hasText) {
        if (pos <= 499 && pos > 249) return CCSprite::createWithSpriteFrameName(hasText ? "GrD_demon0_text.png"_spr : "GrD_demon0.png"_spr);
        if (pos <= 249 && pos > 149) return CCSprite::createWithSpriteFrameName(hasText ? "GrD_demon1_text.png"_spr : "GrD_demon1.png"_spr);
        if (pos <= 149 && pos > 74) return CCSprite::createWithSpriteFrameName(hasText ? "GrD_demon2_text.png"_spr : "GrD_demon2.png"_spr);
        if (pos <= 74 && pos > 24) return CCSprite::createWithSpriteFrameName(hasText ? "GrD_demon3_text.png"_spr : "GrD_demon3.png"_spr);
        if (pos <= 24 && pos > 0) return CCSprite::createWithSpriteFrameName(hasText ? "GrD_demon4_text.png"_spr : "GrD_demon4.png"_spr);
        if (pos == 0) return CCSprite::createWithSpriteFrameName(hasText ? "GrD_demon5_text.png"_spr : "GrD_demon5.png"_spr);
        return nullptr;
    }

    inline static GJSearchObject* getSearchObject(int upper, int lower) {
        if (upper > static_cast<int>(demonIDList.size())) upper = demonIDList.size();
        if (lower < 0) lower = 0;
        if (upper <= lower) return nullptr;

        std::stringstream download;
        bool first = true;
        for (int i = upper - 1; i >= lower; i--) {
            if (!first) download << ",";
            download << demonIDList[i];
            first = false;
        }
        download << "&gameVersion=22";
        return GJSearchObject::create(SearchType::Type19, download.str());
    }
};
#endif
