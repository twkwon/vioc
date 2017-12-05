/**
 * @file     vioc_drv.c
 * @author   Alan K
 * @date     11 Oct 2016
 * @version  0.1
 * @brief
 *
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alan K");
MODULE_DESCRIPTION("VIOC Watcher driver");
MODULE_VERSION("0.1");

static char *name = "vioc";
module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

extern int vioc_attr_create(struct platform_device *dev);
extern int vioc_attr_remove(struct platform_device *dev);


struct vioc_info {
	struct device *dev;
};
static struct vioc_info *vioc;

static long vioc_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	printk("%s ioctl(0x%x)\n", name, cmd);
	return ret;
}

static int vioc_open(struct inode *inode, struct file *filp)
{
	printk("%s: %s\n", name, __func__);
	return 0;
}

static int vioc_release(struct inode *inode, struct file *filp)
{
	printk("%s: %s\n", name, __func__);
	return 0;
}

static struct file_operations vioc_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = vioc_ioctl,
	.open = vioc_open,
	.release = vioc_release,
};

static struct miscdevice vioc_misc = {
	.minor = MISC_DYNAMIC_MINOR, 
	.name = "vioc",
	.fops = &vioc_fops,
};

static int vioc_probe(struct platform_device *pdev)
{
	int ret;

	printk("%s: %s\n", name, __func__);

	ret = misc_register(&vioc_misc);
	if (ret) {
		printk("err: misc_register ret=%d\n", ret);
		goto exit;
	}

	vioc = kzalloc(sizeof(struct vioc_info), GFP_KERNEL);
	vioc->dev = &pdev->dev;

	ret = vioc_attr_create(pdev);
	if (ret) {
		pr_err("err vioc_attr_create\n");
		goto err1;
	}

	return ret;

err1:
	misc_deregister(&vioc_misc);
exit:	
	kfree(vioc);
	return ret;
}

static int vioc_remove(struct platform_device *pdev)
{
	printk("%s remove\n", name);

	misc_deregister(&vioc_misc);
	return 0;
}

static struct platform_device vioc_device = {
	.name = "vioc",
	.id = -1,
};

static struct platform_driver vioc_driver = {
	.probe = vioc_probe,
	.remove = vioc_remove,
	.driver = {
		.name = "vioc",
		.owner = THIS_MODULE,
	},
};

/** @brief The VIOC initialization function
 *  @return returns 0 if successful
 */
static int __init vioc_init(void)
{
	int ret;

	ret = platform_device_register(&vioc_device);
	if (ret)
		pr_err("err: vioc_init(%d)\n", ret);

	ret = platform_driver_register(&vioc_driver);
	if (ret)
		pr_err("err: vioc_init(%d)\n", ret);

	printk("%s: %s\n", name, __func__);
	return ret;
}

/** @brief The VIOC cleanup function
 */
static void __exit vioc_exit(void)
{
	platform_driver_unregister(&vioc_driver);
	platform_device_unregister(&vioc_device);
	printk("%s: %s\n", name, __func__);
}

/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(vioc_init);
module_exit(vioc_exit);
