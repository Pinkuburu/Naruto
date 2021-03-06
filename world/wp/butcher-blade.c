#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name("杀猪刀", ({ "butcher's blade" ,"blade" }));
    set_weight(1000);
    setup_weapon(10, 20, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "twohanded" }) );
        set("unit", "把");
        set("value", 5815);
        set("long", "一把普通拿来剁猪肉的刀，传说有一位密探利用特异功能来操控它打败金枪客。\n" );
    }

    set("apply_weapon/twohanded", 
        ([ "str" : 3,            // 属性：力量
           "attack" : 2,         // 攻击力量
           "armor" : 2,        // 防御力量
    ]) );

    setup();
}

