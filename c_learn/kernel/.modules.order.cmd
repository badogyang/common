cmd_/mnt/d/common/c_learn/kernel/modules.order := {   echo /mnt/d/common/c_learn/kernel/hello_kernel.ko; :; } | awk '!x[$$0]++' - > /mnt/d/common/c_learn/kernel/modules.order
