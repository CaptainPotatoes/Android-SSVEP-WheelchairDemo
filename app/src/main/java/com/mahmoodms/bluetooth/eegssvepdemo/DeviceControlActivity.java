package com.mahmoodms.bluetooth.eegssvepdemo;

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
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.provider.Settings;
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
import com.androidplot.xy.LineAndPointFormatter;
import com.androidplot.xy.SimpleXYSeries;
import com.androidplot.xy.XYPlot;
import com.androidplot.xy.XYStepMode;
import com.beele.BluetoothLe;
import com.opencsv.CSVWriter;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.StringTokenizer;

/**
 * Created by mahmoodms on 5/31/2016.
 */

public class DeviceControlActivity extends Activity implements BluetoothLe.BluetoothLeListener {
    // TODO: 5/15/2017 NEW GRAPH OBJ:
    private GraphAdapter mGraphAdapter;
    private GraphAdapter mGraphAdapterCh2;
    private GraphAdapter mGraphAdapterCh3;
    private GraphAdapter mGraphAdapterCh4;
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

    private boolean mEOGConnected = false;
    private boolean mEEGConnected = false;
    private boolean mWheelchairControllerConnected = false;

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
    private long mClassTime;
    private long mCurrentTime2;

    private String mLastRssi;

    private boolean filterData = false;
    private int points = 0;
    private Menu menu;

    //RSSI:
    private static final int RSSI_UPDATE_TIME_INTERVAL = 2000;
    private Handler mTimerHandler = new Handler();
    private boolean mTimerEnabled = false;

    //Plot Variables:
    private XYPlot eegPlot;
    private Redrawer redrawer;
//    private SimpleXYSeries mGraphAdapter.series;
    private static final int HISTORY_SIZE = 1000;
    private static final int HISTORY_SECONDS = 4;
    private boolean plotImplicitXVals = false;
    private int DATA_RATE_SAMPLES_PER_SECOND = 0;
    //for bounds:
    private BoundaryMode currentBM = BoundaryMode.AUTO;
    //Data Variables:
    private int batteryWarning = 20;//%
    final private boolean initialize = false;
    private String fileTimeStamp = "";
    private double dataRate;
    private double mEOGClass = 0;
    private int mLastButtonPress = 0;

    //Classification
    private boolean mClassifierToUse = true; //Default classifier.
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
        mEegValsTextView = (TextView) findViewById(R.id.ecgValue);
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
//        mGraphAdapter = new GraphAdapter(getApplicationContext(), 1000, "EEG Data Ch 1", false);
        mGraphAdapter = new GraphAdapter(1000, "EEG Data Ch 1", false, Color.BLACK); //Color.parseColor("#19B52C") also, RED, BLUE, etc.
        mGraphAdapter.setPointWidth((float)3);
//        mGraphAdapterCh2 = new GraphAdapter(1000, "EEG Data Ch 2", false, Color.BLUE);
//        mGraphAdapterCh2.setPointWidth((float)2.5);
        eegPlot = (XYPlot) findViewById(R.id.eegPlot);
        //Todo: Graph temporarily uses data index - find alternative to implicit XVals→(seconds)
        if (plotImplicitXVals) {
            mGraphAdapter.series.useImplicitXVals();
            eegPlot.setDomainBoundaries(0, HISTORY_SIZE, BoundaryMode.FIXED);
            eegPlot.setDomainStepMode(XYStepMode.INCREMENT_BY_VAL);
            eegPlot.setDomainStepValue(HISTORY_SIZE/5);
        } else {
            eegPlot.setDomainBoundaries(0, HISTORY_SECONDS, BoundaryMode.FIXED);
            eegPlot.setDomainStepMode(XYStepMode.INCREMENT_BY_VAL);
            eegPlot.setDomainStepValue(HISTORY_SECONDS / 4);
        }
        if(filterData) {
            eegPlot.setRangeBoundaries(-2.5, 2.5, BoundaryMode.AUTO); //EMG only!
            eegPlot.setRangeStepValue(1);
        }
        eegPlot.setRangeStepMode(XYStepMode.INCREMENT_BY_VAL);
        eegPlot.setDomainLabel("Time (seconds)");
        eegPlot.getDomainLabelWidget().pack();
        eegPlot.setRangeLabel("Voltage (mV)");
        eegPlot.getRangeLabelWidget().pack();
        eegPlot.setRangeValueFormat(new DecimalFormat("#.###"));
        eegPlot.setDomainValueFormat(new DecimalFormat("#"));
        eegPlot.getDomainLabelWidget().getLabelPaint().setColor(Color.BLACK);
        eegPlot.getDomainLabelWidget().getLabelPaint().setTextSize(20);
        eegPlot.getRangeLabelWidget().getLabelPaint().setColor(Color.BLACK);
        eegPlot.getRangeLabelWidget().getLabelPaint().setTextSize(20);
        eegPlot.getGraphWidget().getDomainTickLabelPaint().setColor(Color.BLACK);
        eegPlot.getGraphWidget().getRangeTickLabelPaint().setColor(Color.BLACK);
        eegPlot.getGraphWidget().getDomainTickLabelPaint().setTextSize(23); //was 36
        eegPlot.getGraphWidget().getRangeTickLabelPaint().setTextSize(23);
        eegPlot.getGraphWidget().getDomainGridLinePaint().setColor(Color.WHITE);
        eegPlot.getGraphWidget().getRangeGridLinePaint().setColor(Color.WHITE);
        eegPlot.getLegendWidget().getTextPaint().setColor(Color.BLACK);
        eegPlot.getLegendWidget().getTextPaint().setTextSize(20);
        eegPlot.getTitleWidget().getLabelPaint().setTextSize(20);
        eegPlot.getTitleWidget().getLabelPaint().setColor(Color.BLACK);
        eegPlot.addSeries(mGraphAdapter.series,mGraphAdapter.lineAndPointFormatter);
        redrawer = new Redrawer(
                Arrays.asList(new Plot[]{eegPlot}),
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
                clearPlot();
            }
        });
        mLastTime = System.currentTimeMillis();
        mClassTime = System.currentTimeMillis();
        this.registerReceiver(this.mBatInfoReceiver, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
        if(mDeviceName.equals("ECG 1000Hz")) {
            DATA_RATE_SAMPLES_PER_SECOND = 1000;
        } else if (mDeviceName.equals("ECG 500Hz")||mDeviceName.equals("EEG 500Hz")) {
            DATA_RATE_SAMPLES_PER_SECOND = 500;
        } else if (mDeviceName.equals("ECG 250Hz")||mDeviceName.equals("ECGSensor")||mDeviceName.equals("EEG 250Hz")) {
            DATA_RATE_SAMPLES_PER_SECOND = 250;
        } else {
            DATA_RATE_SAMPLES_PER_SECOND = 250;
        }
        //TODO: Test fHC with dummy data:
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
        ToggleButton toggleButton = (ToggleButton) findViewById(R.id.toggleButtonClassifier);
        toggleButton.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                mClassifierToUse = b;
            }
        });
        ToggleButton toggleButton1 = (ToggleButton) findViewById(R.id.toggleButtonWheelchairControl);
        toggleButton1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                mWheelchairControl = b;
            }
        });
    }

    private String androidDeviceBatteryLevel = "-1%";
    private String androidDeviceBatteryStatus = "-1";
    private BroadcastReceiver mBatInfoReceiver = new BroadcastReceiver(){
        @Override
        public void onReceive(Context ctxt, Intent intent) {
            int level = intent.getIntExtra(BatteryManager.EXTRA_LEVEL, 0);
            int status = intent.getIntExtra(BatteryManager.EXTRA_STATUS, -1);
            androidDeviceBatteryLevel = String.valueOf(level) + "%";
            if(status == BatteryManager.BATTERY_STATUS_CHARGING || status == BatteryManager.BATTERY_STATUS_FULL) {
                androidDeviceBatteryStatus = "Charging";
            } else {
                androidDeviceBatteryStatus = "Discharging";
            }
        }
    };

    public String getTimeStamp() {
        return new SimpleDateFormat("yyyy.MM.dd_HH.mm.ss").format(new Date());
    }
    public String getTimeStamp2() {
        return new SimpleDateFormat("MM/dd/yyyy - HH:mm:ss").format(new Date())+" EST\r\n";
    }

    //Write Log File:
    private boolean fileLogInitialized = false;
    private File logFile;
    public void exportLogFile(boolean init, String dataToWrite) {
        if(init) {
            Log.i("exportLogFile", "generated Log file");
            root = Environment.getExternalStorageDirectory();
            File dir = new File(root+"/EEGDataLogs");
            boolean mkdirsA = dir.mkdirs();
            logFile = new File(dir,"Log_"+getTimeStamp()+".txt");
            if (!logFile.exists()) {
                try {
                    logFile.createNewFile();
                } catch (IOException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
            fileLogInitialized = true;
        } else {
            if(logFile.exists()) {
                try {
                    BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(logFile,true));
                    bufferedWriter.append(dataToWrite);
                    bufferedWriter.newLine();
                    bufferedWriter.close();
                } catch (IOException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        }
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
        int fHCMain = jmainInitialization(initialize);
        jmainEegFilt(initialize);
        Log.i("fHCMain","INITIALIZED: "+String.valueOf(fHCMain));
        double[] array0 = new double[1000];
        Arrays.fill(array0,0.0);
//        double[] retArray = jeegcfilt(array0);
        String fileTimeStampConcat = "EEGSensorData_" + getTimeStamp();
        Log.d("onResume-timeStamp", fileTimeStampConcat);
        //TODO (IF ECG/EMG PRESENT ONLY!!!) → We're creating a lot of empty files!!!
        if(!fileLogInitialized) {
            exportLogFile(true, "");
        }
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
        this.unregisterReceiver(mBatInfoReceiver);
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
                    mEEGConnected = true;
                    makeFilterSwitchVisible(true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EEG_CH1_SIGNAL), true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EEG_CH2_SIGNAL), true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EEG_CH3_SIGNAL), true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EEG_CH4_SIGNAL), true);
                }

                if(AppConstant.SERVICE_EOG_SIGNAL.equals(service.getUuid())) {
                    mEOGConnected = true;
                    makeFilterSwitchVisible(true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EOG_CH1_SIGNAL), true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EOG_CH2_SIGNAL), true);
                    for (BluetoothGattCharacteristic c:service.getCharacteristics()) {
                        if(AppConstant.CHAR_EOG_CH3_SIGNAL.equals(c.getUuid())) {
                            mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EOG_CH3_SIGNAL), true);
                        }
                    }
                }

                if (AppConstant.SERVICE_BATTERY_LEVEL.equals(service.getUuid())) {
                    //Read the device battery percentage
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
    private int dataCnt1000 = 0;
    private boolean eeg_ch1_data_on = false;
    private boolean eeg_ch2_data_on = false;
    private boolean eeg_ch3_data_on = false;
    private boolean eeg_ch4_data_on = false;
    //most recent eeg data packet:
    private int[] eeg_ch1_data = new int[6];
    private int[] eeg_ch2_data = new int[6];
    private int[] eeg_ch3_data = new int[6];
    private int[] eeg_ch4_data = new int[6];
    private double[] unfilteredEegSignal = new double[1000]; //250 or 500
    private double[] unfiltEOGCh1 = new double[1000];
    private double[] unfiltEOGCh2 = new double[1000];
    private double[] unfiltEOGCh3 = new double[250];
    private double[] filteredEegSignal = new double[1000];
    //EOG:
    private boolean eog_ch1_data_on = false;
    private boolean eog_ch2_data_on = false;
    private boolean eog_ch3_data_on = false;
    private int[] eog_ch1_data = new int[6];
    private int[] eog_ch2_data = new int[6];
    private int[] eog_ch3_data = new int[6];

    private int packetsReceived = 0;
    // Classification
    private double[] yfitarray = new double[5];

    @Override
    public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
        //TODO: ADD BATTERY MEASURE CAPABILITY IN FIRMWARE: (ble_ADC)
        if (AppConstant.CHAR_BATTERY_LEVEL.equals(characteristic.getUuid())) {
            batteryLevel = characteristic.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT8, 0);
            updateBatteryStatus(batteryLevel, batteryLevel + " %");
            String timeStamp = getTimeStamp2();
            exportLogFile(false, "Battery Level Changed at " + timeStamp + getDetails()+"\r\n");
            Log.i(TAG, "Battery Level :: " + batteryLevel);
        }

        if(AppConstant.CHAR_3CH_EMG_SIGNAL_CH1.equals(characteristic.getUuid())) {

            byte[] dataEmgBytes = characteristic.getValue();
            int byteLength = dataEmgBytes.length;
            //TODO: Remember to check/uncheck plotImplicitXVals (boolean)
            getDataRateBytes(byteLength);
        }

        if(AppConstant.CHAR_3CH_EMG_SIGNAL_CH2.equals(characteristic.getUuid())) {
            byte[] dataEmgBytes = characteristic.getValue();
            int byteLength = dataEmgBytes.length;
            //TODO: Remember to check/uncheck plotImplicitXVals (boolean)
            getDataRateBytes(byteLength);
        }

        if(AppConstant.CHAR_3CH_EMG_SIGNAL_CH3.equals(characteristic.getUuid())) {
            // TODO: 5/7/2017 UPDATE THIS (This is just a copy of EOG plot)
            byte[] dataEmgBytes = characteristic.getValue();
            int byteLength = dataEmgBytes.length;
            //TODO: Remember to check/uncheck plotImplicitXVals (boolean)
            getDataRateBytes(byteLength);
        }

        if (AppConstant.CHAR_EEG_CH1_SIGNAL.equals(characteristic.getUuid())) {
            byte[] dataEEGBytes = characteristic.getValue();
            if(!eeg_ch1_data_on) {
                eeg_ch1_data_on = true;
            }
            getDataRateBytes(dataEEGBytes.length);
            //TODO: Remember to check/uncheck plotImplicitXVals (boolean)
            mGraphAdapter.addDataPoints(dataEEGBytes,3);
            updateGraph(mGraphAdapter.lastTimeValues, mGraphAdapter.lastDataValues);
            Log.e(TAG,"EEG-CH1");
        }

        if (AppConstant.CHAR_EEG_CH2_SIGNAL.equals(characteristic.getUuid())) {
            if(!eeg_ch2_data_on) {
                eeg_ch2_data_on = true;
            }
            byte[] dataEmgBytes = characteristic.getValue();
            int byteLength = dataEmgBytes.length;
            getDataRateBytes(byteLength);
            for (int i = 0; i < byteLength/3; i++) { //0→9
                dataCnt1000++; //count?
                int data = unsignedBytesToInt(dataEmgBytes[3*i], dataEmgBytes[3*i+1], dataEmgBytes[3*i+2]);
                eeg_ch2_data[i] = unsignedToSigned(data, 24);
            }
            Log.e(TAG,"EEG-CH2");
        }

        if (AppConstant.CHAR_EEG_CH3_SIGNAL.equals(characteristic.getUuid())) {
            if(!eeg_ch3_data_on) {
                eeg_ch3_data_on = true;
            }
            byte[] dataEmgBytes = characteristic.getValue();
            int byteLength = dataEmgBytes.length;
            getDataRateBytes(byteLength);
            //TODO: Remember to check/uncheck plotImplicitXVals (boolean)
            for (int i = 0; i < byteLength/3; i++) { //0→9
                dataCnt1000++; //count?
                int data = unsignedBytesToInt(dataEmgBytes[3*i], dataEmgBytes[3*i+1], dataEmgBytes[3*i+2]);
                eeg_ch3_data[i] = unsignedToSigned(data, 24);
            }
            Log.e(TAG,"EEG-CH3");
        }

        if (AppConstant.CHAR_EEG_CH4_SIGNAL.equals(characteristic.getUuid())) {
            if(!eeg_ch4_data_on) {
                eeg_ch4_data_on = true;
            }
            byte[] dataEmgBytes = characteristic.getValue();
            int byteLength = dataEmgBytes.length;
            getDataRateBytes(byteLength);
            //TODO: Remember to check/uncheck plotImplicitXVals (boolean)
            for (int i = 0; i < byteLength/3; i++) { //0→9
                dataCnt1000++; //count?
                int data = unsignedBytesToInt(dataEmgBytes[3*i], dataEmgBytes[3*i+1], dataEmgBytes[3*i+2]);
                eeg_ch4_data[i] = unsignedToSigned(data, 24);
            }
            Log.e(TAG,"EEG-CH4");
        }

        if(eeg_ch1_data_on && eeg_ch2_data_on) {
            eeg_ch1_data_on = false;
            eeg_ch2_data_on = false;
            for (int i = 0; i < 6; i++) {
                writeToDisk24(eeg_ch1_data[i], eeg_ch2_data[i]);
            }
        }


        if(eeg_ch4_data_on && eeg_ch3_data_on && eeg_ch2_data_on && eeg_ch1_data_on) {
            eeg_ch1_data_on = false;
            eeg_ch2_data_on = false;
            eeg_ch3_data_on = false;
            eeg_ch4_data_on = false;
            for (int i = 0; i < 6; i++) {
                writeToDisk24(eeg_ch1_data[i],eeg_ch2_data[i],eeg_ch3_data[i],eeg_ch4_data[i]);
                resetClass();
            }
        }

        // EOG Stuff:
        if (AppConstant.CHAR_EOG_CH1_SIGNAL.equals(characteristic.getUuid())) {
            byte[] dataEmgBytes = characteristic.getValue();
            if(!eog_ch1_data_on) {
                eog_ch1_data_on = true;
            }
            int byteLength = dataEmgBytes.length;
            //TODO: Remember to check/uncheck plotImplicitXVals (boolean)
            getDataRateBytes(byteLength);
            // TODO: 4/18/2017 GRAPHING STUFF
            System.arraycopy(unfilteredEegSignal, 6, unfilteredEegSignal, 0, 1000-6);
            System.arraycopy(explicitXValsLong, 6, explicitXValsLong, 0, 1000-6);
            System.arraycopy(unfiltEOGCh1,6,unfiltEOGCh1,0,1000-6);
            for (int i = 0; i < byteLength/3; i++) { //0→9
                int data = unsignedBytesToInt(dataEmgBytes[3*i], dataEmgBytes[3*i+1], dataEmgBytes[3*i+2]);
                eog_ch1_data[i] = unsignedToSigned(data, 24);
                // TODO: 4/18/2017 PLOTTING STUFF (REMOVE LATER).
                numberDataPointsCh1++;
                timeData = numberDataPointsCh1*0.0040;
                explicitXValsLong[994+i] = timeData;//plus adjustment for offset
                unfilteredEegSignal[994+i] = convert24bitInt(data);
                updateEEG(timeData, eog_ch1_data[i]);
                unfiltEOGCh1[994+i] = convert24bitInt(data);
            }
//            Log.e(TAG,"EOG-CH1");
        }

        if (AppConstant.CHAR_EOG_CH2_SIGNAL.equals(characteristic.getUuid())) {
            if(!eog_ch2_data_on) {
                eog_ch2_data_on = true;
            }
            System.arraycopy(unfiltEOGCh2,6,unfiltEOGCh2,0,1000-6);
            byte[] dataEmgBytes = characteristic.getValue();
            int byteLength = dataEmgBytes.length;
            getDataRateBytes(byteLength);
            for (int i = 0; i < byteLength/3; i++) { //0→9
                int data = unsignedBytesToInt(dataEmgBytes[3*i], dataEmgBytes[3*i+1], dataEmgBytes[3*i+2]);
                eog_ch2_data[i] = unsignedToSigned(data, 24);
                unfiltEOGCh2[994+i] = convert24bitInt(data);
            }
//            Log.e(TAG,"EOG-CH2");
        }

        if (AppConstant.CHAR_EOG_CH3_SIGNAL.equals(characteristic.getUuid())) {
            if(!eog_ch3_data_on) {
                eog_ch3_data_on = true;
            }
            System.arraycopy(unfiltEOGCh3,6,unfiltEOGCh3,0,250-6);
            byte[] dataEmgBytes = characteristic.getValue();
            int byteLength = dataEmgBytes.length;
            getDataRateBytes(byteLength);
            //TODO: Remember to check/uncheck plotImplicitXVals (boolean)
            for (int i = 0; i < byteLength/3; i++) { //0→9
                int data = unsignedBytesToInt(dataEmgBytes[3*i], dataEmgBytes[3*i+1], dataEmgBytes[3*i+2]);
                eog_ch3_data[i] = unsignedToSigned(data, 24);
                unfiltEOGCh3[244+i] = convert24bitInt(data);
            }
//            Log.e(TAG,"EOG-CH3");
        }

        if(eog_ch2_data_on && eog_ch1_data_on) {
            packetsReceived++;
            eog_ch2_data_on = false;
            eog_ch1_data_on = false;
            for (int i = 0; i < 6; i++) {
                writeToDisk24(eog_ch1_data[i],eog_ch2_data[i]);
                resetClass();
            }
            if(packetsReceived==5) {
                packetsReceived=0;
                // TODO: 4/24/2017 EVERY ~60 Dp, call classifier:
                System.arraycopy(yfitarray, 1, yfitarray, 0, 4);
                if (!mClassifierToUse) {
                    final double Y = jeogclassifier(unfiltEOGCh1, unfiltEOGCh2);
                    processClassifiedData(Y,1);
                } else {
                    final double Y = jeogclassifier2(unfiltEOGCh1, unfiltEOGCh2);
                    processClassifiedData(Y,2);
                }

            }
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mAllChannelsReadyTextView.setText(" 2-ch Differential EOG Ready.");
//                    mBatteryLevel.setText("YFITEOG: "+ "{PLACEHOLDER}");
                    mEOGClassTextView.setText("EOG Class\n:"+String.valueOf(mEOGClass));
                }
            });
        } else if(eog_ch3_data_on && eog_ch2_data_on && eog_ch1_data_on) {
            eog_ch3_data_on = false;
            eog_ch2_data_on = false;
            eog_ch1_data_on = false;
            for (int i = 0; i < 6; i++) {
                writeToDisk24(eog_ch1_data[i],eog_ch2_data[i],eog_ch3_data[i]);
                resetClass();
            }
            //CALL Classifier function:
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mAllChannelsReadyTextView.setText(" 3-ch EOG Ready.");
                    mBatteryLevel.setText("YFITEOG: "+ "{PLACEHOLDER}");
                }
            });
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

    private void writeToDisk24(final double ch1, final double ch2) {
        try {
            exportFileWithClass(ch1,ch2);
        } catch (IOException e) {
            Log.e("IOException", e.toString());
        }
    }

    private void writeToDisk24(final int ch1, final int ch2) {
        try {
            double ch1d = convert24bitInt(ch1);
            double ch2d = convert24bitInt(ch2);
            exportFileWithClass(ch1d,ch2d);
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

    private void writeToDisk24(final int ch1, final int ch2, final int ch3) {
        //Add to Back: TODO: Exporting unfiltered data to drive..
        try {
            double ch1d = convert24bitInt(ch1);
            double ch2d = convert24bitInt(ch2);
            double ch3d = convert24bitInt(ch3);
            exportFileWithClass(ch1d,ch2d,ch3d);
        } catch (IOException e) {
            Log.e("IOException", e.toString());
        }
    }

    private void writeToDisk24(final int ch1, final int ch2, final int ch3, final int ch4) {
        //Add to Back: TODO: Exporting unfiltered data to drive..
        try {
            double ch1d = convert24bitInt(ch1);
            double ch2d = convert24bitInt(ch2);
            double ch3d = convert24bitInt(ch3);
            double ch4d = convert24bitInt(ch4);
            exportFileWithClass(ch1d,ch2d,ch3d,ch4d);
        } catch (IOException e) {
            Log.e("IOException", e.toString());
        }
    }


    public double convert24bitInt(final int int24bit) {
        double dividedInt = (double) int24bit/8388607.0;
        return dividedInt*2.42;
    }

    private void clearPlot() {
        if(mGraphAdapter.series!=null) {
            redrawer.pause();
            while(mGraphAdapter.series.size()>0) {
                mGraphAdapter.series.removeFirst();
            }
            adjustGraph(true);
            redrawer.start();
        }
    }

    private double[] explicitXValsLong = new double[1000];
    private int newValsPlotted = 0;

    private void adjustGraph(boolean forceAdjust){
        double max = findGraphMax(mGraphAdapter.series);
        double min = findGraphMin(mGraphAdapter.series);
        if(newValsPlotted%60==0 || forceAdjust) {
            newValsPlotted = 0;
            if(filterData) {
                if (max-min<0.008) {
                    eegPlot.setRangeBoundaries(-0.004, 0.004, BoundaryMode.FIXED);
                    eegPlot.setRangeStepValue(0.008 / 5.0);
                } else {
                    eegPlot.setRangeBoundaries(min-0.004, max+0.004,BoundaryMode.AUTO);
                    eegPlot.setRangeStepValue((0.008+max-min)/5);
                }
            } else {
                if((max-min)!=0) {
                    if(currentBM!=BoundaryMode.AUTO) {
                        eegPlot.setRangeBoundaries(-2.5, 2.5, BoundaryMode.AUTO);
                        currentBM = BoundaryMode.AUTO;
                    }
                    eegPlot.setRangeStepValue((max-min)/5);
                } else {
                    if(currentBM!=BoundaryMode.FIXED) {
                        eegPlot.setRangeBoundaries(min-1, max+1, BoundaryMode.FIXED);
                        currentBM = BoundaryMode.FIXED;
                    }
                    eegPlot.setRangeStepValue(2.0/5.0);
                }
            }
        }
        newMinX = Math.floor(explicitXValsLong[0]);
        newMaxX = Math.floor(explicitXValsLong[999]);
        eegPlot.setDomainBoundaries(newMinX, newMaxX, BoundaryMode.AUTO);
    }

    private void plot(double[] xarray, double[] yarray) {
        if (yarray.length>999) {
            for (int i = 0; i < yarray.length; i++) {
                plotReverse(xarray[i],yarray[i]);
            }
        } else {
            for (int i = 0; i < yarray.length; i++) {
                plot(xarray[i],yarray[i],yarray.length);
            }
        }
    }

    private void plot(double x, double y) {
        if(mGraphAdapter.series.size()>999) {
            mGraphAdapter.series.removeFirst();
        }
        mGraphAdapter.series.addLast(x,y);
        newValsPlotted++;
        if(newValsPlotted>=999) {
            adjustGraph(true);
        }
    }

    private void plotReverse(double x, double y) {
        if(mGraphAdapter.series.size()>999) {
            mGraphAdapter.series.removeLast();
        }
        mGraphAdapter.series.addFirst(x,y);
        newValsPlotted++;
        if(newValsPlotted>=999) {
            adjustGraph(true);
        }
    }

    private void plot(double x, double y, int len) {
        if(mGraphAdapter.series.size()>len-1) {
            mGraphAdapter.series.removeLast();
        }
        mGraphAdapter.series.addFirst(x,y);
        newValsPlotted++;
        if(newValsPlotted>=len-1) {
            adjustGraph(true);
        }
    }
    
    public void updateGraph(final double[] timeData, final double[] data) {
        for (int i = 0; i < data.length; i++) {
            if(!filterData) {
                plot(timeData[i],data[i]);
            }
        }
    }
    
    private int numberDataPointsCh1 = 0;
    private double timeData = 0;
    public void updateEEG(final double timeData, final int value) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mEegValsTextView.setText(" " + " IntVal="+String.valueOf(value));
                double dataVoltage = convert24bitInt(value);
                if(filterData) {
//                    filteredEegSignal = jeegcfilt(unfilteredEegSignal);
                    filteredEegSignal = jeogcfilt(unfilteredEegSignal);
                    if(numberDataPointsCh1%36==0 && numberDataPointsCh1!=0) {
                        plot(explicitXValsLong,filteredEegSignal);
                    }
                } else {
                    plot(timeData,dataVoltage);
                }
            }
        });
    }

    private double findGraphMax(SimpleXYSeries s1, SimpleXYSeries s2, SimpleXYSeries s3, SimpleXYSeries s4) {
        double max1 = (double) s1.getY(0);
        double max2 = (double) s2.getY(0);
        double max3 = (double) s3.getY(0);
        double max4 = (double) s4.getY(0);
        //TODO: smallest array size???
        for (int i = 1; i < s1.size(); i++) {
            double a1 = (double) s1.getY(i);
            if (a1 > max1) {
                max1 = a1;
            }
        }
        for (int i = 0; i < s2.size(); i++) {
            double a2 = (double) s2.getY(i);
            if (a2>max2) {
                max2 = a2;
            }
        }
        for (int i = 0; i < s3.size(); i++) {
            double a3 = (double) s3.getY(i);
            if (a3>max3) {
                max3 = a3;
            }
        }
        for (int i = 0; i < s4.size(); i++) {
            double a4 = (double) s4.getY(i);
            if (a4>max4) {
                max4 = a4;
            }
        }
        double[] maxVals = {max1, max2, max3, max4};
        Arrays.sort(maxVals);
        return maxVals[1];
    }
    private double findGraphMin (SimpleXYSeries s1, SimpleXYSeries s2, SimpleXYSeries s3, SimpleXYSeries s4) {
        double min1 = (double) s1.getY(0);
        double min2 = (double) s2.getY(0);
        double min3 = (double) s3.getY(0);
        double min4 = (double) s4.getY(0);
        for (int i = 1; i < s1.size(); i++) {
            double a1 = (double) s1.getY(i);
            if (a1 < min1) {
                min1 = a1;
            }
        }
        for (int i = 0; i < s2.size(); i++) {
            double a2 = (double) s2.getY(i);
            if (a2 < min2) {
                min2 = a2;
            }
        }
        for (int i = 0; i < s3.size(); i++) {
            double a3 = (double) s3.getY(i);
            if (a3 < min3) {
                min3 = a3;
            }
        }
        for (int i = 0; i < s4.size(); i++) {
            double a4 = (double) s4.getY(i);
            if (a4 < min4) {
                min4 = a4;
            }
        }
        double[] minVals = {min1,min2, min3, min4};
        Arrays.sort(minVals);
        return minVals[4];
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

    private Number newMinX;
    private Number newMaxX;

    private String getDetails() {
        return "Details:\r\n" +
                "Bluetooth LE Device Name: "+mBluetoothDeviceArray[0].getName() + "\r\n" +
                "Bluetooth LE Address: "+mBluetoothDeviceArray[0].getAddress() + "\r\n" +
                "Last Battery Level: "+String.valueOf(batteryLevel)+"% \r\n\r\n" +
                "Last RSSI: "+mLastRssi + "\r\n" +
                "Sampling Rate: "+String.valueOf(DATA_RATE_SAMPLES_PER_SECOND)+"Hz\r\n" +
                "Samples Read: "+String.valueOf(dataCnt1000)+ "\r\n" +
                "Android Device Manufacturer: "+ Build.MANUFACTURER + "\r\n" +
                "Device Model:" + Build.MODEL + "\r\n" +
                "Android Unique ID: " + Settings.Secure.getString(getBaseContext()
                .getContentResolver(), Settings.Secure.ANDROID_ID) + "\r\n" +
                "SDK Version: " + String.valueOf(Build.VERSION.SDK_INT)+ "\r\n" +
                "Version Release: " + Build.VERSION.RELEASE + "\r\n" +
                "Android Battery Level: " + androidDeviceBatteryLevel + "\r\n" +
                "Battery Status: " + androidDeviceBatteryStatus + "\r\n" + "\r\n" +
                "";
    }

    private void resetClass() {
        mCurrentTime2 = System.currentTimeMillis();
        if(mCurrentTime2>(mClassTime+1001)) {
            mEOGClass = 0;
            mClassTime = mCurrentTime2;
        }
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

    /**
     * This function receives a signed 16-bit integer (2-bytes).
     *
     * @param rawValue (byte array)
     * @return -> Signed *int* value
     */
    private int getIntfromByte(byte[] rawValue) {//Assumes 2 bytes
        return (int) rawValue[0] + ((int) rawValue[1] << 8);
    }

    /**
     * Convert an unsigned integer value to a two's-complement encoded
     * signed value.
     */
    private int unsignedToSigned(int unsigned, int size) {
        if ((unsigned & (1 << size - 1)) != 0) {
            unsigned = -1 * ((1 << size - 1) - (unsigned & ((1 << size - 1) - 1)));
        }
        return unsigned;
    }

    /**
     * Convert a signed byte to an unsigned int.
     */
    private int unsignedByteToInt(byte b) {
        return b & 0xFF;
    }

    /**
     * Convert signed bytes to a 16-bit unsigned int.
     */
    private int unsignedBytesToInt(byte b0, byte b1) {
        return (unsignedByteToInt(b0) + (unsignedByteToInt(b1) << 8));
    }

    private int unsignedBytesToInt(byte b0, byte b1, byte b2) {
        return (unsignedByteToInt(b0) + (unsignedByteToInt(b1) << 8) + (unsignedByteToInt(b2) << 16));
    }


    private static final char[] HEX_CHARS = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
            'B', 'C', 'D', 'E', 'F' };
    public static String toHexString(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        int v;
        for (int j = 0; j < bytes.length; j++) {
            v = bytes[j] & 0xFF;
            hexChars[j * 2] = HEX_CHARS[v >>> 4];
            hexChars[j * 2 + 1] = HEX_CHARS[v & 0x0F];
        }
        return new String(hexChars);
    }

    @Override
    public void onReadRemoteRssi(BluetoothGatt gatt, int rssi, int status) {
        uiRssiUpdate(rssi);
        mLastRssi = String.valueOf(rssi)+"db";
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
                exportLogFile(false, "Connected @ "+getTimeStamp()+"\r\n"+"\r\n");
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
                exportLogFile(false, "Disconnected @ "+getTimeStamp()+"\r\n"+"\r\n");
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

    //ECG FIR FILTER:
    private native int jmainInitialization(boolean b);

//    private native double[] jfullHybridClassifier(double[] data1, double[] data2, double[] data3, double[] data4, boolean EOGOnly); //size = 1000

    //ECG BW Filter:
    private native int jmainEegFilt(boolean b);

    private native double[] jeogcfilt(double[] array);

    private native double jeogclassifier(double[] array1, double[] array2);

    private native double jeogclassifier2(double[] array1, double[] array2);
}
