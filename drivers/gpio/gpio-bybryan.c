#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio/driver.h>
#include <linux/of.h>

#define DRIVER_NAME "gpio-bybryan"
#define GPIO_NUM 16

static struct gpio_chip chip;
static int gpio_bybryan_get(struct gpio_chip *chip, unsigned offset)
{
    printk(KERN_INFO "GPIO get %d\n", offset);
    return 0;
}
static int gpio_bybryan_set(struct gpio_chip *chip, unsigned offset, int value)
{
    printk(KERN_INFO "GPIO set %d to %d\n", offset, value);
    return 0;
}
static int gpio_bybryan_direction_input(struct gpio_chip *chip, unsigned offset)
{
    printk(KERN_INFO "GPIO direction input %d\n", offset);
    return 0;
}
static int gpio_bybryan_direction_output(struct gpio_chip *chip, unsigned offset, int value)
{   
    printk(KERN_INFO "GPIO direction output %d to %d\n", offset, value);
    return 0;
}     



static const struct of_device_id gpio_bybryan_of_match[] = {
    { .compatible = "bybryan,gpio-bybryan", },
    {},
};

MODULE_DEVICE_TABLE(of, gpio_bybryan_of_match);
static int gpio_probe_bybryan(struct platform_device *pdev)
{
    chip.label = DRIVER_NAME;
    chip.owner = THIS_MODULE;
    chip.base = -1;
    chip.ngpio = GPIO_NUM;
    chip.can_sleep = 1;
    chip.get = gpio_bybryan_get;
    chip.set = gpio_bybryan_set;
    chip.direction_input = gpio_bybryan_direction_input;
    chip.direction_output = gpio_bybryan_direction_output;
    int ret;
    chip.parent = &pdev->dev;
    ret = gpiochip_add_data(&chip, NULL);
    if (ret) {
        printk(KERN_ERR "Failed to register GPIO chip: %d\n", ret);
        return ret;
    }
    printk(KERN_INFO "GPIO for for bryan is in\n");
    return 0;
}
static void gpio_remove_bybryan(struct platform_device *pdev)
{
    gpiochip_remove(&chip);
    printk(KERN_INFO "GPIO for bryan is out\n");
}

static struct platform_driver gpio_bybryan_driver = {
    .probe = gpio_probe_bybryan,
    .remove = gpio_remove_bybryan,
    .driver = {
        .name = "gpio-bybryan",
        .of_match_table = gpio_bybryan_of_match,
    },
};
static struct platform_device *bybryan_pdev;
static int __init gpio_bybryan_init(void){
    int ret;
    ret = platform_driver_register(&gpio_bybryan_driver);
    if (ret){
        return ret;
    }
    bybryan_pdev = platform_device_register_simple("gpio-bybryan", -1, NULL, 0);
    if (IS_ERR(bybryan_pdev)){
        printk(KERN_ERR "Failed to register platform device: %ld\n", PTR_ERR(bybryan_pdev));
        platform_driver_unregister(&gpio_bybryan_driver);
        return PTR_ERR(bybryan_pdev);
    }
    return 0;
}

static void __exit gpio_bybryan_exit(void){
    platform_device_unregister(bybryan_pdev);
    platform_driver_unregister(&gpio_bybryan_driver);
}


module_init(gpio_bybryan_init);
module_exit(gpio_bybryan_exit);
MODULE_AUTHOR("Bryan<bchimhim15@gmail.com>");
MODULE_DESCRIPTION("GPIO for anything");
MODULE_LICENSE("GPL");