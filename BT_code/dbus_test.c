#include <dbus/dbus.h>
#include <stdio.h>


int main()
{
    DBusConnection *conn;  
    DBusError err;  

    dbus_error_init(&err);  
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);  
    if (dbus_error_is_set(&err)) {  
        fprintf(stderr, "Connection Error (%s)\n", err.message);  
        dbus_error_free(&err);  
        return 1;  
    }

    const char *service = "org.bluez";  
    const char *path = "/org/bluez/hci0"; // hci0是蓝牙适配器名称，根据实际情况修改  
    const char *interface = "org.bluez.Adapter1";  
    const char *method = "StartDiscovery";  
    
    DBusMessage *msg = dbus_message_new_method_call(service, path, interface, method);  
    if (!msg) {  
        fprintf(stderr, "Message Null\n");  
        return 1;  
    }  
    
    DBusPendingCall *pending;  
    if (!dbus_connection_send_with_reply(conn, msg, &pending, -1)) {  
        fprintf(stderr, "Out Of Memory!\n");  
        dbus_message_unref(msg);  
        return 1;  
    }  
    
    dbus_message_unref(msg);  
    
    // 等待回复（可选）  
    // 这里可能需要一个循环来等待和处理回复  
    // dbus_pending_call_block(pending);  
    // DBusMessage *reply = dbus_pending_call_steal_reply(pending);  
    // ... 处理回复 ...  
    // dbus_pending_call_unref(pending);  
    
    // 注意：这里简化了错误处理和回复等待，实际使用中需要完整处理这些情况

    dbus_connection_unref(conn);

    return 0;
}