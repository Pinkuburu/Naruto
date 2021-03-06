/* 忍术：替身术(substitute) */

#include <ansi.h>

inherit SKILL;

string *dodge_msg = ({
    "只听见“碰”的一声，原来$N攻击的是一块大木头",
    "刹那间$n的身体变成了一块大木头",
    "一阵轻烟过去，原来$N攻击的是一块大木头",
});


void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon("substitute");
    setup();
}

// 应用在 dodge 上
int valid_enable(string base_skill) { return base_skill=="dodge"; }

int perform_action(object me, string act, object target)
{
    return notify_fail("现在没有特殊招式可以用。\n");
}

// 忍术
string query_class() { return "ninja"; }

// 显示用，用于 score
int dodge_show(object me, int ability)
{
    if( !me ) return 0;
    if( !me->skill_mapped("dodge") ) return 0;
    if( living(me) ) me->set_temp("evade_message", dodge_msg[random(sizeof(dodge_msg))]);
    return me->query_skill("dodge", 1) + me->query_skill("substitute", 1);
}

int dodge_using(object me, int ability)
{
    int dodge, sub;
    
    if( !me ) return 0;
    if( !me->skill_mapped("dodge") ) return 0;
    
    dodge = SKILL_D("dodge")->dodge_using(me, ability);
    sub = me->query_skill("substitute");

    if( ability && random(200) < random(sub) ) {
        if( living(me) ) me->set_temp("evade_message", dodge_msg[random(sizeof(dodge_msg))]);

        // 消秏一些查克拉
        me->damage_stat("mp", 1+random(5), me);

        // 增加经验值
        me->improve_skill("substitute", 1);

        dodge += sub;
    }

    return dodge;
}

// 技能增加经验, 不需做其它事
void skill_improved(object me, string skill) { }

// 技能升级后会呼叫此函式，必要！
void skill_advanced(object me, string skill)
{
    // 在这里设定技能下次升级所需的经验

    int lv;

    lv = me->query_skill(skill, 1);	// 技能等级

    switch( lv ) {
        case 189: me->set_skill_require(skill, 1500000);  break; // 关卡3 : 189升190 需要一百五十万经验
        case 149: me->set_skill_require(skill, 1000000);  break; // 关卡2 : 149升150 需要一百万经验
        case  99: me->set_skill_require(skill, 500000);   break; // 关卡1 :  99升100 需要五十万经验
        default:  me->set_skill_require(skill, lv*lv*5); break;
    }
}