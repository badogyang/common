# bluetoothd的启动流程

梳理bluetoothd的启动流程，总结一下

```sh
./bluetoothd -h
Usage:
  bluetoothd [OPTION...]

Help Options:
  -h, --help                  Show help options

Application Options:
  -d, --debug=DEBUG           Specify debug options to enable
  -p, --plugin=NAME,..,       Specify plugins to load
  -P, --noplugin=NAME,...     Specify plugins not to load
  -f, --configfile=FILE       Specify an explicit path to the config file
  -C, --compat                Provide deprecated command line interfaces
  -E, --experimental          Enable experimental interfaces
  -n, --nodetach              Run with logging in foreground
  -v, --version               Show version information and exit
```

## 正常启动

./bluetoothd -n --compat &

## 调试

./bluetoothd -n --compat -f /opt/main.conf -d

## 源码：

### 1.从src/main.c中int main开始,解析启动参数

```c
649 int main(int argc, char *argv[])
650 {
651     GOptionContext *context;
652     GError *err = NULL;
653     uint16_t sdp_mtu = 0;
654     uint32_t sdp_flags = 0;
655     int gdbus_flags = 0;
656     guint signal, watchdog;
657     const char *watchdog_usec;
658     /*初始化配置参数main_opt(如果启动时带有-f参数，会将对应的配置赋值给main_opt)*/
659     init_defaults();                                                                                                                                                                                    
660     /*调用glib库,初始化启动参数选项*/
661     context = g_option_context_new(NULL);
662     g_option_context_add_main_entries(context, options, NULL);
663     /*解析启动参数*/
664     if (g_option_context_parse(context, &argc, &argv, &err) == FALSE) {
665         if (err != NULL) {
666             g_printerr("%s\n", err->message);
667             g_error_free(err);
668         } else
669             g_printerr("An unknown error occurred\n");
670         exit(1);
671     }
672
673     g_option_context_free(context);
674     /*如果启动参数中有-v则直接打印版本号然后退出*/
675     if (option_version == TRUE) {
676         printf("%s\n", VERSION);
677         exit(0);
678     }
```



### 2.解析配置文件

```c
680     umask(0077);
681 
682     btd_backtrace_init();
683     /*创建事件处理句柄*/ 
684     event_loop = g_main_loop_new(NULL, FALSE);
685     /*设置信号处理函数，bluetoothd进程结束时释放资源*/
686     signal = setup_signalfd();
687     /*打印初始化*/
688     __btd_log_init(option_debug, option_detach);
689 
690     g_log_set_handler("GLib", G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL |
691                             G_LOG_FLAG_RECURSION,
692                             log_handler, NULL);
693 
694     sd_notify(0, "STATUS=Starting up");
695     /*解析配置文件*/
696     if (option_configfile)
697         main_conf_file_path = option_configfile;
698     else
699         main_conf_file_path = CONFIGDIR "/main.conf";
700 
701     main_conf = load_config(main_conf_file_path);
702 
703     parse_config(main_conf);
704     /*连接dbus*/
705     if (connect_dbus() < 0) {
706         error("Unable to get on D-Bus");
707         exit(1);
708     }
709 
710     if (option_experimental)
711         gdbus_flags = G_DBUS_FLAG_ENABLE_EXPERIMENTAL;
712 
713     g_dbus_set_flags(gdbus_flags);
```



### 3.src/adapter.c中适配器初始化

```c
8677 int adapter_init(void)
8678 {
8679     dbus_conn = btd_get_dbus_connection();/*获取dbus连接后的句柄*/
8680     /*获取mgmt句柄，用于和内核层mgmt进行通信*/
8681     mgmt_master = mgmt_new_default();
8682     if (!mgmt_master) {
8683         error("Failed to access management interface");
8684         return -EIO;
8685     }
8686 
8687     if (getenv("MGMT_DEBUG"))
8688         mgmt_set_debug(mgmt_master, mgmt_debug, "mgmt: ", NULL);
8689 
8690     DBG("sending read version command");
8691     /*向内核发送读取mgmt版本号,具体定义在net/bluetooth/mgmt.c中MGMT_VERSION与MGMT_REVISION*/
8692     if (mgmt_send(mgmt_master, MGMT_OP_READ_VERSION,
8693                 MGMT_INDEX_NONE, 0, NULL,
8694                 read_version_complete, NULL, NULL) > 0)
8695         return 0;
8696 
8697     error("Failed to read management version information");
8698 
8699     return -EIO;
8700 }
```



### 4.mgmt读取版本号返回

```C
8618 static void read_version_complete(uint8_t status, uint16_t length,
8619                     const void *param, void *user_data)
8620 {
8621     const struct mgmt_rp_read_version *rp = param;                                                                                                                                                     
8622 
8623     if (status != MGMT_STATUS_SUCCESS) {
8624         error("Failed to read version information: %s (0x%02x)",
8625                         mgmt_errstr(status), status);
8626         return;
8627     }
8628 
8629     if (length < sizeof(*rp)) {
8630         error("Wrong size of read version response");
8631         return;
8632     }
8633     /*获取到版本号*/
8634     mgmt_version = rp->version;
8635     mgmt_revision = btohs(rp->revision);
8636 
8637     info("Bluetooth management interface %u.%u initialized",
8638                         mgmt_version, mgmt_revision);
8639 
8640     if (mgmt_version < 1) {
8641         error("Version 1.0 or later of management interface required");
8642         abort();
8643     }
8644 
8645     DBG("sending read supported commands command");
8646     /*继续发送指令，获取内核mgmt支持的命令与事件，监听接口添加或删除事件，获取当前接口列表*/
8647     /*
8648      * It is irrelevant if this command succeeds or fails. In case of
8649      * failure safe settings are assumed.
8650      */
8651     mgmt_send(mgmt_master, MGMT_OP_READ_COMMANDS,
8652                 MGMT_INDEX_NONE, 0, NULL,
8653                 read_commands_complete, NULL, NULL);/*仅将数目赋值给相应变量*/
8654 
8655     mgmt_register(mgmt_master, MGMT_EV_INDEX_ADDED, MGMT_INDEX_NONE,
8656                         index_added, NULL, NULL);
8657     mgmt_register(mgmt_master, MGMT_EV_INDEX_REMOVED, MGMT_INDEX_NONE,
8658                         index_removed, NULL, NULL);
8659 
8660     DBG("sending read index list command");
8661 
8662     if (mgmt_send(mgmt_master, MGMT_OP_READ_INDEX_LIST,
8663                 MGMT_INDEX_NONE, 0, NULL,
8664                 read_index_list_complete, NULL, NULL) > 0)
8665         return;
8667     error("Failed to read controller index list");
8668 }
```



### 5.获取到接口数,调用index_add添加接口

```c
8527 static void read_index_list_complete(uint8_t status, uint16_t length,                                                                                                                                  
8528                     const void *param, void *user_data)
8529 {
8530     const struct mgmt_rp_read_index_list *rp = param;
8531     uint16_t num;
8532     int i;
8533 
8534     if (status != MGMT_STATUS_SUCCESS) {
8535         error("Failed to read index list: %s (0x%02x)",
8536                         mgmt_errstr(status), status);
8537         return;
8538     }
8539 
8540     if (length < sizeof(*rp)) {
8541         error("Wrong size of read index list response");
8542         return;
8543     }
8544 
8545     num = btohs(rp->num_controllers);
8546 
8547     DBG("Number of controllers: %d", num);
8548 
8549     if (num * sizeof(uint16_t) + sizeof(*rp) != length) {
8550         error("Incorrect packet size for index list response");
8551         return;
8552     }
8553 
8554     for (i = 0; i < num; i++) {
8555         uint16_t index;
8556 
8557         index = btohs(rp->index[i]);
8558 
8559         DBG("Found index %u", index);
8560         /*添加接口*/
8561         /*
8562          * Pretend to be index added event notification.
8563          *
8564          * It is safe to just trigger the procedure for index
8565          * added notification. It does check against itself.
8566          */
8567         index_added(index, 0, NULL, NULL);
8568     }
8569 }
```



### 6.添加接口(一个接口对应一个adapter)

```c
8463 static void index_added(uint16_t index, uint16_t length, const void *param,
8464                             void *user_data)
8465 {
8466     struct btd_adapter *adapter;                                                                                                                                                                       
8467 
8468     DBG("index %u", index);
8469     /*查看是否已经建立，条件不成立*/
8470     adapter = btd_adapter_lookup(index);
8471     if (adapter) {
8472         btd_warn(adapter->dev_id,
8473             "Ignoring index added for an already existing adapter");
8474         return;
8475     }
8476     /*新建适配器，将main_opt中参数赋值，mgmt句柄赋值*/
8477     adapter = btd_adapter_new(index);
8478     if (!adapter) {
8479         btd_error(index,
8480             "Unable to create new adapter for index %u", index);
8481         return;
8482     }
8483     /*添加到适配器链表中*/
8484     /*
8485      * Protect against potential two executions of read controller info.
8486      *
8487      * In case the start of the daemon and the action of adding a new
8488      * controller coincide this function might be called twice.
8489      *
8490      * To avoid the double execution of reading the controller info,
8491      * add the adapter already to the list. If an adapter is already
8492      * present, the second notification will cause a warning. If the
8493      * command fails the adapter is removed from the list again.
8494      */
8495     adapter_list = g_list_append(adapter_list, adapter);
8496 
8497     DBG("sending read info command for index %u", index);
8498     /*获取内核中所知的蓝牙信息*/
8499     if (mgmt_send(mgmt_master, MGMT_OP_READ_INFO, index, 0, NULL,
8500                     read_info_complete, adapter, NULL) > 0)
8501         return;
8502 
8503     btd_error(adapter->dev_id,
8504             "Failed to read controller info for index %u", index);
8505 
8506     adapter_list = g_list_remove(adapter_list, adapter);
8507 
8508     btd_adapter_unref(adapter);
8509 }
```



### 7.解析蓝牙信息(一)

```C
8205 static void read_info_complete(uint8_t status, uint16_t length,
8206                     const void *param, void *user_data)
8207 {
8208     struct btd_adapter *adapter = user_data;
8209     const struct mgmt_rp_read_info *rp = param;
8210     uint32_t missing_settings;
8211     int err;
8212 
8213     DBG("index %u status 0x%02x", adapter->dev_id, status);
8214 
8215     if (status != MGMT_STATUS_SUCCESS) {
8216         btd_error(adapter->dev_id,
8217                 "Failed to read info for index %u: %s (0x%02x)",
8218                 adapter->dev_id, mgmt_errstr(status), status);
8219         goto failed;
8220     }
8221 
8222     if (length < sizeof(*rp)) {
8223         btd_error(adapter->dev_id,
8224                 "Too small read info complete response");
8225         goto failed;
8226     }
8227     /*获取到蓝牙信息，赋值给adapter*/
8228     /*
8229      * Store controller information for class of device, device
8230      * name, short name and settings.
8231      *
8232      * During the lifetime of the controller these will be updated by
8233      * events and the information is required to keep the current
8234      * state of the controller.
8235      */
8236     adapter->dev_class = rp->dev_class[0] | (rp->dev_class[1] << 8) |
8237                         (rp->dev_class[2] << 16);
8238     adapter->name = g_strdup((const char *) rp->name);
8239     adapter->short_name = g_strdup((const char *) rp->short_name);
8240 
8241     adapter->manufacturer = btohs(rp->manufacturer);
8242     /*获取支持的配置选项，以及当前的配置状态,具体宏定义在lib/mgmt.h中可见*/
8243     adapter->supported_settings = btohl(rp->supported_settings);
8244     adapter->current_settings = btohl(rp->current_settings);
8245     /*清理uuid和devices，防止本次启动后还使用上次bluetoothd中的uuid和扫描到的设备*/
8246     clear_uuids(adapter);
8247     clear_devices(adapter);
```



### ８.解析蓝牙信息(二)

```C
/*检测获取的蓝牙MAC是否正常*/
8249     if (bacmp(&rp->bdaddr, BDADDR_ANY) == 0) {                                                                                                                                                         
8250         if (!set_static_addr(adapter)) {
8251             btd_error(adapter->dev_id,
8252                     "No Bluetooth address for index %u",
8253                     adapter->dev_id);
8254             goto failed;
8255         }
8256     } else {
8257         bacpy(&adapter->bdaddr, &rp->bdaddr);
8258         if (!(adapter->supported_settings & MGMT_SETTING_LE))
8259             adapter->bdaddr_type = BDADDR_BREDR;
8260         else
8261             adapter->bdaddr_type = BDADDR_LE_PUBLIC;
8262     }
8263     /*异或，获取到未被配置的*/
8264     missing_settings = adapter->current_settings ^
8265                         adapter->supported_settings;
8266     /*根据配置文件的参数来，设置蓝牙运行模式分别为DUAL(蓝牙串口),BREDR(经典蓝牙),BLE(低功耗蓝牙),set_mode实际为mgmt_send封装*/
8267     switch (main_opts.mode) {
8268     case BT_MODE_DUAL:
8269         if (missing_settings & MGMT_SETTING_SSP)
8270             set_mode(adapter, MGMT_OP_SET_SSP, 0x01);
8271         if (missing_settings & MGMT_SETTING_LE)
8272             set_mode(adapter, MGMT_OP_SET_LE, 0x01);
8273         if (missing_settings & MGMT_SETTING_BREDR)
8274             set_mode(adapter, MGMT_OP_SET_BREDR, 0x01);
8275         break;
8276     case BT_MODE_BREDR:
8277         if (!(adapter->supported_settings & MGMT_SETTING_BREDR)) {
8278             btd_error(adapter->dev_id,
8279                 "Ignoring adapter withouth BR/EDR support");
8280             goto failed;
8281         }
8282 
8283         if (missing_settings & MGMT_SETTING_SSP)
8284             set_mode(adapter, MGMT_OP_SET_SSP, 0x01);
8285         if (missing_settings & MGMT_SETTING_BREDR)
8286             set_mode(adapter, MGMT_OP_SET_BREDR, 0x01);
8287         if (adapter->current_settings & MGMT_SETTING_LE)
8288             set_mode(adapter, MGMT_OP_SET_LE, 0x00);
8289         break;
8290     case BT_MODE_LE:
8291         if (!(adapter->supported_settings & MGMT_SETTING_LE)) {
8292             btd_error(adapter->dev_id,
8293                 "Ignoring adapter withouth LE support");
8294             goto failed;
8295         }
8296 
8297         if (missing_settings & MGMT_SETTING_LE)
8298             set_mode(adapter, MGMT_OP_SET_LE, 0x01);
8299         if (adapter->current_settings & MGMT_SETTING_BREDR)
8300             set_mode(adapter, MGMT_OP_SET_BREDR, 0x00);
8301         break;
8302     }
8303     /*蓝牙交互安全配置*/
8304     if (missing_settings & MGMT_SETTING_SECURE_CONN)
8305         set_mode(adapter, MGMT_OP_SET_SECURE_CONN, 0x01);
8306 
8307     if (adapter->supported_settings & MGMT_SETTING_PRIVACY)
8308         set_privacy(adapter, main_opts.privacy);
8309     /*设置快速链接*/
8310     if (main_opts.fast_conn &&
8311             (missing_settings & MGMT_SETTING_FAST_CONNECTABLE))
8312         set_mode(adapter, MGMT_OP_SET_FAST_CONNECTABLE, 0x01);
```




### 9.解析蓝牙信息(三)

```C
/*注册dbus接口*/
8314     err = adapter_register(adapter);
8315     if (err < 0) {
8316         btd_error(adapter->dev_id, "Unable to register new adapter");
8317         goto failed;
8318     }
8319     /*监听mgmt上报事件，由于监听事件太多，省略一部分*/
8320     /*
8321      * Register all event notification handlers for controller.
8322      *
8323      * The handlers are registered after a succcesful read of the
8324      * controller info. From now on they can track updates and
8325      * notifications.
8326      */
8327     mgmt_register(adapter->mgmt, MGMT_EV_NEW_SETTINGS, adapter->dev_id,
8328                     new_settings_callback, adapter, NULL);
8329 
8330     mgmt_register(adapter->mgmt, MGMT_EV_CLASS_OF_DEV_CHANGED,
8331                         adapter->dev_id,
8332                         dev_class_changed_callback,
8333                         adapter, NULL);
..............................................
..............................................
8423     mgmt_register(adapter->mgmt, MGMT_EV_PASSKEY_NOTIFY,
8424                         adapter->dev_id,
8425                         user_passkey_notify_callback,
8426                         adapter, NULL);
8427 
8428     set_dev_class(adapter);
8429     /*设置蓝牙名*/
8430     set_name(adapter, btd_adapter_get_name(adapter));
8431     /*开启绑定*/
8432     if (!(adapter->current_settings & MGMT_SETTING_BONDABLE))
8433         set_mode(adapter, MGMT_OP_SET_BONDABLE, 0x01);
8434     /*是否可被连接*/
8435     if (!kernel_conn_control)
8436         set_mode(adapter, MGMT_OP_SET_CONNECTABLE, 0x01);
8437     else if (adapter->current_settings & MGMT_SETTING_CONNECTABLE)
8438         set_mode(adapter, MGMT_OP_SET_CONNECTABLE, 0x00);
8439     /*是否可被扫描到*/
8440     if (adapter->stored_discoverable && !adapter->discoverable_timeout)
8441         set_discoverable(adapter, 0x01, 0);
8442     /*向dbus发送启用蓝牙，最终还是会调用mgmt_send通知内核启用蓝牙(调adapter_init中注册的接口)*/
8443     if (adapter->current_settings & MGMT_SETTING_POWERED)
8444         adapter_start(adapter);
8445 
8446     return;
8447 
8448 failed:
8449     /*
8450      * Remove adapter from list in case of a failure.
8451      *
8452      * Leaving an adapter structure around for a controller that can
8453      * not be initilized makes no sense at the moment.
8454      *
8455      * This is a simplification to avoid constant checks if the
8456      * adapter is ready to do anything.
8457      */
8458     adapter_list = g_list_remove(adapter_list, adapter);
8459 
8460     btd_adapter_unref(adapter);                                                                                                                                                                        
8461 }
```


### 10.适配器注册

```c
7711 static int adapter_register(struct btd_adapter *adapter)
7712 {
7713     struct agent *agent;
7714     struct gatt_db *db;
7715 
7716     if (powering_down)
7717         return -EBUSY;
7718     /*注册dbus调用接口*/
7719     adapter->path = g_strdup_printf("/org/bluez/hci%d", adapter->dev_id);
7720 
7721     if (!g_dbus_register_interface(dbus_conn,
7722                     adapter->path, ADAPTER_INTERFACE,
7723                     adapter_methods, NULL,
7724                     adapter_properties, adapter,
7725                     adapter_free)) {
7726         btd_error(adapter->dev_id,
7727                 "Adapter interface init failed on path %s",
7728                             adapter->path);
7729         g_free(adapter->path);
7730         adapter->path = NULL;
7731         return -EINVAL;
7732     }
7733 
7734     if (adapters == NULL)
7735         adapter->is_default = true;
7736     /*添加到链表中*/
7737     adapters = g_slist_append(adapters, adapter);
7738     /*获取代理，目前应该为NULL*/
7739     agent = agent_get(NULL);
7740     if (agent) {
7741         uint8_t io_cap = agent_get_io_capability(agent);
7742         adapter_set_io_capability(adapter, io_cap);
7743         agent_unref(agent);
7744     }
7745     /*ble与bredr建立监听L2CAP层监听事件，添加gap与gatt基础服务*/
7746     adapter->database = btd_gatt_database_new(adapter);
7747     if (!adapter->database) {
7748         btd_error(adapter->dev_id,
7749                 "Failed to create GATT database for adapter");
7750         adapters = g_slist_remove(adapters, adapter);
7751         return -EINVAL;
7752     }
7753 
7754     /* Don't start advertising managers on non-LE controllers. */
7755     if (adapter->supported_settings & MGMT_SETTING_LE)                                                                                                                                                 
7756         adapter->adv_manager = btd_adv_manager_new(adapter);
7757     else
7758         btd_info(adapter->dev_id,
7759             "LEAdvertisingManager skipped, LE unavailable");
7760     /*添加gatt接口*/
7761     db = btd_gatt_database_get_db(adapter->database);
7762     adapter->db_id = gatt_db_register(db, services_modified,
7763                             services_modified,
7764                             adapter, NULL);
7765     /*获取/var/lib/bluetooth/中的各个参数并配置，目前没用到*/
7766     load_config(adapter);
7767     fix_storage(adapter);
7768     load_drivers(adapter);
7769     btd_profile_foreach(probe_profile, adapter);
7770     clear_blocked(adapter);
7771     load_devices(adapter);
7772 
7773     /* retrieve the active connections: address the scenario where
7774      * the are active connections before the daemon've started */
7775     if (adapter->current_settings & MGMT_SETTING_POWERED)
7776         load_connections(adapter);
7777 
7778     adapter->initialized = TRUE;
7779 
7780     if (main_opts.did_source) {
7781         /* DeviceID record is added by sdpd-server before any other
7782          * record is registered. */
7783         adapter_service_insert(adapter, sdp_record_find(0x10000));
7784         set_did(adapter, main_opts.did_vendor, main_opts.did_product,
7785                 main_opts.did_version, main_opts.did_source);
7786     }
7787 
7788     DBG("Adapter %s registered", adapter->path);
7789 
7790     return 0;
7791 }
```



### 11.回到int main中

```c
 /*注册dbus接口*/
720     btd_device_init();
721     btd_agent_init();
722     btd_profile_init();
723     if (main_opts.mode != BT_MODE_LE) {
724         if (option_compat == TRUE)
725             sdp_flags |= SDP_SERVER_COMPAT;
726         /*创建sdp句柄，赋值本地uuid服务，监听l2cap与unix(sdptool交互)套接字*/
727         start_sdp_server(sdp_mtu, sdp_flags);
728 
729         if (main_opts.did_source > 0)
730             register_device_id(main_opts.did_source,
731                         main_opts.did_vendor,
732                         main_opts.did_product,
733                         main_opts.did_version);
734     }
735 
736     if (mps != MPS_OFF)
737         register_mps(mps == MPS_MULTIPLE);
738     /*初始化各种profile*/
739     /* Loading plugins has to be done after D-Bus has been setup since
740      * the plugins might wanna expose some paths on the bus. However the
741      * best order of how to init various subsystems of the Bluetooth
742      * daemon needs to be re-worked. */
743     plugin_init(option_plugin, option_noplugin);
744 
745     /* no need to keep parsed option in memory */
746     free_options();
747     /*rfkill驱动，默认没有*/
748     rfkill_init();
749 
750     DBG("Entering main loop");
751 
752     sd_notify(0, "STATUS=Running");
753     sd_notify(0, "READY=1");
754     /*看门狗*/
755     watchdog_usec = getenv("WATCHDOG_USEC");
756     if (watchdog_usec) {
757         unsigned int seconds;
758 
759         seconds = atoi(watchdog_usec) / (1000 * 1000);
760         info("Watchdog timeout is %d seconds", seconds);
761         /*定时喂狗*/
762         watchdog = g_timeout_add_seconds_full(G_PRIORITY_HIGH,
763                             seconds / 2,
764                             watchdog_callback,
765                             NULL, NULL);
766     } else
767         watchdog = 0;
768     /*进入循环，处理事件消息*/
769     g_main_loop_run(event_loop);
```

至此，bluetoothd启动完成



