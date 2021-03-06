#include "main.h"

// 为什么会执行到这里，因为以 health 作为 key 注册的，那么肯定 evt 里面的角色 health 是将要发生变化的
void HS_FUNC_SNAPJAW_SHELLFIGHTER(struct hs_card *card, struct hs_event *evt)
{
    struct list_head *card_pos, *event_pos;
    struct hs_card *tmp_card;
    //struct hs_event *evt;
    int ret;
    int left, right, pos;

    // 判断 HEALTH 是否大于 0 ，如果是，判断左右 ATTR.HEALTH 是否发生减少，如果有
    // 移除 evt 中的该随从，改为该随从
    //

    // 从 evt dst 中搜索，位置在左侧的卡牌
    // 该 API 支持在搜索结果的基础上搜索，就是第一个参数填入上一次的搜索结果
    // tmp_card = hs_search(evt->dst, HS_ATTR_BATTLE_POS, HS_OPERATION_EQUAL, left)
    // if (card)
    //   // 替换成该随从，evt 中允许出现多次相同的随从
    //  hs_replace(evt->dst, tmp_card, card);
    //
    // API 需要简化
    // CALLBACK 只能修改 evt 中的信息，属性更改要交给系统，比如抽牌也是
    // PRE FUNC 对 event 进行操作，或者产生新的 event ？反正都是对 event 的添加删除和修改操作，需要相关 API
    // POST FUNC 呢，产生新的 event
}
