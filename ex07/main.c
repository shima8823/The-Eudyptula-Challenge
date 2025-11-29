#include <linux/module.h>
#include <linux/printk.h>
#include <linux/debugfs.h>

MODULE_LICENSE("GPL");

static struct dentry *dir = NULL;

static int add_write_op(void *data, u64 value)
{
	return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(add_fops, NULL, add_write_op, "%llu\n");


static int __init debugfs_init(void) {
	dir = debugfs_create_dir("fortytwo", NULL);

	// assinment 5
	debugfs_create_file("id", 0666, dir, NULL, &add_fops);
	// 
	debugfs_create_file("jiffies", 0444, dir, NULL, &add_fops);
	// 
	debugfs_create_file("foo", 0644, dir, NULL, &add_fops);

	// debugfs_create_u32("sum", 0444, dir, &sum);
	return 0;
}

static void __exit debugfs_exit(void) { 
	debugfs_remove_recursive(dir);
}

module_init(debugfs_init);
module_exit(debugfs_exit);
