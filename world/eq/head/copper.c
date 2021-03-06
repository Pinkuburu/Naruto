#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name(YEL"铜头盔"NOR, ({ "copper helmet", "helmet" }) );

    set_weight(3000);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "顶");
        set("value", 4000);
        set("long", "一顶相当耐用的头盔，怎么摔也摔不坏。");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
            "armor": 8,
        ]));
    }

    setup();
}
