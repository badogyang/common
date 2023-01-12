[FLSYQPUB-184:juanning.zhao_apk_fix:【PR651H】【蓝牙】【必现】蓝牙接受到文件界面缺少返回键 (Iad380193) · Gerrit Code Review](http://10.250.115.17:8080/c/SprdR/platform/packages/apps/Bluetooth/+/13832/)





 modified:   Settings/src/com/android/settings/bluetooth/BluetoothDeviceNamePreferenceController.java
 modified:   Settings/src/com/android/settings/deviceinfo/DeviceNamePreferenceController.java





```XML
<string name="bluetooth_empty_list_bluetooth_off" msgid="316627049372961941">"Com o Bluetooth ativado, o dispositivo pode se comunicar com dispositivos próximos"</string>
<string name="bluetooth_scanning_on_info_message" msgid="4069064120315578780">"Quando o Bluetooth está ativado, o dispositivo pode se comunicar com dispositivos próximos.\n\nPara melhorar a experiência, os apps e serviços podem procurar por dispositivos próximos a qualquer momento, mesmo quando o Bluetooth está desativado. Essa configuração pode ser usada, por exemplo, para melhorar recursos e serviços baseados na localização. É possível alterar isso nas "<annotation id="link">"configurações de verificação"</annotation>"."</string>

<string name="bluetooth_scanning_on_info_message">When Bluetooth is turned on, your device can communicate with other nearby Bluetooth devices.\n\nTo improve device experience, apps and services can still scan for nearby devices at any time, even when Bluetooth is off. This can be used, for example, to improve location-based features and services. You can change this in <annotation id="link">scanning settings</annotation>.</string>


<string name="bluetooth_scanning_on_info_message">"Quando o Bluetooth estiver activado, o seu dispositivo pode comunicar com outros dispositivos Bluetooth próximos.\n\nPara melhorar a experiência do dispositivo, as aplicações e os serviços podem continuar a procurar dispositivos nas proximidades em qualquer momento, mesmo quando Bluetooth está desactivado. Isto pode ser usado, por exemplo, para melhorar as funcionalidades e os serviços baseados na localização. Pode mudar esta opção nas "<annotation id="link">"definições de análise"</annotation>"."</string>

Para melhorar a experiência do dispositivo, as aplicações e os serviços podem continuar a procurar dispositivos nas proximidades em qualquer momento, mesmo quando Bluetooth está desactivado. Isto pode ser usado, por exemplo, para melhorar as funcionalidades e os serviços baseados na localização. Pode mudar esta opção nas definições de análise.
```



![image-20211018180151220](C:\Users\xin.yang6\AppData\Roaming\Typora\typora-user-images\image-20211018180151220.png)





![image-20211018180220230](C:\Users\xin.yang6\AppData\Roaming\Typora\typora-user-images\image-20211018180220230.png)





分屏问题

```xml
android:resizeableActivity="false"
android:launchMode="singleTask"
```





diff --git a/Settings/res/values-my-rZG/strings.xml b/Settings/res/values-my-rZG/strings.xml
index ff5f1f02a..9c6ec3cc0 100755
--- a/Settings/res/values-my-rZG/strings.xml
+++ b/Settings/res/values-my-rZG/strings.xml
@@ -3266,7 +3266,7 @@
   <!-- Bluetooth Tethering subtext [CHAR LIMIT=70]-->
   <string name="bluetooth_tethering_subtext_0">Bluetooth မွတဆင့္ တက္ဘလက္၏ အင္တာနက္ခ်ိတ္ဆက္မႈကို မွ်ေဝပါ</string>
   <!-- Bluetooth Tethering subtext [CHAR LIMIT=70]-->
-  <string name="bluetooth_tethering_subtext" product="default">Bluetooth မွတဆင့္ ဖုန္း၏ အင္တာနက္ခ်ိတ္ဆက္မႈကို မွ်ေဝပါ</string>
+  <string name="bluetooth_tethering_subtext" product="default">ဘလူးတုသ္ မွတဆင့္ ဖုန္း၏ အင္တာနက္ခ်ိတ္ဆက္မႈကို မွ်ေဝပါ</string>^M
   <!-- Bluetooth tethering off subtext - shown when Bluetooth Tethering is turned off [CHAR LIMIT=80]-->
   <string name="bluetooth_tethering_off_subtext_config">Bluetooth မွတဆင့္ ဤ <xliff:g id="device_name">%1$d</xliff:g> ၏ အင္တာနက္ခ်ိတ္ဆက္မႈအား မွ်ေဝျခင္း</string>
   <!-- Bluetooth Tethering settings. Error message shown when trying to connect an 8th device [CHAR LIMIT=50]-->
   
   



diff --git a/Settings/res_app/values-my-rMM/strings.xml b/Settings/res_app/values-my-rMM/strings.xml
index 1409c1101..ef310910e 100755
--- a/Settings/res_app/values-my-rMM/strings.xml
+++ b/Settings/res_app/values-my-rMM/strings.xml
@@ -348,7 +348,7 @@ Without a connection, you can’t:
   <string name="tran_wifi_smart_switcher_summary3">လေယာဉ်မုဒ် သို့မဟုတ် လိုင်းမရှိပါက မရနိုင်ပါ</string>
   <!--TSD: modify TSDT-58 for Settings2.0 by yunfei.wu 20191118 end  TSD: add XLWEHLEQ-1569 by haisong.quan 20190823 start -->
   <string name="wifi_qr_join_msg">WLAN နှင့်ချိတ်ရန် QR ကုဒ်ကို စကန်ဖတ်ပါ</string>
-  <string name="wifi_qr_dialog_summary">ပိတ္ထားခ်ိန္တြင္ အလိုအေလ်ာက္ ပြင့္မည့္ WLAN ႏွင့္ ခ်ိတ္ဆက္ရန္ ခ်ိတ္ဆက္မည္ကို ႏွပ္ပါ။</string>
+  <string name="wifi_qr_dialog_summary">ပိတ်ထားချိန်တွင် အလိုအလျောက် ပွင့်မည့် WLAN နှင့် ချိတ်ဆက်ရန် ချိတ်ဆက်မည်ကို နှိပ်ပါ။</string>
   <string name="wifi_qr_wifi_scan_tipes">\"ဆက်တင်များ > Wi-Fi\" တွင် အောက်ပါအိုင်ကွန်ကို ရှာပြီး စကန်ဖတ်ရန် တို့ပါ</string>
   <string name="wifi_qr_dialog_title">\"<xliff:g id="wifi_ssid">^1</xliff:g>\" နှင့် ချိတ်မလား။</string>
   <string name="wifi_qr_light_on">ဖလက်ရှ်မီး ဖွင့်ရန်</string>

、、



http://jira.transsion.com/browse/KGWKF-1120

```
<string name="wifi_wakeup_summary">Wi\u2011Fi will turn back on near high\u2011quality saved networks, like your home network</string>
```





Blocker

2021-10-20 03:14:56.021 32223-32223/com.android.settings E/AndroidRuntime: FATAL EXCEPTION: main
    Process: com.android.settings, PID: 32223
    java.lang.RuntimeException: Unable to destroy activity {com.android.settings/com.android.settings.bluetooth.DevicePickerActivity}: java.lang.NullPointerException: Attempt to invoke virtual method 'void android.content.Context.sendBroadcast(android.content.Intent, java.lang.String)' on a null object reference
        at android.app.ActivityThread.performDestroyActivity(ActivityThread.java:5268)
        at android.app.ActivityThread.handleDestroyActivity(ActivityThread.java:5299)
        at android.app.servertransaction.DestroyActivityItem.execute(DestroyActivityItem.java:44)
        at android.app.servertransaction.TransactionExecutor.executeLifecycleState(TransactionExecutor.java:176)
        at android.app.servertransaction.TransactionExecutor.execute(TransactionExecutor.java:97)
        at android.app.ActivityThread$H.handleMessage(ActivityThread.java:2135)
        at android.os.Handler.dispatchMessage(Handler.java:106)
        at android.os.Looper.loop(Looper.java:268)
        at android.app.ActivityThread.main(ActivityThread.java:8011)
        at java.lang.reflect.Method.invoke(Native Method)
        at com.android.internal.os.RuntimeInit$MethodAndArgsCaller.run(RuntimeInit.java:635)
        at com.android.internal.os.ZygoteInit.main(ZygoteInit.java:947)
     Caused by: java.lang.NullPointerException: Attempt to invoke virtual method 'void android.content.Context.sendBroadcast(android.content.Intent, java.lang.String)' on a null object reference
        at com.android.settings.bluetooth.DevicePickerFragment.sendDevicePickedIntent(Unknown Source:16)
        at com.android.settings.bluetooth.DevicePickerFragment.onDestroy(Unknown Source:12)
        at androidx.fragment.app.Fragment.performDestroy(Unknown Source:19)
        at androidx.fragment.app.FragmentStateManager.destroy(Unknown Source:125)
        at androidx.fragment.app.FragmentStateManager.moveToExpectedState(Unknown Source:268)
        at androidx.fragment.app.SpecialEffectsController$FragmentStateManagerOperation.complete(Unknown Source:5)
        at androidx.fragment.app.SpecialEffectsController$Operation.cancel(Unknown Source:18)
        at androidx.fragment.app.SpecialEffectsController.forceCompleteAllOperations(Unknown Source:135)
        at androidx.fragment.app.FragmentManager.dispatchStateChange(Unknown Source:36)
        at androidx.fragment.app.FragmentManager.dispatchDestroy(Unknown Source:10)
        at androidx.fragment.app.FragmentController.dispatchDestroy(Unknown Source:4)
        at androidx.fragment.app.FragmentActivity.onDestroy(Unknown Source:5)
        at android.app.Activity.performDestroy(Activity.java:8389)
        at android.app.Instrumentation.callActivityOnDestroy(Instrumentation.java:1351)
        at android.app.ActivityThread.performDestroyActivity(ActivityThread.java:5253)
        at android.app.ActivityThread.handleDestroyActivity(ActivityThread.java:5299) 
        at android.app.servertransaction.DestroyActivityItem.execute(DestroyActivityItem.java:44) 
        at android.app.servertransaction.TransactionExecutor.executeLifecycleState(TransactionExecutor.java:176) 
        at android.app.servertransaction.TransactionExecutor.execute(TransactionExecutor.java:97) 
        at android.app.ActivityThread$H.handleMessage(ActivityThread.java:2135) 
        at android.os.Handler.dispatchMessage(Handler.java:106) 
        at android.os.Looper.loop(Looper.java:268) 
        at android.app.ActivityThread.main(ActivityThread.java:8011) 
        at java.lang.reflect.Method.invoke(Native Method) 
        at com.android.internal.os.RuntimeInit$MethodAndArgsCaller.run(RuntimeInit.java:635) 
        at com.android.internal.os.ZygoteInit.main(ZygoteInit.java:947) 



BluetoothOppLauncherActivity  (相机进入蓝牙分享 -> 弹出蓝牙启动框，这个框会显示分屏)

```shell
      mSurface=Surface(name=ScreenDecorOverlayBottom)/@0x5733a57
      mSurface=Surface(name=ScreenDecorOverlay)/@0x8bf1da1
      mLeash=Surface(name=Surface(name=7614ba9 NavigationBar0)/@0x833fd7b - animation-leash)/@0x2ad1dfe mAnimationType=32
         mCapturedLeash=Surface(name=Surface(name=7614ba9 NavigationBar0)/@0x833fd7b - animation-leash)/@0x2ad1dfe    WindowStateAnimator{858eae3 NavigationBar0}:
       mAnimationIsEntrance=true      mSurface=Surface(name=NavigationBar0)/@0x7a42fac
      mLeash=Surface(name=Surface(name=cc3877f StatusBar)/@0xa8d910a - animation-leash)/@0x276e475 mAnimationType=32
         mCapturedLeash=Surface(name=Surface(name=cc3877f StatusBar)/@0xa8d910a - animation-leash)/@0x276e475    WindowStateAnimator{32073a5 StatusBar}:
      mSurface=Surface(name=StatusBar)/@0x8182c7b
      mLeash=Surface(name=Surface(name=dfdbc3 InputMethod)/@0x8e4bcbe - animation-leash)/@0xc092262 mAnimationType=32
         mCapturedLeash=Surface(name=Surface(name=dfdbc3 InputMethod)/@0x8e4bcbe - animation-leash)/@0xc092262    WindowStateAnimator{84e24b0 InputMethod}:
      mSurface=Surface(name=com.android.bluetooth/com.android.bluetooth.opp.BluetoothOppLauncherActivity)/@0x8936f29
      mSurface=Surface(name=com.transsion.hilauncher/com.android.quickstep.recents_ui_overrides.src.com.android.launcher3.uioverrides.QuickstepLauncher)/@0x55b02ae
       mAnimationIsEntrance=true      mSurface=Surface(name=com.android.systemui.ImageWallpaper)/@0x9be785f
```



![image-20211028133833917](C:\Users\xin.yang6\AppData\Roaming\Typora\typora-user-images\image-20211028133833917.png)





Sirrummaa bakka foyyessuuf appootni fi tajajjillii yeroo kamitiyyuu cimdaa Wi-Fi xinxaluu danda'aa, yeroo Wi-Fi cufameellee. Kan hojirraa kan ooluu, fakkkenyaf, bakka amalaa fi tajajjilaa irratti hunda'ee foyyessuuf. Kanaa GESSITUUN KAN JALQABEEhojimatawwan cincalaaGESSITUUN KAN XUMURAMEE keessaa jijjiruu dandessaa.



Sirrummaa bakkaa foyyeessuuf, yeroo kamitiyyuu appootni fi tajaajilli meeshaalee dhihoo xiinxaluu danda'aa, Yeroo Wi-Fi cufaa ta'eellee.  Kun hojiirra kan ooluu, fakkeenyaf, dalagaa fi tajaajila bakka irratti hunda'ee foyyeessuufi. Kana Hojimatawwan Xiinxalaa keessaa jijjiiruu dandeessaa.



Sirrummaa bakkaa foyyeessuuf, yeroo kamitiyyuu appootni fi tajaajilli meeshaalee dhihoo xiinxaluu danda\'aa, yeroo Wi-Fi cufameellee. Kan hojirraa kan ooluu, fakkkenyaf, bakka amalaa fi tajajjilaa irratti hunda\'ee foyyessuuf. Kanaa  <xliff:g id="link_begin">LINK_BEGIN</xliff:g> Hojimatawwan Xiinxalaa <xliff:g id="link_end">LINK_END</xliff:g> keessaa jijjiiruu dandeessaa.

Sirrummaa bakkaa foyyeessuuf, yeroo kamitiyyuu appootni fi tajaajilli meeshaalee dhihoo xiinxaluu danda'aa, Yeroo Wi-Fi cufaa ta'eellee.  Kun hojiirra kan ooluu, fakkeenyaf, dalagaa fi tajaajila bakka irratti hunda'ee foyyeessuufi. Kana Hojimatawwan Xiinxalaa keessaa jijjiiruu dandeessaa.

Dalaga meeshaa foyyeessuuf, yeroo kamitiyyuu appootni fi tajaajilli meeshaalee dhihoo xiinxaluu danda'aa, Yeroo Biluutuuziin cufaa ta'ellee. Kun hojiirra kan ooluu, fakkeenyaf, dalagaa fi tajaajila bakka irratti hunda'ee foyyeessuufi. Kana Hojimatawwan Xiinxalaa keessaa jijjiiruu dandeessaa.

Yeroo biluutuuziin baname, meeshaan kee meeshaalee biluutuuzii dhihoo jiru waliin wal qunnama.

Yeroo biluutuuziin baname, meeshaan kee meeshaalee biluutuuzii dhihoo jiru waliin wal qunnama.\n\nDalaga meeshaa foyyeessuuf, yeroo kamitiyyuu appootni fi tajaajilli meeshaalee dhihoo xiinxaluu danda\'aa,Yeroo Biluutuuziin cufaa ta\'ellee. Kun hojiirra kan ooluu, fakkeenyaf, dalagaa fi tajaajila bakka irratti hunda\'ee foyyesssuf. kanaa  <annotation id="link"> Hojimatawwan Xiinxalaa </annotation>  keessaa jijjiiruu dandeessaa.





![image-20211101175219582](C:\Users\xin.yang6\AppData\Roaming\Typora\typora-user-images\image-20211101175219582.png)





http://gerrit-cq.transsion.com:8080/#/c/UNISOC/vendor/sprd/modules/wcn/+/5118/  

蓝牙地址



Line 14347: M004462  11-21 15:25:51.854  5121  5140 D BluetoothAdapterService: enable() - Enable called with quiet mode status =  false

M004721  11-21 15:25:52.461 29653 29653 D li      : updateContent: Dashboard

M004BB0  11-21 15:25:52.686 29653 29653 D li      : updateContent: Dashboard

Line 16253: M004BD2  11-21 15:25:52.707  5121  5212 D BluetoothAdapterService: startDiscovery



	Line 14244: M0043FB  11-21 15:25:51.784  5121  5121 D AdapterState: make() - Creating AdapterState
	Line 14245: M0043FC  11-21 15:25:51.786  5121  5142 I AdapterState: OFF : entered 
	Line 14348: M004463  11-21 15:25:51.855  5121  5142 I AdapterState: BLE_TURNING_ON : entered 
	Line 15019: M004702  11-21 15:25:52.445  5121  5142 I AdapterState: BLE_ON : entered 
	Line 15033: M004710  11-21 15:25:52.451  5121  5142 I AdapterState: TURNING_ON : entered 
	Line 16039: M004AFC  11-21 15:25:52.626  5121  5142 I AdapterState: ON : entered 

![img](C:\Users\xin.yang6\AppData\Local\Temp\企业微信截图_16383436298530.png)
![img](C:\Users\xin.yang6\AppData\Local\Temp\企业微信截图_16383435492327.png)





S5BE5D0  12-06 01:38:54.863   871  5595 D BluetoothManagerService: enable(com.android.settings):  mBluetooth =null mBinding = false mState = OFF
S5BED21  12-06 01:38:55.622   871   913 D BluetoothManagerService: Sending BLE State Change: TURNING_ON > ON



Line 19781: S5C0036  12-06 01:38:57.937   871  1057 D BluetoothManagerService: enable(com.android.settings):  mBluetooth =null mBinding = false mState = OFF

Line 21779: S5C0800  12-06 01:38:58.871   871   913 D BluetoothManagerService: Sending BLE State Change: TURNING_ON > ON

、

[[KFBHLJL-690\] 【重庆团队】【Bluetooth】反向语下，蓝牙界面重命名设备界面项目名未反向显示 - Transsion R&D Center JIRA](http://jira.transsion.com/browse/KFBHLJL-690)



repo sync --force-sync frameworks/opt/net/voip



        modified:   Settings/res/values-or/strings.xml
        modified:   Settings/res_itel/values-or/strings_transsion.xml
        modified:   Settings/res_unisoc/values-or/strings_wifi.xml
        modified:   SettingsLib/res/values-or/strings.xml

