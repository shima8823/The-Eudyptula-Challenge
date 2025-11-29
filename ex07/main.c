#include <linux/module.h>
#include <linux/printk.h>
#include <linux/debugfs.h>

MODULE_LICENSE("GPL");

#define SUBDIR_NAME "fortytwo"
#define STUDENT_LOGIN "shshimad"

static struct dentry *dir = NULL;

static int add_write_op(void *data, u64 value)
{
	return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(add_fops, NULL, add_write_op, "%llu\n");

static ssize_t id_read(struct file *file, char __user *buf, size_t count, loff_t *offset) {
	size_t len = strlen(STUDENT_LOGIN);

	// すでに全部読み終わっていればEOF or ユーザが0バイト読みたい場合は何もしない
	if (*offset >= len || count == 0)
		return 0;
	if (count > len - *offset)
		count = len - *offset;
	if (copy_to_user(buf, STUDENT_LOGIN + *offset, count))
		return -EFAULT;
	*offset += count;
	return count;
}

static ssize_t id_write(struct file *file, const char __user *buf, size_t count, loff_t *offset) {
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
static const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.read = id_read,
	.write = id_write,
};

static int __init debugfs_init(void) {
	dir = debugfs_create_dir(SUBDIR_NAME, NULL);

	// assinment 5
	debugfs_create_file("id", 0666, dir, NULL, &id_fops);
	debugfs_create_ulong("jiffies", 0444, dir, (unsigned long *)&jiffies);
	// 
	debugfs_create_file("foo", 0644, dir, NULL, &add_fops);

	return 0;
}

static void __exit debugfs_exit(void) { 
	debugfs_remove_recursive(dir);
}

module_init(debugfs_init);
module_exit(debugfs_exit);
