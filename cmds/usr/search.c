// search.c

#include <ansi.h>

inherit F_CLEAN_UP;

int searching(object me, object env);
int find_drug(object me,int drug_num);

int main(object me, string arg)
{
	int i;
    object env;
    string msg="", *exits, *detail;

	if( me->is_fighting() || me->is_busy() )
		return notify_fail("你正在忙著, 请试著用 halt 停止所忙的事。\n");

    if( !objectp(env=environment(me)) ) return 0;

	message_vision("$N露出谨慎的神情，开始四处翻翻找找。\n\n", me);
	me->start_busy(1);

    if( env->is_area() ) {
    	string temp;
        mapping info, detail_a;
        
        info = me->query("area_info");
        
        // 区域名
        msg = env->query("name");
        
        // short 描述
        if( (temp=env->query_data(info["x_axis"], info["y_axis"], "short")) ) 
            msg += " - " + temp + "\n";
        else msg += "\n";

        // long 描述
        if( (temp=env->query_data(info["x_axis"], info["y_axis"], "long")) ) 
            msg += "    " + temp + "\n";
        else msg += "\n";
        
        // 出口提示
        exits = env->query_exits(info["x_axis"], info["y_axis"]);
        if( (i = sizeof(exits)) ) {
    	    msg += "这里明显的出口是 ";
    	    while( i-- ) msg += HIW+exits[i]+NOR + ( i ? "、" : "。\n" );
        } else msg += "  这里没有任何出口。\n";

        // 细节提示
    	detail_a = env->query_area_detail(info["x_axis"], info["y_axis"]);
    	if( (i=sizeof(detail_a)) ) {
    		msg += "\n存在于这里的事物有：\n";
    		detail = keys(detail_a);
    		while(i--) msg += "  " + detail[i] + "\n";
    	}
    } else {
    	msg = env->query("short") + " -\n";
    	msg += "    " + env->query("long");
        // 出口提示
        if( (i = sizeof(env->query("exits"))) ) {
            exits = keys(env->query("exits"));
    	    msg += "    这里明显的出口是 ";
    	    while( i-- ) msg += HIW+exits[i]+NOR + ( i ? "、" : "。\n" );
        } else msg += "  这里没有任何出口。\n";
    	
    	if( (i=sizeof(env->query("detail"))) ) {
    		msg += "\n存在于这里的事物有：\n";
    		detail = keys(env->query("detail"));
    		while(i--) msg += "  " + detail[i] + "\n";    		
    	}
    }

    me->start_more(msg);

	return 1;
}

int help(object me)
{
	write(@HELP
指令格式﹕search

这个指令可以让你找找你的环境中是否有特殊的东西，许多的任务、暗房、特殊物品
等可用 search 来获得一些端倪或寻获意外的东西。

HELP
	);
	return 1;
}