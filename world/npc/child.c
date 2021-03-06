// write by -Acme-

#include <npc.h>

inherit F_FIGHTER;

void create()
{
    set_name("小孩", ({ "child" }));
    set_race("human");
    set_level(3);
    set("age", 6+random(10));
    set("long", "一个流著鼻涕的小孩，不知在哪玩得全身脏兮兮。\n");

    // 随机移动
    set("chat_chance", 1);
    set("chat_msg", ({
        (: random_move :),
    }));

    setup();

    // 削弱
    set_attr("str", 12);
    set_attr("con", 12);
    set_attr("dex", 12);
    set_attr("int", 12);
    set_temp("apply/attack", -5);         // 攻击力量
    set_temp("apply/defend", -5);         // 防御力量
    set_temp("apply/exact", -5);          // 命中率
    set_temp("apply/evade", -5);          // 回避率
    set_temp("apply/intimidate", -5);     // 攻击能力
    set_temp("apply/wittiness", -5);      // 防御能力

    carry_money("silver", 1);
}