/*  halt.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include "/doc/help.h"
#include <action.h>
#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    SECURED_COMMAND;

    if( me->is_busy() ) {
        me->interrupt(me, INTR_HALT);
        if( ! me->is_busy() ) {
            write("Ok。\n");
            return 1;
        }
        return 0;
    }

    if( !me->is_fighting() )
        return notify_fail("你现在并没有在战斗。\n");

    if( wizardp(me) ) {
        me->remove_all_killer();
        message_vision("$N用巫师的神力停止了这场打斗。\n", me);
    } else {
        me->remove_all_enemy();
        message_vision("$N向后一跃，离开战圈罢手不斗了。\n", me);
    }

    write("Ok.\n");
    return 1;
}

int help(object me)
{
   write(@HELP
指令格式：halt

可以停止所有与你(你)正在进行的比试﹐但是正在跟你性命相搏的敌人通常
不会轻易言和。

这个指令有时候也可以用来停止一些需要较长时间的动作，如修练内功、打
坐等，不过能不能停止要视那个动作而定。
HELP
   );
   return 1;
}
