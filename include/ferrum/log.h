#ifndef FERRUM_LOG_H
#define FERRUM_LOG_H

#include <linux/printk.h>

#ifndef FERRUM_LOG_TAG
#define FERRUM_LOG_TAG "ferrum"
#endif

#define fr_info(fmt, ...) pr_info("[" FERRUM_LOG_TAG "] " fmt, ##__VA_ARGS__)
#define fr_warn(fmt, ...) pr_warn("[" FERRUM_LOG_TAG "] " fmt, ##__VA_ARGS__)
#define fr_err(fmt, ...)  pr_err("[" FERRUM_LOG_TAG "] " fmt, ##__VA_ARGS__)
#define fr_dbg(fmt, ...)  pr_debug("[" FERRUM_LOG_TAG "] " fmt, ##__VA_ARGS__)

#endif /* FERRUM_LOG_H */