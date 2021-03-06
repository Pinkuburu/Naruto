/*  kill.c

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

void do_kill(object, object);

int main(object me, string arg)
{
    object obj;
    string *killer, callname;

    if( !arg ) return notify_fail("你想杀谁﹖\n");

    if( !(obj = present(arg, environment(me)))
    ||	!obj->is_character()
    ||	obj->is_corpse() )
	return notify_fail("这里没有这个人。\n");

    if( userp(obj) ) return notify_fail("你不能对玩家下 Kill 指令。\n");

    if( environment(me)->query("no_fight") ) {
	    if( !obj->query("unprotect_mark") ) {
	        tell_object(me, "这里不准战斗。\n");
	        return 1;
	    }
    }

    if( obj->query("no_fight") ) return notify_fail("对这个人物不能下 Kill 指令。\n");

    if( me->is_busy() ) return notify_fail("你被敌人攻得手忙脚乱 ...\n");

    if( obj==me ) return notify_fail("你那么想死吗？\n");

    me->set_temp("pending/hidden", 0);      // be visible
        
    if( ! me->is_fighting(obj)
    ||	! me->is_killing(obj) ) {
	switch( random(3) )
	{
	case 0:
	    message_vision("\n$N对著$n喝道﹕“" + obj->rank("rude")
		+ "﹗今日不是你死就是我活﹗”\n\n", me, obj);
	    break;
	case 1:
	    message_vision("\n$N对著$n喝道﹕“" + obj->rank("rude")
		+ "，纳命来！”\n\n", me, obj);
	    break;
	case 2:
	    message_vision("\n$N对著$n喝道﹕“" + obj->rank("rude")
		+ "，明年的今天就是你的忌日！”\n\n", me, obj);
	    break;
	}
    }

    do_kill(me, obj);

    return 1;
}

void do_kill(object me, object obj)
{
    me->kill_ob(obj);

    if( obj->is_fighting(me) && obj->is_killing(me) ) return;

    if( userp(obj) )
    {
	/* 对玩家的 kill 指令 */
	obj->fight_ob(me);
	tell_object(obj, HIR "如果你要和" + me->name() 
	    + "性命相搏，请你也对这个人下一次 kill 指令。\n" NOR);
    }
    else {
	/* 对 NPC 的 kill 指令 */
	if( function_exists("accept_kill", obj)
	&&  ! obj->is_killing(me)
	&&  obj->accept_kill(me) ) {
	    return; }

	if( ! obj->is_killing(me) ) obj->kill_ob(me);
	else if( ! obj->is_fighting(me) ) obj->fight_ob(me);
    }
}

int help(object me)
{
  write(@HELP
指令格式 : kill <人物>
 
这个指令让你主动开始攻击一个人物﹐并且尝试杀死对方﹐kill 和 fight 最大的
不同在于双方将会真刀实枪地打斗﹐也就是说﹐会真的受伤。由于 kill 只需单方
面一厢情愿就可以成立﹐因此你对任何人使用 kill 指令都会开始战斗﹐通常如果
对方是 NPC 的话﹐他们也会同样对你使用 kill。

其他相关指令: fight
HELP);
    return 1;
}
 
