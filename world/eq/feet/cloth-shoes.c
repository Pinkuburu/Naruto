#include <ansi.h>
#include <armor.h>

inherit F_FEET_EQ;

void create()
{
    set_name("白布鞋", ({ "white cloth shoes", "shoes" }) );

    set_weight(1200);

    setup_feet_eq();

    if( !clonep() ) {
        set("unit", "双");
        set("value", 1600);
        set("long", "传说中的“bad boy”据说穿了这鞋子，歌喉会变好。\n");
        set("wear_as", "feet_eq");
        set("apply_armor/feet_eq", ([
            "armor": 2,
            "int" : 2,
        ]));
    }

    setup();
}
