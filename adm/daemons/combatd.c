/*  combatd.c 战斗精灵
*/

#pragma save_binary

#include <ansi.h>
#include <combat.h>
#include <skill.h>

inherit F_CLEAN_UP;
inherit F_DBASE;

void create()
{
    seteuid(getuid());
    set("name", "战斗精灵");
    set("id", "combatd");
}

string damage_msg(int damage)
{
	switch( damage ) {
	    case   0..2: return "结果只是勉强造成一处轻微伤害！"; break;
	    case   3..5: return "结果造成轻微的伤害！"; break;
	    case  6..10: return "结果造成一处伤害！"; break;
	    case 11..20: return "结果造成一处严重伤害！"; break;
	    case 21..30: return "结果造成颇为严重的伤害！"; break;
	    case 31..40: return "结果造成相当严重的伤害！"; break;
	    case 41..60: return "结果造成十分严重的伤害！"; break;
	    case 61..80: return "结果造成极其严重的伤害！"; break;
	    default: return "结果造成非常可怕的严重伤害！"; break;
	}
}

string status_msg(int ratio)
{
    switch( ratio ) {
		case    100: return HIG "( $N"HIG"看起来并没有受伤。 )\n" NOR; break;
		case 96..99: return HIG "( $N"HIG"似乎受了点轻伤，不过光从外表看不大出来。 )\n" NOR; break;
		case 91..95: return HIG "( $N"HIG"看起来可能受了点轻伤。 )\n" NOR; break;
		case 81..90: return HIG "( $N"HIG"受了几处伤，不过似乎并不碍事。 )\n" NOR; break;
		case 61..80: return HIY "( $N"HIY"受伤不轻，看起来状况并不太好。 )\n" NOR; break;
		case 41..60: return HIY "( $N"HIY"气息粗重，动作开始散乱，看来所受的伤著实不轻。 )\n" NOR; break;
		case 31..40: return HIY "( $N"HIY"已经伤痕累累，正在勉力支撑著不倒下去。 )\n" NOR; break;
		case 21..30: return HIR "( $N"HIR"受了相当重的伤，只怕会有生命危险。 )\n" NOR; break;
		case 11..20: return HIR "( $N"HIR"伤重之下已经难以支撑，眼看就要倒在地上。 )\n" NOR; break;
		case  5..10: return HIR "( $N"HIR"受伤过重，已经奄奄一息，命在旦夕了。 )\n" NOR; break;
		default:     return HIR "( $N"HIR"受伤过重，已经有如风中残烛，随时都可能断气。 )\n" NOR; break;
    }    
}

varargs void report_status(object ob)
{
    if( !ob->query_stat_maximum("hp") ) return;
    message_vision( status_msg((int)ob->query_stat("hp")*100/(int)ob->query_stat_maximum("hp")), ob);
}

varargs int fight(object me, object victim, string skill, mapping action, object weapon)
{
    int ability;
    string msg;

    if( environment(me)->query("no_fight") && !victim->query("unprotect_mark") && !me->query("unprotect_mark") )
        return 0;

    // 战斗过程断线，停止战斗
    if( ( userp(me) && !interactive(me) ) || ( userp(victim) && !interactive(victim) ) ) return 0;

    // 设定战斗讯息
    me->set_combat_message(action["action"]);
    me->set_temp("last_action", action);

    me->set_temp("evade_message", 0);
    me->set_temp("defend_message", 0);
    
    // 命中率 = 人物能力 + 攻击技能等级 + 招式命中率
    ability = me->exact() + action["exact"];
    
    // 攻击对象是否回避成功
    if( !victim->is_busy() && victim->evade(ability, weapon ? weapon : me) ) {
        // 回避成功，将回避的讯息加入战斗讯息。
        string evade_msg;
        evade_msg = victim->query_temp("evade_message");
        me->add_combat_message("，" + (stringp(evade_msg) ? evade_msg : "但是没中")  );

        // 被攻击者消秏精神
        if( userp(victim) && ability >= 40 ) victim->damage_stat("ap", random(ability/40)+1, me);
        else victim->damage_stat("ap", 1, me);

        ability = -1;
    } else {
    	// 回避失败，进行防御
    	string defend_msg;

        // 攻击伤害力
        if( weapon ) ability = weapon->inflict_damage();
        else ability = me->inflict_damage();

        ability = victim->defend(ability, weapon ? weapon : me);

        // 攻击者消秏精神
        if( userp(me) && ability >= 10) me->damage_stat("ap", random(ability/10) + 1, me);
        else me->damage_stat("ap", 1, me);

        // 加入防御讯息
        defend_msg = victim->query_temp("defend_message");
        me->add_combat_message("，" + (stringp(defend_msg) ? defend_msg : "$n举臂招架") );
    }
    
    if( ability > 0 ) {
        victim->receive_damage(ability, me);
        // 将伤害程度的讯息加入战斗讯息。
        me->add_combat_message("，" + damage_msg(ability) + "(" + ability+ ")\n");
    } else me->add_combat_message("。\n");

    // 送出战斗讯息。
    msg = me->get_combat_message();
    if( stringp(msg) ) {
        string *limbs = victim->query("limbs");
        if( !arrayp(limbs) || !sizeof(limbs) ) msg = replace_string(msg, "$l", "身上");
        else msg = replace_string(msg, "$l", limbs[random(sizeof(limbs))]);
        if( weapon ) msg = replace_string(msg, "$w", weapon->name());
        message_vision( msg, me, victim);
        if( ability > 0 ) report_status(victim);
    }

    // 传回非零值(伤害力)表示攻击者可获得经验
    if( living(victim) ) return ability;
    return 0;
}

void auto_fight(object me, object obj, string type)
{
    if( !userp(me) && !userp(obj) ) return;
    call_out( "start_" + type, 0, me, obj);
}

/* 结仇 */
void start_hatred(object me, object obj)
{
    if( !me || !obj ) return;

    if( !living(me) || (userp(obj) && !interactive(obj)) ) return;
    if( environment(me)->is_area() ) {
        if( !area_environment(me, obj) ) return;
    } else {
        if( environment(me) != environment(obj) ) return;
        if( environment(me)->query("no_fight") ) return;
    }

    if( me->is_fighting(obj) && obj->is_fighting(me) ) return;  // add by -Acme-

    message_vision( HIW"$N"HIW"和$n"HIW"仇人相见分外眼红，立刻打了起来！\n"NOR, me, obj);

    me->kill_ob(obj);
    if( userp(obj) ) obj->fight_ob(me);
    else obj->kill_ob(me);
//    me->attack(obj);
}

/* 世仇 */
void start_vendetta(object me, object obj)
{
    if( !me || !obj ) return;

    if( !living(me) || (userp(obj) && !interactive(obj)) ) return;
    if( environment(me)->is_area() ) {
        if( !area_environment(me, obj) ) return;
    } else {
        if( environment(me) != environment(obj) ) return;
        if( environment(me)->query("no_fight") ) return;
    }

    if( me->is_fighting(obj) && obj->is_fighting(me) ) return;  // add by -Acme-

    message_vision( HIW"$N"HIW"和$n"HIW"仇人相见分外眼红，立刻打了起来！\n"NOR, me, obj);

    me->kill_ob(obj);
    if( userp(obj) ) obj->fight_ob(me);
    else obj->kill_ob(me);
//    me->attack(obj);
}

/* 侵略 */
void start_aggressive(object me, object obj)
{
    int def;

    if( !me || !obj ) return;

    if( !living(me) || (userp(obj) && !interactive(obj)) ) return;
    if( environment(me)->is_area() ) {
        if( !area_environment(me, obj) ) return;
    } else {
        if( environment(me) != environment(obj) ) return;
        if( environment(me)->query("no_fight") ) return;
    }

    message_vision( HIW"$N"HIW"一见到$n"HIW"二话不说，立刻就打了起来！\n" NOR, me, obj);

    if( me->is_fighting(obj) && obj->is_fighting(me) ) return;  // add by -Acme-

    me->kill_ob(obj);
    if( userp(obj) ) obj->fight_ob(me);
    else obj->kill_ob(me);
//    me->attack(obj);
}

void announce(object ob, string event)
{
    switch(event) {
	    case "dead":
    		message_vision("\n$N呻吟了几声，咳出一口鲜血，便断了气。\n\n", ob);
	        break;
	    case "unconcious":
    	    message_vision("\n$N脚下一个不稳﹐跌在地上一动也不动了。\n\n", ob);
        	break;
	    case "revive":
    	    message_vision("\n$N慢慢睁开眼睛﹐清醒了过来。\n\n", ob);
        	break;
    }
}