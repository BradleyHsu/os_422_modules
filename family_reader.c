// SPDX-License-Identifier: GPL-2.0
/*
 * Sample kobject implementation
 *
 * Copyright (C) 2004-2007 Greg Kroah-Hartman <greg@kroah.com>
 * Copyright (C) 2007 Novell Inc.
 */
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/sched.h>

/*
 * This module shows how to create a simple subdirectory in sysfs called
 * /sys/kernel/kobject-example  In that directory, 3 files are created:
 * "process_int", "baz", and "bar".  If an integer is written to these files, it can be
 * later read out of it.
 */

static int process_int;

/*
 * The "process_int" file where a static variable is read from and written to.
 */
static ssize_t process_int_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%d\n", process_int);
}

static ssize_t process_int_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	int ret;
	struct pid *vpid;
	struct task_struct *current_task;
	pid_t cur_pid;
	char *cur_comm;
	const pid_t INIT = 1;

	ret = kstrtoint(buf, 10, &process_int);
	vpid = find_vpid(process_int);

	if (vpid == NULL) {
		printk("find_vpid failed");
		return -1;
	}

	current_task = get_pid_task(vpid, PIDTYPE_PID);
	cur_pid = current_task->pid;
	cur_comm = current_task->comm;

	while (cur_pid != INIT) {
		printk("pid: %d, comm: %s \n", cur_pid, cur_comm);

		current_task = current_task->real_parent;
		cur_pid = current_task->pid;
		cur_comm = current_task->comm;
	}

	printk("pid: %d, comm: %s \n", cur_pid, cur_comm);

	if (ret < 0)
		return ret;

	return count;
}

/* Sysfs attributes cannot be world-writable. */
static struct kobj_attribute process_int_attribute =
	__ATTR(process_int, 0664, process_int_show, process_int_store);


/*
 * Create a group of attributes so that we can create and destroy them all
 * at once.
 */
static struct attribute *attrs[] = {
	&process_int_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

/*
 * An unnamed attribute group will put all of the attributes directly in
 * the kobject directory.  If we specify a name, a subdirectory will be
 * created for the attributes with the directory being the name of the
 * attribute group.
 */
static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *example_kobj;

static int __init example_init(void)
{
	int retval;

	/*
	 * Create a simple kobject with the name of "kobject_example",
	 * located under /sys/kernel/
	 *
	 * As this is a simple directory, no uevent will be sent to
	 * userspace.  That is why this function should not be used for
	 * any type of dynamic kobjects, where the name and number are
	 * not known ahead of time.
	 */
	example_kobj = kobject_create_and_add("family_reader", kernel_kobj);
	if (!example_kobj)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_group(example_kobj, &attr_group);
	if (retval)
		kobject_put(example_kobj);

	return retval;
}

static void __exit example_exit(void)
{
	kobject_put(example_kobj);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Greg Kroah-Hartman <greg@kroah.com>");
