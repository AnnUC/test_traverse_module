#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
MODULE_LICENSE("GPL");
#include "traverse_function.h"
#include <linux/dram_interface.h>


static int __init traverse_module_init(void)
{
	//traverse (void* arena_start_ptr, size_t VpageNO, size_t* len);
	printk(KERN_INFO "traverse_init");
	register_traverse(0,traverse);
	printk(KERN_INFO "error in traverse");
	return 0;
}

static void __exit traverse_module_exit(void)
{
	return;
}

module_init(traverse_module_init);
module_exit(traverse_module_exit);
