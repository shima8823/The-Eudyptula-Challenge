#include <linux/module.h>
#include <linux/printk.h>
#include <linux/miscdevice.h>
// errno: include/uapi/asm-generic/errno-base.h

MODULE_LICENSE("GPL");

#define DEVICE_NAME "fortytwo"
#define STUDENT_LOGIN "shshimad"

static ssize_t ft_read(struct file *file, char __user *buf, size_t count, loff_t *offset) {
	int len = strlen(STUDENT_LOGIN);

	ssize_t ret = len;

	if (*offset >= len || copy_to_user(buf, STUDENT_LOGIN, len)) {
			pr_info("copy_to_user failed\n");
			ret = 0;
	}

	*offset += len;

	return ret;
}

static ssize_t ft_write(struct file *file, const char __user *buf, size_t count, loff_t *offset) {

	char kbuf[256];

	if (copy_from_user(kbuf, buf, count))
		return -EFAULT;

	if (strncmp(kbuf, STUDENT_LOGIN, count) != 0)
		return -EINVAL;

	return count;
}

static const struct file_operations ft_fops = {
	.owner = THIS_MODULE,
	.read = ft_read,
	.write = ft_write,
};

static struct miscdevice ft_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &ft_fops,
};

static int __init device_init(void) {
	return misc_register(&ft_misc_device);
}

static void __exit device_exit(void) { 
	misc_deregister(&ft_misc_device);
}

module_init(device_init);
module_exit(device_exit);
