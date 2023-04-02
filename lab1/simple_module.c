/* simple_module.c - a simple template for a loadable kernel module in Linux,
 *    based on the hello world kernel module example on pages 338-339 of Robert
 *       Love's "Linux Kernel Development, Third Edition."
 *        */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

//static vars
static ktime_t timer_interval;
static struct hrtimer timer;

//params
static ulong log_sec = 1;
static ulong log_nsec = 0;

module_param(log_sec, ulong, 0);
module_param(log_nsec, ulong, 0);

//timer callback
static enum hrtimer_restart timer_callback(struct hrtimer *timer)
{
    printk(KERN_ALERT "lab 1 module timer callback");
    hrtimer_forward_now(timer, timer_interval);
    return HRTIMER_RESTART;
}


/* init function - logs that initialization happened, returns success */
static int 
simple_init(void)
{
    printk(KERN_ALERT "lab 1 module initialized\n");
    hrtimer_init(timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    timer.function = &timer_callback;
    hrtimer_start(timer, timer_interval, HRTIMER_MODE_REL)
    return 0;
}

/* exit function - logs that the module is being removed */
static void 
simple_exit(void)
{
    printk(KERN_ALERT "lab 1 module is being unloaded\n");
    hrtimer_cancel(timer);
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("LKD Chapter 17");
MODULE_DESCRIPTION ("Simple CSE 422 Module Template");
