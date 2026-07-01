#include "../lib/log.h"

#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Logging module for kernel");

void __log_print(int level, const char* level_str, const char* fmt, ...)
{
  va_list args;
  char buf[128];

  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  switch ( level )
  {
    case LOG_LEVEL_ERR:
    {
      pr_err("[%s] %s\n",  level_str, buf);
      break;
    }
    case LOG_LEVEL_WARN:
    {
      pr_warn("[%s] %s\n", level_str, buf);
      break;
    }
    case LOG_LEVEL_INFO:
    {
      pr_info("[%s] %s\n", level_str, buf);
      break;
    }
    case LOG_LEVEL_DEBUG:
    {
      pr_debug("[%s] %s\n", level_str, buf);
      break;
    }
    default:
    {
      pr_info("[%s] %s\n", level_str, buf);
      break;
    }
  }
}

