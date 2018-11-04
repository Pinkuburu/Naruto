// write by -Acme-

#include <npc.h>

inherit F_VILLAGER;

void create()
{
    set_name("路人", ({ "passerby" }));
    set_race("human");
    set_level(5);
    set("age", random(50)+18);
    set("long", "一位很普通的路人，一天在路上也可以碰见几十个。\n");
    // 随机移动
    set("chat_chance", 1);
    set("chat_msg", ({
        (: random_move :),
    }));
    setup();
    carry_money("silver", 5);
    carry_object(_DIR_EQ_"cloth/cloth.c")->wear();
}