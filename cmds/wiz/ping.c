/*  ping.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <intermud.h>
#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string addr;
    int port;

    SECURED_WIZARD_COMMAND;

    if( !arg || sscanf(arg, "%s %d", addr, port)!=2 )
	return notify_fail("指令格式：ping <IP 地址> <UDP 埠>\n");

    INTERMUD_SERVICE("ping")->send_request(addr, port);
    write("Ok.\n");
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式：ping <IP 地址> <UDP 埠>

这个指令可以用来测试远端的 MUD 是否执行中。

请注意第二个参数的埠号是 Intermud 的 UDP 埠号，不是 MUD 连线登入的
埠号。如果对方是 ES2-based 的 MUD 而你不知道他们的 UDP 埠，可以试
试看 ES2 mudlib 的预设值，也就是登入的埠号 +4。

相关说明：
	mudlist
HELP
    );
    return 1;
}
