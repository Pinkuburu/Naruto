/*  suicide.c

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

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object link_ob;
    string passwd;

    SECURED_COMMAND;

    if( !(link_ob = me->link()) )
        return notify_fail("你的连线物件不见了﹐现在无法自杀。\n");

    if( !arg ) return notify_fail("指令格式﹕suicide <密码>\n");

    passwd = link_ob->query("password");
    if( crypt(arg, passwd) != passwd )
        return notify_fail("密码错误﹗\n");

    log_file("static/SUICIDE",
        sprintf("[%s] %s committed suicide from %s\n",
	    ctime(time()), geteuid(me), query_ip_name(me)) );
    CHANNEL_D->do_chnnel(me, "sys", me->name() + "自杀了。\n");

    rm( link_ob->query_save_file() );
    rm( me->query_save_file() );
    
    write("好吧，希望还能很快再 ES2 中看到您。\n");
    tell_room(environment(me), me->name() +
        "自杀了﹐以后你再也看不到这个人了。\n", ({me}));

    destruct(link_ob);
    destruct(me);

    return 1;
}

int help (object me)
{
    write(@HELP
指令格式﹕suicide <密码>
 
如果因为某种原因你不想活了﹐你可以用这个自杀指令来删除你的人物﹐自杀
以后会将你的人物存档完全自硬碟上消除﹐无法救回了﹐所以自杀前请务必三
思。

这个指令并不是让玩家拿来消极地威胁巫师或 mud管理者的﹐自杀一个人物对
玩家跟巫师而言﹐都不是乐意见到的﹐因为这和现实生活中的“死亡”在某种
层面上是具有相同意义的﹐因此选择自杀的途径之前请先确定你已经想清楚并
确实明白这个指令的功用。
HELP
    );
    return 1;
}
