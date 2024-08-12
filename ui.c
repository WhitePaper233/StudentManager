#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mode.h"
#include "table.h"
#include "ui.h"

// ��յ�ǰ��Ļ�ĺ���
void clear_screen() {
#ifdef _WIN32
    // �����Windowsϵͳ��ʹ��"cls"��������
    system("cls");
#else
    // ���������ϵͳ����Linux��MacOS����ʹ��"clear"��������
    system("clear");
#endif
}

// ��ʾ���ݵĺ���
void ui_put_next(UI* ui, const char* next) {
    // ���Ŀ���ַ������� ��ֻ������󳤶� ��ֹ���������
    if (strlen(next) > UI_MAX - 1) {
        for (int i = 0; i < UI_MAX - 1; ++i) {
            // �����ַ�����ֱ���ﵽ��󳤶�
            ui->output[i] = next[i];
        }
        // ���ַ���ĩβ��ӿ��ַ�����ȷ���ַ�����ȷ����
        ui->output[UI_MAX - 1] = '\0';
        return;
    }
    // ����ַ�������û�г�����󳤶ȣ�ֱ�Ӹ���
    strcpy(ui->output, next);

    // ����
    clear_screen();
    // ��ӡ���
    printf("%s", ui->output);
}

// ��ӡ���
void ui_print_table(UI* ui, Table* table) {
    clear_screen();
    table_print(table);
}

// ����back�ص�������ĺ���
void ui_back_main(Mode* mode) {
    // ��ӡ��ʾ��Ϣ
    printf(BACK_MAIN);
    // ����һ���ַ����飬���ڴ洢�û�����
    char back[UI_MAX];
    while (1) {
        // ��ȡ�û�����
        scanf("%s", back);
        // ����û��������"b"
        if (strcmp(back, "b") == 0) {
            // �л�ģʽ��������
            *mode = MAIN;
            // ����ѭ��
            break;
        }
    }
}
