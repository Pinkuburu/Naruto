#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string str)
{
	object ob, where, *ob_list;
	string msg="", dir_str;
	int i;

	if (!str) {
		ob_list = sort_array( users(), (:
			strcmp( environment($1) ? file_name(environment($1)) : "",
					environment($2) ? file_name(environment($2)) : "" )
		:) );
        msg  = "╭──────────────────────────────────────╮\n";
        msg += "│"BBLU"        名称(ID)        "NOR" "BGRN"                       地   点                     "NOR"│\n";
		foreach(ob in ob_list) {
			if( !environment(ob) || !ob->visible(this_player()) ) continue;
			msg += sprintf("│%12s%-12s %-51s│\n",
				ob->name(),
				"("+ ob->query_id() +")",
				file_name(environment(ob)) + 
				( environment(ob)->is_area() ? " (" + ob->query("area_info/x_axis") + "," + ob->query("area_info/y_axis") + ")" : "")
			);
		}
        msg += "╰──────────────────────────────────────╯";
		me->start_more(msg);
		return 1;
	}

	ob = find_player(str);
	if( !ob ) ob = find_living(str);
	if( !ob ) ob = LOGIN_D->find_body(str);
	if( ob && ob->visible(me) ) {
		where = environment(ob);
		if (!where) return notify_fail("这个人不知道在那里耶...\n");
		printf("%s(%s) 现在在 %s (%s), %s\n",
			ob->name(),
			ob->query("id"),
			where->is_area() ? where->query("name") + "("+ob->query("area_info/x_axis")+","+ob->query("area_info/y_axis")+")": where->query("short"),
			file_name(where),
			ob->link() ? ( living(ob) ? "发呆 " + query_idle(ob)/60 + " 分钟" : "昏倒中") : (userp(ob) ? "断线中":"NPC 类"), 
		);
		return 1;
	}

	// locate the children of object
	dir_str = resolve_path(me->query("cwd"), str);
	if( sizeof(ob_list = children(dir_str)) ) {
		for(i=0; i<sizeof(ob_list); i++) {
			if( !ob = environment(ob_list[i]) ) continue;
			msg += sprintf("%O\n", ob );
			// show until ob is room
			while( ob = environment(ob) )
				msg += sprintf("%O\n", ob );
			msg += "\n";
		}
		msg += "Object " + dir_str + ".c located.\n";
		this_player()->start_more(msg);
		return 1;
	}
	if (!ob) return notify_fail("现在没 " + str + " 这个人物 (include ppl and npc), 也没有 " + dir_str + ".c 的存在.\n");
}

int help(object me)
{
	write(@HELP
指令格式: where <参数>

参数可为:
无          列出所有 users 所在位置。
grain       找使用者 "grain" . 若无, 则列出全名为 "grain" 之 npc位置, 且会
            秀出 "发呆", "昏倒", "断线", "NPC" 等状态显示。
kao yeu     列出全名 ( set_name 中第一个 string ), 为前述之 npc位置。
/obj/bottle 列出所有档名为前述之物件位置, 且会一层层秀出, 直到所处环境为
            room 为止。
bottle      同上行, 但只搜寻当前目录 (current directory)。
bottle.c    同上行。
HELP);
	return 1;
}
