#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mode.h"
#include "table.h"
#include "ui.h"

// 清空当前屏幕的函数
void clear_screen() {
#ifdef _WIN32
    // 如果是Windows系统，使用"cls"命令清屏
    system("cls");
#else
    // 如果是其他系统（如Linux或MacOS），使用"clear"命令清屏
    system("clear");
#endif
}

// 显示内容的函数
void ui_put_next(UI* ui, const char* next) {
    // 如果目标字符串过大 则只复制最大长度 防止缓冲区溢出
    if (strlen(next) > UI_MAX - 1) {
        for (int i = 0; i < UI_MAX - 1; ++i) {
            // 复制字符串，直到达到最大长度
            ui->output[i] = next[i];
        }
        // 在字符串末尾添加空字符，以确保字符串正确结束
        ui->output[UI_MAX - 1] = '\0';
        return;
    }
    // 如果字符串长度没有超过最大长度，直接复制
    strcpy(ui->output, next);

    // 清屏
    clear_screen();
    // 打印输出
    printf("%s", ui->output);
}

// 打印表格
void ui_print_table(UI* ui, Table* table) {
    clear_screen();
    table_print(table);
}

// 输入back回到主界面的函数
void ui_back_main(Mode* mode) {
    // 打印提示信息
    printf(BACK_MAIN);
    // 创建一个字符数组，用于存储用户输入
    char back[UI_MAX];
    while (1) {
        // 读取用户输入
        scanf("%s", back);
        // 如果用户输入的是"b"
        if (strcmp(back, "b") == 0) {
            // 切换模式到主界面
            *mode = MAIN;
            // 跳出循环
            break;
        }
    }
}
