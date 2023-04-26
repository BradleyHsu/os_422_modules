#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/time.h>
#include <linux/kthread.h>
#include <linux/mm.h>

#include <asm/uaccess.h>

static uint nr_structs = 2000;
module_param(nr_structs, uint, 0644); 

static struct task_struct * kthread = NULL;

#define ARR_SIZE 8
typedef struct datatype_t {
   unsigned int array[ARR_SIZE];
} datatype;

unsigned int nr_pages;
unsigned int nr_structs_per_page;
unsigned int order;

static struct page * pages;

static unsigned int
my_get_order(unsigned int value)
{
    unsigned int shifts = 0;

    if (!value)
        return 0;

    if (!(value & (value - 1)))
        value--;

    while (value > 0) {
        value >>= 1;
        shifts++;
    }

    return shifts;
}

static int
thread_fn(void * data)
{
    unsigned long page_frame_number;
    unsigned long p_addr;
    unsigned long v_addr;
    int i = 0;
    int j = 0;
    int k = 0;
    printk("Hello from thread %s. nr_structs=%u\n", current->comm, nr_structs);

    printk(KERN_INFO "Page size: %lu\n, datatype struct size %u, num structs per page %lu\n", PAGE_SIZE, sizeof(datatype), PAGE_SIZE/sizeof(datatype));
    while (!kthread_should_stop()) {
        schedule();
    }

    nr_structs_per_page = PAGE_SIZE/sizeof(datatype);
    nr_pages = nr_structs/nr_structs_per_page;
    if (nr_structs % nr_structs_per_page != 0) {
        nr_pages++;
    }
    order = my_get_order(nr_pages);
    printk(KERN_INFO "nr_pages %u, nr_structs_per_page %u, order %u\n", nr_pages, nr_structs_per_page, order);

    pages = alloc_pages(GFP_KERNEL, order);
    if (!pages) {
        printk(KERN_ERR "Failed to allocate pages\n");
        return -ENOMEM;
    }

    page_frame_number = page_to_pfn(pages);
    p_addr = page_to_phys(pages);
    v_addr = (unsigned long) page_address(pages);

    for (i = 0; i < nr_pages; i++) {
        unsigned long cur_page = (unsigned long) p_addr + i*PAGE_SIZE;
        for (j = 0; j < nr_structs_per_page; j++) {
            unsigned long cur_struct = cur_page + (j * sizeof(datatype));
            datatype * this_struct = (struct datatype_t *) __va(cur_struct);
            for (k = 0; k < ARR_SIZE; k++) {
                this_struct->array[k] = i*nr_structs_per_page*ARR_SIZE + j*ARR_SIZE + k;
                if ((i == 0) && (j == 0)) {
                    printk(KERN_INFO "this_struct->array[%d] = %u\n", k, this_struct->array[k]);
                }
            }
        }
    }



    return 0;
}

static int
kernel_memory_init(void)
{
    printk(KERN_INFO "Loaded kernel_memory module\n");

    kthread = kthread_create(thread_fn, NULL, "k_memory");
    if (IS_ERR(kthread)) {
        printk(KERN_ERR "Failed to create kernel thread\n");
        //print page size using macro PAGE_SIZE, how large the datatype struct is using sizeof, and how many pages are needed to store nr_structs of datatype
        return PTR_ERR(kthread);
    }
    
    wake_up_process(kthread);

    return 0;
}

static void 
kernel_memory_exit(void)
{
    kthread_stop(kthread);
    printk(KERN_INFO "Unloaded kernel_memory module\n");
}

module_init(kernel_memory_init);
module_exit(kernel_memory_exit);

MODULE_LICENSE ("GPL");
