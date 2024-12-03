#include "base/file_handling.h"
#include "base/list.h"

int** parse_reports(String file);
bool is_report_safe(int* report);
bool is_report_with_dampener_safe(int* report, int* temp_list);
int count_safe_reports(int** reports);
int count_safe_reports_with_dampener(int** reports);

int main(void) {
    String file;
    if (!file_loadstr(&file, "../res/day02.txt")) {
        return 1;
    }

    int** reports = parse_reports(file);

    int safe_reports = count_safe_reports(reports);
    printf("a: %d\n", safe_reports);
    
    int safe_reports_dampened = count_safe_reports_with_dampener(reports);
    printf("b: %d\n", safe_reports_dampened);

    list_free(reports);
    free(file.chars);
    return 0;
}

int** parse_reports(String file) {
    int** levels = list(int*);
    int* current_level;

    StringView* lines = string_split(file, '\n');
    list_foreach(lines, StringView, line) {
        current_level = list(int);
        StringView* split = string_split(*(String*)line, ' ');

        for (int j = 0; j < list_length(split); j++) {
            int value = atoi(split[j].chars);
            list_append(current_level, value);
        }

        list_free(split);
        list_append(levels, current_level);
    }

    list_free(lines);

    return levels;
}


bool is_report_safe(int* report) {
    if (list_length(report) <= 1) {
        return true;
    }

    bool is_descending = report[0] > report[1]; 

    int prev_value = report[0];
    for (int i = 1; i < list_length(report); i++) {
        int value = report[i];
        if (is_descending && value > prev_value) {
            return false;
        }
        else if (!is_descending && value < prev_value) {
            return false;
        }

        int difference = abs(value - prev_value);

        if (difference == 0 || difference > 3) {
            return false;
        }
        prev_value = value;
    }   

    return true;
}

bool is_report_with_dampener_safe(int* report, int* temp_list) {
    if (is_report_safe(report)) {
        return true;
    }

    for (int i = 0; i < list_length(report); i++) {
        list_copyto(report, temp_list);
        list_remove(temp_list, i);
        if (is_report_safe(temp_list)) {
            return true;
        }
        list_reset(temp_list);
    }

    return false;
}

int count_safe_reports(int** reports) {
    int safe_reports = 0;
    for (int i = 0; i < list_length(reports); i++) {
        int* report = reports[i];
        safe_reports += is_report_safe(report);
    }
    return safe_reports;
}

int count_safe_reports_with_dampener(int** reports) {
    int safe_reports = 0;
    int* temp_list = list(int);
    for (int i = 0; i < list_length(reports); i++) {
        int* report = reports[i];
        safe_reports += is_report_with_dampener_safe(report, temp_list);
        list_reset(temp_list);
    }
    list_free(temp_list);
    return safe_reports;
}
