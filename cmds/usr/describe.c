/*  describe.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string *txt;

    if( !arg ) return notify_fail("指令格式﹕describe <描述>\n");

    txt = explode(arg, "\n");
    if( (strlen(arg) > 400) || (sizeof(txt) > 8) )
        return notify_fail("您的叙述太长了﹐请控制在八行、两百字以内。\n");

    arg = implode(txt, "\n") + "\n";

    me->set("long", arg);
    write("Ok。\n");
    return 1;
}

int help()
{
    write(@TEXT
指令格式﹕describe <描述>

这个指令让你设定当别人用 look 指令看你时﹐对你的描述﹐通常当你
的描述超过一行时可以用 to describe 的方式来输入。
TEXT
    );
    return 1;
}
