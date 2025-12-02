#include <linux/module.h>
#include <linux/printk.h>
#include <linux/debugfs.h>

MODULE_LICENSE("GPL");

#define SUBDIR_NAME "fortytwo"
#define STUDENT_LOGIN "shshimad"

static struct dentry *dir;

// foo file
static DEFINE_MUTEX(foo_mutex);
static char foo_data[PAGE_SIZE];
static size_t foo_len;

static ssize_t id_read(struct file *file, char __user *buf, size_t count,
		       loff_t *offset)
{
	size_t len = strlen(STUDENT_LOGIN);

	if (*offset >= len || count == 0)
		return 0;
	if (count > len - *offset)
		count = len - *offset;
	if (copy_to_user(buf, STUDENT_LOGIN + *offset, count))
		return -EFAULT;
	*offset += count;
	return count;
}

static ssize_t id_write(struct file *file, const char __user *buf, size_t count,
			loff_t *offset)
{
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

static ssize_t foo_read(struct file *file, char __user *buf, size_t count,
			loff_t *offset)
{
	mutex_lock(&foo_mutex);

	size_t len = foo_len;

	if (*offset >= len || count == 0) {
		mutex_unlock(&foo_mutex);
		return 0;
	}

	if (count > len - *offset)
		count = len - *offset;

	if (copy_to_user(buf, foo_data + *offset, count)) {
		mutex_unlock(&foo_mutex);
		return -EFAULT;
	}
	*offset += count;

	mutex_unlock(&foo_mutex);
	return count;
}

static ssize_t foo_write(struct file *file, const char __user *buf,
			 size_t count, loff_t *offset)
{
	mutex_lock(&foo_mutex);

	if (count > sizeof(foo_data)) {
		mutex_unlock(&foo_mutex);
		return -EINVAL;
	}
	if (copy_from_user(foo_data, buf, count)) {
		mutex_unlock(&foo_mutex);
		return -EFAULT;
	}
	foo_len = count;

	mutex_unlock(&foo_mutex);
	return count;
}

static const struct file_operations foo_fops = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write,
};

static int __init debugfs_init(void)
{
	dir = debugfs_create_dir(SUBDIR_NAME, NULL);
	if (!dir) {
		pr_err("Failed to create \"%s\" dir.\n", SUBDIR_NAME);
		return -ENODEV;
	}

	debugfs_create_file("id", 0666, dir, NULL, &id_fops);
	debugfs_create_ulong("jiffies", 0444, dir, (unsigned long *)&jiffies);
	debugfs_create_file("foo", 0644, dir, NULL, &foo_fops);

	return 0;
}

static void __exit debugfs_exit(void)
{
	debugfs_remove_recursive(dir);
}

module_init(debugfs_init);
module_exit(debugfs_exit);
