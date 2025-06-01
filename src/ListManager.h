#ifndef LISTMANAGER_H
#define LISTMANAGER_H

#include <vector>
#include <string>
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "json.hpp"

using namespace geode::prelude;
using json = nlohmann::json;

class ListManager {
public:
    inline static std::vector<int> demonIDList;
    inline static bool firstTimeOpen;
    inline static int filterType;
    inline static bool isSupremeSearching;

    inline static void parseRequestString(const std::string& str) {
        demonIDList.clear();

        try {
            json j = json::parse(str);

            if (!j.is_array()) {
                FLAlertLayer::create("Error", "pos.json is not an array!", "OK")->show();
                return;
            }
            for (const auto& item : j) {
                if (!item.is_object() || !item.contains("level_id")) {
                    FLAlertLayer::create("Error", "Invalid item in pos.json!", "OK")->show();
                    return;
                }

                auto level_id = item["level_id"];
                if (!level_id.is_number_integer()) {
                    FLAlertLayer::create("Error", "level_id must be an integer!", "OK")->show();
                    return;
                }
                demonIDList.push_back(level_id.get<int>());
            }
            if (demonIDList.empty()) {
                FLAlertLayer::create("Error", "No levels found in pos.json!", "OK")->show();
            }
        } catch (const json::exception& e) {
            FLAlertLayer::create("Error", "Failed to parse pos.json! " + std::string(e.what()), "OK")->show();
            return;
        }
    }

    inline static int getPositionOfID(int id) {
        for (unsigned int i = 0; i < demonIDList.size(); i++) {
            if (demonIDList.at(i) == id) return i;
        }
        return -1;
    }

    inline static CCSprite* getSpriteFromPosition(int pos, bool hasText) {
        if (pos <= 35 && pos >= 24) {
            return hasText ? CCSprite::createWithSpriteFrameName("GrD_demon0_text.png"_spr)
                           : CCSprite::createWithSpriteFrameName("GrD_demon0.png"_spr);
        }
        if (pos <= 23 && pos >= 16) {
            return hasText ? CCSprite::createWithSpriteFrameName("GrD_demon1_text.png"_spr)
                           : CCSprite::createWithSpriteFrameName("GrD_demon1.png"_spr);
        }
        if (pos <= 15 && pos >= 8) {
            return hasText ? CCSprite::createWithSpriteFrameName("GrD_demon2_text.png"_spr)
                           : CCSprite::createWithSpriteFrameName("GrD_demon2.png"_spr);
        }
        if (pos <= 7 && pos >= 4) {
            return hasText ? CCSprite::createWithSpriteFrameName("GrD_demon3_text.png"_spr)
                           : CCSprite::createWithSpriteFrameName("GrD_demon3.png"_spr);
        }
        if (pos <= 3 && pos > 0) {
            return hasText ? CCSprite::createWithSpriteFrameName("GrD_demon4_text.png"_spr)
                           : CCSprite::createWithSpriteFrameName("GrD_demon4.png"_spr);
        }
        if (pos == 0) {
            if (Mod::get()->getSettingValue<bool>("grandpa-demon-disable")) {
                return hasText ? CCSprite::createWithSpriteFrameName("GrD_demon4_text.png"_spr)
                               : CCSprite::createWithSpriteFrameName("GrD_demon4.png"_spr);
            } else {
                return hasText ? CCSprite::createWithSpriteFrameName("GrD_demon5_text.png"_spr)
                               : CCSprite::createWithSpriteFrameName("GrD_demon5.png"_spr);
            }
        }
        return CCSprite::create();
    }

    inline static GJSearchObject* getSearchObject(int upper, int lower) {
        std::stringstream download;
        bool first = true;
        if (!(upper == 0 && lower == 0)) {
            for (unsigned int i = upper; i > lower; i--) {
                if (!first) {
                    download << ",";
                }
                if (i < demonIDList.size()) {
                    download << std::to_string(demonIDList.at(i));
                }
                first = false;
            }
        } else {
            if (!demonIDList.empty()) {
                download << std::to_string(demonIDList.at(0));
            }
        }

        download << "&gameVersion=22";
        return GJSearchObject::create(SearchType::Type19, download.str());
    }
};

#endif
