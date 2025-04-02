#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 
  
int init_module(void) {
	pr_info("Hello world !\n");
	return 0; 
}

void cleanup_module(void) { 
	pr_info("Cleaning up module.\n"); 
}
MODULE_LICENSE("GPL");