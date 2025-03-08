#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/utils/web.hpp>
#include "ListManager.h"
#include <string>
using namespace geode::prelude;

class $modify(MenuLayer) {
    struct Fields {
        EventListener<web::WebTask> m_listener;
    };

    bool init() {
        if (!MenuLayer::init()) return false;

        if (ListManager::firstTimeOpen) {
            return true;
        }

        m_fields->m_listener.bind([] (web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                auto str = res->string().unwrapOr("Failed.");
                if (res->code() != 200 || str == "Failed." || str == "-1") {
                    FLAlertLayer::create("Error", "Could not load data from CPS.\nPlease check your internet connection!", "OK")->show();
                    return;
                }

                ListManager::parseRequestString(str);
                ListManager::firstTimeOpen = true;
                ListManager::filterType = -1;
            }
        });

        auto req = web::WebRequest();
        m_fields->m_listener.setFilter(req.get("https://cps.ps.fhgdps.com/database/pos.json"));

        return true;
    }
};
