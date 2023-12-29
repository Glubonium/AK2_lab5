#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>


MODULE_AUTHOR("Zahorodnii Vladyslav <vladzag2004@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint hello_counter = 1;
module_param(hello_counter, uint, 0444);
MODULE_PARM_DESC(hello_counter, "'Hello, world!' counter:");

struct event_data {
	struct list_head list_node;
	ktime_t timestamp;
};

static LIST_HEAD(head_node);

static int __init hello_init(void)
{
	int i;
	if (hello_counter == 0)	{
		pr_warn("Parameter is 0.\n");
	} else if ((hello_counter >= 5) && (hello_counter <= 10)) {
		pr_warn("Parameter is between 5 and 10.\n");
	} else if (hello_counter > 10) {
		pr_err("Parameter is larger than 10.\n");
		BUG_ON(1);
		return -EINVAL;
	}


	for (i = 0; i < hello_counter; i++) {
		struct event_data *event = NULL;
		event = kmalloc(sizeof(*event), GFP_KERNEL);
		if (i == 5) {
			event = 0;
		}
		event->timestamp = ktime_get();
		list_add(&event->list_node, &head_node);
		printk(KERN_EMERG "Hello, world!\n");
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct event_data *event, *tmp;
	list_for_each_entry_safe(event, tmp, &head_node, list_node) {
		ktime_t duration = ktime_sub(ktime_get(), event->timestamp);
		pr_info("Event time: %lld ns\n", ktime_to_ns(duration));
		list_del(&event->list_node);
		kfree(event);
	}
}

module_init(hello_init);
module_exit(hello_exit);
