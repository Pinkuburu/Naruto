/* recall.c */

#include <ansi.h>
#include <login.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    if( !objectp(me) ) return 0;

    if( me->is_fighting() )
        return notify_fail("你还在战斗中，无法使用 recall 指令。\n");

    if( me->busy() )
        return notify_fail("你正忙著，无法使用 recall 指令。\n");

    if( environment(me) && environment(me)->query("no_recall") )
        return notify_fail("你所在的地方无法使用 recall 指令。\n");

    if( me->query_temp("no_recall") )
        return notify_fail("你现在无法使用 recall 指令。\n");

    message_vision(HIM"$N闭上眼睛，嘴里轻诵蔡小林(Jolin)的名号，慢慢地被一阵七彩云雾包住了。\n"NOR, me);

    me->start_busy(1);

    call_out("do_move", 2, me, arg);

    return 1;
}

int do_move(object me, string arg)
{
    string tmp;
    object room;

    if( arg == "start" ) tmp = START_ROOM;
    else {
    	if( !(tmp = me->query("recoverroom")) ) tmp = START_ROOM;
    }

    if( !objectp(room=load_object(tmp)) ) 
        if( !objectp(room=load_object(START_ROOM)) )
            tell_object(me, "突然时空发生一阵错乱！你无法移动！\n");

    message_vision(HIG"只见$N"HIG"的身体化成片片雪花，消失在半空了....\n"NOR,me);

    me->move(room);

    message_vision(HIW"一阵清风吹来阵阵白雾，$N的身影渐渐凝结成型﹗\n"NOR,me);

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式﹕ recall [start]

这个指令可以让你迅速返回重生点。若加参数 start 则会回到巫师神殿。
HELP);
    return 1;
}