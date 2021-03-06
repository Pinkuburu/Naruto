/*  chpass.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <login.h>
#include <command.h>

inherit F_CLEAN_UP;

int
main(object me, string arg)
{
    object ob;
    string userid, new_passwd;

#ifndef SAVE_USER
    return notify_fail("系统目前不储存使用者资料。\n");
#endif

    SECURED_ADMIN_COMMAND;

    if( previous_object() != this_player(1) ) {
        log_file("static/CHPASS", sprintf("[%s] %s call_other chpass command\n",
            ctime(time()), file_name(previous_object())));
        return notify_fail("这个指令不能被间接呼叫。\n");
    }

    // Write every attempt to this command.
    log_file("static/CHPASS",
        sprintf("[%s] %s %s\n", ctime(time()), geteuid(this_player(1)), arg));

    if( sscanf(arg, "%s %s", userid, new_passwd) != 2 )
        return notify_fail("指令格式：chpass <使用者代号> <密码>\n");

    if( wizhood(userid)=="(admin)" )
        return notify_fail("不能修改系统管理者的密码。\n");

    if( find_player(userid) )
        return notify_fail("不能修改连线中的使用者密码。\n");

    seteuid(getuid());
    ob = new(LOGIN_OB);
    ob->set("id", userid);
    if( file_size(ob->query_save_file()) < 0 )
        return notify_fail("没有这个使用者。\n");

    if( ! ob->restore() )
        return notify_fail("使用者资料读取失败。\n");

    if(    ob->set("password", crypt(new_passwd, 0))
    &&    ob->save() )
        write("使用者 " + userid + " 密码变更成功\。\n");
    else
        write("使用者资料储存失败。\n");

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : chpass <使用者代号> <密码>
 
这个指令是系统管理者专用的，可以修改除了管理者以外，其他人的密码。所有尝试
执行这个指令的动作，无论成功或失败，都会被 log 下来。
 
HELP
    );
    return 1;
}
 
