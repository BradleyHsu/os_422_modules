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
#include <linux/path.h>
#include <linux/mount.h>
#include <linux/dcache.h>
#include <linux/fs.h>
#include <linux/list.h>


static struct task_struct *thread;

static int print_task_fields(void *data) {
    struct task_struct *current_task;
    struct path pwd_path, root_path;
    struct dentry *pwd_dentry, *root_dentry;
    struct dentry *dentry_cursor;
    struct list_head *pos;

    current_task = current;

    printk(KERN_INFO "Thread started\n");

    get_fs_pwd(current_task->fs, &pwd_path);
    get_fs_root(current_task->fs, &root_path);

    pwd_dentry = pwd_path.dentry;
    root_dentry = root_path.dentry;

    printk(KERN_INFO "pwd dentry: %px\n", pwd_dentry);
    printk(KERN_INFO "root dentry: %px\n", root_dentry);

    if (pwd_dentry != root_dentry) {
        printk(KERN_INFO "pwd d_iname: %s\n", pwd_dentry->d_iname);
        printk(KERN_INFO "root d_iname: %s\n", root_dentry->d_iname);
    } else {
        printk(KERN_INFO "pwd and root d_iname: %s\n", pwd_dentry->d_iname);
    }
    spin_lock(&root_dentry->d_lock);
    list_for_each(pos, &root_dentry->d_subdirs) {
        dentry_cursor = list_entry(pos, struct dentry, d_child);
        if (!list_empty(&dentry_cursor->d_subdirs)) {
            printk(KERN_INFO "Non-empty subdir entry d_iname: %s\n", dentry_cursor->d_iname);
        }
    }
    spin_unlock(&root_dentry->d_lock);
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
