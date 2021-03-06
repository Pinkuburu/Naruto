// drop.c

#include <login.h>
inherit F_CLEAN_UP;

int do_drop(object me, object obj);

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object obj, *inv, obj2;
    int i, amount;
    string item;

    if( me->is_busy() )
        return notify_fail("你正忙著其他事﹗\n");

    if(!arg) return notify_fail("你要丢弃什么东西﹖\n");
    if( !environment(me) ) return 0;

    if(sscanf(arg, "%d %s", amount, item)==2) {
        if( !objectp(obj = present(item, me)) )
            return notify_fail("你身上没有这样东西。\n");
        if( stringp(obj->query("no_drop")) || obj->query("keep_this") )
            return notify_fail("这个东西没办法丢弃，或是已经keep了...\n");
        if( !obj->query_amount() )
            return notify_fail( obj->name() + "不能被分开丢弃。\n");
        if( amount < 1 )
            return notify_fail("东西的数量至少是一个。\n");
        if( amount > obj->query_amount() )
            return notify_fail("你没有那么多的" + obj->name() + "。\n");
        else if( amount == (int)obj->query_amount() )
            return do_drop(me, obj);
        else {
            obj->set_amount( (int)obj->query_amount() - amount );
            seteuid(MUDLIB_UID);
            obj2 = new(base_name(obj));
            seteuid(getuid());
            obj2->set_amount(amount);
            return do_drop(me, obj2);
        }
    }

    if(arg=="all") {
        inv = all_inventory(me);
        for(i=0; i<sizeof(inv); i++) {
            do_drop(me, inv[i]);
        }
        write("Ok.\n");
        return 1;
    }

    if(!objectp(obj = present(arg, me)))
        return notify_fail("你身上没有这样东西。\n");
    return do_drop(me, obj);
}

int do_drop(object me, object obj)
{
    mixed no_drop;

    if( no_drop = obj->query("no_drop") )
        return notify_fail( stringp(no_drop) ? no_drop : "这样东西不能随意丢弃。\n");

    if( obj->query("keep_this") ) 
    	return notify_fail("这样东西你已经 keep 无法丢弃。\n");
    
    if( obj->query("equipped") )
    	return notify_fail("这样东西你已经装备著，无法丢弃。\n");

    if( environment(me)->is_area() ) {
    	if( !area_move(environment(me), obj, me->query("area_info/x_axis"), me->query("area_info/y_axis")) ) return 0;
    } else if( !obj->move(environment(me)) ) return 0;

    if( obj->is_character() )
        message_vision("$N将$n从背上放了下来﹐躺在地上。\n", me, obj);
    else {
        message_vision( sprintf("$N丢下一%s$n。\n", obj->query("unit")), me, obj );
		if( !obj->query("value") && !obj->value() ) {
		    write("因为这样东西并不值钱﹐所以人们并不会注意到它的存在。\n");
			destruct(obj);
			return 1;
	    }
	}

	// 不值钱的东西早就被 dest 掉了... 这里没有 obj.... -elon 12/2/96
	if( obj ) obj->delete("keep_this");

	return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : drop <物品名称>
 
这个指令可以让你丢下你所携带的物品.
例如:
drop all	丢下所有未 keep 的物品.
drop sword 2	丢下你所携带的第二把 sword.
drop 200 gold	丢下二百两黄金. 
HELP
    );
    return 1;
}
