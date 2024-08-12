#ifndef __UI_H__
#define __UI_H__

#include <stdlib.h>

#include "mode.h"
#include "table.h"

// UI 最大长度
#define UI_MAX 1024

// 主界面
#define MAIN_SCREEN \
    ">>\n\
+----------------- 学生信息管理系统 -----------------+\n\
| 输入以下序号以进行相关操作:                        |\n\
| (1) 添加学生信息                                   |\n\
| (2) 输出所有学生信息                               |\n\
| (3) 查找学生                                       |\n\
| (4) 修改学生信息                                   |\n\
| (5) 删除学生                                       |\n\
| (6) 退出程序                                       |\n\
+----------------------------------------------------+\n\
>> 请输入您的选择:\n"

// 主界面错误提示
#define MAIN_ERROR "无效的操作, 请重新输入\n"

// 添加学生提示
#define CREATE_TIP \
    ">> 请按照 \"学号 姓名 成绩\" 的格式依次输入要添加的学生信息:\n"
// 添加学生成功提示
#define CREATE_SUCCESS "添加学生 %s 成功!\n"
// 添加学生失败提示
#define CREATE_FAILURE "添加学生失败，请重试！\n"

// 输出学生信息失败提示
#define PRINT_FAILURE "输出学生信息失败，请重试！\n"

// 查找学生提示
#define IF_QUERY_ID_TIP "是否设置学号查询条件？(y/n)\n"
#define INPUT_QUERY_ID "请输入学号：\n"
#define IF_QUERY_NAME_TIP "是否设置姓名查询条件？(y/n)\n"
#define INPUT_QUERY_NAME "请输入姓名：\n"
#define IF_QUERY_SCORE_TIP "是否设置成绩查询条件？(y/n)\n"
#define INPUT_QUERY_SCORE "请输入成绩：\n"
#define QUERY_CONDITION "请选择条件：\n1. 等于\n2. 大于\n3. 小于\n4. 大于等于\n5. 小于等于\n6. 不等于\n7. 取消\n"
#define QUERY_ERROR "无效的操作, 输入 b 返回主菜单\n"
#define QUERY_TIP ">> 请输入要查找的学生学号:\n"
// 查找学生成功提示
#define QUERY_SUCCESS \
    "查找到学生 %d, 信息如下:\n\
学号: %d\n\
姓名: %s\n\
成绩: %d\n"
// 查找学生失败提示
#define QUERY_FAILURE "查找学生失败，请重试！\n"

// 排序提示
#define IF_SORT "是否设置排序？(y/n)\n"
#define SORT_OPTION "请选择排序条件：\n1. 按学号排序\n2. 按成绩排序\n"
#define SORT_ERROR "无效的操作, 输入 b 返回主菜单\n"
#define SORT_DIRECTION "请选择排序方向:\n1. 升序\n2. 降序\n"

// 修改学生提示
#define UPDATE_TIP \
    ">> 请按照 \"学号 姓名 成绩\" 的格式依次输入要修改的学生信息:\n"
// 修改学生成功提示
#define UPDATE_SUCCESS "修改学生 %d 信息成功!\n"
// 修改学生失败提示
#define UPDATE_FAILURE "修改学生信息失败，请重试！\n"

// 删除学生提示
#define DELETE_TIP ">> 请输入要删除的学生学号:\n"
// 删除学生成功提示
#define DELETE_SUCCESS "删除学生 %d 成功!\n"
// 删除学生失败提示
#define DELETE_FAILURE "删除学生失败，请重试！\n"

// 返回主菜单
#define BACK_MAIN ">> 输入 b 返回主菜单\n"

/**
 * @brief UI 结构体表示用户界面
 */
typedef struct {
    char output[UI_MAX];
} UI;

/**
 * @brief 清空当前屏幕
 */
void clear_screen();

/**
 * @brief 将下一个字符串放入UI中。
 *
 * @param ui UI对象指针。
 * @param next 要放入UI的字符串。
 */
void ui_put_next(UI* ui, const char* next);

/**
 * @brief 打印表格
 *
 * @param ui UI对象指针。
 * @param table Table对象指针。
 */
void ui_print_table(UI* ui, Table* table);

/**
 * @brief 返回主界面
 *
 * @param mode 模式指针
 */
void ui_back_main(Mode* mode);

#endif
