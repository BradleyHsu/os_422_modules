/* simple_module.c - a simple template for a loadable kernel module in Linux,
 *    based on the hello world kernel module example on pages 338-339 of Robert
 *       Love's "Linux Kernel Development, Third Edition."
 *        */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/sched.h>
#include <linux/kthread.h>

//params
static ulong log_sec = 1;
static ulong log_nsec = 0;

module_param(log_sec, ulong, 0);
module_param(log_nsec, ulong, 0);

//static vars
static ktime_t timer_interval; 
static struct hrtimer timer;
static struct task_struct *task = NULL;
int return_value;

static int body_callback(void *data)
{
    int iterations = 0; 
    printk(KERN_ALERT "lab 1 module body callback starting\n");
    do {
        iterations = iterations + 1;
        printk(KERN_ALERT "lab 1 module body callback\n");
        printk(KERN_DEBUG "nvcsw: %lu, nivcsw: %lu\n", current->nvcsw, current->nivcsw);
        printk(KERN_DEBUG "iterations: %d\n", iterations);
        set_current_state(TASK_INTERRUPTIBLE);
        schedule();
    } while(!kthread_should_stop());
    printk(KERN_ALERT "lab 1 module body callback terminating\n");
    return 0;
}

//timer callback
static enum hrtimer_restart timer_callback(struct hrtimer *timer)
{
    printk(KERN_ALERT "lab 1 module timer callback\n");
    wake_up_process(task);
    hrtimer_forward_now(timer, timer_interval);
    return HRTIMER_RESTART;
}


/* init function - logs that initialization happened, returns success */
static int 
k_monitor_init(void)
{
    printk(KERN_ALERT "k_monitor module initialized\n");

    task = kthread_run(body_callback, NULL, "k_monitor");
    if(IS_ERR(task))
    {
        printk(KERN_ALERT "k_monitor module failed to create thread\n");
        printk(KERN_DEBUG "nvcsw: %lu, nivcsw: %lu\n", current->nvcsw, current->nivcsw);
        return PTR_ERR(task);
    }

    timer_interval = ktime_set(log_sec, log_nsec);
    hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    timer.function = &timer_callback;
    hrtimer_start(&timer, timer_interval, HRTIMER_MODE_REL);
    return 0;
}

/* exit function - logs that the module is being removed */
static void 
k_monitor_exit(void)
{
    printk(KERN_ALERT "k_monitor module is being unloaded\n");
    return_value = kthread_stop(task);
    hrtimer_cancel(&timer);
}

module_init(k_monitor_init);
module_exit(k_monitor_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("LKD Chapter 17");
MODULE_DESCRIPTION ("Simple CSE 422 Module Template");
