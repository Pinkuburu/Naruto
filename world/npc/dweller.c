// man.c

#include <npc.h>

inherit F_VILLAGER;

void create()
{
    set_name("居民", ({ "dweller" }) );
    set_race("human");
    set_level(7);
    set("age", random(50)+18);
    set("long", "一个居住在这附近的居民，看起来普普通通，没什么特别。\n");

    // 随机移动
    set("chat_chance", 1);
    set("chat_msg", ({
        (: random_move :),
    }));
    setup();

    carry_money("silver", 8);
    carry_object(_DIR_WP_"radish.c")->wield();
    carry_object(_DIR_EQ_"cloth/cloth.c")->wear();
}
