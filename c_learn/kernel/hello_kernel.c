#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void) {
    printk(KERN_INFO "Hello, world!");
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Goodbye, world!");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
//MODULE_AUTHOR("Bing");
//MODULE_DESCRIPTION("A simple example Linux module.");
//MODULE_VERSION("0.01");

static int rtl_send_cmd_and_recv(char * command, char * recv_buf)
{
    FILE * pwfile;
    if(command == NULL || recv_buf == NULL){
        return -2;
    }

    memset(recv_buf, 0, MAX_RECV_BUF_SIZE);
    printf("%s send cmd[%s]\n", LOG_PREFIX, command);
    pwfile = popen(command , "r");  
    if (!pwfile) {  
        printf("%s error, send cmd[%s]\n", LOG_PREFIX, command);
        return -1;
    }

    fread(recv_buf, sizeof(char), MAX_RECV_BUF_SIZE -1, pwfile);
    printf("%s get recv_buff[%s]\n", LOG_PREFIX, recv_buf);
    
    pclose(pwfile);
    return 0;
}


