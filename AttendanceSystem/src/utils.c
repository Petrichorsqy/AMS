/**
 * @file utils.c
 * @brief 工具函数模块
 * @details 提供系统通用的工具函数
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <Windows.h>
#include "structures.h"

 /**
  * @brief 获取考勤状态的中文描述
  * @param status 考勤状态枚举值
  * @return 对应的中文状态描述字符串
  */
const char* get_status_string(AttendanceStatus status) {
    switch (status) {
    case NORMAL:        return "正常出勤";
    case LATE:          return "迟到";
    case LEAVE_EARLY:   return "早退";
    case WORK_OVERTIME: return "加班";
    case EVECTION:      return "出差";
    case ASK_FOR_LEAVE: return "请假";
    case ABSENT:        return "缺勤";
    default:            return "未知状态";
    }
    }

/**
 * @brief 获取当前日期时间字符串
 * @param buffer 输出缓冲区
 * @param size 缓冲区大小
 * @param format 时间格式
 * @return 成功返回1，失败返回0
 */
int get_current_datetime(char* buffer, size_t size, const char* format) {
    time_t now = time(NULL);
    if (now == -1) {
        return 0;
    }

    struct tm* timeinfo = localtime(&now);
    if (timeinfo == NULL) {
        return 0;
    }

    if (strftime(buffer, size, format, timeinfo) == 0) {
        return 0;
    }

    return 1;
}

/**
 * @brief 安全字符串复制
 * @param dest 目标缓冲区
 * @param src 源字符串
 * @param dest_size 目标缓冲区大小
 * @details 防止缓冲区溢出
 */
void safe_strcpy(char* dest, const char* src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return;
    }

    size_t src_len = strlen(src);
    size_t copy_len = (src_len < dest_size - 1) ? src_len : dest_size - 1;

    strncpy(dest, src, copy_len);
    dest[copy_len] = '\0';
}

/**
 * @brief 去除字符串两端的空白字符
 * @param str 要处理的字符串
 */
void trim_string(char* str) {
    if (str == NULL) return;

    char* end;

    // 去除前导空白
    while (isspace((unsigned char)*str)) {
        str++;
    }

    // 如果整个字符串都是空白
    if (*str == '\0') {
        return;
    }

    // 去除尾部空白
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    // 写入终止符
    *(end + 1) = '\0';
}

/**
 * @brief 验证日期字符串格式
 * @param date_str 日期字符串
 * @return 格式正确返回1，错误返回0
 */
int validate_date_string(const char* date_str) {
    if (strlen(date_str) != 10) {
        return 0;
    }

    if (date_str[4] != '-' || date_str[7] != '-') {
        return 0;
    }

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit((unsigned char)date_str[i])) {
            return 0;
        }
    }

    int year, month, day;
    if (sscanf(date_str, "%d-%d-%d", &year, &month, &day) != 3) {
        return 0;
    }

    // 基本范围检查
    if (year < 2000 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31) {
        return 0;
    }

    // 月份天数检查
    int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // 闰年检查
    if (month == 2) {
        int is_leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (day > (28 + is_leap)) {
            return 0;
        }
    }
    else if (day > days_in_month[month - 1]) {
        return 0;
    }

    return 1;
}

/**
 * @brief 验证时间字符串格式
 * @param time_str 时间字符串
 * @return 格式正确返回1，错误返回0
 */
int validate_time_string(const char* time_str) {
    if (strlen(time_str) != 5) {
        return 0;
    }

    if (time_str[2] != ':') {
        return 0;
    }

    for (int i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (!isdigit((unsigned char)time_str[i])) {
            return 0;
        }
    }

    int hour, minute;
    if (sscanf(time_str, "%d:%d", &hour, &minute) != 2) {
        return 0;
    }

    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        return 0;
    }

    return 1;
}

/**
 * @brief 比较两个日期字符串
 * @param date1 第一个日期
 * @param date2 第二个日期
 * @return date1早于date2返回-1，相等返回0，晚于返回1
 */
int compare_dates(const char* date1, const char* date2) {
    return strcmp(date1, date2);
}

/**
 * @brief 比较两个时间字符串
 * @param time1 第一个时间
 * @param time2 第二个时间
 * @return time1早于time2返回-1，相等返回0，晚于返回1
 */
int compare_times(const char* time1, const char* time2) {
    return strcmp(time1, time2);
}

/**
 * @brief 计算两个时间的时间差（小时）
 * @param start_time 开始时间
 * @param end_time 结束时间
 * @return 时间差（小时）
 */
float calculate_time_difference(const char* start_time, const char* end_time) {
    int start_hour, start_min, end_hour, end_min;

    if (sscanf(start_time, "%d:%d", &start_hour, &start_min) != 2 ||
        sscanf(end_time, "%d:%d", &end_hour, &end_min) != 2) {
        return 0.0;
    }

    float hours = (float)((end_hour - start_hour) + (end_min - start_min) / 60.0);

    // 处理跨午夜的情况
    if (hours < 0) {
        hours += 24.0;
    }

    return hours;
}

/**
 * @brief 检查字符串是否为空或仅包含空白字符
 * @param str 要检查的字符串
 * @return 为空或空白返回1，否则返回0
 */
int is_string_empty_or_whitespace(const char* str) {
    if (str == NULL) {
        return 1;
    }

    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return 0;
        }
        str++;
    }

    return 1;
}

/**
 * @brief 将字符串转换为小写
 * @param str 要转换的字符串
 */
void string_to_lower(char* str) {
    if (str == NULL) return;

    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

/**
 * @brief 将字符串转换为大写
 * @param str 要转换的字符串
 */
void string_to_upper(char* str) {
    if (str == NULL) return;

    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

/**
 * @brief 格式化文件大小显示
 * @param size 文件大小（字节）
 * @param buffer 输出缓冲区
 * @param buffer_size 缓冲区大小
 */
void format_file_size(long size, char* buffer, size_t buffer_size) {
    if (size < 1024) {
        snprintf(buffer, buffer_size, "%ld B", size);
    }
    else if (size < 1024 * 1024) {
        snprintf(buffer, buffer_size, "%.2f KB", size / 1024.0);
    }
    else if (size < 1024 * 1024 * 1024) {
        snprintf(buffer, buffer_size, "%.2f MB", size / (1024.0 * 1024.0));
    }
    else {
        snprintf(buffer, buffer_size, "%.2f GB", size / (1024.0 * 1024.0 * 1024.0));
    }
}

/**
 * @brief 获取文件扩展名
 * @param filename 文件名
 * @param extension 扩展名输出缓冲区
 * @param buffer_size 缓冲区大小
 */
void get_file_extension(const char* filename, char* extension, size_t buffer_size) {
    if (filename == NULL || extension == NULL || buffer_size == 0) {
        return;
    }

    const char* dot = strrchr(filename, '.');
    if (dot == NULL || dot == filename) {
        extension[0] = '\0';
        return;
    }

    safe_strcpy(extension, dot + 1, buffer_size);
}

/**
 * @brief 检查文件是否存在
 * @param filename 文件名
 * @return 存在返回1，不存在返回0
 */
int file_exists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

/**
 * @brief 创建目录（如果需要）
 * @param path 目录路径
 * @return 成功返回1，失败返回0
 */
int create_directory_if_needed(const char* path) {
    // 简化版本，在实际系统中可能需要使用系统调用
    printf("检查目录: %s\n", path);
    return 1;
}

/**
 * @brief 显示进度条
 * @param current 当前进度
 * @param total 总进度
 * @param description 进度描述
 */
void show_progress_bar(int current, int total, const char* description) {
    if (total <= 0) return;

    const int bar_width = 50;
    float progress = (float)current / total;
    int pos = (int)(bar_width * progress);

    printf("\r%s [", description);
    for (int i = 0; i < bar_width; i++) {
        if (i < pos) {
            printf("=");
        }
        else if (i == pos) {
            printf(">");
        }
        else {
            printf(" ");
        }
    }
    printf("] %d%% (%d/%d)", (int)(progress * 100), current, total);

    if (current >= total) {
        printf("\n");
    }

    fflush(stdout);
}

/**
 * @brief 清除屏幕
 * @details 根据操作系统调用相应的清屏命令
 */
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * @brief 等待指定毫秒数
 * @param milliseconds 毫秒数
 */
void sleep_ms(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

/**
 * @brief 安全的时间输入，格式为HH:MM
 * @param buffer 输出缓冲区
 * @param size 缓冲区大小
 * @param prompt 提示信息
 * @return 成功返回0，失败返回-1
 */
int safe_input_time(char* buffer, int size, const char* prompt) {
    if (size < 6) { // 需要至少6个字符：HH:MM + '\0'
        return -1;
    }

    char input[20];

    while (1) {
        printf("%s", prompt);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            continue;
        }

        input[strcspn(input, "\n")] = 0;

        // 清理空格
        char cleaned[20];
        int j = 0;
        for (int i = 0; input[i] != '\0' && j < 19; i++) {
            if (input[i] != ' ') {
                cleaned[j++] = input[i];
            }
        }
        cleaned[j] = '\0';

        // 验证长度
        if (strlen(cleaned) != 5) {
            printf("错误: 时间格式应为 HH:MM (如 09:00)\n");
            continue;
        }

        // 验证格式
        if (cleaned[2] != ':') {
            printf("错误: 时间格式应为 HH:MM (中间用冒号分隔)\n");
            continue;
        }

        // 验证小时
        if (!isdigit((unsigned char)cleaned[0]) || !isdigit((unsigned char)cleaned[1])) {
            printf("错误: 小时部分应为数字\n");
            continue;
        }

        // 验证分钟
        if (!isdigit((unsigned char)cleaned[3]) || !isdigit((unsigned char)cleaned[4])) {
            printf("错误: 分钟部分应为数字\n");
            continue;
        }

        // 转换为数字验证范围
        int hour = (cleaned[0] - '0') * 10 + (cleaned[1] - '0');
        int minute = (cleaned[3] - '0') * 10 + (cleaned[4] - '0');

        if (hour < 0 || hour > 23) {
            printf("错误: 小时应在 00-23 之间\n");
            continue;
        }

        if (minute < 0 || minute > 59) {
            printf("错误: 分钟应在 00-59 之间\n");
            continue;
        }

        // 复制到输出缓冲区
        strncpy(buffer, cleaned, size - 1);
        buffer[size - 1] = '\0';

        return 0;
    }
}


/**
 * @brief 安全的日期输入，格式为YYYY-MM-DD
 * @param buffer 输出缓冲区
 * @param size 缓冲区大小
 * @param prompt 提示信息
 * @return 成功返回0，失败返回-1
 */
int safe_input_date(char* buffer, int size,const char* prompt) {
    if (size < 11) { // YYYY-MM-DD + '\0'
        return -1;
    }

    char input[20];

    while (1) {
        printf("%s", prompt);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            continue;
        }

        input[strcspn(input, "\n")] = 0;

        // 清理空格
        char cleaned[20];
        int j = 0;
        for (int i = 0; input[i] != '\0' && j < 19; i++) {
            if (input[i] != ' ') {
                cleaned[j++] = input[i];
            }
        }
        cleaned[j] = '\0';

        // 验证长度
        if (strlen(cleaned) != 10) {
            printf("错误: 日期格式应为 YYYY-MM-DD (如 2025-12-17)\n");
            continue;
        }

        // 验证格式
        if (cleaned[4] != '-' || cleaned[7] != '-') {
            printf("错误: 日期格式应为 YYYY-MM-DD (用短横线分隔)\n");
            continue;
        }

        // 验证年份
        for (int i = 0; i < 4; i++) {
            if (!isdigit((unsigned char)cleaned[i])) {
                printf("错误: 年份部分应为数字\n");
                continue;
            }
        }

        // 验证月份
        for (int i = 5; i < 7; i++) {
            if (!isdigit((unsigned char)cleaned[i])) {
                printf("错误: 月份部分应为数字\n");
                continue;
            }
        }

        // 验证日期
        for (int i = 8; i < 10; i++) {
            if (!isdigit((unsigned char)cleaned[i])) {
                printf("错误: 日期部分应为数字\n");
                continue;
            }
        }

        // 转换为数字验证范围
        int year = (cleaned[0] - '0') * 1000 + (cleaned[1] - '0') * 100 +
            (cleaned[2] - '0') * 10 + (cleaned[3] - '0');
        int month = (cleaned[5] - '0') * 10 + (cleaned[6] - '0');
        int day = (cleaned[8] - '0') * 10 + (cleaned[9] - '0');

        if (year < 2000 || year > 2100) {
            printf("错误: 年份应在 2000-2100 之间\n");
            continue;
        }

        if (month < 1 || month > 12) {
            printf("错误: 月份应在 1-12 之间\n");
            continue;
        }

        // 简单的日期验证（不考虑闰年）
        int max_days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if (day < 1 || day > max_days[month - 1]) {
            printf("错误: %d月应在 1-%d 日之间\n", month, max_days[month - 1]);
            continue;
        }

        // 复制到输出缓冲区
        strncpy(buffer, cleaned, size - 1);
        buffer[size - 1] = '\0';

        return 0;
    }
}

/**
 * @brief 安全的字符串输入，带边界检查和空格清理
 * @param buffer 输出缓冲区
 * @param size 缓冲区大小
 * @param prompt 提示信息
 * @return 成功返回0，失败返回-1
 */
int safe_input_string(char* buffer, int size,const char* prompt) {
    if (size <= 0) return -1;

	printf("%s",prompt);
    // 使用大缓冲区接收输入
    char input[256];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }

    // 去掉换行符
    input[strcspn(input, "\n")] = 0;

    // 计算实际长度（去掉首尾空格）
    int start = 0;
    int end = strlen(input) - 1;

    // 跳过开头的空格
    while (start <= end && isspace((unsigned char)input[start])) {
        start++;
    }

    // 跳过结尾的空格
    while (end >= start && isspace((unsigned char)input[end])) {
        end--;
    }

    // 计算实际长度
    int actual_len = end - start + 1;

    if (actual_len <= 0) {
        buffer[0] = '\0';
        return -1; // 空输入
    }

    if (actual_len >= size) {
        printf("错误: 输入过长，最多允许 %d 个字符\n", size - 1);
        return -1;
    }

    // 复制有效部分到缓冲区
    int j = 0;
    for (int i = start; i <= end; i++) {
        buffer[j++] = input[i];
    }
    buffer[j] = '\0';

    return 0;
}

/**
 * @brief 安全的整数输入
 * @param min 最小值
 * @param max 最大值
 * @param prompt 提示信息
 * @return 输入的整数值
 */
int safe_input_int(int min, int max, const char* prompt) {
    int value;
    char input[50];
    char* endptr;

    while (1) {
        printf("%s", prompt);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            continue;
        }

        // 转换为整数
        value = (int)strtol(input, &endptr, 10);

        // 检查转换是否成功
        if (endptr == input) {
            printf("错误: 请输入有效的数字\n");
            continue;
        }

        // 检查是否有多余字符
        for (; *endptr != '\0'; endptr++) {
            if (!isspace((unsigned char)*endptr)) {
                printf("错误: 输入包含非数字字符\n");
                value = min - 1; // 设置为无效值
                break;
            }
        }

        // 检查范围
        if (value < min || value > max) {
            printf("错误: 请输入 %d 到 %d 之间的数字\n", min, max);
            continue;
        }

        return value;
    }
}

// utils.c
/**
 * @brief 清空标准输入缓冲区
 */
void flush_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
