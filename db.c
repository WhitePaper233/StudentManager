#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"

/**
 * @brief �ж��Ƿ��������
 *
 * @param cond ����
 * @param val ֵ
 * @param target Ŀ��ֵ
 *
 * @return �Ƿ��������
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
    // �ж����ݿ��ļ��Ƿ����
    FILE* fp = fopen(DB_PATH, "r");
    if (fp == NULL) {
        // �������򴴽�
        fp = fopen(DB_PATH, "wb");
        if (fp == NULL) {
            return -1;
        }
    }
    fclose(fp);

    // �����ݿ��ļ�
    db->file = fopen(DB_PATH, "rb+");
    if (db->file == NULL) {
        return -1;
    }

    // ��ȡ���ݿ��С
    fseek(db->file, 0, SEEK_END);
    db->size = ftell(db->file) / sizeof(Student);
    fseek(db->file, 0, SEEK_SET);

    // ������ݿ��СΪ 0���򴴽�һ��������
    if (db->size == 0) {
        db->list = NULL;
        return 0;
    }

    // ��������� ����ļ�������������
    db->list = (Node*)malloc(sizeof(Node));
    Node* node = db->list;
    for (int i = 0; i < db->size; ++i) {
        fread(&(node->student), sizeof(Student), 1, db->file);
        // �����ȡ���ļ�ĩβ��������ѭ��
        if (i == db->size - 1) {
            node->next = NULL;
            break;
        }
        // ������һ���ڵ�
        node->next = (Node*)malloc(sizeof(Node));
        node = node->next;
    }
    fseek(db->file, 0, SEEK_SET);

    return 0;
}

int db_insert(DB* db, const Student* student) {
    // �����½ڵ�
    Node* node = (Node*)malloc(sizeof(Node));
    node->student.id = student->id;
    strcpy(node->student.name, student->name);
    node->student.score = student->score;
    node->next = NULL;

    // ������ݿ�Ϊ�գ���ֱ�Ӳ���
    if (db->list == NULL) {
        db->list = node;
    } else {
        // ������ݿⲻΪ�գ�����뵽����β��
        Node* p = db->list;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = node;
    }
    ++(db->size);

    // �������
    if (db_commit(db) != 0) {
        return -1;
    }

    // ���¼������ݿ�
    if (db_reload(db) != 0) {
        return -1;
    }

    return 0;
}

int db_delete(DB* db, const int id) {
    // ������ݿ�Ϊ�գ���ֱ�ӷ���
    if (db->list == NULL) {
        return -1;
    }

    // ���Ҫɾ�����ǵ�һ���ڵ�
    if (db->list->student.id == id) {
        Node* node = db->list;
        db->list = node->next;
        free(node);
    } else {
        // ��������
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

    // �������
    if (db_commit(db) != 0) {
        return -1;
    }

    // ���¼������ݿ�
    if (db_reload(db) != 0) {
        return -1;
    }

    return 0;
}

Node* db_query(DB* db, const Filter filter) {
    // ������ݿ�Ϊ�գ���ֱ�ӷ���
    if (db->list == NULL) {
        return NULL;
    }

    // ��������˹���������������������ؽ��
    // ����һ�����������ڴ�Ų�ѯ���
    Node* result = NULL;
    Node* tail = NULL;

    // ��������
    Node* node = db->list;
    while (node != NULL) {
        // �ж��Ƿ��������
        bool flag = true;

        // �ж� id �Ƿ��������
        if (filter.enable_id &&
            !filte(filter.id_cond, node->student.id, filter.id)) {
            flag = false;
        }
        // �ж� name �Ƿ��������
        if (filter.enable_name &&
            strcmp(node->student.name, filter.name) != 0) {
            flag = false;
        }
        // �ж� score �Ƿ��������
        if (filter.enable_score &&
            !filte(filter.score_cond, node->student.score, filter.score)) {
            flag = false;
        }

        // �����������������뵽�������
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
    // ������ݿ�Ϊ�գ���ֱ�ӷ���
    if (db->list == NULL) {
        return -1;
    }

    // ��������
    Node* node = db->list;
    while (node != NULL) {
        if (node->student.id == student->id) {
            node->student.id = student->id;
            strcpy(node->student.name, student->name);
            node->student.score = student->score;

            // �������
            if (db_commit(db) != 0) {
                return -1;
            }

            // ���¼������ݿ�
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
    // ����ļ�
    fclose(db->file);
    db->file = fopen(DB_PATH, "wb");
    if (db->file == NULL) {
        return -1;
    }

    // ������д���ļ�
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
    // �ͷ�֮ǰ������ڴ�
    Node* node = db->list;
    while (node != NULL) {
        Node* next = node->next;
        free(node);
        node = next;
    }

    // ���¼������ݿ�
    fclose(db->file);
    db->file = fopen(DB_PATH, "rb+");
    if (db->file == NULL) {
        return -1;
    }

    // ��ȡ���ݿ��С
    fseek(db->file, 0, SEEK_END);
    db->size = ftell(db->file) / sizeof(Student);
    fseek(db->file, 0, SEEK_SET);

    // ������ݿ��СΪ 0���򴴽�һ��������
    if (db->size == 0) {
        db->list = NULL;
        return 0;
    }

    // ��������� ����ļ�������������
    db->list = (Node*)malloc(sizeof(Node));
    node = db->list;
    for (int i = 0; i < db->size; ++i) {
        fread(&(node->student), sizeof(Student), 1, db->file);
        // �����ȡ���ļ�ĩβ��������ѭ��
        if (i == db->size - 1) {
            node->next = NULL;
            break;
        }
        // ������һ���ڵ�
        node->next = (Node*)malloc(sizeof(Node));
        node = node->next;
    }
    fseek(db->file, 0, SEEK_SET);

    return 0;
}

void db_free(DB* db) {
    // �ͷ������ڴ�
    Node* node = db->list;
    while (node != NULL) {
        Node* next = node->next;
        free(node);
        node = next;
    }

    // �ر��ļ�
    fclose(db->file);
}