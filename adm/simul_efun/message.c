/*  message.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

varargs void tell_area(mixed area, int x, int y, string str, object *exclude)
{
	if( area && area->is_area()) {
		object *obs, ob;
		if( !area->check_scope(x, y) ) return;
		obs = area->query_inventory(x, y);
		foreach(ob in obs) {
			if( objectp(ob) )  {
			    if( exclude ) {
				    if( member_array(ob, exclude) == -1 )
				        message("vision", str, ob);
				} else message("vision", str, ob);
			}
		}
	}
}

varargs void message_vision(string msg, object me, object you)
{
    string my_gender, your_gender, my_name, your_name;
    string str1, str2, str3;

    if( ! me ) return;
    my_name= me->name();
    my_gender = me->query("gender");
    str1 = replace_string(msg,  "$P", gender_self(my_gender));
    str1 = replace_string(str1, "$N", gender_self(my_gender));
    if( you ) {
        if( you != me ) {
		    your_name= you->name();
		    your_gender= you->query("gender");
	    	str2 = replace_string(msg,  "$P", gender_pronoun(my_gender));
		    str2 = replace_string(str2, "$p", gender_self(your_gender));
		    str2 = replace_string(str2, "$N", my_name);
	    	str2 = replace_string(str2, "$n", gender_self(your_gender));
		    message("vision", str2, you);
		    str1 = replace_string(str1, "$p", gender_pronoun(your_gender));
	    	str1 = replace_string(str1, "$n", your_name);
		} else {
		    str1 = replace_string(str1, "$p", gender_self(my_gender) + "自己");
	    	str1 = replace_string(str1, "$n", gender_self(my_gender) + "自己");
		}
    }
    message("vision", str1, me);

    if(environment(me)) {
		str3 = replace_string(msg,  "$P", my_name);
		str3 = replace_string(str3, "$N", my_name);
		if( you ) {
		    if( you != me ) {
				str3 = replace_string(str3, "$p", your_name);
				str3 = replace_string(str3, "$n", your_name);
		    } else {
				str3 = replace_string(str3, "$p",
				gender_pronoun(my_gender) + "自己");
				str3 = replace_string(str3, "$n",
				gender_pronoun(my_gender) + "自己");
		    }
		}
		if( environment(me)->is_area() ) {
    		object *obs, ob;
    		mapping area_info = me->query("area_info");
    		if( !area_info ) return;
        	if( !environment(me)->check_scope(area_info["x_axis"], area_info["y_axis"]) ) return;
    		obs = environment(me)->query_inventory(area_info["x_axis"], area_info["y_axis"]);
        	foreach(ob in obs) {
        		if( objectp(ob) && ob != me && ob != you ) message("vision", str3, ob);
        	}

		} else message("vision", str3,  environment(me), ({ me, you }) );
    }
}

void tell_object(object ob, string str)
{
    if( ob ) message("tell_object", str, ob);
}

/*
  用法: 把讯息 str  送给房间 ob 中的所有物件. ob 可以是此房间的
        档案名称. 如果指定第三个参数 exclude, 在 exclude 这个阵
        列里的物件都收不到此段讯息.
*/
varargs void tell_room(mixed ob, string str, object *exclude)
{
    if( ob ) message("tell_room", str, ob, exclude);
}

void shout(string str)
{
    message("shout", str, users(), this_player());
}

void write(string str)
{
    if( this_player() ) message("write", str, this_player());
    else if( previous_object() ) message("write", str, previous_object());
    else efun::write(str);
}

varargs void say(string str, mixed exclude)
{
    if( living(previous_object()) ) {
    	if( environment(previous_object())->is_area() ) {
    		mapping area_info;
    		area_info = previous_object()->query("area_info");
    		if( !area_info ) return;
    		tell_area(environment(previous_object()), area_info["x_axis"], area_info["y_axis"], str, exclude);
    	} else message("say", str, environment(previous_object()), previous_object());
	} else if( this_player() ) {
    	if( environment(this_player())->is_area() ) {
    		mapping area_info;
    		area_info = previous_object()->query("area_info");
    		if( !area_info ) return;
    		tell_area(environment(previous_object()), area_info["x_axis"], area_info["y_axis"], str, exclude);
		} else message("say", str, environment(this_player()), this_player());
	}
}
