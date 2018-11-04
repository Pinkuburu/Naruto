#include <ansi.h>
#include <armor.h>
inherit F_HAND_EQ;

void create()
{
    set_name("布手套", ({ "cotton gloves","gloves" }) );

    set_weight(800);
    setup_hand_eq();

    if( !clonep() ) {
        set("unit", "双");
        set("value", 100);
        set("long", "一个很普通的手套，看起来没什么特别之处。\n");
        set("wear_as", "hand_eq");
        set("apply_armor/hand_eq", ([
            "armor": 1,
        ]));
    }

    setup();
}
