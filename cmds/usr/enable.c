/*  enable.c - enable a special skill with another skill

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

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    mapping map;
    string *types, *skill, ski, map_to;
    int i, modify;

    SECURED_COMMAND;

    if( !arg ) {
        map = me->query_skill_map();
        if( !mapp(map) ) map = ([]);

        write("以下是你目前使用中的特殊技能。\n");
        foreach( skill, map_to in map ) {
            printf("%-20s：%s\n", to_chinese(skill),
                map_to ? to_chinese(map_to) : "不使用");
        }
        return 1;
    }

    if( sscanf(arg, "%s with %s", ski, map_to)==2 ) {

//        if( !me->query_skill(map_to, 1) && !me->query_learn(map_to) )
        if( !me->query_skill(map_to, 1)  )
            return notify_fail("你不会这种技能。\n");

        if( !function_exists("valid_enable", SKILL_D(map_to)) )
            return notify_fail( "“" + to_chinese(map_to) + "”不能和其他技能合用。\n");

        notify_fail("这个技能不能当成这种用途。\n");
        if( !SKILL_D(map_to)->valid_enable(ski) )
            return 0;

        me->map_skill(map_to, map_to);
        me->map_skill(ski, map_to);

        write("你决定使用“" + to_chinese(map_to) + "”当作" + to_chinese(ski) + "。\n");
        return 1;
    }

//    if( !me->query_learn(arg) )
    if( !me->query_skill(arg, 1) )
        return notify_fail("你没有学过这种技能。\n");

    if( me->skill_mapped(arg)==arg ) {
        write("你在适当的场合会自动使用这个技能。\n");
        return 1;
    }

    me->map_skill(arg);
    write("你决定恢复使用所学有关“" + to_chinese(arg) + "”的技巧。\n");

    return 1;
}
int help(object me)
{
    write(@HELP
指令格式 : enable <技能种类> with <技能名称>   <- 使用特殊技能、武术
           enable <技能种类>                   <- 只用基本技能

这个指令让你指定所要用的技能﹐需指明技能种类和技能名称。如果不加参数则会
显示出技能种类及你目前所使用的技能名称。

其他相关指令：disable、skills、abandon
HELP);
    return 1;
}
