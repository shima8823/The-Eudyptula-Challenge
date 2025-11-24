#include <linux/module.h>
#include <linux/printk.h>
#include <linux/miscdevice.h>
// errno: include/uapi/asm-generic/errno-base.h

MODULE_LICENSE("GPL");

#define DEVICE_NAME "fortytwo"
#define STUDENT_LOGIN "shshimad"

static ssize_t ft_read(struct file *file, char __user *buf, size_t count, loff_t *offset) {
	size_t len = strlen(STUDENT_LOGIN);

	// すでに全部読み終わっていれば EOF or ユーザが0バイト読みたい場合は何もしない
	if (*offset >= len || count == 0)
		return 0;

	// 残りの文字数を超えてコピーしないように調整
	if (count > len - *offset)
		count = len - *offset;

	// 今回は失敗した時点でERROR
	if (copy_to_user(buf, STUDENT_LOGIN + *offset, count))
		return -EFAULT;

	// 読み進めた分だけオフセットを進める
	*offset += count;

	return count;
}

static ssize_t ft_write(struct file *file, const char __user *buf, size_t count, loff_t *offset) {
	char kbuf[256];

	if (count > sizeof(kbuf))
		return -EINVAL;
	if (copy_from_user(kbuf, buf, count))
		return -EFAULT;
	if (count != strlen(STUDENT_LOGIN) ||
		strncmp(kbuf, STUDENT_LOGIN, count) != 0)
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
