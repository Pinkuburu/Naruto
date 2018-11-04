/*  mv.c

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
    string src, dst;
 
    SECURED_WIZARD_COMMAND;

    seteuid(geteuid(me));
    if (!arg || sscanf(arg, "%s %s", src, dst)!=2 ) return
    notify_fail("ָ���ʽ: mv <ԭ����> <Ŀ�굵��> \n");
 
    src = resolve_path(me->query("cwd"), src);
    dst = resolve_path(me->query("cwd"), dst);
 
    if( !rename(src, dst) )
	write("Ok.\n");
    else
	write("��û���㹻�Ķ�дȨ����\n");
    return 1;
}
 
int help(object me)
{
    write(@HELP
ָ���ʽ : mv <ԭ����> <Ŀ�굵��>
 
��ָ�������(��)�޸�ĳ��������Ŀ¼���ơ�
HELP
    );
    return 1;
}
 