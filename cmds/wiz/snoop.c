/*  snoop.c

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
#include <command.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object ob;

    SECURED_WIZARD_COMMAND;

    if( !arg ) {
        if( objectp(ob = query_snooping(me)) )
            write("你现在正在监听" + ob->query("name") + "所收到的讯息。\n");
        return 1;
    } else if( arg=="none" ) {
        if( objectp(ob = query_snooping(me))
        &&    wiz_level(ob) >= wiz_level(me) )
            tell_object(ob, HIW + me->name(1) + "停止监听你所收到的讯息。\n" NOR);
        snoop(me);
        write("Ok.\n");
        return 1;
    }

    ob = find_player(arg);
    if(!ob) ob = find_living(arg);
    if(!ob || !ob->visible(me)) return notify_fail("没有这个人。\n");

    if( wizhood(me) != "(admin)"
    &&    wiz_level(me) <= wiz_level(ob) || wiz_level(me) < 3)
        return notify_fail("你没有监听" + ob->name() + "所收听讯息的权利。\n");

    if( me==ob ) return notify_fail("请用 snoop none 解除监听。\n");
        
    snoop(me, ob);
    write("你现在开始窃听" + ob->name(1) + "所收到的讯息。\n");
    if( userp(ob) ) {
	log_file("SNOOP_PLAYER",
	    sprintf("[%s] %s snoops %O\n", ctime(time()), geteuid(me), ob));
	if( wizardp(ob) && wiz_level(me) < wiz_level("(arch)") )
	    tell_object(ob, HIW + me->name(1) + "开始监听你所收到的讯息。\n" NOR);
    }

    return 1;
}

int help()
{
    write(@TEXT
指令格式﹕snoop <某人>|none

监听其他使用者所收听的讯息﹐snoop none 则取消监听。
TEXT
    );
    return 1;
}
