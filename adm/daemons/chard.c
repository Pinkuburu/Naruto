/*  chard.c - protected character operation daemon

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma save_binary

#include <login.h>
#include <statistic.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

// setup_char()
//
// This function is called when characters need setup (after create() is
// called). The major task of this function is to keep consistency of
// user data and NPCs. Because the game system may change with development
// of mudlib, there might be minor or major changes in creating character
// objects. Also, this function serves as default creation function of 
// NPCs that saves wizards' work on initializing NPCs.

/*  setup()
 *
 *  一般性的人物设定函数。这个函数用来对人物物件做一些初始化的设定。
 */
void setup_char(object ob)
{
    mapping apply;

    // 如果人物没有设定种族、职业、等级，给一个预设值
    if( !ob->query_race() ) ob->set_race("human");
    if( !ob->query_class() ) ob->set_class("commoner");
    if( !ob->query_level() ) ob->set_level(1);

    ob->init_statistic();
    ob->init_attribute();

    // 初始化人物的暂时状态
    if( mapp(apply = ob->query("perm_apply") ) ) ob->set_temp("apply", copy(apply));
    else ob->set_temp("apply", ([]));

/* 
    针对人物的生命型态(life_form)，执行系统预设的设定
    TYPE_STATIC        这项基本数值是不会自动更新的
    TYPE_HEALTH     这项基本数值是会随时间自动恢复的
    TYPE_WASTING    这项基本数值是会随时间慢慢减少的
*/
    ob->set_stat_regenerate("ap", TYPE_HEALTH);
    ob->set_stat_regenerate("hp", TYPE_HEALTH);
    ob->set_stat_regenerate("mp", TYPE_HEALTH);

    // 执行种族及职业的初始化，种族和职业物件可以修改前面初始化过的属性。
    RACE_D(ob->query_race())->setup(ob);
    CLASS_D(ob->query_class())->setup(ob);
    ob->add_path("/daemon/race/" + ob->query_race() + "/");
    ob->add_path("/daemon/class/" + ob->query_class() + "/");
}

varargs object make_corpse(object victim, object killer)
{
    int amount;
    mapping bounty;
    object corpse, inv;
    string score, corpse_ob;

    // 巫师不需要产生尸体
    if( wizardp(victim) ) return 0;

    // Notify inventory item to save data if nessessary.
    all_inventory(victim)->owner_is_killed(killer);

    // Check if using custom corpse object.
    corpse_ob = victim->query("corpse_ob");
    if( !corpse_ob ) corpse_ob = CORPSE_OB;

    seteuid(MUDLIB_UID);
    catch(corpse = new(corpse_ob));
    seteuid(getuid());

    corpse->set_name( victim->name(1) + "的尸体", "corpse of " + capitalize(victim->query("id")), "corpse" );
    corpse->set("long", "一具" + victim->name(1) + "的尸体，看起来歪七扭八的且不断散发出恶臭。\n");
    corpse->set("age", victim->query("age"));
    corpse->set("gender", victim->query("gender"));
    corpse->set("victim_name", victim->name(1));
    corpse->set("level", victim->query_level());
    corpse->set("userp", userp(victim));

    corpse->set_weight( victim->query_weight() );
    corpse->set_max_encumbrance( victim->query_max_encumbrance() );
    corpse->start_decay( userp(victim) ? 120 : 60 );

    if( environment(victim)->is_area() ) {
        area_move(environment(victim), corpse, victim->query("area_info/x_axis"), victim->query("area_info/y_axis"));
    } else corpse->move(environment(victim));

    // 身上所有物品转移到尸体上
    foreach(inv in all_inventory(victim)) inv->move(corpse);

    victim->set_temp("corpse", corpse);

    if( !killer ) killer = victim->last_damage_giver();

    if( objectp(killer) ) {
        object *team;

        // 杀人者是npc且是某人所拥有，那杀人者即是某人
        if( !userp(killer) && objectp(killer->query_owner()) ) killer = killer->query_owner();

        team = killer->query_team();

        // 登录世仇标记
        if( victim->query("vendetta_mark") ) killer->add("vendetta/" + victim->query("vendetta_mark"), 1);

        // 如果杀的是人类的话加杀业 1
        if( victim->query("humanoid") ) killer->gain_score("kills", 1);

        // 获得经验 (如果victim有设定bounty的话)
        if( mapp(bounty = victim->query("bounty")) ) {
            foreach(score, amount in bounty) {
                if( team ) {
                    object ob;
                    if( environment(killer)->is_area() ) {
                        foreach(ob in team) if( area_environment(killer, ob) )
                            ob->gain_score(score, amount);
                    } else {
                        foreach(ob in team) if( environment(killer) == environment(ob) )
                            ob->gain_score(score, amount);
                    }
                }
                else killer->gain_score(score, amount);
            }
        }

        // 增加学习点数
        if( team ) {
            object ob;
            if( environment(killer)->is_area() ) {
                foreach(ob in team) if( area_environment(killer, ob) )
                    ob->add_point("learn", victim->query_level());
            } else {
                foreach(ob in team) if( environment(killer) == environment(ob) )
                    ob->add_point("learn", victim->query_level());
            }
        } else killer->add_point("learn", victim->query_level());
        
        // Finally, let class daemon check this kill.
        //    CLASS_D(killer->query_class())->kill_check(killer, victim, corpse);
    }        
    return corpse;
}