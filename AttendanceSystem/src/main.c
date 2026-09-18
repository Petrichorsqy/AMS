/**
 * @file main.c
 * @brief 考勤管理系统主程序
 * @details 程序入口点，负责系统初始化和主循环
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "structures.h"
#include "data_manager.h"
#include "file_operations.h"
#include "menu_system.h"
#include "utils.h"

 // 全局变量定义
AttendanceRecord attendance_records[MAX_RECORDS];  // 考勤记录数组
int record_count = 0;                             // 当前记录数量
const char* data_filename = "attendance_data.dat"; // 默认数据文件名


/**
 * @brief 主函数
 * @return 程序退出码
 */
int main() {
    // 设置控制台编码为UTF-8（Windows）
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // 系统初始化
    system_init();

    int main_choice;

    // 主循环
    do {
        clear_screen();
        display_title();
        display_main_menu();

        main_choice = get_menu_selection(0, 5);

        switch (main_choice) {
        case 1: // 考勤记录管理
            handle_record_management();
            break;
        case 2: // 数据查询统计
            handle_query_statistics();
            break;
        case 3: // 文件操作
            handle_file_operations();
            break;
        case 4: // 系统设置
            handle_system_settings();
            break;
        case 5: // 显示所有记录
            display_all_records();
            break;
        case 0: // 退出系统
            system_cleanup();
            break;
        }

    } while (main_choice != 0);

    return 0;
}