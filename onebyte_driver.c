#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>


//KERNEL SPACE MACRO AND GLOBAL VARIABLES
#define MAJOR_NUMBER 61
char *onebyte_data = NULL;

//FORWARD DECLARATION
int onebyte_open(struct inode *inode, struct file *filep);
int onebyte_release(struct inode *inode, struct file *filep);
ssize_t onebyte_read(struct file *filep, char *buf, size_t
count, loff_t *f_pos);
ssize_t onebyte_write(struct file *filep, const char *buf,
size_t count, loff_t *f_pos);
static void onebyte_exit(void);

//ONEBYTE_FILESTRUCT
struct file_operations onebyte_fops = {
	read: onebyte_read,
	write: onebyte_write,
	open: onebyte_open,
	release: onebyte_release
};

//ONEBYTE_OPEN
int onebyte_open(struct inode *inode, struct file *filep){
	return 0; // always successful
}

//ONEBYTE_RELEASE
int onebyte_release(struct inode *inode, struct file *filep){
	return 0; // always successful
}

//ONEBYTE_READ
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos){

	int curr_pos;
	curr_pos=(1-*f_pos)-raw_copy_to_user(buf, onebyte_data+*f_pos, 1);
	*f_pos+=curr_pos;
	return curr_pos;
}

//ONEBYTE_WRITE
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos){

}

//ONEBYTE_INIT
static int onebyte_init(void){
	int result;
	result= register_chrdev(MAJOR_NUMBER,"onebyte",&onebyte_fops);
	if(result<0){
		return result;
	}
	onebyte_data=kmalloc(sizeof(char),GFP_KERNEL);
	if(!onebyte_data){
		onebyte_exit();
		return -ENOMEM;
	}
	*onebyte_data='X';
	printk(KERN_INFO "Onebyte device successfully initialised\r\rn");
	return 0;
}

//ONEBYTE_EXIT
static void onebyte_exit(void){
	if(onebyte_data){
		kfree(onebyte_data);
		onebyte_data=NULL;
	}
	unregister_chrdev(MAJOR_NUMBER, "onebyte");
	printk(KERN_ALERT "Onebyte device module unloaded\n");
}

MODULE_LICENSE("GPL");
module_init(onebyte_init);
module_exit(onebyte_exit);



