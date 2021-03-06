/*  fight.c

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
#include <char.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object obj, old_target;
    int rg, rk, rgm, rkm;

    if( environment(me)->query("no_fight") )
        return notify_fail("这里禁止战斗。\n");
 
    if( !arg
    ||	!objectp(obj = present(arg, environment(me)))
    ||	!obj->is_character()
    ||	obj->is_corpse() )
	return notify_fail("你想攻击谁﹖\n");

    if( obj->query("no_fight") ) return notify_fail("对这个人物不能下 Fight 指令。\n");

    if( !me->visible(obj) )
	return notify_fail( obj->name() + "看不见你，不知道你在向他挑战。\n");

    if( me->is_apprentice_of(obj) )
	return notify_fail( obj->name() + "是你的师父﹗\n");

    if( obj->is_fighting(me) )
	return notify_fail("加油﹗加油﹗加油﹗\n");

    if( !living(obj) && inherits(F_ATTACK, obj) )
	return notify_fail( "和" + obj->name() + "战斗？\n"); 

    if( obj==me ) return notify_fail("你不能攻击自己。\n");

    if( userp(obj) && (object)obj->query_temp("pending/fight")!=me ) {
	message_vision("\n$N对著$n说道﹕" 
	    + me->rank("self") + me->name() + "，领教这位"
	    + obj->rank("respectful") + "的高招﹗\n\n", me, obj);

	if( objectp(old_target = me->query_temp("pending/fight"))
	&&  obj != old_target
	&&  environment(old_target)==environment(me) )
	    message_vision("\n$N向$n抱拳做揖，说道："
		+ me->rank("self") + "改日再行请教。\n", me, old_target);
	me->set_temp("pending/fight", obj);

	tell_object(obj, YEL "如果你愿意和对方进行比试，请你也对" + me->name() + "("+(string)me->query("id")+")"+ "下一次 fight 指令。\n" NOR);
	write(YEL "由于对方是由玩家控制的人物，你必须等对方同意才能进行比试。\n" NOR);
	return 1;
    }

    if( obj->query("civilized") ) {
	if( me->is_fighting() )
	    message_vision( "\n$N对著$n说道：一并领教这位"
		+ obj->rank("respectful") + "的高招﹗\n\n", me, obj);
	else
	    message_vision( "\n$N对著$n说道﹕" 
		+ me->rank("self") + me->name() + "，领教这位"
		+ obj->rank("respectful") + "的高招﹗\n\n", me, obj);

	if( obj->query_temp("pending/fight")==me ) {
	    obj->start_busy(1);
//	delete else, 让双方都busy而非先下fight指令的人busy only -dragoon
//	else 
	    me->start_busy(1);
	}

	if( !userp(obj) && !obj->accept_fight(me) ) {
	    write("看起来" + obj->name() + "并不想跟你较量。\n");
	    return 1;
	}

	me->fight_ob(obj);
	obj->fight_ob(me);
    } else {
	message_vision("\n$N大喝一声，开始对$n发动攻击﹗\n\n", me, obj);
	me->fight_ob(obj);
	obj->kill_ob(me);
    }

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : fight <人物>
 
这个指令让你向一个人物“讨教”或者是“切磋武艺”﹐这种形式的战斗纯粹是
点到为止﹐因此只会消耗体力﹐不会真的受伤﹐但是并不是所有的  NPC 都喜欢
打架﹐因此有许多状况你的比武要求会被拒绝。
 
其他相关指令: kill

PS. 如果对方不愿意接受你的挑战﹐你仍然可以迳行用 kill 指令开始战斗。
HELP);
    return 1;
}
 
