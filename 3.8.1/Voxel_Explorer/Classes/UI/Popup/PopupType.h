//
//  PopupType.h
//  Tone_Skipper
//
//  Created by 创李 on 15/9/18.
//
//

#ifndef Tone_Skipper_PopupType_h
#define Tone_Skipper_PopupType_h

enum ePopupType
{
    ePopupInvalid = -1,
    ePopupRole,             //角色弹窗
    ePopupEquipItem,        //装备道具弹窗
    ePopupItem,             //道具弹窗
    ePopupInfo,             //信息弹窗
    ePopupPause,            //暂停弹窗
    ePopupItemShopBuy,      //商店道具购买弹窗
    ePopupItemShopSell,
    ePopupNurse,            //护士弹窗
    ePopupLittleWitch,      //随机事件弹窗
    ePopupOldMan,           //智者
    ePopupGambleShop,           //赌博
    ePopupAlchemyShop,          //炼金
    ePopupMagicShop,        //魔法物品
    ePopupWeaponShop,       //装备物品
    ePopupTask,             //任务
    ePopupDialogue,
    ePopupCount
};
typedef enum {
    TIP_DEFAULT,
    TIP_POSITIVE,
    TIP_NEGATIVE,
    TIP_WARNING,
    TIP_NEUTRAL,
    TIP_QUESTION,
    TIP_EFFECT,
    TIP_BLUE,
    TIP_DODGE,
    TIP_BOLOCK,
    TIP_CRITICAL_STRIKE,

}TipTypes;
#endif
