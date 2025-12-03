#include <linux/module.h>
#include <linux/printk.h>
#include <linux/dcache.h>
#include <linux/fs_struct.h>
#include <linux/nsproxy.h>
#include <linux/rbtree.h>
#include <linux/slab.h>
#include <../fs/mount.h>

MODULE_LICENSE("GPL");

static int __init misc_init(void)
{
	struct mnt_namespace *ns = current->nsproxy->mnt_ns;
	char *buf;
	struct path root;
	
	buf = kmalloc(PATH_MAX, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	get_fs_root(current->fs, &root);
	for (struct rb_node *node = ns->mnt_first_node; node; node = rb_next(node)) {
		struct mount *mnt = rb_entry(node, struct mount, mnt_node);
		const struct path mnt_path = {
			.mnt = &mnt->mnt,
			.dentry = mnt->mnt.mnt_root
		};

		// skip rootfs
		if (!path_is_under(&mnt_path, &root))
			continue;
		char *mount_point = dentry_path_raw(mnt->mnt_mountpoint, buf, PATH_MAX);
		if (IS_ERR(mount_point))
			continue;
		const char *name = mnt->mnt_mountpoint->d_name.name;
		if (strcmp(name, "/") == 0)
			name = "root";
		pr_info("%s\t%s\n", name, mount_point);
	}
	path_put(&root);
	kfree(buf);
	return 0;
}

static void __exit hello_exit(void) { 
	pr_info("Cleaning up module.\n");
}

module_init(misc_init);
module_exit(hello_exit);
