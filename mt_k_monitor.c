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
static int NUM_THREADS = 4;
static ktime_t timer_interval; 
static struct hrtimer timer;
static struct task_struct *task[NUM_THREADS] = {NULL}; 
int return_value;
int test;

static int body_callback(void *data)
{
    int iterations = 0; 
    printk(KERN_ALERT "lab 1 module body callback starting");
    do {
        iterations = iterations + 1;
        printk(KERN_ALERT "lab 1 module body callback");
        printk(KERN_DEBUG "nvcsw: %lu, nivcsw: %lu\n", current->nvcsw, current->nivcsw);
        printk(KERN_DEBUG "iterations: %d", iterations);
        printk(KERN_DEBUG "thread: %d", smp_processor_id());
        set_current_state(TASK_INTERRUPTIBLE);
        schedule();
    } while(!kthread_should_stop());
    printk(KERN_ALERT "lab 1 module body callback terminating");
    return 0;
}

//timer callback
static enum hrtimer_restart timer_callback(struct hrtimer *timer)
{
    printk(KERN_ALERT "lab 1 module timer callback");
    for (int thread = 0; thread < NUM_THREADS; thread++)
    {
        wake_up_process(task[thread]);
    }
    hrtimer_forward_now(timer, timer_interval);
    return HRTIMER_RESTART;
}


/* init function - logs that initialization happened, returns success */
static int 
k_monitor_init(void)
{
    printk(KERN_ALERT "k_monitor module initialized\n");
    
    for(int thread = 0; thread < NUM_THREADS; thread++) {
        task[thread] = kthread_create(body_callback, NULL, "k_monitor");
        if(IS_ERR(task[thread]))
        {
            printk(KERN_ALERT "k_monitor module failed to create thread\n");
            printk(KERN_DEBUG "nvcsw: %lu, nivcsw: %lu\n", current->nvcsw, current->nivcsw);
            return PTR_ERR(task[thread]);
        }
    }

    for (int thread = 0; thread < NUM_THREADS; thread++) {
        kthread_bind(task[thread], thread);
    }   
    for (int thread = 0; thread < NUM_THREADS; thread++) {
        wake_up_process(task[thread]);
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
    for (int thread = 0; thread < NUM_THREADS; thread++) {
        kthread_stop(task[thread]);
    }
    hrtimer_cancel(&timer);
}

module_init(k_monitor_init);
module_exit(k_monitor_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("LKD Chapter 17");
MODULE_DESCRIPTION ("Simple CSE 422 Module Template");
