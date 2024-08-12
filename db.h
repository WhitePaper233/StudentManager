#ifndef __DB_H__
#define __DB_H__
#include <stdbool.h>
#include <stdio.h>

// 数据库文件路径
#define DB_PATH "./students_data.bin"

/**
 * @brief 比较条件枚举
 *
 * 该枚举用于表示比较条件，包括大于、等于和小于。
 */
typedef enum {
    EQ = 1,
    GT = 2,
    LT = 3,
    GTE = 4,
    LTE = 5,
    NE = 6,
} Condition;

/**
 * @brief 过滤器结构体
 *
 * 该结构体用于表示一个过滤器，包括是否启用过滤条件、过滤条件的值和过滤条件的比较条件。
 */
typedef struct {
    bool enable_id, enable_name, enable_score;
    int id;
    char name[32];
    int score;
    Condition id_cond, score_cond;
} Filter;

/**
 * @brief 初始化过滤器
 *
 * @param filter 过滤器指针
 */
void filter_init(Filter* filter);

/**
 * @brief 设置过滤器的过滤条件
 *
 * @param filter 过滤器指针
 * @param id 学号
 * @param cond 比较条件
 */
void filter_set_id(Filter* filter, int id, Condition cond);

/**
 * @brief 设置过滤器的过滤条件
 *
 * @param filter 过滤器指针
 * @param name 姓名
 */
void filter_set_name(Filter* filter, const char* name);

/**
 * @brief 设置过滤器的过滤条件
 *
 * @param filter 过滤器指针
 * @param score 成绩
 * @param cond 比较条件
 */
void filter_set_score(Filter* filter, int score, Condition cond);

/**
 * @brief 学生信息结构体
 */
typedef struct {
    int id;         // 学号
    char name[32];  // 姓名
    int score;      // 成绩
} Student;

/**
 * @brief 链表节点结构体
 */
typedef struct __Node__ {
    Student student;        // 学生信息
    struct __Node__* next;  // 下一个链表节点
} Node;

/**
 * @brief 创建链表节点
 *
 * @param student 学生信息指针
 * @return Node* 返回创建的链表节点
 */
void node_free(Node* node);

/**
 * @brief 数据库结构体
 *
 * 该结构体用于表示一个数据库，包含数据大小、数据文件指针和数据链表。
 */
typedef struct {
    int size;    // 数据大小
    FILE* file;  // 数据文件指针
    Node* list;  // 数据链表
} DB;

/**
 * @brief 初始化数据库
 *
 * 这个函数用于初始化学生信息数据库
 *
 * @param db 指向数据库结构体的指针
 * @return int 返回是否初始化成功的整数，0表示成功，其他值表示失败
 */
int db_initialize(DB* db);

/**
 * @brief 插入学生信息到数据库中
 *
 * @param db 数据库指针
 * @param student 学生信息指针
 * @return int 返回插入操作的结果，成功返回0，其他值表示失败
 */
int db_insert(DB* db, const Student* student);

/**
 * @brief 删除数据库中的一条记录
 *
 * @param db 指向数据库结构体的指针
 * @param id 要删除的学生学号
 * @return int 返回删除操作的结果，成功返回0，其他值表示失败
 */
int db_delete(DB* db, const int id);

/**
 * @brief 查询数据库中的一条记录
 *
 * @param db 指向数据库结构体的指针
 * @param filter 查询条件
 * @return Node* 返回查询结果，如果没有找到则返回NULL
 */
Node* db_query(DB* db, const Filter filter);

/**
 * @brief 更新数据库中的学生信息
 *
 * @param db 数据库指针
 * @param student 学生信息指针
 * @return int 更新结果，0表示成功，-1表示失败
 */
int db_update(DB* db, const Student* student);

/**
 * @brief 提交数据库中的更改
 *
 * 此函数提交数据库中的更改，并确保它们被永久保存。
 *
 * @param db 指向数据库对象的指针。
 * @return 如果提交成功，则返回0，否则返回-1。
 */
int db_commit(DB* db);

/**
 * @brief 重新加载数据库
 *
 * 此函数重新加载由DB指针指定的数据库。
 *
 * @param db 指向数据库的指针。
 * @return int 如果数据库成功重新加载，则返回0，否则返回非零值。
 */
int db_reload(DB* db);

/**
 * @brief 释放数据库资源
 *
 * 释放数据库资源，包括关闭数据库连接、释放内存等操作。
 *
 * @param db 数据库指针
 */
void db_free(DB* db);

#endif
