/* bank.c write by -Acme */

inherit ROOM;

void init()
{
	add_action("do_withdraw", "withdraw");
	add_action("do_deposit", "deposit");
}

int do_withdraw(string arg)
{
    int amount;
    string money;
    object money_ob, bond;

    seteuid(getuid());

    if( !arg || sscanf(arg, "%d %s", amount, money)!=2 )
        return notify_fail("指令格式﹕withdraw <数量> <货币种类>。\n");

    if( amount < 0 )
        return notify_fail("你不能提领零以下的货币。\n");

    if( amount > 30000) 
        return notify_fail("你不能一次领太多。\n");    

    if( file_size("obj/money/" + money + ".c") < 0 )
	    return notify_fail("你要提领哪一种钱﹖\n");

    if( catch(money_ob = new("/obj/money/" + money)) ) return 0;

    money_ob->set_amount(amount);
    if( this_player()->query("bank") < money_ob->value() ) {
        destruct(money_ob);
        return notify_fail("你的户头里没有这么多钱。\n");
    }

    if( !money_ob->move(this_player()) ) {
        destruct(money_ob);
        return notify_fail("你身上带不了这许\多钱﹐提少一点吧。\n");
    }

    // 从帐户扣钱
    this_player()->add("bank", -money_ob->value());

    write("你从银行提出" + chinese_number(amount) + money_ob->query("base_unit") + money_ob->name() + "。\n");

	return 1;
}

int do_deposit(string arg)
{
    int amount;
    string money;
    object money_ob;

    seteuid(getuid());

    if( !arg || sscanf(arg, "%d %s", amount, money) != 2 )
        return notify_fail("指令格式﹕deposit <数量> <货币种类>。\n");

    if( amount < 0 )
        return notify_fail("你不能存入零以下的钱币。\n");
        
    if( !money_ob = present(money + "_money", this_player()) )
        return notify_fail("你身上没有这种钱币。\n");

    if( money_ob->query_amount() < amount )
        return notify_fail("你身上没有这么多的" + money_ob->name() + "。\n");

    write("你将" + chinese_number(amount) + money_ob->query("base_unit") + money_ob->name() + "存入银行。\n");

    // 加在银行里
    this_player()->add("bank", money_ob->query("base_value") * amount);

    // 身上的钱数量减少
    money_ob->add_amount( -amount );

    if( money_ob->query_amount() <= 0 ) destruct(money_ob);

    this_player()->save_autoload();
    this_player()->save();

	return 1;
}