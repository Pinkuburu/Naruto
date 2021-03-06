/* transfer.c write by -Acme- */
#include <ansi.h>

inherit ROOM;

mapping record = ([
    1: ([ "name": HIR"火之国"NOR+BGRN"木叶村"NOR,
          "file": _DIR_AREA_"muye/inn.c",
         "price": 10 ]),
/*
    2: ([ "name": "水之国雾忍村",
          "file": _DIR_AREA_"muye/inn.c",
         "price": 100 ]),
*/
]);

string do_list()
{
    int index, size;
    string msg;

    msg  = "╭────────────────────────╮\n";
    msg += "│五行传送特别服务                                │\n";
    msg += "├────────────────────────┤\n";
    msg += "│编号  单价/银两  地区                           │\n";
    msg += "├────────────────────────╯\n";

    if( (size=sizeof(record)) )
        for(index=1;index<=size;index++)
            msg += sprintf("│%4d  %9d  %s\n", index, record[index]["price"], record[index]["name"]);

    msg += "├────────────────────────╮\n";
    msg += "│输入 transfer <编号> 进行传送。                 │\n";
    msg += "╰────────────────────────╯";

    return msg;
}

void create()
{
    set("short", "五行传送");
    set("long", "这里充斥著一股强烈的查克拉，可能是某位忍者在此布下特别的\n"
                "忍术，你可以发现四周挂满了忍术卷轴，而在你的正前方立著一张大\n"
                "牌子，上头乱七八糟地写了一堆文字。\n" );

    set("detail", ([
        "牌子" : (: do_list :),
    ]));

    set("exits", ([
        "east" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 21,
                       "y_axis" : 20,
                  ]),
        "south" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 18,
                       "y_axis" : 23,
                  ]),
        "west" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 15,
                       "y_axis" : 20,
                  ]),
        "north" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 18,
                       "y_axis" : 17,
                  ]),
    ]));

    setup();
}

void init()
{
    add_action("do_transfer", "transfer");
}

int do_transfer(string arg)
{
    int number;
    object me = this_player(), room;

    if( me->is_busy() || me->is_fighting() ) 
        return notify_fail("你现在没有空。\n");

    if( !arg )
        return notify_fail("你想要传送到哪里？\n");

    if( sscanf(arg, "%d", number) != 1 )
        return notify_fail("你想要传送到哪里？\n");

    if( undefinedp(record[number]) )
        return notify_fail("你想要传送到哪里？\n");

    if( me->query("bank") < record[number]["price"] )
        return notify_fail("你银行里的钱不足，无法传送。\n");

    if( !objectp(room = load_object(record[number]["file"])) )
        return notify_fail("你现在无法传送到那个地方。\n");

    message_vision("四周突然刮起了一阵强风，$N一瞬间就被吹走了...\n", me);

    // 移动成功才需付钱
    if( me->move(room) ) me->add("bank", -record[number]["price"]);

    message_vision("四周突然刮起了一阵强风，$N被强风带过来这里...\n", me);

    me->start_busy(3);

    return 1;
}