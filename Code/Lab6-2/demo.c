#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define MAJOR_NUM 60
#define MODULE_NAME "demo"

static int iCount = 0;
static char userChar[100];

int gpio_export(unsigned int gpio)
{
	int len;
	char buf[64];

	len = snprintf(buf, sizeof(buf), "%d", gpio);

	struct file *fp;
	loff_t pos = 0;
	mm_segment_t old_fs;
	old_fs = get_fs();
	set_fs(get_ds());

	fp = filp_open("/sys/class/gpio/export", O_WRONLY, 0);
	vfs_write(fp, buf, len, &pos);

	filp_close(fp, NULL);
	set_fs(old_fs);

	return 0;
}

int gpio_set_dirout(unsigned int gpio)
{
	char buf[64];
	
	snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", gpio);

	struct file *fp;
	loff_t pos = 0;
	mm_segment_t old_fs;
	old_fs = get_fs();
	set_fs(get_ds());

	fp = filp_open(buf, O_WRONLY, 0);

	vfs_write(fp, "out", 4, &pos);
	filp_close(fp, NULL);
	set_fs(old_fs);
	return 0;
}

static ssize_t drv_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
    printk("Read: Enter Read function\n");

	//copy_from_user(userChar, buf, count);
	userChar[3] = '\0';

	//printk("%s, %ld\n", userChar, count);
	char buff[64];
	
	struct file *fp;
	loff_t pos = 0;
	mm_segment_t old_fs;

	old_fs = get_fs();
	set_fs(get_ds());
	
	snprintf(buff, sizeof(buff), "/sys/class/gpio/gpio%s/value", userChar);
	fp = filp_open(buff, O_RDONLY, 0);
	//printk("openfile %s\n", buff);
	vfs_read(fp, buff, 2, &pos);
	filp_close(fp, NULL);
	copy_to_user(buf, buff, 2);
	set_fs(old_fs);
	
    return count;
};

static ssize_t drv_write(struct file *filp, const char *buf, size_t count, loff_t *ppos)
{
    printk("Write: Enter Write function\n");
    printk("iCount: %d\n", iCount++);
    printk("W_buf_size: %d\n", (int)count);

    //raw_copy_from_user(userChar, buf, count);
    copy_from_user(userChar, buf, count);
	printk("%s, %ld\n", userChar, count);
	char buff[64];
	char status[2] = "";
	status[0] = userChar[3];

    userChar[count - 1] = '\0';
	userChar[3] = '\0';

    printk("userChar: %s\n", userChar);
    //printk("userChar: %d\n", (int)sizeof(userChar));
    //iCount++;

	struct file *fp;
	loff_t pos = 0;
	mm_segment_t old_fs;

	old_fs = get_fs();
	set_fs(get_ds());

	snprintf(buff, sizeof(buff), "/sys/class/gpio/gpio%s/value", userChar);
	printk("openfile %s\n", buff);
	fp = filp_open(buff, O_WRONLY, 0);
	printk("Status: %s\n", status);

	vfs_write(fp, status, 2, &pos);

	filp_close(fp, NULL);
	set_fs(old_fs);

    return count;
};

long drv_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("I/O Control: Enter I/O Control function\n");
	//printk("%d, %ld\n", cmd, arg);
	switch(cmd)
	{
		case 0:
			snprintf(userChar, sizeof(userChar), "%ld", arg);
			break;
		default:
			break;
	}
	
	return 0;
}
static int drv_open(struct inode *inode, struct file *filp)
{
	printk("Open: Enter open function\n");
	return 0;
}
static int drv_release(struct inode *inode, struct file *filp)
{
	printk("Release: Enter release function\n");
	return 0;
}

struct file_operations drv_fops = 
{
    read: drv_read,
    write: drv_write,
    unlocked_ioctl: drv_ioctl,
    open: drv_open,
    release: drv_release,
};

static int __init demo_init(void)
{
    if (register_chrdev(MAJOR_NUM, "demo", &drv_fops) < 0)
    {
        printk("<1>%s: can't get major %d\n", MODULE_NAME, MAJOR_NUM);
        return (-EBUSY);
    }
    gpio_export(466);
    gpio_export(255);
    gpio_export(429);
    gpio_export(427);
    gpio_set_dirout(466);
    gpio_set_dirout(255);
    gpio_set_dirout(429);
    gpio_set_dirout(427);

    printk("<1>%s: started\n", MODULE_NAME);
    return 0;
}

static void __exit demo_exit(void)
{
    unregister_chrdev(MAJOR_NUM, "demo");
    printk("<1>%s: removed\n", MODULE_NAME);
}



module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");

