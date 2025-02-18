#include <linux/module.h>
#include <linux/uaccess.h>
// #include <linux/kernel.h>
// #include <linux/init.h>
// #include <linux/kthread.h>
// #include <linux/delay.h>

#define DEVICE_NAME "loopback"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Emer");
MODULE_DESCRIPTION("A simple kernel module.");
MODULE_VERSION("1.0");

static int major_number; // stores dynamically allocated major number
static char buffer [BUFFER_SIZE]; // internal buffer size
static size_t buffer_size = 0; // keeps track of how much in buffer

static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "device opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "device closed\n");
    return 0;
}

//function to handle write and log operations
static ssize_t device_read(struct file *file, const char __user *user buffer, size_t len, loff_t *offset){
    size_t bytes_to_read = min(len, buffer_size);
    if (copy_to_user(user_buffer, buffer, bytes_to_read)){ // copy data to user space
        return -EFAULT;
    }
    buffer_size = 0;
    printk(KERN_INFO "Device read %zu bytes\n", bytes_to_read); // log device upon read
    return bytes_to_read;
}

// add function to handle write operations

static struct task_struct *thread1;
static struct task_struct *thread2;

// Thread function for thread 1
static int thread_fn1(void *data) {
    printk(KERN_INFO "[Thread 1] Starting thread 1\n");
    while (!kthread_should_stop()) {
        printk(KERN_INFO "[Thread 1] Hello from thread 1\n");
        ssleep(2); // Sleep for 2 seconds
    }
    printk(KERN_INFO "[Thread 1] Stopping thread 1\n");
    return 0;
}

// Thread function for thread 2
static int thread_fn2(void *data) {
    printk(KERN_INFO "[Thread 2] Starting thread 2\n");
    while (!kthread_should_stop()) {
        printk(KERN_INFO "[Thread 2] Hello from thread 2\n");
        ssleep(3); // Sleep for 3 seconds
    }
    printk(KERN_INFO "[Thread 2] Stopping thread 2\n");
    return 0;
}

// Function called when the module is loaded
static int __init my_module_init(void) {
    printk(KERN_INFO "[Module] Loading kernel module\n");

    // Create and run thread 1
    thread1 = kthread_run(thread_fn1, NULL, "thread1");
    if (IS_ERR(thread1)) {
        printk(KERN_ERR "[Module] Failed to create thread 1\n");
        return PTR_ERR(thread1);
    }

    // Create and run thread 2
    thread2 = kthread_run(thread_fn2, NULL, "thread2");
    if (IS_ERR(thread2)) {
        printk(KERN_ERR "[Module] Failed to create thread 2\n");
        // Stop thread 1 if thread 2 creation fails
        kthread_stop(thread1);
        return PTR_ERR(thread2);
    }

    printk(KERN_INFO "[Module] Kernel module loaded successfully\n");
    return 0; // Return 0 means success
}

// Function called when the module is unloaded
static void __exit my_module_exit(void) {
    printk(KERN_INFO "[Module] Unloading kernel module\n");

    // Stop thread 1
    if (thread1) {
        kthread_stop(thread1);
        printk(KERN_INFO "[Module] Thread 1 stopped\n");
    }

    // Stop thread 2
    if (thread2) {
        kthread_stop(thread2);
        printk(KERN_INFO "[Module] Thread 2 stopped\n");
    }

    printk(KERN_INFO "[Module] Kernel module unloaded successfully\n");
}

// Register module entry and exit points
module_init(my_module_init);
module_exit(my_module_exit);
