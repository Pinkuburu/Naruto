/* quest.c wirte by -Acme-

   任务的格式
   ID = ([ "name":                  // 任务名称
           "detail":                // 任务细节说明
           "trigger":               // 任务触发语句
           "end":                   // 任务结束必要达到的任务值
        ]);
*/

#include <ansi.h>
//#include <quest.h>

inherit "/adm/daemons/questd.c";

int do_list_quest(string branch, string name)
{
    int index, size;
    mapping bran;
    string msg;

    msg  = "╭─────────────────────────────╮\n";
    msg += sprintf("│%-58s│\n", name+"任务列表" );
    msg += "├─────────────────────────────┤\n";
    msg += sprintf("│"BRED"%6s"NOR" "BBLU"%|51s"NOR"│\n", " 编号 ", " 任务名称 ");

    if( mapp(bran = QUEST_D->query_branch(branch)) && (size = sizeof(bran)) ) {
        for(index=1;index<=size;index++) {
            if( undefinedp(bran[index]) ) continue;
            msg += sprintf("│%|6d %-51s│\n", index, bran[index]["name"] );
        }
    }

    msg += "╰─────────────────────────────╯";

    this_player()->start_more(msg);

    return 1;
}

int do_show_detail(string branch, int id)
{
    string msg;
    mapping quest;

    if( !mapp(quest = QUEST_D->query_quest(branch, id)) )
        return notify_fail("没有这个任务编号。\n");

    msg  = sprintf(BRED"%|6d"NOR" "BBLU"%|55s"NOR"\n", id,  quest["name"] );
    msg += "───────────────────────────────\n";
    msg += quest["detail"] + "\n";
    msg += "───────────────────────────────";
    if( wizardp(this_player()) ) {
        msg += "\n触发句: " + quest["trigger"] + "\n";
        msg += "结束步骤: " + quest["end"];
    }

    this_player()->start_more(msg);

    return 1;
}

int main(object me, string arg)
{
    int id;
    string select;

    if( !arg ) return notify_fail("请输入 help quest 了解指令格式。\n");

    if( sscanf(arg, "-%s %d", select, id) == 2 )
        return do_show_detail(select , id);

    if( sscanf(arg, "-%s", select) == 1 ) {
        switch( select ) {
            case "mu": return do_list_quest("mu", "木叶村"); break;  // 木叶
            case "yi": return do_list_quest("yi", "音隐村"); break;  // 音隐
            case "wn": return do_list_quest("wn", "雾隐村"); break;  // 雾隐
            case "sn": return do_list_quest("sn", "砂隐村"); break;  // 砂隐
            case "yn": return do_list_quest("yn", "雨隐村"); break;  // 雨隐
            case "yy": return do_list_quest("yy", "云隐村"); break;  // 云隐
            case "cn": return do_list_quest("cn", "草隐村"); break;  // 草隐
            case "pn": return do_list_quest("pn", "瀑隐村"); break;  // 瀑隐
            case "or": return do_list_quest("or", "其它"); break;    // 其它
            default: return notify_fail("参数错误，请输入 help quest 了解指令格式。\n");
        }
    }

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式：
  quest <忍者村选项>           列出某忍者村的所有任务
                               如： quest -mu 会列出所有木叶村的任务列表。
  quest <忍者村选项> <编号>    如： quest -mu 1 会显示木叶村编号为1的这个任务说明

忍者村选项：
  -mu 木叶村(muye)             -yi 音隐村(yinyin)
  -wn 雾隐村(wuyin)            -sn 砂隐村(shayin)
  -yn 雨隐村(yuyin)            -yy 云隐村(yunyin)
  -cn 草隐村(caoyin)           -pn 瀑隐村(puyin)
  -or 其它(other)
HELP);
    return 1;
}