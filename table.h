#ifndef __TABLE_H__
#define __TABLE_H__
#include <stdbool.h>
#include "db.h"

#define DEFAULT_TABLE_CAP 16

/**
 * @brief �����ֶ�
 *
 * ��ö�����Ͷ����������ֶ�
 */
typedef enum {
    ID = 1,
    SCORE = 2,
} SortField;

/**
 * @brief ����ʽ
 *
 * ��ö�����Ͷ���������ʽ
 */
typedef enum {
    ASC = 1,   // ����
    DESC = 2,  // ����
} SortType;

/**
 * @brief ��������
 *
 * �ýṹ�嶨������������
 */
typedef struct {
    bool enable;
    SortType sort_type;
    SortField sort_by;
} SortCondition;

/**
 * @brief ��ʼ����������
 *
 * �ú������ڳ�ʼ����������
 *
 * @param sc ָ�����������ṹ���ָ��
 * @param enable �Ƿ���������
 */
void sort_condition_init(SortCondition* sc, bool enable);

/**
 * @brief ��������ʽ
 *
 * �ú���������������ʽ
 *
 * @param sc ָ�����������ṹ���ָ��
 * @param type ����ʽ
 */
void sort_condition_set_type(SortCondition* sc, SortType type);

/**
 * @brief ���������ֶ�
 *
 * �ú����������������ֶ�
 *
 * @param sc ָ�����������ṹ���ָ��
 * @param field �����ֶ�
 */
void sort_condition_set_field(SortCondition* sc, SortField field);

/**
 * @brief ���ṹ��
 */
typedef struct {
    int len, cap;
    Student* records;
} Table;

/**
 * @brief ��ʼ�����
 *
 * ����������ڳ�ʼ�����
 *
 * @param table ָ����ṹ���ָ��
 * @return int �����Ƿ��ʼ���ɹ���������0��ʾ�ɹ�������ֵ��ʾʧ��
 */
int table_init(Table* table);

/**
 * @brief ����¼�¼
 *
 * ��������������������һ���¼�¼
 *
 * @param table ָ����ṹ���ָ��
 * @param record ��Ҫ��ӵļ�¼
 */
void table_append(Table* table, Student record);

/**
 * @brief ��ӡ���
 *
 * �ú����������ն˸�ʽ��������
 *
 * @param table ָ����ṹ���ָ��
 */
void table_print(Table* table);

/**
 * @brief �ͷű��
 *
 * �ú��������ͷű���ڴ�ռ�
 *
 * @param table ָ����ṹ���ָ��
 */
void table_free(Table* table);

/**
 * @brief �������
 *
 * �ú�������������
 *
 * @param table ָ����ṹ���ָ��
 * @param sort_type ����ʽ
 */
void sort_table(Table* table, SortCondition sc);

#endif