package com.mahmoodms.bluetooth.eegssvepwheelchairdemo;

import android.app.ActionBar;
import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.ColorDrawable;
import android.net.Uri;
import android.os.BatteryManager;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.support.v4.app.NavUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.androidplot.Plot;
import com.androidplot.util.Redrawer;
import com.androidplot.xy.BoundaryMode;
import com.androidplot.xy.SimpleXYSeries;
import com.androidplot.xy.XYPlot;
import com.androidplot.xy.XYStepMode;
import com.beele.BluetoothLe;
import com.opencsv.CSVWriter;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;

/**
 * Created by mahmoodms on 5/31/2016.
 */

public class DeviceControlActivity extends Activity implements BluetoothLe.BluetoothLeListener {
    // Graphing Variables:
    private GraphAdapter mGraphAdapterCh1;
    private GraphAdapter mGraphAdapterCh2;
    private GraphAdapter mGraphAdapterCh3;
    private GraphAdapter mGraphAdapterCh4;
    public static XYPlotAdapter mPlotAdapter;
    public static Redrawer redrawer;
    private boolean plotImplicitXVals = false;
    private final static String TAG = DeviceControlActivity.class.getSimpleName();

    //LocalVars
    private String mDeviceName;
    private String mDeviceAddress;
    private boolean mConnected;
    //Class instance variable
    private BluetoothLe mBluetoothLe;
    private BluetoothManager mBluetoothManager = null;
    //Connecting to Multiple Devices
    private String[] deviceMacAddresses = null;
    private BluetoothDevice[] mBluetoothDeviceArray = null;
    private BluetoothGatt[] mBluetoothGattArray = null;
    private BluetoothGattService mLedService = null;
    private int mWheelchairGattIndex;

//    private boolean mEOGConnected = false;
    private boolean mEEGConnected = false;
    private boolean mEEGConnected_2ch = false;

    //Layout - TextViews and Buttons
    private TextView mEegValsTextView;
    private TextView mBatteryLevel;
    private TextView mDataRate;
    private TextView mAllChannelsReadyTextView;
    private TextView mEOGClassTextView;
    private TextView mYfitTextView;
    private Button mExportButton;
    private Switch mFilterSwitch;
    private long mLastTime;
    private long mCurrentTime;
    private long mClassTime; //DON'T DELETE!!!

    private boolean filterData = false;
    private int points = 0;
    private Menu menu;

    //RSSI:
    private static final int RSSI_UPDATE_TIME_INTERVAL = 2000;
    private Handler mTimerHandler = new Handler();
    private boolean mTimerEnabled = false;

    //Data Variables:
    private int batteryWarning = 20;//%
    private String fileTimeStamp = "";
    private double dataRate;
    private double mEOGClass = 0;
    private int mLastButtonPress = 0;

    //Classification
    private boolean mWheelchairControl = false; //Default classifier.

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_control);
        //Set orientation of device based on screen type/size:
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        //Recieve Intents:
        Intent intent = getIntent();
        deviceMacAddresses = intent.getStringArrayExtra(MainActivity.INTENT_DEVICES_KEY);
        String[] deviceDisplayNames = intent.getStringArrayExtra(MainActivity.INTENT_DEVICES_NAMES);
        mDeviceName = deviceDisplayNames[0];
        mDeviceAddress = deviceMacAddresses[0];
        Log.d(TAG, "Device Names: "+Arrays.toString(deviceDisplayNames));
        Log.d(TAG, "Device MAC Addresses: "+ Arrays.toString(deviceMacAddresses));
        Log.d(TAG,Arrays.toString(deviceMacAddresses));
        //Set up action bar:
        if (getActionBar() != null) {
            getActionBar().setDisplayHomeAsUpEnabled(true);
        }
        ActionBar actionBar = getActionBar();
        actionBar.setBackgroundDrawable(new ColorDrawable(Color.parseColor("#6078ef")));
        //Flag to keep screen on (stay-awake):
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        //Set up TextViews
        mEegValsTextView = (TextView) findViewById(R.id.eegValue);
        mExportButton = (Button) findViewById(R.id.button_export);
        mFilterSwitch = (Switch) findViewById(R.id.filterSwitch);
        mBatteryLevel = (TextView) findViewById(R.id.batteryText);
        mDataRate = (TextView) findViewById(R.id.dataRate);
        mAllChannelsReadyTextView = (TextView) findViewById(R.id.allChannelsEnabledText);
        mAllChannelsReadyTextView.setText("  Waiting For EOG Device.");
        mDataRate.setText("...");
        mYfitTextView = (TextView) findViewById(R.id.textViewYfit);
        //Initialize Bluetooth
        ActionBar ab = getActionBar();
        ab.setTitle(mDeviceName);
        ab.setSubtitle(mDeviceAddress);
        initializeBluetoothArray();
        // Initialize our XYPlot reference:
        mGraphAdapterCh1 = new GraphAdapter(1000, "EEG Data Ch 1", false, false, Color.BLUE); //Color.parseColor("#19B52C") also, RED, BLUE, etc.
        mGraphAdapterCh2 = new GraphAdapter(1000, "EEG Data Ch 2", false, false, Color.BLACK); //Color.parseColor("#19B52C") also, RED, BLUE, etc.
        mGraphAdapterCh3 = new GraphAdapter(1000, "EEG Data Ch 3", false, false, Color.RED); //Color.parseColor("#19B52C") also, RED, BLUE, etc.
        mGraphAdapterCh4 = new GraphAdapter(1000, "EEG Data Ch 4", false, false, Color.GREEN); //Color.parseColor("#19B52C") also, RED, BLUE, etc.
        //PLOT CH1 By default
        mGraphAdapterCh1.plotData = true;
        mGraphAdapterCh1.setPointWidth((float)2);
        mGraphAdapterCh2.setPointWidth((float)3);
        mGraphAdapterCh3.setPointWidth((float)3);
        mGraphAdapterCh4.setPointWidth((float)3);
        if(plotImplicitXVals) mGraphAdapterCh1.series.useImplicitXVals();
        if(filterData) mPlotAdapter.filterData();
        mPlotAdapter = new XYPlotAdapter(findViewById(R.id.eegPlot), plotImplicitXVals, 1000);
        mPlotAdapter.xyPlot.addSeries(mGraphAdapterCh1.series, mGraphAdapterCh1.lineAndPointFormatter);
        mPlotAdapter.xyPlot.addSeries(mGraphAdapterCh2.series, mGraphAdapterCh2.lineAndPointFormatter);
        mPlotAdapter.xyPlot.addSeries(mGraphAdapterCh3.series, mGraphAdapterCh3.lineAndPointFormatter);
        mPlotAdapter.xyPlot.addSeries(mGraphAdapterCh4.series, mGraphAdapterCh4.lineAndPointFormatter);

        redrawer = new Redrawer(
                Arrays.asList(new Plot[]{mPlotAdapter.xyPlot}),
                100, false);
        mExportButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    saveDataFile(true);
                } catch (IOException e) {
                    Log.e(TAG, "IOException in saveDataFile");
                    e.printStackTrace();
                }
                Uri uii;
                uii = Uri.fromFile(file);
                Intent exportData = new Intent(Intent.ACTION_SEND);
                exportData.putExtra(Intent.EXTRA_SUBJECT, "Ion Sensor Data Export Details");
                exportData.putExtra(Intent.EXTRA_STREAM, uii);
                exportData.setType("text/html");
                startActivity(exportData);
            }
        });
        makeFilterSwitchVisible(false);
        mFilterSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                filterData = isChecked;
            }
        });
        mLastTime = System.currentTimeMillis();
        mClassTime = System.currentTimeMillis();
        Button upButton = (Button) findViewById(R.id.buttonUp);
        Button downButton = (Button) findViewById(R.id.buttonDown);
        Button leftButton = (Button) findViewById(R.id.buttonLeft);
        Button rightButton = (Button) findViewById(R.id.buttonRight);
        Button centerButton = (Button) findViewById(R.id.buttonMiddle);
        Button blinkButton = (Button) findViewById(R.id.buttonSB);
        Button doubleBlinkButton = (Button) findViewById(R.id.buttonDB);
        mEOGClassTextView = (TextView) findViewById(R.id.eogClass);
        upButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(mConnected) {
                    byte[] bytes = new byte[1];
                    bytes[0] = (byte) 0x01;
                    if(mLedService!=null)
                        mBluetoothLe.writeCharacteristic(mBluetoothGattArray[mWheelchairGattIndex], mLedService.getCharacteristic(AppConstant.CHAR_WHEELCHAIR_CONTROL),bytes);
                }
                mEOGClass = 3;
                mLastButtonPress = 3;
                mClassTime = System.currentTimeMillis();
            }
        });
        downButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(mConnected) {
                    byte[] bytes = new byte[1];
                    bytes[0] = (byte) 0xFF;
                    if(mLedService!=null)
                        mBluetoothLe.writeCharacteristic(mBluetoothGattArray[mWheelchairGattIndex], mLedService.getCharacteristic(AppConstant.CHAR_WHEELCHAIR_CONTROL),bytes);
                }
                mEOGClass = 6;
                mLastButtonPress = 6;
                mClassTime = System.currentTimeMillis();
            }
        });
        leftButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(mConnected) {
                    byte[] bytes = new byte[1];
                    bytes[0] = (byte) 0x0F;
                    if(mLedService!=null)
                        mBluetoothLe.writeCharacteristic(mBluetoothGattArray[mWheelchairGattIndex], mLedService.getCharacteristic(AppConstant.CHAR_WHEELCHAIR_CONTROL),bytes);
                }
                mEOGClass = 4;
                mLastButtonPress = 4;
                mClassTime = System.currentTimeMillis();
            }
        });
        rightButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(mConnected) {
                    byte[] bytes = new byte[1];
                    bytes[0] = (byte) 0xF0;
                    if(mLedService!=null)
                        mBluetoothLe.writeCharacteristic(mBluetoothGattArray[mWheelchairGattIndex], mLedService.getCharacteristic(AppConstant.CHAR_WHEELCHAIR_CONTROL),bytes);
                }
                mEOGClass = 5;
                mLastButtonPress = 5;
                mClassTime = System.currentTimeMillis();
            }
        });
        centerButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(mConnected) {
                    byte[] bytes = new byte[1];
                    bytes[0] = (byte) 0x00;
                    if(mLedService!=null) {
                        mBluetoothLe.writeCharacteristic(mBluetoothGattArray[mWheelchairGattIndex], mLedService.getCharacteristic(AppConstant.CHAR_WHEELCHAIR_CONTROL),bytes);
                    }
                }
                switch (mLastButtonPress) {
                    case 3:
                        mEOGClass = 6;
                        break;
                    case 4:
                        mEOGClass = 5;
                        break;
                    case 5:
                        mEOGClass = 4;
                        break;
                    case 6:
                        mEOGClass = 3;
                        break;
                    default:
                        mEOGClass = 0;
                        break;
                }
                mLastButtonPress = 0;
                mClassTime = System.currentTimeMillis();
            }
        });
        blinkButton.setOnClickListener(new View.OnClickListener() {
                @Override
            public void onClick(View view) {
                mEOGClass = 1;
                mClassTime = System.currentTimeMillis();
            }
        });
        doubleBlinkButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mEOGClass = 2;
                mClassTime = System.currentTimeMillis();
            }
        });
        ToggleButton toggleButton1 = (ToggleButton) findViewById(R.id.toggleButtonWheelchairControl);
        toggleButton1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                mWheelchairControl = b;
            }
        });

        ToggleButton ch1 = (ToggleButton) findViewById(R.id.toggleButtonCh1);
        ch1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                mGraphAdapterCh1.setPlotData(b);
            }
        });
        ToggleButton ch2 = (ToggleButton) findViewById(R.id.toggleButtonCh2);
        ch2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                mGraphAdapterCh2.setPlotData(b);
            }
        });
        ToggleButton ch3 = (ToggleButton) findViewById(R.id.toggleButtonCh3);
        ch3.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                mGraphAdapterCh3.setPlotData(b);
            }
        });
        ToggleButton ch4 = (ToggleButton) findViewById(R.id.toggleButtonCh4);
        ch4.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                mGraphAdapterCh4.setPlotData(b);
            }
        });
    }

    public String getTimeStamp() {
        return new SimpleDateFormat("yyyy.MM.dd_HH.mm.ss").format(new Date());
    }

    private boolean fileSaveInitialized = false;
    private CSVWriter csvWriter;
    private File file;
    private File root;

    /**
     *
     * @param terminate - if True, terminates CSVWriter Instance
     * @throws IOException
     */
    public void saveDataFile(boolean terminate) throws IOException {
        if(terminate && fileSaveInitialized) {
            csvWriter.flush();
            csvWriter.close();
            fileSaveInitialized = false;
        }
    }
    /**
     * Initializes CSVWriter For Saving Data.
     * @throws IOException bc
     */
    public void saveDataFile() throws IOException {
        root = Environment.getExternalStorageDirectory();
        fileTimeStamp = "EEGTrainingData_"+getTimeStamp();
        if(root.canWrite()) {
            File dir = new File(root.getAbsolutePath()+"/EEGTrainingData");
            dir.mkdirs();
            file = new File(dir, fileTimeStamp+".csv");
            if(file.exists() && !file.isDirectory()) {
                Log.d(TAG, "File "+file.toString()+" already exists - appending data");
                FileWriter fileWriter = new FileWriter(file, true);
                csvWriter = new CSVWriter(fileWriter);
            } else {
                csvWriter = new CSVWriter(new FileWriter(file));
            }
            fileSaveInitialized = true;
        }
    }

    public void exportFileWithClass(double eegData1, double eegData2, double eegData3, double eegData4) throws IOException {
        if (fileSaveInitialized) {
            String[] valueCsvWrite = new String[5];
            valueCsvWrite[0] = eegData1 + "";
            valueCsvWrite[1] = eegData2 + "";
            valueCsvWrite[2] = eegData3 + "";
            valueCsvWrite[3] = eegData4 + "";
            valueCsvWrite[4] = mEOGClass + "";
            csvWriter.writeNext(valueCsvWrite,false);
        }
    }

    public void exportFileWithClass(double eegData1, double eegData2, double eegData3) throws IOException {
        if (fileSaveInitialized) {
            String[] valueCsvWrite = new String[4];
            valueCsvWrite[0] = eegData1 + "";
            valueCsvWrite[1] = eegData2 + "";
            valueCsvWrite[2] = eegData3 + "";
            valueCsvWrite[3] = mEOGClass + "";
            csvWriter.writeNext(valueCsvWrite,false);
        }
    }

    public void exportFileWithClass(double eegData1, double eegData2) throws IOException {
        if (fileSaveInitialized) {
            String[] writeCSVValue = new String[3];
            writeCSVValue[0] = eegData1 + "";
            writeCSVValue[1] = eegData2 + "";
            writeCSVValue[2] = mEOGClass + "";
            csvWriter.writeNext(writeCSVValue,false);
        }
    }

    @Override
    public void onResume() {
        makeFilterSwitchVisible(true);
        jmainInitialization(true);
        String fileTimeStampConcat = "EEGSensorData_" + getTimeStamp();
        Log.d("onResume-timeStamp", fileTimeStampConcat);
        if(!fileSaveInitialized) {
            try {
                saveDataFile();
            } catch (IOException ex) {
                Log.e("IOEXCEPTION:", ex.toString());
            }
        }
        redrawer.start();
        super.onResume();
    }
    
    @Override
    protected void onPause() {
        redrawer.pause();
        makeFilterSwitchVisible(false);
        super.onPause();
    }

    private void initializeBluetoothArray() {
        mBluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothDeviceArray = new BluetoothDevice[deviceMacAddresses.length];
        mBluetoothGattArray = new BluetoothGatt[deviceMacAddresses.length];
        Log.d(TAG, "Device Addresses: "+Arrays.toString(deviceMacAddresses));
        if(deviceMacAddresses!=null) {
            for (int i = 0; i < deviceMacAddresses.length; i++) {
                mBluetoothDeviceArray[i] = mBluetoothManager.getAdapter().getRemoteDevice(deviceMacAddresses[i]);
            }
        } else {
            Log.e(TAG, "No Devices Queued, Restart!");
            Toast.makeText(this, "No Devices Queued, Restart!", Toast.LENGTH_SHORT).show();
        }
        mBluetoothLe = new BluetoothLe(this, mBluetoothManager, this);
        for (int i = 0; i < mBluetoothDeviceArray.length; i++) {
            mBluetoothGattArray[i] = mBluetoothLe.connect(mBluetoothDeviceArray[i],false);
            Log.e(TAG,"Connecting to Device: "+String.valueOf(mBluetoothDeviceArray[i].getName()+" "+mBluetoothDeviceArray[i].getAddress()));
            if("WheelchairControl".equals(mBluetoothDeviceArray[i].getName())) {
                mWheelchairGattIndex = i;
                Log.e(TAG,"mWheelchairGattIndex: "+mWheelchairGattIndex);
            }
        }
    }

    private void setNameAddress(String name_action, String address_action) {
        MenuItem name = menu.findItem(R.id.action_title);
        MenuItem address = menu.findItem(R.id.action_address);
        name.setTitle(name_action);
        address.setTitle(address_action);
        invalidateOptionsMenu();
    }

    @Override
    protected void onDestroy() {
        redrawer.finish();
        disconnectAllBLE();
        try {
            saveDataFile(true);
        } catch (IOException e) {
            Log.e(TAG, "IOException in saveDataFile");
            e.printStackTrace();
        }
        stopMonitoringRssiValue();
        super.onDestroy();
    }

    private void disconnectAllBLE() {
        if(mBluetoothLe!=null) {
            for (BluetoothGatt bluetoothGatt:mBluetoothGattArray) {
                mBluetoothLe.disconnect(bluetoothGatt);
                mConnected = false;
                resetMenuBar();
            }
        }
    }

    private void resetMenuBar() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if(menu!=null) {
                    menu.findItem(R.id.menu_connect).setVisible(true);
                    menu.findItem(R.id.menu_disconnect).setVisible(false);
                }
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_device_control, menu);
        getMenuInflater().inflate(R.menu.actionbar_item, menu);
        if (mConnected) {
            menu.findItem(R.id.menu_connect).setVisible(false);
            menu.findItem(R.id.menu_disconnect).setVisible(true);
        } else {
            menu.findItem(R.id.menu_connect).setVisible(true);
            menu.findItem(R.id.menu_disconnect).setVisible(false);
        }
        this.menu = menu;
        setNameAddress(mDeviceName, mDeviceAddress);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.menu_connect:
                if (mBluetoothLe != null) {
                    initializeBluetoothArray();
                }
                connect();
                return true;
            case R.id.menu_disconnect:
                if (mBluetoothLe != null) {
                    disconnectAllBLE();
                }
                return true;
            case android.R.id.home:
                if (mBluetoothLe != null) {
                    disconnectAllBLE();
                }
                NavUtils.navigateUpFromSameTask(this);
                onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private void connect() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                MenuItem menuItem = menu.findItem(R.id.action_status);
                menuItem.setTitle("Connecting...");
            }
        });
    }

    @Override
    public void onServicesDiscovered(BluetoothGatt gatt, int status) {
        Log.i(TAG, "onServicesDiscovered");
        if (status == BluetoothGatt.GATT_SUCCESS) {
            for (BluetoothGattService service : gatt.getServices()) {
                if ((service == null) || (service.getUuid() == null)) {
                    continue;
                }
                if (AppConstant.SERVICE_DEVICE_INFO.equals(service.getUuid())) {
                    //Read the device serial number
                    mBluetoothLe.readCharacteristic(gatt, service.getCharacteristic(AppConstant.CHAR_SERIAL_NUMBER));
                    //Read the device software version
                    mBluetoothLe.readCharacteristic(gatt, service.getCharacteristic(AppConstant.CHAR_SOFTWARE_REV));
                }
                if(AppConstant.SERVICE_WHEELCHAIR_CONTROL.equals(service.getUuid())) {
                    mLedService = service;
                    Log.i(TAG,"BLE Wheelchair Control Service found");
                }

                if(AppConstant.SERVICE_3CH_EMG_SIGNAL.equals(service.getUuid())) {
                    makeFilterSwitchVisible(true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_3CH_EMG_SIGNAL_CH1),true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_3CH_EMG_SIGNAL_CH2),true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_3CH_EMG_SIGNAL_CH3),true);
                }

                if(AppConstant.SERVICE_EEG_SIGNAL.equals(service.getUuid())) {
                    makeFilterSwitchVisible(true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EEG_CH1_SIGNAL), true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EEG_CH2_SIGNAL), true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EEG_CH3_SIGNAL), true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EEG_CH4_SIGNAL), true);
                }

                if(AppConstant.SERVICE_EOG_SIGNAL.equals(service.getUuid())) {
                    makeFilterSwitchVisible(true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EOG_CH1_SIGNAL), true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EOG_CH2_SIGNAL), true);
                    for (BluetoothGattCharacteristic c:service.getCharacteristics()) {
                        if(AppConstant.CHAR_EOG_CH3_SIGNAL.equals(c.getUuid())) {
                            mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EOG_CH3_SIGNAL), true);
                        }
                    }
                }

                if (AppConstant.SERVICE_BATTERY_LEVEL.equals(service.getUuid())) { //Read the device battery percentage
//                    mBluetoothLe.readCharacteristic(gatt, service.getCharacteristic(AppConstant.CHAR_BATTERY_LEVEL));
//                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_BATTERY_LEVEL), true);
                }

                if (AppConstant.SERVICE_MPU.equals(service.getUuid())) {
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_MPU_COMBINED), true);
                }
            }
        }
    }

    private void makeFilterSwitchVisible(final boolean visible) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (visible) {
                    mFilterSwitch.setVisibility(View.VISIBLE);
                    mExportButton.setVisibility(View.VISIBLE);
                    mEegValsTextView.setVisibility(View.VISIBLE);
                } else {
                    mExportButton.setVisibility(View.INVISIBLE);
                    mFilterSwitch.setVisibility(View.INVISIBLE);
                    mEegValsTextView.setVisibility(View.INVISIBLE);
                }
            }
        });
    }

    private int batteryLevel = -1;
    @Override
    public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
        Log.i(TAG, "onCharacteristicRead");
        if (status == BluetoothGatt.GATT_SUCCESS) {
            if (AppConstant.CHAR_BATTERY_LEVEL.equals(characteristic.getUuid())) {
                batteryLevel = characteristic.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT8, 0);
                updateBatteryStatus(batteryLevel, batteryLevel + " %");
                Log.i(TAG, "Battery Level :: " + batteryLevel);
            }
        } else {
            Log.e(TAG, "onCharacteristic Read Error" + status);
        }
    }
    private boolean eeg_ch1_data_on = false;
    private boolean eeg_ch2_data_on = false;
    private boolean eeg_ch3_data_on = false;
    private boolean eeg_ch4_data_on = false;
    private int packetNumber = -1;
    private int packetNumber_2ch = -1;
    //most recent eeg data packet:
    //EOG:
    // Classification
    private double[] yfitarray = new double[5];

    @Override
    public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
        //TODO: ADD BATTERY MEASURE CAPABILITY IN FIRMWARE: (ble_ADC)
        if (AppConstant.CHAR_EEG_CH1_SIGNAL.equals(characteristic.getUuid())) {
            byte[] dataEEGBytes = characteristic.getValue();
            if(!eeg_ch1_data_on) {
                eeg_ch1_data_on = true;
            }
            getDataRateBytes(dataEEGBytes.length);
//            if(mEEGConnected) mGraphAdapterCh1.addDataPoints(dataEEGBytes,3,packetNumber);
            if(mEEGConnected_2ch) mGraphAdapterCh1.addDataPoints(dataEEGBytes,3,packetNumber_2ch);
        }

        if (AppConstant.CHAR_EEG_CH2_SIGNAL.equals(characteristic.getUuid())) {
            if(!eeg_ch2_data_on) {
                eeg_ch2_data_on = true;
            }
            byte[] dataEEGBytes = characteristic.getValue();
            int byteLength = dataEEGBytes.length;
            getDataRateBytes(byteLength);
//            if(mEEGConnected) mGraphAdapterCh2.addDataPoints(dataEEGBytes,3,packetNumber);
            if(mEEGConnected_2ch) mGraphAdapterCh2.addDataPoints(dataEEGBytes,3,packetNumber_2ch);
        }

        if (AppConstant.CHAR_EEG_CH3_SIGNAL.equals(characteristic.getUuid())) {
            if(!eeg_ch3_data_on) {
                eeg_ch3_data_on = true;
            }
            byte[] dataEEGBytes = characteristic.getValue();
            int byteLength = dataEEGBytes.length;
            getDataRateBytes(byteLength);
            if(mEEGConnected) mGraphAdapterCh3.addDataPoints(dataEEGBytes,3,packetNumber);
        }

        if (AppConstant.CHAR_EEG_CH4_SIGNAL.equals(characteristic.getUuid())) {
            if(!eeg_ch4_data_on) {
                eeg_ch4_data_on = true;
            }
            byte[] dataEEGBytes = characteristic.getValue();
            int byteLength = dataEEGBytes.length;
            getDataRateBytes(byteLength);
            if(mEEGConnected) mGraphAdapterCh4.addDataPoints(dataEEGBytes,3,packetNumber);
        }
        // TODO: 5/15/2017 2-Channel EEG:
        if(eeg_ch1_data_on && eeg_ch2_data_on) {
            packetNumber_2ch++;
            mEEGConnected_2ch = true;
            eeg_ch1_data_on = false;
            eeg_ch2_data_on = false;
            for (int i = 0; i < 6; i++) {
                if(mGraphAdapterCh1.lastDataValues!=null&&mGraphAdapterCh2.lastDataValues!=null)
                writeToDisk24(mGraphAdapterCh1.lastDataValues[i], mGraphAdapterCh2.lastDataValues[i]);
            }
            //Adjust graph?
            if(packetNumber_2ch%10==0) {
                double max_ch1 = findGraphMax(mGraphAdapterCh1.series);
                double min_ch1 = findGraphMin(mGraphAdapterCh1.series);
                double max_ch2 = findGraphMax(mGraphAdapterCh2.series);
                double min_ch2 = findGraphMin(mGraphAdapterCh2.series);
                double max = (max_ch1>max_ch2)?max_ch1:max_ch2;
                double min = (min_ch1<min_ch2)?min_ch1:min_ch2;
                mPlotAdapter.adjustPlot(mGraphAdapterCh2,max,min);
            }
            if(packetNumber_2ch%20==0) {
//                mEOGClass = jssvepclassifier1(mGraphAdapterCh1.unfilteredSignal);
//                Log.e(TAG,"CLASS: ["+String.valueOf(mEOGClass)+"]");
            }

        }
//        if(eeg_ch4_data_on && eeg_ch3_data_on && eeg_ch2_data_on && eeg_ch1_data_on) {
//            packetNumber++;
//            mEEGConnected = true;
//            eeg_ch1_data_on = false;
//            eeg_ch2_data_on = false;
//            eeg_ch3_data_on = false;
//            eeg_ch4_data_on = false;
//            for (int i = 0; i < 6; i++) {
//                writeToDisk24(mGraphAdapterCh1.lastDataValues[i],mGraphAdapterCh2.lastDataValues[i],
//                        mGraphAdapterCh3.lastDataValues[i],mGraphAdapterCh4.lastDataValues[i]);
////                resetClass();
//            }
//        }
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mAllChannelsReadyTextView.setText(" 2-ch Differential EOG Ready.");
//                    mBatteryLevel.setText("YFITEOG: "+ "{PLACEHOLDER}");
                mEOGClassTextView.setText("EOG Class\n:"+String.valueOf(mEOGClass));
            }
        });
        // EOG Stuff: TODO: IF USING GET FROM PREVIOUS VERSION.
    }

    private double findGraphMax(SimpleXYSeries s) {
        if (s.size() > 0) {
            double max = (double)s.getY(0);
            for (int i = 1; i < s.size(); i++) {
                double a = (double)s.getY(i);
                if(a>max) {
                    max = a;
                }
            }
            return max;
        } else
            return 0.0;
    }


    private double findGraphMin(SimpleXYSeries s) {
        if (s.size()>0) {
            double min = (double)s.getY(0);
            for (int i = 1; i < s.size(); i++) {
                double a = (double)s.getY(i);
                if(a<min) {
                    min = a;
                }
            }
            return min;
        } else {
            return 0.0;
        }
    }

    private void processClassifiedData(final double Y, final int classifier) {
        //Add to end;
        yfitarray[4] = Y;
        //Analyze:
        Log.e(TAG, "C" + String.valueOf(classifier) + " YfitArray: "+Arrays.toString(yfitarray));
        final boolean checkLastThreeMatches = lastThreeMatches(yfitarray);
        if(checkLastThreeMatches) {
            //Get value:
            Log.e(TAG,"Found fit: "+String.valueOf(yfitarray[4]));
            // TODO: 4/27/2017 CONDITION :: CONTROL WHEELCHAIR
            if(mWheelchairControl) {
                executeWheelchairCommand((int)yfitarray[4]);
            }
        }
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.e(TAG,"EOGClassifier, Y: "+String.valueOf(Y));
                if(checkLastThreeMatches)
                    mYfitTextView.setText("YFIT"+String.valueOf(classifier)+":\n"+String.valueOf(Y));
                double sum = yfitarray[0]+yfitarray[1]+yfitarray[2]+yfitarray[3]+yfitarray[4];
                if(sum==0) {
                    mYfitTextView.setText("YFIT"+String.valueOf(classifier)+":\n"+String.valueOf(Y));
                }
            }
        });
    }

    private void executeWheelchairCommand(int command) {
        byte[] bytes = new byte[1];
        switch(command) {
            case 1:
                bytes[0] = (byte) 0x00;
                break;
            case 2:
                bytes[0] = (byte) 0x00;
                break;
            case 3:
                bytes[0] = (byte) 0x01;
                break;
            case 4:
                bytes[0] = (byte) 0x0F;
                break;
            case 5:
                bytes[0] = (byte) 0xF0;
                break;
            case 6:
                bytes[0] = (byte) 0xF0;
                break;
            default:
                break;
        }
        if(mLedService!=null) {
            mBluetoothLe.writeCharacteristic(mBluetoothGattArray[mWheelchairGattIndex],mLedService.getCharacteristic(AppConstant.CHAR_WHEELCHAIR_CONTROL),bytes);
        }
    }

    private void writeToDisk24(final double ch1, final double ch2, final double ch3, final double ch4) {
        try {
            exportFileWithClass(ch1,ch2,ch3,ch4);
        } catch (IOException e) {
            Log.e("IOException", e.toString());
        }
    }

    private void writeToDisk24(final double ch1, final double ch2) {
        try {
            exportFileWithClass(ch1,ch2);
        } catch (IOException e) {
            Log.e("IOException", e.toString());
        }
    }

    private boolean lastThreeMatches(double[] yfitarray) {
        boolean b0 = false;
        boolean b1 = false;
        if(yfitarray[4]!=0) {
            b0 = (yfitarray[4] == yfitarray[3]);
            b1 = (yfitarray[3] == yfitarray[2]);
        }
        return b0 && b1;
    }

    private void getDataRateBytes(int bytes) {
        mCurrentTime = System.currentTimeMillis();
        points += bytes;
        if (mCurrentTime > (mLastTime + 5000)) {
            dataRate = (points / 5);
            points = 0;
            mLastTime = mCurrentTime;
            Log.e(" DataRate:", String.valueOf(dataRate) + " Bytes/s");
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mDataRate.setText(String.valueOf(dataRate)+ " Bytes/s");
                }
            });
        }
    }

    @Override
    public void onReadRemoteRssi(BluetoothGatt gatt, int rssi, int status) {
        uiRssiUpdate(rssi);
        String lastRssi = String.valueOf(rssi)+"db";
    }

    @Override
    public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
        switch (newState) {
            case BluetoothProfile.STATE_CONNECTED:
                mConnected = true;
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        if(menu!=null) {
                            menu.findItem(R.id.menu_connect).setVisible(false);
                            menu.findItem(R.id.menu_disconnect).setVisible(true);
                        }
                    }
                });
                Log.i(TAG, "Connected");
                updateConnectionState(getString(R.string.connected));
                invalidateOptionsMenu();
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mDataRate.setTextColor(Color.BLACK);
                        mDataRate.setTypeface(null, Typeface.NORMAL);
                    }
                });
                //Start the service discovery:
                gatt.discoverServices();
                startMonitoringRssiValue();
                break;
            case BluetoothProfile.STATE_DISCONNECTED:
                mConnected = false;
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        if(menu!=null) {
                            menu.findItem(R.id.menu_connect).setVisible(true);
                            menu.findItem(R.id.menu_disconnect).setVisible(false);
                        }
                    }
                });
                Log.i(TAG, "Disconnected");
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mDataRate.setTextColor(Color.RED);
                        mDataRate.setTypeface(null, Typeface.BOLD);
                        mDataRate.setText("0 Hz");
                    }
                });
                //TODO: ATTEMPT TO RECONNECT:
                updateConnectionState(getString(R.string.disconnected));
                stopMonitoringRssiValue();
                invalidateOptionsMenu();
                break;
            default:
                break;
        }
    }

    public void startMonitoringRssiValue() {
        readPeriodicallyRssiValue(true);
    }

    public void stopMonitoringRssiValue() {
        readPeriodicallyRssiValue(false);
    }

    public void readPeriodicallyRssiValue(final boolean repeat) {
        mTimerEnabled = repeat;
        // check if we should stop checking RSSI value
        if (!mConnected || mBluetoothGattArray == null || !mTimerEnabled) {
            mTimerEnabled = false;
            return;
        }

        mTimerHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (mBluetoothGattArray == null  || !mConnected) {
                    mTimerEnabled = false;
                    return;
                }
                // request RSSI value
                mBluetoothGattArray[0].readRemoteRssi();
                // add call it once more in the future
                readPeriodicallyRssiValue(mTimerEnabled);
            }
        }, RSSI_UPDATE_TIME_INTERVAL);
    }

    @Override
    public void onCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic
            characteristic, int status) {
        Log.i(TAG, "onCharacteristicWrite :: Status:: " + status);
    }

    @Override
    public void onDescriptorWrite(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
    }

    @Override
    public void onDescriptorRead(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
        Log.i(TAG, "onDescriptorRead :: Status:: " + status);
    }

    @Override
    public void onError(String errorMessage) {
        Log.e(TAG, "Error:: " + errorMessage);
    }

    private void updateConnectionState(final String status) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (status.equals(getString(R.string.connected))) {
                    Toast.makeText(getApplicationContext(), "Device Connected!", Toast.LENGTH_SHORT).show();
                } else if (status.equals(getString(R.string.disconnected))) {
                    Toast.makeText(getApplicationContext(), "Device Disconnected!", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    private void updateBatteryStatus(final int percent, final String status) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (percent <= batteryWarning) {
                    mBatteryLevel.setTextColor(Color.RED);
                    mBatteryLevel.setTypeface(null, Typeface.BOLD);
                    Toast.makeText(getApplicationContext(), "Charge Battery, Battery Low " + status, Toast.LENGTH_SHORT).show();
                } else {
                    mBatteryLevel.setTextColor(Color.GREEN);
                    mBatteryLevel.setTypeface(null, Typeface.BOLD);
                }
                mBatteryLevel.setText(status);
            }
        });
    }

    private void uiRssiUpdate(final int rssi) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                MenuItem menuItem = menu.findItem(R.id.action_rssi);
                MenuItem status_action_item = menu.findItem(R.id.action_status);
                final String valueOfRSSI = String.valueOf(rssi) + " dB";
                menuItem.setTitle(valueOfRSSI);
                if (mConnected) {
                    String newStatus = "Status: " + getString(R.string.connected);
                    status_action_item.setTitle(newStatus);
                } else {
                    String newStatus = "Status: " + getString(R.string.disconnected);
                    status_action_item.setTitle(newStatus);
                }
            }
        });
    }

    /*
    * Application of JNI code:
    */
    static {
        System.loadLibrary("android-jni");
    }

    public native int jmainInitialization(boolean b);

    public native double jssvepclassifier1(double[] array);

}
