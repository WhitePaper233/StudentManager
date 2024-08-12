#ifndef __DB_H__
#define __DB_H__
#include <stdbool.h>
#include <stdio.h>

// ���ݿ��ļ�·��
#define DB_PATH "./students_data.bin"

/**
 * @brief �Ƚ�����ö��
 *
 * ��ö�����ڱ�ʾ�Ƚ��������������ڡ����ں�С�ڡ�
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
 * @brief �������ṹ��
 *
 * �ýṹ�����ڱ�ʾһ���������������Ƿ����ù�������������������ֵ�͹��������ıȽ�������
 */
typedef struct {
    bool enable_id, enable_name, enable_score;
    int id;
    char name[32];
    int score;
    Condition id_cond, score_cond;
} Filter;

/**
 * @brief ��ʼ��������
 *
 * @param filter ������ָ��
 */
void filter_init(Filter* filter);

/**
 * @brief ���ù������Ĺ�������
 *
 * @param filter ������ָ��
 * @param id ѧ��
 * @param cond �Ƚ�����
 */
void filter_set_id(Filter* filter, int id, Condition cond);

/**
 * @brief ���ù������Ĺ�������
 *
 * @param filter ������ָ��
 * @param name ����
 */
void filter_set_name(Filter* filter, const char* name);

/**
 * @brief ���ù������Ĺ�������
 *
 * @param filter ������ָ��
 * @param score �ɼ�
 * @param cond �Ƚ�����
 */
void filter_set_score(Filter* filter, int score, Condition cond);

/**
 * @brief ѧ����Ϣ�ṹ��
 */
typedef struct {
    int id;         // ѧ��
    char name[32];  // ����
    int score;      // �ɼ�
} Student;

/**
 * @brief ����ڵ�ṹ��
 */
typedef struct __Node__ {
    Student student;        // ѧ����Ϣ
    struct __Node__* next;  // ��һ������ڵ�
} Node;

/**
 * @brief ��������ڵ�
 *
 * @param student ѧ����Ϣָ��
 * @return Node* ���ش���������ڵ�
 */
void node_free(Node* node);

/**
 * @brief ���ݿ�ṹ��
 *
 * �ýṹ�����ڱ�ʾһ�����ݿ⣬�������ݴ�С�������ļ�ָ�����������
 */
typedef struct {
    int size;    // ���ݴ�С
    FILE* file;  // �����ļ�ָ��
    Node* list;  // ��������
} DB;

/**
 * @brief ��ʼ�����ݿ�
 *
 * ����������ڳ�ʼ��ѧ����Ϣ���ݿ�
 *
 * @param db ָ�����ݿ�ṹ���ָ��
 * @return int �����Ƿ��ʼ���ɹ���������0��ʾ�ɹ�������ֵ��ʾʧ��
 */
int db_initialize(DB* db);

/**
 * @brief ����ѧ����Ϣ�����ݿ���
 *
 * @param db ���ݿ�ָ��
 * @param student ѧ����Ϣָ��
 * @return int ���ز�������Ľ�����ɹ�����0������ֵ��ʾʧ��
 */
int db_insert(DB* db, const Student* student);

/**
 * @brief ɾ�����ݿ��е�һ����¼
 *
 * @param db ָ�����ݿ�ṹ���ָ��
 * @param id Ҫɾ����ѧ��ѧ��
 * @return int ����ɾ�������Ľ�����ɹ�����0������ֵ��ʾʧ��
 */
int db_delete(DB* db, const int id);

/**
 * @brief ��ѯ���ݿ��е�һ����¼
 *
 * @param db ָ�����ݿ�ṹ���ָ��
 * @param filter ��ѯ����
 * @return Node* ���ز�ѯ��������û���ҵ��򷵻�NULL
 */
Node* db_query(DB* db, const Filter filter);

/**
 * @brief �������ݿ��е�ѧ����Ϣ
 *
 * @param db ���ݿ�ָ��
 * @param student ѧ����Ϣָ��
 * @return int ���½����0��ʾ�ɹ���-1��ʾʧ��
 */
int db_update(DB* db, const Student* student);

/**
 * @brief �ύ���ݿ��еĸ���
 *
 * �˺����ύ���ݿ��еĸ��ģ���ȷ�����Ǳ����ñ��档
 *
 * @param db ָ�����ݿ�����ָ�롣
 * @return ����ύ�ɹ����򷵻�0�����򷵻�-1��
 */
int db_commit(DB* db);

/**
 * @brief ���¼������ݿ�
 *
 * �˺������¼�����DBָ��ָ�������ݿ⡣
 *
 * @param db ָ�����ݿ��ָ�롣
 * @return int ������ݿ�ɹ����¼��أ��򷵻�0�����򷵻ط���ֵ��
 */
int db_reload(DB* db);

/**
 * @brief �ͷ����ݿ���Դ
 *
 * �ͷ����ݿ���Դ�������ر����ݿ����ӡ��ͷ��ڴ�Ȳ�����
 *
 * @param db ���ݿ�ָ��
 */
void db_free(DB* db);

#endif
