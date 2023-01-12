AdapterService下的updateAdapterState 通过AIDL跨进程调用  --》 BluetoothManagerService

``` java
 void updateAdapterState(int prevState, int newState) {
        mAdapterProperties.setState(newState);
        invalidateBluetoothGetStateCache();
        if (mCallbacks != null) {
            int n = mCallbacks.beginBroadcast();
            debugLog("updateAdapterState() - Broadcasting state " + BluetoothAdapter.nameForState(
                    newState) + " to " + n + " receivers.");
            for (int i = 0; i < n; i++) {
                try {
                    mCallbacks.getBroadcastItem(i).onBluetoothStateChange(prevState, newState);    //跨进程调用  --》 BluetoothManagerService.IBlueToothCallback.onBluetoothStateChange()
                } catch (RemoteException e) {
                    debugLog("updateAdapterState() - Callback #" + i + " failed (" + e + ")");
                }
            }
            mCallbacks.finishBroadcast();
        }

```

