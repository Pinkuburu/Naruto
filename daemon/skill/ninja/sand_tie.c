/* 忍术：砂缚柩(sand tie) */

#include <ansi.h>

#define SKILL_NAME "sand tie"

inherit SKILL;

// 忍术
string query_class() { return "ninja"; }

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon(SKILL_NAME);
    setup();
}

// 技能增加经验, 不需做其它事
void skill_improved(object me, string skill) { }

// 技能升级后会呼叫此函式，必要！
void skill_advanced(object me, string skill)
{
    // 在这里设定技能下次升级所需的经验

    int lv;

    lv = me->query_skill(skill, 1);					// 技能等级

    switch( lv ) {
    	case 189: me->set_skill_require(skill, 1500000);  break; // 关卡3 : 189升190 需要一百五十万经验
    	case 149: me->set_skill_require(skill, 1000000);  break; // 关卡2 : 149升150 需要一百万经验
    	case  99: me->set_skill_require(skill, 500000);   break; // 关卡1 :  99升100 需要五十万经验
    	default:  me->set_skill_require(skill, lv*lv*5); break;
    }
}