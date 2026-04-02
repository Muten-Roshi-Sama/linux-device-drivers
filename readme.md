# Device Drivers for Linux

One of the fundamental devices we can control using a driver is a **character device**.
By registering a device as such, we can communicate with it using basic `read` and `write` system calls.

In this first introduction, we will create a dummy device that simply stores what we send it (`write`) and can spit it back upon request (`read`).

## Major and minor numbers
Devices have a major and a minor number. The former is unique to a category of devices, the latter is unique within a device group and is handled at the discretion of the group.

`/proc/devices` holds the major numbers currently in use by the OS, whilst `/usr/include/linux/major.h` lists reserved major numbers.

When creating your own device, you could simply pick an available major number, but this is **ad practice**. Your code becomes less portable (nothing guarantees your that this specific value is not reserved no another platform) and harder to maintain (what if the value you picked becomes reserved upstream in the future?).
Instead we will see how to gently ask the kernel for an available value and work with what we get.


## Device driver

First of all, we need a **file operations structure** that defines the function callbacks for various actions.
The structure is defined in `<linux/fs.h>`.
```C
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.read = driver_read,
	.write = driver_write
};
```

We need two functions to **open and close** the device. In this instance, they don't do anything particular.
```C
static int driver_open(struct inode *device_file, struct file *instance){
	printk("read-write module - open was called!\n");
	return 0;
}

static int driver_close(struct inode *device_file, struct file *instance){
	printk("read-write module - close was called!\n");
	return 0;
}
```