#include <zstack.h>
#include "hs.h"
#include "hs_attr.h"

struct hs_card *hs_draw_a_card(struct list_head *head) // + this
{
    struct list_head *cardpos;
    struct hs_card *card;
    struct list_head *attrpos;
    struct hs_attr *attr;

    // 遍历所有卡牌，遍历卡牌的每一个属性，找到在库中的卡牌，然后将其属性更改
    // 通用流程：解析事件，遍历所有卡牌，遍历卡牌的每一个属性，找到匹配的，执行属性更改前操作，属性更改，执行属性更改后操作
    // 所以，抽牌，仅仅需要将 event 添加进去就好了，不需要自己去遍历
    list_for_each(cardpos, head) {
        card = list_entry(cardpos, struct hs_card, list);

        list_for_each(attrpos, &card->attr) {
            attr = list_entry(attrpos, struct hs_attr, list);

            if ((attr->id == HS_ATTR_REGION) && (attr->value == HS_ATTR_REGION_LIB)) {
                // !!! value should be changed be system
                //attr->value = HS_ATTR_REGION_HAND;

                // add region change event to queue

                //list_add_tail();

                return card;
            }
        }
    }

    return NULL;
}
