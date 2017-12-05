#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>

/** @brief Show the vioc paths
 */
static ssize_t path_show(struct device *dev,
                              struct device_attribute *attr, char *buf)
{
	char path[8][4][64];

	sprintf(path[0][0], "RDMA00 - SC00 - VIQE00 - LUT00 - WMIX00 - DISP0");

	return sprintf(buf, "PATH00: %s\n", path[0][0]);
}
DEVICE_ATTR(path, S_IRUGO, path_show, NULL);

/** @brief Create the device files in "/sys/devices/platform/vioc/"
 *  @return returns 0 if successful
 */
int vioc_attr_create(struct platform_device *dev)
{
	int ret;

	ret = device_create_file(&dev->dev, &dev_attr_path);

	return ret;
}

/** @brief Remove the device files
 *  @return void
 */
void vioc_attr_remove(struct platform_device *dev)
{
	device_remove_file(&dev->dev, &dev_attr_path);
}
