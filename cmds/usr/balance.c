/* balance.c by -Acme- */

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object who;
	
    if( arg && !wizardp(me) )
    	return notify_fail("只有巫师才能查询别人的存款。\n");

    if( !arg ) who = me;
    else {
        if( !who ) who = present(arg, environment(me));
        if( !who ) who = find_living(arg);
        if( !who ) return notify_fail("没有 " + arg + " 这个人物。\n");
        write("[" + who->name() + "] ");
    }

    tell_object(me, "你目前有" + chinese_number(who->query("bank")) + "两银子的存款。\n");

    return 1;
}

int help()
{
    write(@TEXT
指令格式﹕balance [某人]

这个指令可以查询你银行目前的存款。
TEXT);
    return 1;
}
