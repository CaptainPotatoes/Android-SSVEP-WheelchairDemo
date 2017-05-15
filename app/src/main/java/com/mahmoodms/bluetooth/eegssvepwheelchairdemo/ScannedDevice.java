package com.mahmoodms.bluetooth.eegssvepwheelchairdemo;

import android.bluetooth.BluetoothDevice;

/**
 * Created by mahmoodms on 5/31/2016.
 */
public class ScannedDevice {
    private static final String UNKNOWN = "Unknown";
    /**
     * BluetoothDevice
     */
    private BluetoothDevice bluetoothDevice;
    /**
     * RSSI
     */
    private int rssiValue;
    /**
     * Display Name
     */
    private String deviceDisplayName;
    /**
     * Device MAC Address
     */
    private String deviceDiplayAddress;

    public ScannedDevice(BluetoothDevice device, int rssi) {
        if(device == null) {
            throw new IllegalArgumentException("BluetoothDevice == Null");
        }
        bluetoothDevice = device;
        deviceDisplayName = device.getName();
        if((deviceDisplayName==null)||(deviceDisplayName.length()==0)) {
            deviceDisplayName = UNKNOWN;
        }
        rssiValue = rssi;
        deviceDiplayAddress = device.getAddress();
    }

    public BluetoothDevice getDevice() {
        return bluetoothDevice;
    }

    public int getRssi() {
        return rssiValue;
    }

    public void setRssi(int rssi) {
        rssiValue = rssi;
    }

    public String getDisplayName() {
        return deviceDisplayName;
    }

    public void setDisplayName(String displayName) {
        deviceDisplayName = displayName;
    }

    public String getDeviceMac() {
        return deviceDiplayAddress;
    }

    public void setDeviceMac(String deviceAddress) {
        deviceDiplayAddress = deviceAddress;
    }
}
