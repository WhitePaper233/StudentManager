#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "db.h"
#include "table.h"

#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

// ��ȡ�ն˿��
int get_terminal_width() {
#ifdef _WIN32
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
#else
    {
        struct winsize ws;
        ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
        return ws.ws_col;
    }
#endif
}

// ��ȡ�ַ�����ʾ���
int get_char_width(wchar_t wc) {
    if (wc == L'\t') {
        // �Ʊ��
        return 4;
    } else if (wc >= 0x4E00 && wc <= 0x9FFF) {
        // �����ַ�
        return 2;
    } else {
        // �����ַ�
        return 1;
    }
}

// �����ַ����ն˿��
int string_width(const char* str) {
    int width = 0;
    wchar_t wc;
    size_t bytes;
    setlocale(LC_ALL, "");

    while (*str != '\0') {
        bytes = mbtowc(&wc, str, MB_CUR_MAX);
        if (bytes == -1) {
            // ������������������Ч�Ķ��ֽ��ַ�
            return -1;
        } else if (bytes == 0) {
            // �����ַ�������
            break;
        } else {
            // ���㵱ǰ�ַ��Ŀ�Ȳ��ۼ�
            width += get_char_width(wc);
            str += bytes;
        }
    }
    return width;
}

void sort_condition_init(SortCondition *sc, bool enable) {
    sc->enable = enable;
}

void sort_condition_set_type(SortCondition *sc, SortType type) {
    sc->sort_type = type;
}

void sort_condition_set_field(SortCondition *sc, SortField field) {
    sc->sort_by = field;
}

int table_init(Table* table) {
    table->len = 0;
    table->cap = DEFAULT_TABLE_CAP;
    table->records = (Student*)malloc(sizeof(Student) * table->cap);
    if (table->records == NULL)
        return 1;
    return 0;
}

void table_append(Table* table, Student record) {
    if (table->len == table->cap) {
        table->cap *= 2;
        table->records =
            (Student*)realloc(table->records, sizeof(Student) * table->cap);
    }
    table->records[table->len++] = record;
}

void table_print(Table* table) {
    int field_width = get_terminal_width() / 3;
    char field_names[][7] = {"ѧ��", "����", "�ɼ�"};
    putchar('|');
    for (int field = 0; field < 3; ++field) {
        for (int i = 0; i < (field_width - 6) / 2; ++i) {
            putchar('-');
        }
        printf("%s", field_names[field]);
        for (int i = 0; i < (field_width - 6) / 2; ++i) {
            putchar('-');
        }

        putchar('|');
    }
    putchar('\n');

    for (int line = 0; line < table->len; ++line) {
        for (int field = 0; field < 3; ++field) {
            putchar('|');
            char *str, s[128];
            switch (field) {
                case 0: {
                    sprintf(s, "%d", table->records[line].id);
                    str = s;
                    break;
                }
                case 1: {
                    str = table->records[line].name;
                    break;
                }
                case 2: {
                    sprintf(s, "%d", table->records[line].score);
                    str = s;
                    break;
                }
            }
            printf("%s", str);
            for (int i = 0; i < (field_width / 2 * 2 - string_width(str) - 2);
                 ++i)
                putchar(' ');
        }
        putchar('|');
        putchar('\n');
    }
}

void sort_table(Table* table, SortCondition sc) {
    if (!sc.enable) return;

    for (int i = 0; i < table->len - 1; ++i) {
        for (int j = 0; j < table->len - i - 1; ++j) {
            int cmp = 0;
            if (sc.sort_by == ID) {
                cmp = table->records[j].id - table->records[j + 1].id;
            } else if (sc.sort_by == SCORE) {
                cmp = table->records[j].score - table->records[j + 1].score;
            }

            if ((sc.sort_type == ASC) && cmp > 0) {
                Student temp = table->records[j];
                table->records[j] = table->records[j + 1];
                table->records[j + 1] = temp;
            } else if ((sc.sort_type == DESC) && cmp < 0) {
                Student temp = table->records[j];
                table->records[j] = table->records[j + 1];
                table->records[j + 1] = temp;
            }
        }
    }
}

void table_free(Table* table) {
    free(table->records);
    table->records = NULL;
    table->len = 0;
    table->cap = 0;
}
