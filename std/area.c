/* area.c write by -Acme-
   这里负责区域物件的载入、清除、重设等工作。
*/

#pragma save_binary

#include <area.h>
#include <dbase.h>

inherit F_DBASE;
inherit F_CLEAN_UP;
inherit F_SAVE;

inherit AREA_MAP;

// 区域载入的物件集
// Location Of Loaded Objects
// 所有区域载入之物件的座标另成一个集合，以方便快速查找
string *LOLO = ({});

////////////////////////////////////////////////////////////
// 一般设定、检查、查询
////////////////////////////////////////////////////////////
// 传回有载入物件的座标集
string *query_LOLO() { return LOLO; }
// 加入一个座标元素在LOLO集中
void add_LOLO(string location) { if( member_array(location, LOLO) == -1 ) LOLO += ({ location }); }
// 删除一个座标元素在LOLO集中
void del_LOLO(string location) { if( member_array(location, LOLO) != -1 ) LOLO -= ({ location }); }

// 储存区域资料的路径位置
string query_save_file() { return file_name(this_object()); }

// 绝对接受物件移入区域物件
int receive_object(object ob, int from_inventory) { return 1; }

// 区域检查
int is_area() { return 1; }

// 设定某座标要载入的物件档
int set_loads(int x, int y, string filename, int amount)
{
	if( !check_scope(x, y) ) return 0;
	if( amount <= 0 ) return 0;
	if( undefinedp(area[y][x]["loads"]) )
		area[y][x]["loads"] = ([]);
	area[y][x]["loads"][filename] = amount;
	
	// 加入快速搜寻集
	add_LOLO((string)y+","+(string)x);
	return 1;
}

// 将某座标要载入的物件档移除
int del_loads(int x, int y)
{
	if( !check_scope(x, y) ) return 0;
	if( undefinedp(area[y][x]["loads"]) ) return 1;
	map_delete(area[y][x], "loads");
	
	// 移除快速搜寻集
	del_LOLO((string)y+","+(string)x);
	return 1;
}

// 将某座标的已载入物件移除
int del_loaded(int x, int y)
{
	if( !check_scope(x, y) ) return 0;
	if( undefinedp(area[y][x]["loaded"]) ) return 1;
	map_delete(area[y][x], "loaded");
	return 1;
}

////////////////////////////////////////////////////////////
// 区域里的物件载入、清除
////////////////////////////////////////////////////////////
object make_inventory(string file, int x, int y)
{
	string *exits;
    object ob;

    file = file_path(file);
    ob = new(file);

    // Support for uniqueness
    if( ob->violate_unique() ) ob = ob->create_replica();
    if( !ob ) return 0;

    ob->set("area_info/x_axis", x);
    ob->set("area_info/y_axis", y);
    ob->set("area_info/x_axis_old", x);
    ob->set("area_info/y_axis_old", y);

    // 将物件移到区域里，并检查是否有guard某方向
	if( move_in(x, y, ob) ) {
		if( (exits=ob->query("guard_exit")) ) {
			int i = sizeof(exits);
			while(i--) this_object()->set_area_guard(x, y, exits[i], ob);
		}
	    ob->move(this_object());
	}
	else destruct(ob);

    return ob;
}

void reset_callout(int temp)
{
    int i, j, x, y, amount, t=0;
    mapping ob;
	string file;

	if( !sizeof(LOLO) || temp >= sizeof(LOLO) ) return;
	
	for(i=temp;i<sizeof(LOLO);i++) {
        if( sscanf(LOLO[i], "%d,%d", y, x) != 2 ) continue;
        if( !check_scope(x, y) ) continue;
        if( undefinedp(area[y][x]["loads"]) ) continue;
        if( !mapp(ob = area[y][x]["loaded"]) ) ob = ([]);
        foreach( file, amount in area[y][x]["loads"] ) {
            for(j=amount;j>0;j--) {
                if( objectp(ob[file + " " + j]) ) continue;
                ob[file + " " + j] = make_inventory(file, x, y);
            }
            area[y][x]["loaded"] = ob;
        }
        t++;
        if( t >= 5 ) {
            call_out("reset_callout", 1+random(3), i+1);
            return;
        }
    }
	return;
}

void reset()
{
    int i, amount, x, y;
    mapping ob;
    string file, location;

    // 清空LOO座标集
    LOO = ({});

    // 物件载入集有资料, 递回延迟呼叫产生mob以免cost过多
    call_out("reset_callout", 1, 0);
    return;
}

int clean_up(int inherit_flag)
{
    int i, y, x;
    mapping items;
    string file;
    object ob;

    i = sizeof(LOLO);
    while(i--)
		if( sscanf(LOLO[i], "%d,%d", y, x) == 2 ) {
        	if( !mapp(items = query_loaded(x, y)) ) continue;
        	foreach(file, ob in items)
	    		if( objectp(ob) && ob->is_character() && environment(ob) != this_object() )
					return 1;
    	}

    return ::clean_up(inherit_flag);
}

// 区域消灭时呼叫此函式
void remove()
{
    int i, y, x, cnt;
    mapping items;
    string file, *objects;
    object ob;

	i = sizeof(LOLO);
	while(i--)
		if( sscanf(LOLO[i], "%d,%d", y, x) == 2 ) {
			if( !mapp(items = query_loaded(x, y)) ) continue;
			foreach(file, ob in items) {
				if( objectp(ob) ) {
					message("vision", ob->name() + "化成轻飘飘的白烟消散了。\n", environment(ob));
					destruct(ob);
					cnt++;
			}
		}
		// 清空座标物件集，以免往后出错
		map_delete(area[y][x], "objects");
		
        // 清空座标已载入物件之资料，以免往后出错 
        map_delete(area[y][x], "loaded");
    }

    if( cnt && this_player() ) write("警告：" + cnt + " 个NPC由此区域创造并被强制删除。\n");
}

int save()
{
	int i, j, x_size, y_size;
    string *LOO_BAK;
    mapping ICON_BAK;
    mixed *AREA_BAK;
    
	// 存档前将不需要储取的变数清空，以免往后发生无法预期的错误
	LOO_BAK = LOO;
	ICON_BAK = icon;
	AREA_BAK = area;
	
	LOO = ({});
	icon = ([]);
	
	x_size = this_object()->query("x_axis_size");
	y_size = this_object()->query("y_axis_size");
	
    // 每格座标中的暂时变数也要清空
	for(i=0;i<y_size;i++) {
		for(j=0;j<x_size;j++) {
            map_delete(area[i][j], "loaded");
            map_delete(area[i][j], "objects");
        }
    }
	
	if( ::save() ) {
		LOO = LOO_BAK;
		icon = ICON_BAK;
		area = AREA_BAK;
		return 1;
	}
	
	return 0;
}

void setup()
{
	int i;
    string file;

	seteuid(getuid());

	// 没有储存档
	if( !restore() ) {
    	int j, y_size, x_size;

        // 将区域资料变数 area 作初始化
        y_size = this_object()->query("y_axis_size");
        x_size = this_object()->query("x_axis_size");
        if( !y_size || y_size<AREA_YAXIS_MIN || y_size>AREA_YAXIS_MAX ) y_size = DEFAULT_YAXIS_SIZE;
        if( !x_size || x_size<AREA_XAXIS_MIN || x_size>AREA_XAXIS_MAX ) x_size = DEFAULT_XAXIS_SIZE;

        // 要求记忆体
        area = allocate( y_size );
        for(i=0;i<sizeof(area);i++) {
            area[i] = allocate( x_size );
            for(j=0;j<sizeof(area[i]);j++) area[i][j]=([]);
        }
        
        // 清空相关物件集
        LOO = ({});
        LOLO = ({});
        
        // 储存
        save();
	}

    // update 相对路径
    file = base_name(this_object());
    i = strlen(file);
    while(i--) if( file[i..i] == "/" ) break;
    set("file_path", file[0..i] );
    
	this_object()->reset();

}
