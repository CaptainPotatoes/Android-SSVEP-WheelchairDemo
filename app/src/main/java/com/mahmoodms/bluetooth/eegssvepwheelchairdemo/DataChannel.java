package com.mahmoodms.bluetooth.eegssvepwheelchairdemo;

import com.google.common.primitives.Bytes;

/**
 * Created by mmahmood31 on 9/19/2017.
 * For Handling BLE incoming data packets.
 */

class DataChannel {
    boolean chEnabled;
    private static boolean MSBFirst;
    byte[] characteristicDataPacketBytes;
    short packetCounter;
    private int totalDataPointsReceived;
    private byte[] dataBuffer;

    DataChannel(boolean chEnabled, boolean MSBFirst) {
        this.packetCounter = 0;
        this.totalDataPointsReceived = 0;
        this.chEnabled = chEnabled;
        setMSBFirst(MSBFirst);
    }

    private static void setMSBFirst(boolean MSBFirst) {
        DataChannel.MSBFirst = MSBFirst;
    }

    /**
     * If 'dataBuffer' is not null, concatenate new data using Guava lib
     * else: initialize dataBuffer with new data.
     * @param newDataPacket new data packet received via BLE>
     */
    void handleNewData(byte[] newDataPacket) {
        this.characteristicDataPacketBytes = newDataPacket;
        if(this.dataBuffer!=null) {
            this.dataBuffer = Bytes.concat(this.dataBuffer, newDataPacket);
        } else {
            this.dataBuffer = newDataPacket;
        }
        this.totalDataPointsReceived += newDataPacket.length/3;
        this.packetCounter++;
    }

    void addToGraphBuffer(GraphAdapter graphAdapter) {
        for (int i = 0; i < this.dataBuffer.length/3; i++) {
            graphAdapter.addDataPoint(bytesToDouble(this.dataBuffer[3*i], this.dataBuffer[3*i+1], this.dataBuffer[3*i+2]), this.totalDataPointsReceived-this.dataBuffer.length/3 + i);
        }
        this.dataBuffer = null;
        this.packetCounter = 0;
    }

    static double bytesToDouble(byte a1, byte a2, byte a3) {
        int a;
        if (MSBFirst) {
            a = unsignedToSigned(unsignedBytesToInt(a3, a2, a1), 24);
        } else {
            a = unsignedToSigned(unsignedBytesToInt(a1, a2, a3), 24);
        }
        return ((double) a / 8388607.0) * 2.25;
    }

    private static int unsignedToSigned(int unsigned, int size) {
        if ((unsigned & (1 << size - 1)) != 0) {
            unsigned = -1 * ((1 << size - 1) - (unsigned & ((1 << size - 1) - 1)));
        }
        return unsigned;
    }

    private static int unsignedBytesToInt(byte b0, byte b1, byte b2) {
        return (unsignedByteToInt(b0) + (unsignedByteToInt(b1) << 8) + (unsignedByteToInt(b2) << 16));
    }

    private static int unsignedByteToInt(byte b) {
        return b & 0xFF;
    }
}
