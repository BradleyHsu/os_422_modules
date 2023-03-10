/* simple_module.c - a simple template for a loadable kernel module in Linux,
 *    based on the hello world kernel module example on pages 338-339 of Robert
 *       Love's "Linux Kernel Development, Third Edition."
 *        */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

const int thread_count = 4
/* init function - logs that initialization happened, returns success */
static int 
race_init(void)
{
    printk(KERN_ALERT "simple module initialized\n");
    return 0;
}

/* exit function - logs that the module is being removed */
static void 
race_exit(void)
{
    printk(KERN_ALERT "simple module is being unloaded\n");
}

module_init(race_init);
module_exit(race_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("LKD Chapter 17");
MODULE_DESCRIPTION ("Simple CSE 422 Module Template");
