/* simple_module.c - a simple template for a loadable kernel module in Linux,
 *    based on the hello world kernel module example on pages 338-339 of Robert
 *       Love's "Linux Kernel Development, Third Edition."
 *        */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/types.h>

#define num_threads 4
#define num_iters 1000000

atomic_t shared_data;
struct task_struct *thread_structs[num_threads];


static int thread_fn(void *data) {
    int iter;
    printk("starting initialization of thread %d\n");
    for (iter = 0; iter < num_iters; iter++) {
        atomic_add(1, &shared_data);
    }
    printk("done initialization of thread %d\n", *data);
    while (!kthread_should_stop()) {
        schedule();
    }
    return 0;
}

/* init function - logs that initialization happened, returns success */
static int race_init(void) {
    int cpu; 
    atomic_set(&shared_data, 0);
    for (cpu = 0; cpu < num_threads; cpu++) {
        thread_structs[cpu] = kthread_create(&thread_fn, CPU, "thread");
        kthread_bind(thread_structs[cpu], cpu);
        wake_up_process(thread_structs[cpu]);
    }
    printk(KERN_ALERT "atomic module initialized\n");
    return 0;
}

/* exit function - logs that the module is being removed */
static void race_exit(void) {
    int cpu;
    for (cpu = 0; cpu < num_threads; cpu++) {
        kthread_stop(thread_structs[cpu]);
    }
    printk(KERN_ALERT "atomic module unloaded\n got final shared_data value %d\n", atomic_read(&shared_data));
}

module_init(race_init);
module_exit(race_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("LKD Chapter 17");
MODULE_DESCRIPTION ("Simple CSE 422 Module Template");
