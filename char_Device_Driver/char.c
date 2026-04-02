/* cha.c */
#include <linux/module.h>
#include <linux/init.h>  /* Located in /usr/lib/modules/5.15.102-1-MANJARO/build/include/linux/init.h */
#include <linux/fs.h> 
#include <linux/cdev.h> 


/* Meta information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("VAS");
MODULE_DESCRIPTION("Char device kernel module.");


#define DRIVER_NAME "MyDevice" 
#define DRIVER_CLASS "MyModuleClass" 

static dev_t my_device_number;
static struct class *my_class;
static struct cdev my_device;



// ========= fn =========
static ssize_t diver_read(struct file *File, char *user_buffer, size_t cont, loff_t *offs); 

static struct file_operations fops = { 
    .owner = THIS_MODULE
    // .read = driver_read 
};

// ====================== INIT ===========================
static int __init ModuleInit(void) {
	/* Allocate device number */
	if( alloc_chrdev_region(&my_device_number, 0, 1, DRIVER_NAME) < 0) {
		printk("Device number could not be allocated\n");
		return -1;
	}
	printk("read-write: Device number major: %d, minor: %d registered\n", MAJOR(my_device_number), MINOR(my_device_number));

	/* Create device class */
	if ( (my_class = class_create(DRIVER_CLASS)) == NULL) {
		printk("Device class can't be created\n");
		goto ClassError;
	}

	/* Create device file */
	if (device_create(my_class, NULL, my_device_number, NULL, DRIVER_NAME) == NULL) {
		printk("Can't create device file\n");
		goto FileError;
	}

	/* Init device file */
	cdev_init(&my_device, &fops);

	/* Registering device to kernel */
	if (cdev_add(&my_device, my_device_number, 1) == -1) {
		printk("Can't register device to kernel\n");
		goto AddError;
	}

	return 0;

AddError:
	device_destroy(my_class, my_device_number);

FileError:
	class_destroy(my_class);

ClassError:
	unregister_chrdev(my_device_number, DRIVER_NAME);
	return -1;
}






// ====================== INIT ===========================
static void __exit ModuleExit(void) {
	cdev_del(&my_device);
	device_destroy(my_class, my_device_number);
	class_destroy(my_class);
	unregister_chrdev(my_device_number, DRIVER_NAME);
}







module_init(ModuleInit);
module_exit(ModuleExit);