/*  cd.c - change directory command

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    string dir;

    SECURED_COMMAND;

    seteuid(geteuid(this_player(1)));

    if( !arg || arg=="" ) dir = user_home( geteuid(me) );
    else if( arg[0]=='/' ) dir = arg;
    else dir = resolve_path(me->query("cwd"), arg);

    if(file_size(dir)!=-2)
	return notify_fail("没有这个目录。\n");

    if( dir[<1] != '/' ) dir += "/";

    me->set("cwd", dir);
    write(dir + "\n");

    return 1;    
}

int help(object me)
{
    write(@HELP
指令格式：cd [<子目录名>]

设定你的工作目录，用 .. 可以指定上一层目录。

不加任何参数时，会直接回到自己 HOME 目录。
HELP
    );
    return 1;
}
