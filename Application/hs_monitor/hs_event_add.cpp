#include <zstack.h>
#include "hs.h"
#include "hs_attr.h"

// 将某张卡牌的某属性改成对应的值，这里不是 delta 值？是的，需要的时候做减法即可
// 抽一张牌： 谁 将 谁 的 REIGON 属性从 LIB 改成 HAND
// operation = equal, add
//  "region=lib|..."
// 不能在这里解析 dst ，因为后续的环境后变化，执行时确定 DST

// "type=retinue|hero,camp=enemy,select=random; blood = blood - 1")
// JSON format
void hs_event_add(struct hs_event *evt, struct hs_card *src, char *info)
{

}
