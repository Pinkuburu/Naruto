/* eat.c */

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	int stuff;
    object ob;

    if( me->is_busy() ) return 0;

    if( !arg || arg=="" ) return notify_fail("你要吃什么﹖\n");

    ob = present(arg, me);
    if( !ob ) ob = present(arg, environment(me));
    if( !ob ) return notify_fail("这里没有这样东西 ...。\n");

    stuff = ob->query("food_stuff");
    if( !stuff || !ob->stuff_ob(me)) {
        write("这样东西不能吃...\n");
        return 1;
    }

    message_vision("$N吃下了一" + ( ob->query("base_unit") ? ob->query("base_unit") : ob->query("unit") ) +
    	           "$n。\n", me, ob);
    
    ob->add_amount(-1);
    if( ob->query_amount() <= 0 ) destruct(ob);

    return 1;
}

int help()
{
    write(@TEXT
指令格式：eat <食物>

这个指令用来让你的人物进食或服用药物。
TEXT);
    return 1;
}
