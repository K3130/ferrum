#pragma once
/*
 *  Этот модуль овечает за вывод лога в пространство ядра.
 *  Есть уровни логирования но пока без проверки и хранения состояния.
 *  По факту просто обёртка.
 *
 *
*/
#include <linux/types.h>
#include <linux/printk.h>

#define LOG_LEVEL_ERR     0
#define LOG_LEVEL_WARN    1
#define LOG_LEVEL_INFO    2
#define LOG_LEVEL_DEBUG   3

#define log_err(fmt, ...)   __log_print(LOG_LEVEL_ERR,   "ERR",  fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...)  __log_print(LOG_LEVEL_WARN,  "WARN", fmt, ##__VA_ARGS__)
#define log_info(fmt, ...)  __log_print(LOG_LEVEL_INFO,  "INFO", fmt, ##__VA_ARGS__)
#define log_debug(fmt, ...) __log_print(LOG_LEVEL_DEBUG, "DEBUG", fmt, ##__VA_ARGS__)

void __log_print(int level, const char* level_str, const char* fmt, ...)
    __attribute__((format(printf, 3, 4)));
