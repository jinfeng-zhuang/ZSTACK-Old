#include <zstack.h>
#include <stdio.h>

struct test_t {
    int id;
    struct list_head list;
};

static struct list_head head;

int test_list(void)
{
    int i;
    struct test_t *test;
    struct list_head *pos;

    TRACE;

    INIT_LIST_HEAD(&head);

    for (i = 0; i < 10; i++) {
        test = (struct test_t *)malloc(sizeof(struct test_t));
        if (test) {
            test->id = i;
            list_add_tail(&test->list, &head);
        }
    }

    list_for_each(pos, &head) {
        test = list_entry(pos, struct test_t, list);
        info("ID = %d\n", test->id);
    }

    return 0;
}
