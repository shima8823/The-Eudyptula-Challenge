#include <linux/module.h>
#include <linux/printk.h>

#include <linux/proc_fs.h>
#include <linux/dcache.h>
#include <linux/fs_struct.h>
#include <linux/nsproxy.h>
#include <linux/rbtree.h>
#include <linux/slab.h>
#include <../fs/mount.h>

MODULE_LICENSE("GPL");

#define SUBDIR_NAME "mymounts"

static struct proc_dir_entry *proc_dir;

static ssize_t procfile_read(struct file *file, char __user *user,
			     size_t user_size, loff_t *offset)
{
	struct mnt_namespace *ns = current->nsproxy->mnt_ns;
	char *buf;
	char *output;
	struct path root;
	ssize_t res;
	int len = 0;

	output = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!output)
		return -ENOMEM;
	buf = kmalloc(PATH_MAX, GFP_KERNEL);
	if (!buf) {
		kfree(output);
		return -ENOMEM;
	}

	get_fs_root(current->fs, &root);
	for (struct rb_node *node = ns->mnt_first_node; node; node = rb_next(node)) {
		struct mount *mnt = rb_entry(node, struct mount, mnt_node);
		const struct path mnt_path = {
			.mnt = &mnt->mnt,
			.dentry = mnt->mnt.mnt_root
		};

		/* skip rootfs */
		if (!path_is_under(&mnt_path, &root))
			continue;
		char *mount_point = dentry_path_raw(mnt->mnt_mountpoint, buf, PATH_MAX);

		if (IS_ERR(mount_point))
			continue;
		const char *name = mnt->mnt_mountpoint->d_name.name;

		if (strcmp(name, "/") == 0)
			name = "root";
		if (len >= PAGE_SIZE)
			break;
		len += snprintf(output + len, PAGE_SIZE - len, "%s\t%s\n", name, mount_point);
	}
	if (len < 0)
		len = 0;
	else if (len > PAGE_SIZE)
		len = PAGE_SIZE;
	res = simple_read_from_buffer(user, user_size, offset, output, len);
	path_put(&root);
	kfree(buf);
	kfree(output);
	return res;
}

static const struct proc_ops proc_file_fops = {
	.proc_read = procfile_read,
};

static int __init proc_mount_init(void)
{
	proc_dir = proc_create(SUBDIR_NAME, 0444, NULL, &proc_file_fops);

	if (!proc_dir)
		return -ENOMEM;
	return 0;
}

static void __exit proc_mount_exit(void)
{
	proc_remove(proc_dir);
}

module_init(proc_mount_init);
module_exit(proc_mount_exit);
