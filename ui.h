#ifndef __UI_H__
#define __UI_H__

#include <stdlib.h>

#include "mode.h"
#include "table.h"

// UI ��󳤶�
#define UI_MAX 1024

// ������
#define MAIN_SCREEN \
    ">>\n\
+----------------- ѧ����Ϣ����ϵͳ -----------------+\n\
| ������������Խ�����ز���:                        |\n\
| (1) ���ѧ����Ϣ                                   |\n\
| (2) �������ѧ����Ϣ                               |\n\
| (3) ����ѧ��                                       |\n\
| (4) �޸�ѧ����Ϣ                                   |\n\
| (5) ɾ��ѧ��                                       |\n\
| (6) �˳�����                                       |\n\
+----------------------------------------------------+\n\
>> ����������ѡ��:\n"

// �����������ʾ
#define MAIN_ERROR "��Ч�Ĳ���, ����������\n"

// ���ѧ����ʾ
#define CREATE_TIP \
    ">> �밴�� \"ѧ�� ���� �ɼ�\" �ĸ�ʽ��������Ҫ��ӵ�ѧ����Ϣ:\n"
// ���ѧ���ɹ���ʾ
#define CREATE_SUCCESS "���ѧ�� %s �ɹ�!\n"
// ���ѧ��ʧ����ʾ
#define CREATE_FAILURE "���ѧ��ʧ�ܣ������ԣ�\n"

// ���ѧ����Ϣʧ����ʾ
#define PRINT_FAILURE "���ѧ����Ϣʧ�ܣ������ԣ�\n"

// ����ѧ����ʾ
#define IF_QUERY_ID_TIP "�Ƿ�����ѧ�Ų�ѯ������(y/n)\n"
#define INPUT_QUERY_ID "������ѧ�ţ�\n"
#define IF_QUERY_NAME_TIP "�Ƿ�����������ѯ������(y/n)\n"
#define INPUT_QUERY_NAME "������������\n"
#define IF_QUERY_SCORE_TIP "�Ƿ����óɼ���ѯ������(y/n)\n"
#define INPUT_QUERY_SCORE "������ɼ���\n"
#define QUERY_CONDITION "��ѡ��������\n1. ����\n2. ����\n3. С��\n4. ���ڵ���\n5. С�ڵ���\n6. ������\n7. ȡ��\n"
#define QUERY_ERROR "��Ч�Ĳ���, ���� b �������˵�\n"
#define QUERY_TIP ">> ������Ҫ���ҵ�ѧ��ѧ��:\n"
// ����ѧ���ɹ���ʾ
#define QUERY_SUCCESS \
    "���ҵ�ѧ�� %d, ��Ϣ����:\n\
ѧ��: %d\n\
����: %s\n\
�ɼ�: %d\n"
// ����ѧ��ʧ����ʾ
#define QUERY_FAILURE "����ѧ��ʧ�ܣ������ԣ�\n"

// ������ʾ
#define IF_SORT "�Ƿ���������(y/n)\n"
#define SORT_OPTION "��ѡ������������\n1. ��ѧ������\n2. ���ɼ�����\n"
#define SORT_ERROR "��Ч�Ĳ���, ���� b �������˵�\n"
#define SORT_DIRECTION "��ѡ��������:\n1. ����\n2. ����\n"

// �޸�ѧ����ʾ
#define UPDATE_TIP \
    ">> �밴�� \"ѧ�� ���� �ɼ�\" �ĸ�ʽ��������Ҫ�޸ĵ�ѧ����Ϣ:\n"
// �޸�ѧ���ɹ���ʾ
#define UPDATE_SUCCESS "�޸�ѧ�� %d ��Ϣ�ɹ�!\n"
// �޸�ѧ��ʧ����ʾ
#define UPDATE_FAILURE "�޸�ѧ����Ϣʧ�ܣ������ԣ�\n"

// ɾ��ѧ����ʾ
#define DELETE_TIP ">> ������Ҫɾ����ѧ��ѧ��:\n"
// ɾ��ѧ���ɹ���ʾ
#define DELETE_SUCCESS "ɾ��ѧ�� %d �ɹ�!\n"
// ɾ��ѧ��ʧ����ʾ
#define DELETE_FAILURE "ɾ��ѧ��ʧ�ܣ������ԣ�\n"

// �������˵�
#define BACK_MAIN ">> ���� b �������˵�\n"

/**
 * @brief UI �ṹ���ʾ�û�����
 */
typedef struct {
    char output[UI_MAX];
} UI;

/**
 * @brief ��յ�ǰ��Ļ
 */
void clear_screen();

/**
 * @brief ����һ���ַ�������UI�С�
 *
 * @param ui UI����ָ�롣
 * @param next Ҫ����UI���ַ�����
 */
void ui_put_next(UI* ui, const char* next);

/**
 * @brief ��ӡ���
 *
 * @param ui UI����ָ�롣
 * @param table Table����ָ�롣
 */
void ui_print_table(UI* ui, Table* table);

/**
 * @brief ����������
 *
 * @param mode ģʽָ��
 */
void ui_back_main(Mode* mode);

#endif
