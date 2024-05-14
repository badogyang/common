    1.查看内核输出级别
    cat /proc/sys/kernel/printk
    4 4 1 7
     
    2.修改内核输出级别
    echo '7 7 1 7' | sudo tee /proc/sys/kernel/printk
     
    四个数字分别代表以下的意思：
    console_loglevel
    控制台日志级别：优先级高于该值的消息将被打印至控制台
    default_message_loglevel
    缺省的消息日志级别：将用该优先级来打印没有优先级的消息
    minimum_console_loglevel
    最低的控制台日志级别：控制台日志级别可被设置的最小值（最高优先级）
    default_console_loglevel
    缺省的控制台日志级别：控制台日志级别的缺省值
