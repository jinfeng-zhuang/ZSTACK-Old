#ifndef HS_H
#define HS_H

struct hs_attr;
struct hs_card;
struct hs_event;

///////////////////////////////////////////////////////////////////////////////

// 双向链表，可以标识自己，还能遍历所有
// 应该还包含所有匹配的卡牌：都在 evt 的 dst 中
typedef void (*hs_on_event)(struct hs_card *card, struct hs_event *evt);

///////////////////////////////////////////////////////////////////////////////

// 属性是一个丰富的对象
// 属性有值，有运算方法，有响应机制，有比较方法，有互联方法等
// 而且响应机制是自定义的
struct hs_attr {
    int id;             // 信号
    int compare;        // 大于，小于，等于，用于搜索时的描述
    int operate;        // 赋值，加法，乘法
    int value[3];       // 信号当前电平，最小值，最大值
    hs_on_event pre;    // 信号变化前
    hs_on_event post;   // 信号变化后

    // for search
    //struct list_head search;

    struct list_head list;
};

// 一连串的，比如奥术飞弹
struct hs_event {
    struct hs_card *src;

    struct list_head *dst;
    struct list_head attr2match; // 用于生成 dst

    struct hs_attr operation;

    struct list_head list;
};

struct hs_card {
    unsigned int uuid;      // 用于索引静态的卡牌库

    unsigned int id;        // 用于索引内存中的卡牌，添加时自动生成
    struct list_head attr;  // 卡牌属性和响应机制列表，初始化时动态生成，从 JSON 文件中读取

    struct list_head list;  // 用于串联

    // for search
    struct list_head search;
};

///////////////////////////////////////////////////////////////////////////////

extern struct hs_card search_result;

///////////////////////////////////////////////////////////////////////////////
// Based on card list to search

// I/O may be a list
extern struct hs_card *hs_search(struct attr *attr);
extern struct list_head *hs_event_queue(void);
extern struct list_head *hs_card_queue(void);

#endif
