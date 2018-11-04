#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name(HIK"锅子"NOR, ({ "boiler" }) );

    set_weight(2000);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "个");
        set("value", 400);
        set("long", "一个看起来很坚固的锅子，两端还有把手。");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
            "armor": 4,
        ]));
    }

    setup();
}
