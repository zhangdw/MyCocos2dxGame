//
//  AchievementManager.hpp
//  Voxel_Explorer
//
//  Created by 创李 on 15/11/12.
//
//

#ifndef AchievementManager_hpp
#define AchievementManager_hpp
#include "AchieveProperty.hpp"
#include "ChaosNumber.h"
#include "ArchiveManager.h"

const std::string ARCHIVE_ACHIEVEMENT_KEY = "ARCHIVE_ACHIEVEMENT_KEY";
const std::string ARCHIVE_ACHIEVEMENT_ITEM_COMMPLE = "ARCHIVE_ACHIEVEMENT_ITEM_COMMPLE";
const std::string ARCHIVE_ACHIEVEMENT_ITEM_UNLOCK = "ARCHIVE_ACHIEVEMENT_ITEM_UNLOCK";

class AchievementManager:public iArchive {
    
    AchievementManager();
public:
    virtual ~AchievementManager();
    static AchievementManager* getInstance();
    bool loadAchieveData();
    void handleAchievement(eAchievementDetailType achiId);
    void checkAllAchievement();
    void checkKillMonsterAchievement();
    void checkKillBossAchievement();
    void checkRoleDeadAchievement();
    void checkHaveGoldAchievement();
    void checkFoundHideAchievement();
    void checkBoxAchievement();
    void checkAchieveUnlock(AchieveProperty* prop);
    void sortAchieves();
    virtual bool load(const cocos2d::ValueMap& rootNode );
    virtual bool save(cocos2d::ValueMap& rootNode);
   
    const cocos2d::Vector<AchieveProperty*> & getAllAchieves() const{ return m_vAllAchieves;}
    AchieveProperty* getAchievement(eAchievementDetailType type);
protected:
    
    void updateAchieve(AchieveProperty * achieve);
   
private:
    cocos2d::Vector<AchieveProperty*> m_vAllAchieves;
 
 };

#endif /* AchievementManager_hpp */
