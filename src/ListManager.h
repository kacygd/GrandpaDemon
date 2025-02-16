#ifndef LISTMANAGER_H
#define LISTMANAGER_H
#include <vector>
#include <string>
#include <stdexcept>

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
                } catch (const std::exception& e) {
                    break; // Bỏ qua lỗi chuyển đổi số
                }
                isFound = str.find("_id");
            }
        }

        inline static int getPositionOfID(int id) {
            for (size_t i = 0; i < demonIDList.size(); i++) {
                if (demonIDList[i] == id) return i;
            }
            return -1;
        }

        inline static CCSprite* getSpriteFromPosition(int pos, bool hasText) {
            std::string spriteName;
            if (pos <= 499 && pos > 249) {
                spriteName = hasText ? "GrD_demon0_text.png" : "GrD_demon0.png";
            } else if (pos <= 249 && pos > 149) {
                spriteName = hasText ? "GrD_demon1_text.png" : "GrD_demon1.png";
            } else if (pos <= 149 && pos > 74) {
                spriteName = hasText ? "GrD_demon2_text.png" : "GrD_demon2.png";
            } else if (pos <= 74 && pos > 24) {
                spriteName = hasText ? "GrD_demon3_text.png" : "GrD_demon3.png";
            } else if (pos <= 24 && pos > 0) {
                spriteName = hasText ? "GrD_demon4_text.png" : "GrD_demon4.png";
            } else if (pos == 0) {
                if (Mod::get()->getSettingValue<bool>("grandpa-demon-disable")) {
                    spriteName = hasText ? "GrD_demon4_text.png" : "GrD_demon4.png";
                } else {
                    spriteName = hasText ? "GrD_demon5_text.png" : "GrD_demon5.png";
                }
            } else {
                return nullptr; // Trả về nullptr nếu không khớp điều kiện
            }
            return CCSprite::createWithSpriteFrameName(spriteName.c_str());
        }

        inline static GJSearchObject* getSearchObject(int upper, int lower) {
            std::stringstream download;
            bool first = true;
            if (!(upper == 0 && lower == 0) && upper < demonIDList.size()) {
                for (size_t i = upper; i > lower; i--) {
                    if (!first) download << ",";
                    download << std::to_string(demonIDList[i]);
                    first = false;
                }
            } else if (!demonIDList.empty()) {
                download << std::to_string(demonIDList[0]);
            }
            download << "&gameVersion=22";
            return GJSearchObject::create(SearchType::Type19, download.str());
        }
};

#endif
