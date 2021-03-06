/*  hp.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>

inherit F_CLEAN_UP;

private string status_color(object me, string stat);

int main(object me, string arg)
{
    object ob;
    
    if( !wizardp(me) || !arg ) ob = me;
    else {
		if( !ob )    ob = present(arg, environment(me));
		if( !ob )    ob = find_living(arg);
		if( !ob )    return notify_fail("没有 " + arg + " 这个人物。\n");
                write("[" + ob->name() + "] ");
    }    

    printf( "精神值 %s%d/%d" NOR "  体力值 %s%d/%d" NOR "  查克拉 %s%d/%d\n"NOR,
		status_color(ob, "ap"),
		ob->query_stat("ap"),
		ob->query_stat_maximum("ap"),
		status_color(ob, "hp"),
		ob->query_stat("hp"),
		ob->query_stat_maximum("hp"),
		status_color(ob, "mp"),
		ob->query_stat("mp"),
		ob->query_stat_maximum("mp"),
    );
    return 1;
}

private string status_color(object me, string stat)
{
    int current = me->query_stat(stat);
    int max = me->query_stat_maximum(stat);
    int ratio = max ? (current * 100) / max : 100;

    switch( ratio ) {
        case  0..30: return HIR;
		case 31..80: return HIY;
		case 81..100: return HIG;
		default: return HIC;
	}
}

int help(object me)
{
write(@HELP
指令格式 : hp [<人物 ID>]

用途 : 帮助你看到目前的简要状态。巫师可以查他人的简要状态。
HELP
     );
     return 1;
}
