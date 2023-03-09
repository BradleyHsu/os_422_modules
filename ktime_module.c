/* simple_module.c - a simple template for a loadable kernel module in Linux,
 *    based on the hello world kernel module example on pages 338-339 of Robert
 *       Love's "Linux Kernel Development, Third Edition."
 *        */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timekeeping.h>
#include <linux/time64.h>
#include <linux/ktime.h>


static ktime_t init_time;
static ktime_t exit_time;

/* init function - logs that initialization happened, returns success */
static int 
simple_init(void)
{
    printk(KERN_ALERT "ktime_module initialized\n");
    init_time = ktime_get();
    return 0;
}

/* exit function - logs that the module is being removed */
static void 
simple_exit(void)
{
    ktime_t k_elapsed_time;
    struct timespec64 elapsed_time;
    exit_time = ktime_get();
    k_elapsed_time = ktime_sub(exit_time, init_time);
    elapsed_time = ktime_to_timespec64(k_elapsed_time);
    printk(KERN_ALERT "ktime_module is being unloaded\n elapsed time: %d sec %ld ns \n", \
                       (int)elapsed_time.tv_sec, elapsed_time.tv_nsec);
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("LKD Chapter 17");
MODULE_DESCRIPTION ("Simple CSE 422 Module Template");
