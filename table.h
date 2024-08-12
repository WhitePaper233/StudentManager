#ifndef __TABLE_H__
#define __TABLE_H__
#include <stdbool.h>
#include "db.h"

#define DEFAULT_TABLE_CAP 16

/**
 * @brief 排序字段
 *
 * 该枚举类型定义了排序字段
 */
typedef enum {
    ID = 1,
    SCORE = 2,
} SortField;

/**
 * @brief 排序方式
 *
 * 该枚举类型定义了排序方式
 */
typedef enum {
    ASC = 1,   // 升序
    DESC = 2,  // 降序
} SortType;

/**
 * @brief 排序条件
 *
 * 该结构体定义了排序条件
 */
typedef struct {
    bool enable;
    SortType sort_type;
    SortField sort_by;
} SortCondition;

/**
 * @brief 初始化排序条件
 *
 * 该函数用于初始化排序条件
 *
 * @param sc 指向排序条件结构体的指针
 * @param enable 是否启用排序
 */
void sort_condition_init(SortCondition* sc, bool enable);

/**
 * @brief 设置排序方式
 *
 * 该函数用于设置排序方式
 *
 * @param sc 指向排序条件结构体的指针
 * @param type 排序方式
 */
void sort_condition_set_type(SortCondition* sc, SortType type);

/**
 * @brief 设置排序字段
 *
 * 该函数用于设置排序字段
 *
 * @param sc 指向排序条件结构体的指针
 * @param field 排序字段
 */
void sort_condition_set_field(SortCondition* sc, SortField field);

/**
 * @brief 表格结构体
 */
typedef struct {
    int len, cap;
    Student* records;
} Table;

/**
 * @brief 初始化表格
 *
 * 这个函数用于初始化表格
 *
 * @param table 指向表格结构体的指针
 * @return int 返回是否初始化成功的整数，0表示成功，其他值表示失败
 */
int table_init(Table* table);

/**
 * @brief 添加新记录
 *
 * 这个函数用于向表格中添加一条新记录
 *
 * @param table 指向表格结构体的指针
 * @param record 需要添加的记录
 */
void table_append(Table* table, Student record);

/**
 * @brief 打印表格
 *
 * 该函数用于向终端格式化输出表格
 *
 * @param table 指向表格结构体的指针
 */
void table_print(Table* table);

/**
 * @brief 释放表格
 *
 * 该函数用于释放表格内存空间
 *
 * @param table 指向表格结构体的指针
 */
void table_free(Table* table);

/**
 * @brief 表格排序
 *
 * 该函数用于排序表格
 *
 * @param table 指向表格结构体的指针
 * @param sort_type 排序方式
 */
void sort_table(Table* table, SortCondition sc);

#endif