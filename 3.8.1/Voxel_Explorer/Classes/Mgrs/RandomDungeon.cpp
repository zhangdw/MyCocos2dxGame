//
//  RandomDungeon.cpp
//  Voxel_Explorer
//
//  Created by wang haibo on 15/10/21.
//
//

#include "RandomDungeon.hpp"
#include "UtilityHelper.h"
#include "GameScene.h"
#include "NpcDataManager.hpp"
#include "LevelResourceManager.h"
#include "BaseBoss.hpp"
USING_NS_CC;

DUNGEON_TYPE DT_SelectGroup1[] = { DT_SEWER, DT_PRISON, DT_FANE, DT_MINES, DT_CAVE, DT_TOMB };
DUNGEON_TYPE DT_SelectGroup2[] = { DT_DWARF_CASTLE, DT_MAGA_TOWER, DT_ORC_FORTRESS, DT_ELF_FOREST,DT_TROLL_TEMPLE };
DUNGEON_TYPE DT_SelectGroup3[] = { DT_BEHOLDER_CASTLE, DT_WARP_SPACE, DT_DRAGON_LAIR, DT_LICH_TOMB };

DUNGEON_TYPE DT_SelectAll[] = { DT_SEWER, DT_PRISON, DT_FANE, DT_MINES, DT_CAVE, DT_TOMB, DT_DWARF_CASTLE, DT_MAGA_TOWER, DT_ORC_FORTRESS, DT_ELF_FOREST,DT_TROLL_TEMPLE, DT_BEHOLDER_CASTLE, DT_WARP_SPACE, DT_DRAGON_LAIR, DT_LICH_TOMB };


std::vector<DUNGEON_TYPE> RandomDungeon::SELECT_GROUP1(DT_SelectGroup1, DT_SelectGroup1+ sizeof(DT_SelectGroup1)/sizeof(DUNGEON_TYPE));
std::vector<DUNGEON_TYPE> RandomDungeon::SELECT_GROUP2(DT_SelectGroup2, DT_SelectGroup2+ sizeof(DT_SelectGroup2)/sizeof(DUNGEON_TYPE));
std::vector<DUNGEON_TYPE> RandomDungeon::SELECT_GROUP3(DT_SelectGroup3, DT_SelectGroup3+ sizeof(DT_SelectGroup3)/sizeof(DUNGEON_TYPE));
std::vector<DUNGEON_TYPE> RandomDungeon::SELECT_ALL(DT_SelectAll, DT_SelectAll+ sizeof(DT_SelectAll)/sizeof(DUNGEON_TYPE));

const char* DUNGEON_NAMES[] = {
    "DN_UNKNOWN",
    
    "DN_SEWER",
    "DN_PRISON",
    "DN_FANE",
    "DN_MINES",
    "DN_CAVE",
    "DN_TOMB",
    
    "DN_DWARF_CASTLE",
    "DN_MAGA_TOWER",
    "DN_ORC_FORTRESS",
    "DN_ELF_FOREST",
    "DN_TROLL_TEMPLE",
    
    "DN_BEHOLDER_CASTLE",
    "DN_WARP_SPACE",
    "DN_DRAGON_LAIR",
    "DN_LICH_TOMB",
    
    "DN_MAX"
};
const char* DUNGEON_BOSS_NAMES[] = {
    "DN_BOSS_UNKNOWN",
    
    "DN_BOSS_SEWER",
    "DN_BOSS_PRISON",
    "DN_BOSS_FANE",
    "DN_BOSS_MINES",
    "DN_BOSS_CAVE",
    "DN_BOSS_TOMB",
    
    "DN_BOSS_DWARF_CASTLE",
    "DN_BOSS_MAGA_TOWER",
    "DN_BOSS_ORC_FORTRESS",
    "DN_BOSS_ELF_FOREST",
    "DN_BOSS_TROLL_TEMPLE",
    
    "DN_BOSS_BEHOLDER_CASTLE",
    "DN_BOSS_WARP_SPACE",
    "DN_BOSS_DRAGON_LAIR",
    "DN_BOSS_LICH_TOMB",
    
    "DN_BOSS_MAX"
};

RandomDungeon* g_pRandomDungeonInstance = nullptr;
RandomDungeon* RandomDungeon::getInstance()
{
    if( g_pRandomDungeonInstance == nullptr )
        g_pRandomDungeonInstance = new RandomDungeon();
    
    return g_pRandomDungeonInstance;
}
RandomDungeon::RandomDungeon()
{
    m_pCurrentNode = nullptr;
    m_nCurrentSelectGroup = 0;
    m_nDifficultClass = 1;
}
RandomDungeon::~RandomDungeon()
{
    CC_SAFE_DELETE(m_pCurrentNode->m_pLeftNode);
    CC_SAFE_DELETE(m_pCurrentNode->m_pRightNode);
    CC_SAFE_DELETE(m_pCurrentNode);
}
void RandomDungeon::update(float delta)
{
    NpcDataManager::getInstance()->update(delta);
}
bool RandomDungeon::build()
{
    if(m_pCurrentNode == nullptr)
    {
        DUNGEON_TYPE type = generateDungeonNodeType();
        if(type == DT_UNKNOWN)
            return false;
        m_pCurrentNode = new (std::nothrow) DungeonNode();
        if(!m_pCurrentNode)
            return false;
        m_pCurrentNode->retain();
        m_pCurrentNode->m_Type = type;
        ///for debug
        m_pCurrentNode->m_Type = DT_MINES;
        m_pCurrentNode->m_nNodeDepth = 1;
        assignedDungeonNode(m_pCurrentNode);
        m_pCurrentNode->autorelease();
        generateNextDungeonNode();
    }
    return true;
}
void RandomDungeon::selectDungeonNode(bool left)
{
    if(left)
    {
        CCASSERT(m_pCurrentNode != nullptr && m_pCurrentNode->m_pLeftNode != nullptr, "m_pCurrentNode != nullptr && m_pCurrentNode->m_pLeftNode != nullptr");
        if(m_pCurrentNode->m_pRightNode != nullptr)
        {
            m_UnSelected.push_back(m_pCurrentNode->m_pRightNode->m_Type);
            m_pCurrentNode->m_pRightNode = nullptr;
        }
        m_pCurrentNode = m_pCurrentNode->m_pLeftNode;
        generateNextDungeonNode();
    }
    else
    {
        CCASSERT(m_pCurrentNode != nullptr && m_pCurrentNode->m_pRightNode != nullptr && m_pCurrentNode->m_pLeftNode != nullptr, "m_pCurrentNode != nullptr && m_pCurrentNode->m_pRightNode != nullptr && m_pCurrentNode->m_pLeftNode != nullptr");
        if(m_pCurrentNode->m_pLeftNode != nullptr)
        {
            m_UnSelected.push_back(m_pCurrentNode->m_pLeftNode->m_Type);
            m_pCurrentNode->m_pLeftNode = nullptr;
        }
        m_pCurrentNode = m_pCurrentNode->m_pRightNode;
        generateNextDungeonNode();
    }
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(scene);
}
bool RandomDungeon::generateNextDungeonNode()
{
    if(!m_pCurrentNode)
        return false;
    DUNGEON_TYPE type = generateDungeonNodeType();
    if(type == DT_UNKNOWN)
    {
        type = generateDungeonNodeType();
        if(type == DT_UNKNOWN)
            return false;
    }
    if(m_pCurrentNode->m_pLeftNode == nullptr)
    {
        m_pCurrentNode->m_pLeftNode = new (std::nothrow) DungeonNode();
        if(!m_pCurrentNode->m_pLeftNode)
            return false;
        m_pCurrentNode->m_pLeftNode->retain();
        m_pCurrentNode->m_pLeftNode->m_Type = type;
        m_pCurrentNode->m_pLeftNode->m_nNodeDepth = m_pCurrentNode->m_nNodeDepth + 1;
        assignedDungeonNode(m_pCurrentNode->m_pLeftNode);
        m_pCurrentNode->m_pLeftNode->autorelease();
    }
    type = generateDungeonNodeType();
    if( (type != DT_UNKNOWN) && (m_pCurrentNode->m_pRightNode == nullptr) )
    {
        m_pCurrentNode->m_pRightNode = new (std::nothrow) DungeonNode();
        if(!m_pCurrentNode->m_pRightNode)
            return false;
        m_pCurrentNode->m_pRightNode->retain();
        m_pCurrentNode->m_pRightNode->m_Type = type;
        m_pCurrentNode->m_pRightNode->m_nNodeDepth = m_pCurrentNode->m_nNodeDepth + 1;
        assignedDungeonNode(m_pCurrentNode->m_pRightNode);
        m_pCurrentNode->m_pRightNode->autorelease();
    }
    if(!LevelResourceManager::getInstance()->initLevelRes())
    {
        CCLOGERROR("load level resource failed!");
        return false;
    }
    if(!NpcDataManager::getInstance()->initNpcData())
    {
        CCLOGERROR("load npc data failed!");
        return false;
    }
    return true;
}
DUNGEON_TYPE RandomDungeon::generateDungeonNodeType()
{
    DUNGEON_TYPE ret = DT_UNKNOWN;
    if (m_nCurrentSelectGroup == 0 && m_UnSelected.empty()) {
        m_UnSelected = SELECT_GROUP1;
        m_nCurrentSelectGroup = 1;
    }
    
    if(m_nCurrentSelectGroup == 1)
    {
        if(m_UnSelected.empty())
        {
            m_UnSelected = SELECT_GROUP1;
            m_nCurrentSelectGroup = 1;
            m_nDifficultClass = m_nDifficultClass +1;
            return generateDungeonNodeType();
        }
        else
        {
            int rand = cocos2d::random(0, (int)m_UnSelected.size()-1);
            ret = m_UnSelected[rand];
            m_UnSelected.erase(m_UnSelected.begin() + rand);
        }
    }
    ///此版本只做6关，后续先注释掉
//    if(m_nCurrentSelectGroup == 1)
//    {
//        if(m_UnSelected.empty())
//        {
//            m_UnSelected = SELECT_GROUP2;
//            m_nCurrentSelectGroup = 2;
//            return ret;
//        }
//        else
//        {
//            int rand = cocos2d::random(0, (int)m_UnSelected.size()-1);
//            ret = m_UnSelected[rand];
//            m_UnSelected.erase(m_UnSelected.begin() + rand);
//        }
//    }
//    if(m_nCurrentSelectGroup == 2)
//    {
//        if(m_UnSelected.empty())
//        {
//            m_UnSelected = SELECT_GROUP3;
//            m_nCurrentSelectGroup = 3;
//            return ret;
//        }
//        else
//        {
//            int rand = cocos2d::random(0, (int)m_UnSelected.size()-1);
//            ret = m_UnSelected[rand];
//            m_UnSelected.erase(m_UnSelected.begin() + rand);
//        }
//    }
//    if(m_nCurrentSelectGroup == 3)
//    {
//        if(m_UnSelected.empty())
//        {
//            m_UnSelected = SELECT_ALL;
//            m_nCurrentSelectGroup = 4;
//            m_nDifficultClass = m_nDifficultClass +1;
//            return ret;
//        }
//        else
//        {
//            int rand = cocos2d::random(0, (int)m_UnSelected.size()-1);
//            ret = m_UnSelected[rand];
//            m_UnSelected.erase(m_UnSelected.begin() + rand);
//        }
//    }
//    if(m_nCurrentSelectGroup == 4)
//    {
//        if(m_UnSelected.empty())
//        {
//            m_UnSelected = SELECT_ALL;
//            m_nCurrentSelectGroup = 4;
//            m_nDifficultClass = m_nDifficultClass +1;
//            return generateDungeonNodeType();
//        }
//        else
//        {
//            int rand = cocos2d::random(0, (int)m_UnSelected.size()-1);
//            ret = m_UnSelected[rand];
//            m_UnSelected.erase(m_UnSelected.begin() + rand);
//        }
//    }
    return ret;
}
void RandomDungeon::assignedDungeonNode(DungeonNode* node)
{
    if(!node)
        return;
    node->m_strDungeonName = UtilityHelper::getLocalString(DUNGEON_NAMES[node->m_Type]);
    node->m_strDungeonBossName = UtilityHelper::getLocalString(DUNGEON_BOSS_NAMES[node->m_Type]);
    if (node->m_Type < DT_DWARF_CASTLE)
    {
        node->m_nTotalNum = cocos2d::random(5, 8);
        node->m_nTransmutationDepth = cocos2d::random(3, 5);
    }
    else if(node->m_Type > DT_TROLL_TEMPLE)
    {
        node->m_nTotalNum = cocos2d::random(10, 15);
        node->m_nTransmutationDepth = cocos2d::random(5, 10);
    }
    else
    {
        node->m_nTotalNum = cocos2d::random(8, 12);
        node->m_nTransmutationDepth = cocos2d::random(4, 8);
    }
}
std::string RandomDungeon::getCurrentBossName() const
{
    std::string name;
    DUNGEON_TYPE dungeontype = m_pCurrentNode->m_Type;
    
    if (dungeontype == DUNGEON_TYPE::DT_SEWER)
    {
        name = UtilityHelper::getLocalString(BOSS_MODEL_NAMES[BaseBoss::BossType::BT_SLIMEKING]);
    }else if (dungeontype == DUNGEON_TYPE::DT_PRISON)
    {
        name = UtilityHelper::getLocalString(BOSS_MODEL_NAMES[BaseBoss::BossType::BT_WARDEN]);
    }else if (dungeontype == DUNGEON_TYPE::DT_FANE)
    {
        name = UtilityHelper::getLocalString(BOSS_MODEL_NAMES[BaseBoss::BossType::BT_ARCHBISHOP]);
    }else if (dungeontype == DUNGEON_TYPE::DT_MINES)
    {
        name = UtilityHelper::getLocalString(BOSS_MODEL_NAMES[BaseBoss::BossType::BT_KOBOLDLEADER]);
    }else if (dungeontype == DUNGEON_TYPE::DT_CAVE)
    {
        name = UtilityHelper::getLocalString(BOSS_MODEL_NAMES[BaseBoss::BossType::BT_GIANT]);
    }else if (dungeontype == DUNGEON_TYPE::DT_TOMB)
    {
        name = UtilityHelper::getLocalString(BOSS_MODEL_NAMES[BaseBoss::BossType::BT_SKELETONKING]);
    }
    return name;
}
void RandomDungeon::load()
{
}
void RandomDungeon::save()
{
}