// arrest command..

inherit F_CLEAN_UP;

int main(object me, string str)
{
        object ob;
        int i;

        seteuid(getuid(me));
        if(!str) return notify_fail("指令格式: arrest <使用者>\n");
        ob = LOGIN_D->find_body(str);
        if(!ob) return notify_fail("线上的使用者中没有这个人。\n");
	if(ob->query("level") > 4 ) 
		return notify_fail("他不在是被保护的等级不用取消他的保护\n");

	seteuid(geteuid(ob));
	if (ob->query("get_kill") == 1) {
		ob->set("get_kill", 0) ; 
		tell_object(ob, "巫师("+me->query("id")+")恢复你的保护\n");
		tell_object(me,"你恢复了玩家("+ob->query("id")+")的保护\n");
		}
	else {
		ob->set("get_kill", 1) ;
		tell_object(ob, "巫师("+me->query("id")+")取消你的保护\n");
                tell_object(me,"你取消玩家("+ob->query("id")+")的保护\n");
		}
        return 1;
}

int help(object me)
{
write(@HELP
指令格式 : arrest <某人>

此指令可让你取消特定恶劣玩家的等级保护
HELP
    );
    return 1;
}
