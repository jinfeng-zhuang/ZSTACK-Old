
// 操作：赋值和加法，乘法，除法，开根号（适用于数学系玩家）

// 奥术飞弹
// event_add(NULL, "range:type=retinue|hero,camp=enemy,select=random", blood, add, -1)
// event_add(evt, "range:type=retinue|hero,camp=enemy,select=random", blood, add, -1)
// event_add(evt, "range:type=retinue|hero,camp=enemy,select=random", blood, add, -1)

// 迸射流星，这里说明不能分开，否则 target 就失效了！！！
// event_add(NULL, "range:type=retinue,camp=enemy,select=target", blood, add, -1)
// left = target.left, right = target.right // 不能在此处生成结果，需要即时生成
// event_add(evt, "range:type=retinue,camp=enemy,select=target.left", blood, add, -1)
// event_add(evt, "range:type=retinue,camp=enemy,select=target.right", blood, add, -1)
// 如上，需要支持一些变量，比如用户选择的 TARGET 或者 ID 语句就简单一些就行

// 钳嘴龟盾卫
// 修改最后生成的 dst 直到它的生命值 <= 0

// 毁
// event_add(NULL, "dst:type=retinue,attack>4", field, equal, die)
