#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "游戏室(１)");
	set("long", @LONG
这是一间休闲娱乐场所，附近可能摆放一些好玩的东西
LONG);

	set("exits", ([
		"east" : __DIR__"game2",
		"south" : __DIR__"game4",
	]));

    set("objects", ([
    	_DIR_ITEM_"big2.c" : 1,
    ]));

    set("no_fight", 1);
	setup();
}