package com.mahmoodms.bluetooth.eegssvepdemo;

import android.content.Context;
import android.graphics.Color;

import com.androidplot.xy.LineAndPointFormatter;
import com.androidplot.xy.SimpleXYSeries;

/**
 * Created by mahmoodms on 5/15/2017.
 */

public class GraphAdapter {
    // Variables

    public SimpleXYSeries series;
    public LineAndPointFormatter lineAndPointFormatter;
    private int seriesHistoryDataPoints;
    private int seriesHistorySeconds;
    private int numberDataPoints = 0;
    private double currentTimeStamp = 0;
    private boolean plotImplicitXvals;
    public double[] lastTimeValues;
    public double[] lastDataValues;
    private double[] unfilteredSignal;
    private double[] explicitXVals;

    private Context context; //>? Dont need?

    // Set/Get Methods (Don't need yet)

    // Constructor
    public GraphAdapter(/*Context context, */int seriesHistoryDataPoints, String XYSeriesTitle, boolean useImplicitXVals, int lineAndPointFormatterColor) {
//        this.context = context;
        //default values
        this.seriesHistoryDataPoints = seriesHistoryDataPoints;
        this.seriesHistorySeconds = seriesHistoryDataPoints/250;
        this.numberDataPoints = 0;
        this.currentTimeStamp = 0.0;
        this.plotImplicitXvals = false;
        this.lineAndPointFormatter = new LineAndPointFormatter(lineAndPointFormatterColor, null, null, null);
        setPointWidth(5); //Def value:
        //Initialize arrays:
        this.unfilteredSignal = new double[seriesHistoryDataPoints];
        this.explicitXVals = new double[seriesHistoryDataPoints];
        // Initialize series
        this.series = new SimpleXYSeries(XYSeriesTitle);
        if(useImplicitXVals) this.series.useImplicitXVals();

    }

    public void setPointWidth(float width) {
        this.lineAndPointFormatter.getLinePaint().setStrokeWidth(width);
    }

    // Manipulation Methods
        //Call - addDataPoints(rawData[], 24);
    public void addDataPoints(byte[] newDataPoints, int bytesPerInt) {
        int byteLength = newDataPoints.length;
        int intArraySize = byteLength/bytesPerInt;
        int[] dataArrInts = new int[byteLength/bytesPerInt];
        lastTimeValues = new double[byteLength/bytesPerInt];
        lastDataValues = new double[byteLength/bytesPerInt];
        int startIndex = seriesHistoryDataPoints-intArraySize;
        //shift old data backwards:
        System.arraycopy(unfilteredSignal, intArraySize, unfilteredSignal, 0, startIndex);
        System.arraycopy(explicitXVals, intArraySize, explicitXVals, 0, startIndex);
        // Parse new data to ints:
        switch (bytesPerInt) {
            case 2: //16-bit
                for (int i = 0; i < byteLength/bytesPerInt; i++) {
                    dataArrInts[i] = unsignedToSigned(unsignedBytesToInt(newDataPoints[2*i],newDataPoints[2*i+1]),16);
                    numberDataPoints++;
                }
                break;
            case 3: //24-bit
                for (int i = 0; i < byteLength/bytesPerInt; i++) {
                    dataArrInts[i] = unsignedToSigned(unsignedBytesToInt(newDataPoints[3*i],newDataPoints[3*i+1],newDataPoints[3*i+2]),24);
                    numberDataPoints++;
                    explicitXVals[startIndex+i] = numberDataPoints*0.004;
                    unfilteredSignal[startIndex+i] = convert24bitInt(dataArrInts[i]);
                    //Last Values (for plotting):
                    lastTimeValues[i] = numberDataPoints*0.004;
                    lastDataValues[i] = convert24bitInt(dataArrInts[i]);
                }
                break;
            default:
                break;
        }
        //Should be 6 data points:
    }

    //Blah:
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

    private int unsignedBytesToInt(byte b0, byte b1) {
        return (unsignedByteToInt(b0) + (unsignedByteToInt(b1) << 8));
    }

    private int unsignedBytesToInt(byte b0, byte b1, byte b2) {
        return (unsignedByteToInt(b0) + (unsignedByteToInt(b1) << 8) + (unsignedByteToInt(b2) << 16));
    }

    /**
     * Convert a signed byte to an unsigned int.
     */
    private int unsignedByteToInt(byte b) {
        return b & 0xFF;
    }

    public double convert24bitInt(final int int24bit) {
        double dividedInt = (double) int24bit/8388607.0;
        return dividedInt*2.42;
    }
}
