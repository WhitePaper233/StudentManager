#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"
#include "mode.h"
#include "table.h"
#include "ui.h"

/**
 * @brief 处理程序主界面
 *
 * @param ui 用户界面
 * @param mode 模式
 */
void handle_main_screen(UI* ui, Mode* mode);

/**
 * @brief 处理创建学生信息
 *
 * @param ui 用户界面
 * @param db 数据库
 * @param mode 模式
 */
void handle_create(UI* ui, DB* db, Mode* mode);

/**
 * @brief 处理输出学生信息
 *
 * @param ui 用户界面
 * @param db 数据库
 * @param mode 模式
 */
void handle_output(UI* ui, DB* db, Mode* mode);

/**
 * @brief 处理删除学生信息
 *
 * @param ui 用户界面
 * @param db 数据库
 * @param mode 模式
 */
void handle_delete(UI* ui, DB* db, Mode* mode);

/**
 * @brief 处理查询学生信息
 *
 * @param ui 用户界面
 * @param db 数据库
 * @param mode 模式
 */
void handle_query(UI* ui, DB* db, Mode* mode);

/**
 * @brief 处理更新学生信息
 *
 * @param ui 用户界面
 * @param db 数据库
 * @param mode 模式
 */
void handle_update(UI* ui, DB* db, Mode* mode);

/**
 * @brief 处理退出程序
 *
 * @param ui 用户界面
 * @param db 数据库
 */
void handle_exit(UI* ui, DB* db);

int main() {
    UI* ui = (UI*)malloc(sizeof(UI));  // 创建 UI 对象
    Mode mode = MAIN;                  // 初始化程序模式
    // 初始化数据库
    DB db;
    db_initialize(&db);

    // 程序主循环
    while (1)
        switch (mode) {
            case MAIN: {
                handle_main_screen(ui, &mode);
                break;
            }
            case CREATE: {
                handle_create(ui, &db, &mode);
                break;
            }
            case OUTPUT: {
                handle_output(ui, &db, &mode);
                break;
            }
            case DELETE: {
                handle_delete(ui, &db, &mode);
                break;
            }
            case QUERY: {
                handle_query(ui, &db, &mode);
                break;
            }
            case UPDATE: {
                handle_update(ui, &db, &mode);
                break;
            }
            case EXIT:
                handle_exit(ui, &db);
        }
    return 0;
}

void handle_main_screen(UI* ui, Mode* mode) {
    ui_put_next(ui, MAIN_SCREEN);
    int op;
    scanf("%d", &op);
    if (op < 1 || op > 6) {
        ui_put_next(ui, MAIN_ERROR);
        return;
    }
    *mode = op;
}

void handle_create(UI* ui, DB* db, Mode* mode) {
    ui_put_next(ui, CREATE_TIP);
    Student student;
    scanf("%d %s %d", &student.id, student.name, &student.score);
    int ret = db_insert(db, &student);
    if (ret != 0) {
        ui_put_next(ui, CREATE_FAILURE);
    } else {
        char output[UI_MAX];
        sprintf(output, CREATE_SUCCESS, student.name);
        ui_put_next(ui, output);
    }
    ui_back_main(mode);
}

void handle_output(UI* ui, DB* db, Mode* mode) {
    if (db->list == NULL) {
        ui_put_next(ui, PRINT_FAILURE);
        ui_back_main(mode);
        return;
    }

    ui_put_next(ui, "");
    Student student;
    Node* node = db->list;
    Table table;
    table_init(&table);
    for (int i = 0; i < db->size; ++i) {
        student = node->student;
        table_append(&table, student);
        node = node->next;
    }
    char c;
    ui_put_next(ui, IF_SORT);
    scanf(" %c", &c);
    SortCondition sc;
    if (c == 'y') {
        sort_condition_init(&sc, true);
        int op, field;

        ui_put_next(ui, SORT_OPTION);
        scanf("%d", &field);
        if (field != 1 && field != 2) {
            table_free(&table);
            ui_put_next(ui, SORT_ERROR);
            ui_back_main(mode);
            return;
        }
        sort_condition_set_field(&sc, field);

        ui_put_next(ui, SORT_DIRECTION);
        scanf("%d", &op);
        if (op != 1 && op != 2) {
            table_free(&table);
            ui_put_next(ui, SORT_ERROR);
            ui_back_main(mode);
            return;
        }
        sort_condition_set_type(&sc, op);
    } else if (c == 'n') {
        sort_condition_init(&sc, false);
    } else {
        ui_put_next(ui, SORT_ERROR);
        table_free(&table);
        ui_back_main(mode);
        return;
    }
    sort_table(&table, sc);

    ui_print_table(ui, &table);
    table_free(&table);

    ui_back_main(mode);
}

void handle_delete(UI* ui, DB* db, Mode* mode) {
    ui_put_next(ui, DELETE_TIP);
    int id;
    scanf("%d", &id);
    int ret = db_delete(db, id);
    if (ret != 0) {
        ui_put_next(ui, DELETE_FAILURE);
    } else {
        char output[UI_MAX];
        sprintf(output, UPDATE_SUCCESS, id);
        ui_put_next(ui, output);
    }
    ui_back_main(mode);
}

void handle_query(UI* ui, DB* db, Mode* mode) {
    Filter filter;
    filter_init(&filter);

    ui_put_next(ui, IF_QUERY_ID_TIP);
    char c;
    scanf(" %c", &c);
    if (c != 'y' && c != 'n') {
        ui_put_next(ui, QUERY_ERROR);
        ui_back_main(mode);
        return;
    }
    if (c == 'y') {
        ui_put_next(ui, QUERY_CONDITION);
        int op;
        scanf("%d", &op);
        if (op == 7) {
            ui_back_main(mode);
            return;
        }
        if (op < 1 || op > 6) {
            ui_put_next(ui, QUERY_ERROR);
            ui_back_main(mode);
            return;
        }
        ui_put_next(ui, INPUT_QUERY_ID);
        int id;
        scanf("%d", &id);
        filter_set_id(&filter, id, op);
    }

    ui_put_next(ui, IF_QUERY_NAME_TIP);
    scanf(" %c", &c);
    if (c != 'y' && c != 'n') {
        ui_put_next(ui, QUERY_ERROR);
        ui_back_main(mode);
        return;
    }
    if (c == 'y') {
        ui_put_next(ui, INPUT_QUERY_NAME);
        char name[32];
        scanf("%s", name);
        filter_set_name(&filter, name);
    }

    ui_put_next(ui, IF_QUERY_SCORE_TIP);
    scanf(" %c", &c);
    if (c != 'y' && c != 'n') {
        ui_put_next(ui, QUERY_ERROR);
        ui_back_main(mode);
        return;
    }
    if (c == 'y') {
        ui_put_next(ui, QUERY_CONDITION);
        int op;
        scanf("%d", &op);
        if (op == 7) {
            ui_back_main(mode);
            return;
        }
        if (op < 1 || op > 6) {
            ui_put_next(ui, QUERY_ERROR);
            ui_back_main(mode);
            return;
        }
        ui_put_next(ui, INPUT_QUERY_SCORE);
        int score;
        scanf("%d", &score);
        filter_set_score(&filter, score, op);
    }

    Node *node = db_query(db, filter), *list_head = node;
    if (node == NULL) {
        ui_put_next(ui, QUERY_FAILURE);
        ui_back_main(mode);
        return;
    }

    Table table;
    table_init(&table);
    while (node != NULL) {
        table_append(&table, node->student);
        node = node->next;
    }

    node_free(list_head);

    ui_put_next(ui, IF_SORT);
    scanf(" %c", &c);
    SortCondition sc;
    if (c == 'y') {
        sort_condition_init(&sc, true);
        int op, field;

        ui_put_next(ui, SORT_OPTION);
        scanf("%d", &field);
        if (field != 1 && field != 2) {
            table_free(&table);
            ui_put_next(ui, SORT_ERROR);
            ui_back_main(mode);
            return;
        }
        sort_condition_set_field(&sc, field);

        ui_put_next(ui, SORT_DIRECTION);
        scanf("%d", &op);
        if (op != 1 && op != 2) {
            table_free(&table);
            ui_put_next(ui, SORT_ERROR);
            ui_back_main(mode);
            return;
        }
        sort_condition_set_type(&sc, op);
    } else if (c == 'n') {
        sort_condition_init(&sc, false);
    } else {
        table_free(&table);
        ui_put_next(ui, SORT_ERROR);
        ui_back_main(mode);
        return;
    }
    sort_table(&table, sc);

    ui_print_table(ui, &table);
    table_free(&table);

    ui_back_main(mode);
}

void handle_update(UI* ui, DB* db, Mode* mode) {
    ui_put_next(ui, UPDATE_TIP);
    Student student;
    scanf("%d %s %d", &student.id, student.name, &student.score);
    int ret = db_update(db, &student);
    if (ret != 0) {
        ui_put_next(ui, UPDATE_FAILURE);
    } else {
        char output[UI_MAX];
        sprintf(output, UPDATE_SUCCESS, student.id);
        ui_put_next(ui, output);
    }
    ui_back_main(mode);
}

void handle_exit(UI* ui, DB* db) {
    db_free(db);
    clear_screen();
    free(ui);
    exit(EXIT_SUCCESS);
}
