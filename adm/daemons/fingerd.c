/*  fingerd.c */ 

#pragma save_binary

#include <origin.h>
#include <ansi.h>

inherit F_CLEAN_UP;

string *hide_ip = ({ "acme", "jolin" });

private object acquire_login_ob(string id);

private void create() { seteuid(getuid()); }

string age_string(int time)
{
    int month, day, hour;

    time /= 3600;
    hour = time % 24;
    time /= 24;
    day = time % 30;
    month = time / 30;
    return (month ? month + "m" : "") + (day ? day + "d" : "") + hour + "h";
}

string finger_all()
{
    object *ob;
    string msg;
    int i;

    ob = sort_array( users(), (: strcmp(query_ip_number($1), query_ip_number($2)) :) );
    msg  = "╭────────────────────────────────────╮\n";
    msg += "│"BBLU"        名称(ID)        "NOR" "BGRN"               DNS             "NOR" "BYEL"发呆"NOR" "BMAG" 连线时数 "NOR"│\n";
    for(i=0; i<sizeof(ob); i++) {
	    if( this_player() && !ob[i]->visible(this_player()) ) continue;
	    msg += sprintf("│%12s%-12s %|31s %|4d %|10s│\n",
	              ob[i]->query("name"),
	              "("+ ob[i]->query("id") + ")",
	              (member_array(ob[i]->query_id(), hide_ip) != -1 ) ? "127.0.0.1" : query_ip_name(ob[i]),
                  (int)query_idle(ob[i])/60,
	              ob[i]->link() ? age_string(ob[i]->link()->query("time_aged")) : "n/a"
	          );
    }
    msg += "╰────────────────────────────────────╯";
    return msg;
}

string finger_user(string name)
{
    object ob, body;
    string msg, mud;
    int invis, age, wiz_mode = 0;

    ob = acquire_login_ob(name);
    if( !ob ) return "没有这位使用者。\n";
    body = find_player(name);

    if( this_player(1) && (wizardp(this_player(1)) || body==this_player(1)) ) wiz_mode = 1;

    age = ob->query("time_aged");
    invis = ob->query("invis") && (!body || !this_player() || wiz_level(body) > wiz_level(this_player()));

    msg  = "╭─────────────────────────────╮\n";
    msg += sprintf("│"BBLU"姓    名：%-24s  权限等级：%-12s"NOR"│\n",
                   ob->query("name") + "(" + capitalize(ob->query("id")) + ")",
                   wizhood(name) );
    msg += "│                                                          │\n";
    msg += sprintf("│电子邮件：%-48s"NOR"│\n", wiz_mode ? ob->query("email") : "<机密>" );
    if( wiz_mode ) {
        if( member_array(ob->query("id"), hide_ip) != -1 )
        	msg += sprintf("│上次连线：%-48s"NOR"│\n", "127.0.0.1" + " (" + CHINESE_D->chinese_date((int)ob->query("last_on")) + ")");
        else msg += sprintf("│上次连线：%-48s"NOR"│\n", ob->query("last_from") + " (" + CHINESE_D->chinese_date((int)ob->query("last_on")) + ")" );
    } else msg += sprintf("│上次连线：%-48s"NOR"│\n", "<机密>" + " (" + CHINESE_D->chinese_date((int)ob->query("last_on")) + ")");
    msg += sprintf("│时数总合：%-48s"NOR"│\n", chinese_period(age) );
    msg += "│                                                          │\n";
	if( objectp(body) ) {
		if( interactive(body) ) {
			if( wiz_mode ) {
			    if( member_array(ob->query("id"), hide_ip) != -1 )
			    	msg += sprintf("│连线ＩＰ："HIG"<连线中>"NOR" %-39s│\n", "127.0.0.1");
			    else msg += sprintf("│连线ＩＰ："HIG"<连线中>"NOR" %-39s│\n", query_ip_name(body));
			} else msg += sprintf("│连线ＩＰ："HIG"<连线中>"NOR" %-39s│\n", "<机密>");
            msg += sprintf("│连线时数：%-48s"NOR"│\n", chinese_period(time() - ob->query_temp("login_time")) );
        } else msg += sprintf("│连线ＩＰ："HIR"<断线中>"NOR" %-39s│\n", "");
    }
    msg += "┴─────────────────────────────┴\n";

    if( ob->query("profile") ) msg += ob->query("profile");

    msg += "───────────────────────────────\n";

    if( !ob->body() ) destruct(ob);

    return msg;
}

object acquire_login_ob(string id)
{
    object ob;

    if( origin()==ORIGIN_CALL_OTHER ) {
	if( geteuid(previous_object())!=ROOT_UID
	&&  base_name(previous_object())!=MAILBOX_OB ) return 0;
    }

    if( ob = find_player(id) ) {
	// Check if the player is linkdead
	if( ob->link() ) return ob->link();
    }

    ob = new(LOGIN_OB);
    ob->set("id", id);
    return ob->restore() ? ob : 0;
}
