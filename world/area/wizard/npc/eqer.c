#include <npc.h>

inherit F_VENDOR;

void create()
{
    set_name("防大师", ({ "armor master", "master" }));
    set_race("human");
    set_level(50);
    set("age", random(50)+18);

    set("long", "无奸不成商，十足的奸商样的老奸商人。\n");

    // 设定商人所贩卖的物品
    // set_goods(代码, 种类, 编号, 数量)
    // 商人能贩卖的东西只能由 query 指令里查 eq, wp, food 能查出来的物品
    // 如 set_goods(1, "food", 1, 10) 表示贩卖 query 指令查出来的food编号为1的食品十个
    // 若数量设定 -1 表示无限量供应
    // 代码是用于 buy 指令上，如 buy 1 则会向商人买下代码为1的商品

    set_goods(1, "eq", 1, 10);
    set_goods(2, "eq", 2, 10);
    set_goods(3, "eq", 3, 10);
    set_goods(4, "eq", 4, 10);
    set_goods(5, "eq", 5, 10);
    set_goods(6, "eq", 6, 10);
    set_goods(7, "eq", 7, 10);
    set_goods(8, "eq", 8, 10);
    set_goods(9, "eq", 9, 10);
    set_goods(10, "eq", 10, 10);
    set_goods(11, "eq", 11, 10);
    set_goods(12, "eq", 12, 10);
    set_goods(13, "eq", 13, 10);
    set_goods(14, "eq", 14, 10);
    set_goods(15, "eq", 15, 10);
    set_goods(16, "eq", 16, 10);
    set_goods(17, "eq", 17, 10);
    set_goods(18, "eq", 18, 10);
    set_goods(19, "eq", 19, 10);
    set_goods(20, "eq", 20, 10);
    set_goods(21, "eq", 21, 10);
    set_goods(22, "eq", 22, 10);
    set_goods(23, "eq", 23, 10);
    set_goods(24, "eq", 24, 10);
    set_goods(25, "eq", 25, 10);

    setup();
}
