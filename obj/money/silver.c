// silver.c

inherit MONEY;

void create()
{
	set_name("银子", ({"silver", "silver_money"}));
	if( !clonep() ) {
		set("money_id", "silver");
		set("long", "白花花的银子﹐人见人爱的银子。\n");
		set("unit", "些");
		set("material", "silver");
		set("base_value", 1);
		set("base_unit", "两");
		set("base_weight", 5);
	}
	set_amount(1);
	setup();
}

