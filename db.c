#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"

/**
 * @brief 判断是否符合条件
 *
 * @param cond 条件
 * @param val 值
 * @param target 目标值
 *
 * @return 是否符合条件
 */
bool filte(Condition cond, int val, int target) {
    switch (cond) {
        case EQ:
            return val == target;
        case GT:
            return val > target;
        case LT:
            return val < target;
        case GTE:
            return val >= target;
        case LTE:
            return val <= target;
        case NE:
            return val != target;
        default:
            return false;
    }
}

void filter_init(Filter* filter) {
    filter->enable_id = false;
    filter->enable_name = false;
    filter->enable_score = false;
}

void filter_set_id(Filter* filter, int id, Condition cond) {
    filter->enable_id = true;
    filter->id = id;
    filter->id_cond = cond;
}

void filter_set_name(Filter* filter, const char* name) {
    filter->enable_name = true;
    strcpy(filter->name, name);
}

void filter_set_score(Filter* filter, int score, Condition cond) {
    filter->enable_score = true;
    filter->score = score;
    filter->score_cond = cond;
}

void node_free(Node* node) {
    if (node->next != NULL) {
        node_free(node->next);
    }
    free(node);
}

int db_initialize(DB* db) {
    // 判断数据库文件是否存在
    FILE* fp = fopen(DB_PATH, "r");
    if (fp == NULL) {
        // 不存在则创建
        fp = fopen(DB_PATH, "wb");
        if (fp == NULL) {
            return -1;
        }
    }
    fclose(fp);

    // 打开数据库文件
    db->file = fopen(DB_PATH, "rb+");
    if (db->file == NULL) {
        return -1;
    }

    // 读取数据库大小
    fseek(db->file, 0, SEEK_END);
    db->size = ftell(db->file) / sizeof(Student);
    fseek(db->file, 0, SEEK_SET);

    // 如果数据库大小为 0，则创建一个空链表
    if (db->size == 0) {
        db->list = NULL;
        return 0;
    }

    // 如果有数据 则从文件创建数据链表
    db->list = (Node*)malloc(sizeof(Node));
    Node* node = db->list;
    for (int i = 0; i < db->size; ++i) {
        fread(&(node->student), sizeof(Student), 1, db->file);
        // 如果读取到文件末尾，则跳出循环
        if (i == db->size - 1) {
            node->next = NULL;
            break;
        }
        // 创建下一个节点
        node->next = (Node*)malloc(sizeof(Node));
        node = node->next;
    }
    fseek(db->file, 0, SEEK_SET);

    return 0;
}

int db_insert(DB* db, const Student* student) {
    // 创建新节点
    Node* node = (Node*)malloc(sizeof(Node));
    node->student.id = student->id;
    strcpy(node->student.name, student->name);
    node->student.score = student->score;
    node->next = NULL;

    // 如果数据库为空，则直接插入
    if (db->list == NULL) {
        db->list = node;
    } else {
        // 如果数据库不为空，则插入到链表尾部
        Node* p = db->list;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = node;
    }
    ++(db->size);

    // 保存更改
    if (db_commit(db) != 0) {
        return -1;
    }

    // 重新加载数据库
    if (db_reload(db) != 0) {
        return -1;
    }

    return 0;
}

int db_delete(DB* db, const int id) {
    // 如果数据库为空，则直接返回
    if (db->list == NULL) {
        return -1;
    }

    // 如果要删除的是第一个节点
    if (db->list->student.id == id) {
        Node* node = db->list;
        db->list = node->next;
        free(node);
    } else {
        // 遍历链表
        Node* node = db->list;
        while (node->next != NULL) {
            if (node->next->student.id == id) {
                Node* next = node->next;
                node->next = next->next;
                free(next);
                break;
            }
            node = node->next;
        }
    }
    --(db->size);

    // 保存更改
    if (db_commit(db) != 0) {
        return -1;
    }

    // 重新加载数据库
    if (db_reload(db) != 0) {
        return -1;
    }

    return 0;
}

Node* db_query(DB* db, const Filter filter) {
    // 如果数据库为空，则直接返回
    if (db->list == NULL) {
        return NULL;
    }

    // 如果设置了过滤条件，则根据条件返回结果
    // 创建一个新链表，用于存放查询结果
    Node* result = NULL;
    Node* tail = NULL;

    // 遍历链表
    Node* node = db->list;
    while (node != NULL) {
        // 判断是否符合条件
        bool flag = true;

        // 判断 id 是否符合条件
        if (filter.enable_id &&
            !filte(filter.id_cond, node->student.id, filter.id)) {
            flag = false;
        }
        // 判断 name 是否符合条件
        if (filter.enable_name &&
            strcmp(node->student.name, filter.name) != 0) {
            flag = false;
        }
        // 判断 score 是否符合条件
        if (filter.enable_score &&
            !filte(filter.score_cond, node->student.score, filter.score)) {
            flag = false;
        }

        // 如果符合条件，则插入到结果链表
        if (flag) {
            Node* temp = (Node*)malloc(sizeof(Node));
            temp->student.id = node->student.id;
            strcpy(temp->student.name, node->student.name);
            temp->student.score = node->student.score;
            temp->next = NULL;

            if (result == NULL) {
                result = temp;
                tail = temp;
            } else {
                tail->next = temp;
                tail = temp;
            }
        }

        node = node->next;
    }

    return result;
}

int db_update(DB* db, const Student* student) {
    // 如果数据库为空，则直接返回
    if (db->list == NULL) {
        return -1;
    }

    // 遍历链表
    Node* node = db->list;
    while (node != NULL) {
        if (node->student.id == student->id) {
            node->student.id = student->id;
            strcpy(node->student.name, student->name);
            node->student.score = student->score;

            // 保存更改
            if (db_commit(db) != 0) {
                return -1;
            }

            // 重新加载数据库
            if (db_reload(db) != 0) {
                return -1;
            }

            return 0;
        }
        node = node->next;
    }

    return -1;
}

int db_commit(DB* db) {
    // 清空文件
    fclose(db->file);
    db->file = fopen(DB_PATH, "wb");
    if (db->file == NULL) {
        return -1;
    }

    // 将链表写入文件
    Node* node = db->list;
    while (node != NULL) {
        fwrite(&(node->student), sizeof(Student), 1, db->file);
        node = node->next;
    }
    fflush(db->file);
    fseek(db->file, 0, SEEK_SET);

    return 0;
}

int db_reload(DB* db) {
    // 释放之前链表的内存
    Node* node = db->list;
    while (node != NULL) {
        Node* next = node->next;
        free(node);
        node = next;
    }

    // 重新加载数据库
    fclose(db->file);
    db->file = fopen(DB_PATH, "rb+");
    if (db->file == NULL) {
        return -1;
    }

    // 读取数据库大小
    fseek(db->file, 0, SEEK_END);
    db->size = ftell(db->file) / sizeof(Student);
    fseek(db->file, 0, SEEK_SET);

    // 如果数据库大小为 0，则创建一个空链表
    if (db->size == 0) {
        db->list = NULL;
        return 0;
    }

    // 如果有数据 则从文件创建数据链表
    db->list = (Node*)malloc(sizeof(Node));
    node = db->list;
    for (int i = 0; i < db->size; ++i) {
        fread(&(node->student), sizeof(Student), 1, db->file);
        // 如果读取到文件末尾，则跳出循环
        if (i == db->size - 1) {
            node->next = NULL;
            break;
        }
        // 创建下一个节点
        node->next = (Node*)malloc(sizeof(Node));
        node = node->next;
    }
    fseek(db->file, 0, SEEK_SET);

    return 0;
}

void db_free(DB* db) {
    // 释放链表内存
    Node* node = db->list;
    while (node != NULL) {
        Node* next = node->next;
        free(node);
        node = next;
    }

    // 关闭文件
    fclose(db->file);
}