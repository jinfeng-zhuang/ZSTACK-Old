#ifndef HS_ATTR_H
#define HS_ATTR_H

#include "hs.h"

enum hs_attr_e {
    HS_ATTR_CRYSTAL,            // 水晶
    HS_ATTR_SNEER,              // 嘲讽
    HS_ATTR_MAGIC_DAMAGE,       // 法伤

    HS_ATTR_TYPE,
    HS_ATTR_PRIO,
    HS_ATTR_BLOOD,
    HS_ATTR_ATTACK,
    HS_ATTR_COST,

    HS_ATTR_BLOOD_MAX,
    HS_ATTR_BLOOD_CUR,

    HS_ATTR_ATTACK_CUR,

    HS_ATTR_RACE,
    HS_ATTR_REGION,

    HS_ATTR_BATTLE_POS,

    HS_ATTR_MAX
};

enum hs_attr_type_e {
    HS_ATTR_TYPE_RETINUE,
    HS_ATTR_TYPE_MAGIC,
    HS_ATTR_TYPE_HERO,
    HS_ATTR_TYPE_CRYSTAL,
    HS_ATTR_TYPE_TIMER,
    HS_ATTR_TYPE_BATTLE_GROUND,
    HS_ATTR_TYPE_MAX
};

enum hs_attr_region {
    HS_ATTR_REGION_HAND,
    HS_ATTR_REGION_LIB,
    HS_ATTR_REGION_BATTLE,
    HS_ATTR_REGION_DIE,
    HS_ATTR_REGION_TMP,
    HS_ATTR_REGION_MAX
};

// 一些通用接口
// 抽一张牌，同时会发送抽卡事件到队列中
extern struct hs_card *hs_draw_a_card(struct list_head *head);

#endif
