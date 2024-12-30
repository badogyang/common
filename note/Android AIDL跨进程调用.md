AIDL (Android Interface Definition Language) 是一种IDL 语言，用于生成可以在Android设备上两个进程之间进行进程间通信(interprocess communication, IPC)的代码。如果在一个进程中（例如Activity）要调用另一个进程中（例如Service）对象的操作，就可以使用AIDL生成可序列化的参数。


## 服务端实现

1.创建aidl文件，IMyService.aidl

```java
// IMyService.aidl
package com.example.server;

// Declare any non-default types here with import statements

interface IMyService {
    String sayHello();
}
```



2.aidl服务端实现

``` java
public class RemoteService extends Service {

    @Nullable
    @Override
    /**
    这里返回下面的mBinder
    */
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    private final IMyService.Stub mBinder = new IMyService.Stub() {
        @Override
        public String sayHello() throws RemoteException {
            return "Hello Client";
        }
    };
}

```

4.启动service

``` java
public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        //MainActivity.this 代表当前页面，RemoteService.class 代表接下来要启动的Service类
        Intent intent = new Intent(MainActivity.this, RemoteService.class);
        startService(intent);
    }
}
```

5.在AndroidManifest.xml中添加字段

``` xml
<activity android:name=".MainActivity"
          android:exported="true">
    <intent-filter>
        <action android:name="android.intent.action.MAIN" />

        <category android:name="android.intent.category.LAUNCHER" />
    </intent-filter>
</activity>
<!-- 添加以下字段 -->
<service android:name=".RemoteService"
         android:exported="true">
    <intent-filter>
        <action android:name="com.example.server.RemoteService"/>
    </intent-filter>
</service>
```



## 客户端实现

1.把服务端的aidl文件从aidl文件夹开始一起复制过来，放到客户端同级目录

2.编译一次生成aidl代码

3.编写客户端代码

``` java
public class MainActivity extends AppCompatActivity {

    private IMyService mService;
    
    //连接服务
    private ServiceConnection conn = new ServiceConnection(){
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
            mService = IMyService.Stub.asInterface(iBinder);

            if (mService != null) {
                try {
                    Log.d("AIDL_TEST", "绑定成功" + mService.sayHello());
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
            } else {
                Log.d("AIDL_TEST", "绑定失败");
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mService = null;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Intent intent=new Intent("com.example.server.RemoteService");
        //注意这里直接使用类名不行，需要加上.class
        //Android 5.0 (Lollipop) 之后的规定。 不能用包名的方式定义Service Intent,
        // 而要用显性声明:   new Intent(context, xxxService.class);
        Intent intent=new Intent();
        intent.setComponent(new ComponentName("com.example.server", "com.example.server.RemoteService"));
        bindService(intent, conn, BIND_AUTO_CREATE);

        Button btn = findViewById(R.id.btn);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    Toast.makeText(MainActivity.this, mService.sayHello(), Toast.LENGTH_SHORT).show();
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
            }
        });
    }
}
```



AIDL跨进程调用最重要的地方就是服务的绑定连接

``` java
//连接服务
private ServiceConnection conn = new ServiceConnection(){
    @Override
    public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
        mService = IMyService.Stub.asInterface(iBinder);

        if (mService != null) {
            try {
                Log.d("AIDL_TEST", "绑定成功" + mService.sayHello());
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        } else {
            Log.d("AIDL_TEST", "绑定失败");
        }
    }

    @Override
    public void onServiceDisconnected(ComponentName componentName) {
        mService = null;
    }
};

//这两段代码就是在实现跨进程服务的绑定
Intent intent=new Intent();
intent.setComponent(new ComponentName("com.example.server", "com.example.server.RemoteService"));
bindService(intent, conn, BIND_AUTO_CREATE);
```



