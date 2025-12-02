#include <linux/module.h>
#include <linux/printk.h>
#include <linux/namei.h>
#include <linux/dcache.h>
#include <linux/list.h>
#include <linux/nsproxy.h>
#include <linux/rbtree.h>
#include <../fs/mount.h>

MODULE_LICENSE("GPL");

static int __init misc_init(void)
{
	struct mount *mnt;
	struct mnt_namespace *ns = current->nsproxy->mnt_ns;

	mnt = rb_entry(ns->mnt_first_node, struct mount, mnt_node);
	pr_info("my_mount: %s\n", mnt->mnt_devname);
	
	// mount リストを走査
	
	return 0;
}

static void __exit hello_exit(void) { 
	pr_info("Cleaning up module.\n");
}

module_init(misc_init);
module_exit(hello_exit);
