#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "structures.h"

// 数据管理函数声明
void init_attendance_data(AttendanceRecord records[], int* count);
int add_attendance_record(AttendanceRecord records[], int* count, const AttendanceRecord* new_record);
int delete_record_by_index(AttendanceRecord records[], int* count, int index);
int find_record_index(AttendanceRecord records[], int count, const char* employee_id, const char* date);
int modify_attendance_record(AttendanceRecord records[], int count, int index, const AttendanceRecord* updated_record);
void calculate_employee_statistics(AttendanceRecord records[], int count, const char* employee_id);
void sort_records_by_id(AttendanceRecord records[], int count, int ascending);
void sort_records_by_date(AttendanceRecord records[], int count, int ascending);
void sort_records_by_work_hours(AttendanceRecord records[], int count, int ascending);
void find_duplicate_records(AttendanceRecord records[], int count);
void clean_invalid_records(AttendanceRecord records[], int* count);

#endif