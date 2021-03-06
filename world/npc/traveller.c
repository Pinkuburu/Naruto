//traveller.c

#include <npc.h>

inherit F_VILLAGER;

void create()
{
    set_name("过路客", ({ "traveller" }) );
    set("unit", "位");
    set("gender", "male");
    set_level(6);
    set("age", 18+random(60));
    set("long", "一位行色匆匆，满脸风霜的过路客，一身劲装，似乎随时又要上路去。\n");
    set("chat_chance", 1);
    set("chat_msg", ({
        "过路客喘了几口气道：呼 呼... 好累，真远。\n",
        "过路客似乎累的讲不出话来了。\n",
        (: command,"sweat":),
        (: random_move :),
    }));
    setup();
    carry_money("silver", 10);
    carry_object(_DIR_EQ_"cloth/cloth.c")->wear();
}

int accept_fight(object ob)
{
    do_chat("过路客擦了擦脖子上的汗水笑道：“打啥，都快累翻了还打！”\n");
    return 0;
}