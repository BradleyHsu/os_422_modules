/* simple_module.c - a simple template for a loadable kernel module in Linux,
 *    based on the hello world kernel module example on pages 338-339 of Robert
 *       Love's "Linux Kernel Development, Third Edition."
 *        */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/fs_struct.h>
#include <linux/fdtable.h>
#include <linux/nsproxy.h>
#include <asm/current.h>


static struct task_struct *thread;

static int print_task_fields(void *data) {
    struct task_struct *current_task;
    current_task = current;

    printk(KERN_INFO "Thread started\n");

    printk(KERN_INFO "fs: %px\n", current_task->fs);
    printk(KERN_INFO "files: %px\n", current_task->files);
    printk(KERN_INFO "nsproxy: %px\n", current_task->nsproxy);

    printk(KERN_INFO "Thread finished\n");

    return 0;
}

/* init function - logs that initialization happened, returns success */
static int 
simple_init(void)
{
    printk(KERN_ALERT "vfs module initialized\n");
    thread = kthread_run(print_task_fields, NULL, "task_fields_thread");
    if (IS_ERR(thread)) {
        printk(KERN_ERR "Failed to create kernel thread\n");
        return PTR_ERR(thread);
    }

    return 0;
}

/* exit function - logs that the module is being removed */
static void 
simple_exit(void)
{
    printk(KERN_ALERT "vfs module is being unloaded\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("LKD Chapter 17");
MODULE_DESCRIPTION ("Simple CSE 422 Module Template");
