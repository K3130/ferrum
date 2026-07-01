#include "ferrum/lib/log.h"

#include <linux/init.h>
#include <linux/module.h>

static int __init vnetint_init(void)
{
  int result = 0;
  log_info("Module init");
  return result;
}

static void __exit vnetint_exit(void)
{
  log_info("Module exit");
}


module_init(vnetint_init);
module_exit(vnetint_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("vnetint_module");
