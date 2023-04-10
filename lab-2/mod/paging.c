#include <linux/init.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/memory.h>
#include <linux/mm.h>

#include <paging.h>

static atomic_t alloc_count = ATOMIC_INIT(0);
static atomic_t free_count = ATOMIC_INIT(0);
static unsigned int demand_paging = 1;
module_param(demand_paging, uint, 0644);


struct vma_private {
    atomic_t ref_count;
    int num_pages;
    spinlock_t pages_lock;
    struct page *pages[500];
};


void increment_alloc_count(void) {
    atomic_inc(&alloc_count);
}

void increment_free_count(void) {
    atomic_inc(&free_count);
}

int increment_ref_count(struct vma_private *vma_priv) {
    return atomic_inc_return(&vma_priv->ref_count);
}

int decrement_ref_count(struct vma_private *vma_priv) {
    return atomic_dec_return(&vma_priv->ref_count);
}

struct vma_private* get_vma_private(struct vm_area_struct *vma) {
    return (struct vma_private *)vma->vm_private_data;
}

struct vma_private* alloc_vma_private(struct vm_area_struct *vma) {
    int size = sizeof(struct vma_private);
    struct vma_private *vma_priv = kmalloc(size, GFP_KERNEL);
    atomic_t *ref_count;
    if (vma_priv == NULL) {
        printk(KERN_ERR "Could not allocate vma_priv");
        return NULL;
    }
    memset(vma_priv, 0, size);
    ref_count = &vma_priv->ref_count;
    atomic_set(ref_count, 1);
    spin_lock_init(&vma_priv->pages_lock);
    vma->vm_private_data = vma_priv;
    return vma_priv;
}

void free_vma_private(struct vm_area_struct *vma) {
    kfree(vma->vm_private_data);
}

void free_vma_pages(struct vma_private *vma_priv) {
    int page;
    for (page = 0; page < vma_priv->num_pages; page++) {
        if (vma_priv->pages[page] == NULL) {
            printk(KERN_ERR "Page is null\n");
        }
        __free_pages(vma_priv->pages[page], 0);
        increment_free_count();
    }
}

void handle_close(struct vm_area_struct *vma) {
    //free all pages
    struct vma_private *vma_priv = get_vma_private(vma);
    int references = decrement_ref_count(vma_priv);
    if (references == 0) {
        free_vma_pages(vma_priv);
        free_vma_private(vma);
    }
}

void append_new_address(struct vm_area_struct *vma, struct page *page) {
    struct vma_private *vma_priv;
    unsigned long flags;

    vma_priv = get_vma_private(vma);

    spin_lock_irqsave(&vma_priv->pages_lock, flags);
    vma_priv->pages[vma_priv->num_pages] = page;
    vma_priv->num_pages++;
    spin_unlock_irqrestore(&vma_priv->pages_lock, flags);

    if (vma_priv->num_pages == 500) {
        printk(KERN_ERR "Too many pages");
    }
}

static int alloc_vma_page(struct vm_area_struct *vma, unsigned long fault_address) {
    struct page *new_page;
    int err;
    new_page = alloc_page(GFP_KERNEL);
    if (new_page == NULL) {
        printk(KERN_ERR "Failed to allocate page");
        return VM_FAULT_OOM;
    }

    append_new_address(vma, new_page);

    err = remap_pfn_range(vma, PAGE_ALIGN(fault_address), page_to_pfn(new_page), PAGE_SIZE, vma->vm_page_prot);
    if (err != 0) {
        printk(KERN_ERR "Failed to remap page to VMA");
        __free_pages(new_page, 0);
        return VM_FAULT_SIGBUS;
    }

    increment_alloc_count();
    printk(KERN_INFO "paging_vma_fault() success: took a page fault at VA 0x%lx\n", fault_address);
    return VM_FAULT_NOPAGE;
}
static int
do_fault(struct vm_area_struct * vma,
         unsigned long           fault_address)
{
    int err = alloc_vma_page(vma, fault_address);
    return err;
}

static vm_fault_t
paging_vma_fault(struct vm_fault * vmf)
{
    struct vm_area_struct * vma = vmf->vma;
    unsigned long fault_address = (unsigned long)vmf->address;

    return do_fault(vma, fault_address);
}

static void
paging_vma_open(struct vm_area_struct * vma)
{
    printk(KERN_INFO "paging_vma_open() invoked\n");
    alloc_vma_private(vma);
}

static void
paging_vma_close(struct vm_area_struct * vma)
{
    printk(KERN_INFO "paging_vma_close() invoked\n");
    handle_close(vma);
}

static struct vm_operations_struct
paging_vma_ops = 
{
    .fault = paging_vma_fault,
    .open  = paging_vma_open,
    .close = paging_vma_close
};

int allocate_pages_for_vma(struct vm_area_struct *vma) {
    unsigned long addr;
    int err = VM_FAULT_NOPAGE;

    // Iterate through the virtual address range
    for (addr = vma->vm_start; addr < vma->vm_end; addr += PAGE_SIZE) {
        // Allocate a page
        err = alloc_vma_page(vma, addr);
        if (err != VM_FAULT_NOPAGE) {
            break;
        }
    }

    return err;
}

/* vma is the new virtual address segment for the process */
static int
paging_mmap(struct file           * filp,
            struct vm_area_struct * vma)
{
    int err;
    if (!demand_paging) {
        err = VM_FAULT_NOPAGE;
    } else {
        err = 0;
    }
    /* prevent Linux from mucking with our VMA (expanding it, merging it 
     * with other VMAs, etc.)
     */
    vma->vm_flags |= VM_IO | VM_DONTCOPY | VM_DONTEXPAND | VM_NORESERVE
              | VM_DONTDUMP | VM_PFNMAP;

    /* setup the vma->vm_ops, so we can catch page faults */
    vma->vm_ops = &paging_vma_ops;

    if (get_vma_private(vma) == NULL) {
        alloc_vma_private(vma);
    } 

    printk(KERN_INFO "paging_mmap() invoked: new VMA for pid %d from VA 0x%lx to 0x%lx\n",
        current->pid, vma->vm_start, vma->vm_end);

    if (!demand_paging) {
        err = allocate_pages_for_vma(vma);
        if (allocate_pages_for_vma(vma) != VM_FAULT_NOPAGE) {
            printk(KERN_ERR "Failed to allocate pages for VMA");
        }
    }
    return err;
}

static struct file_operations
dev_ops =
{
    .mmap = paging_mmap,
};

static struct miscdevice
dev_handle =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = PAGING_MODULE_NAME,
    .fops = &dev_ops,
};
/*** END device I/O **/


/*** Kernel module initialization and teardown ***/
static int
kmod_paging_init(void)
{
    int status;

    /* Create a character device to communicate with user-space via file I/O operations */
    status = misc_register(&dev_handle);
    if (status != 0) {
        printk(KERN_ERR "Failed to register misc. device for module\n");
        return status;
    }

    printk(KERN_INFO "Loaded kmod_paging module v8\n");

    return 0;
}

static void
kmod_paging_exit(void)
{
    /* Deregister our device file */
    misc_deregister(&dev_handle);

    printk(KERN_INFO "Unloaded kmod_paging module\n");
    printk(KERN_INFO "Allocated %d pages, freed %d pages\n", atomic_read(&alloc_count), atomic_read(&free_count));
}

module_init(kmod_paging_init);
module_exit(kmod_paging_exit);

/* Misc module info */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Who are you?");
MODULE_DESCRIPTION("Please describe this module.");
