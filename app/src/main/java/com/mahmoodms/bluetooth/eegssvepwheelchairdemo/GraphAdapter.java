package com.mahmoodms.bluetooth.eegssvepwheelchairdemo;

import com.androidplot.xy.LineAndPointFormatter;
import com.androidplot.xy.SimpleXYSeries;

/**
 * Created by mahmoodms on 5/15/2017.
 */

public class GraphAdapter {
    // Variables
    private boolean filterData;
    public int intArraySize;
    SimpleXYSeries series;
    LineAndPointFormatter lineAndPointFormatter;
    private int seriesHistoryDataPoints;
    double[] lastTimeValues;
    double[] lastDataValues;
    double[] unfilteredSignal;
    boolean plotData;

    // Set/Get Methods (Don't need yet)
    public void setPlotData(boolean plotData) {
        this.plotData = plotData;
        if (!plotData) {
            clearPlot();
        }
    }

    // Constructor
    public GraphAdapter(int seriesHistoryDataPoints, String XYSeriesTitle, boolean useImplicitXVals, boolean filterData, int lineAndPointFormatterColor) {
        //default values
        this.filterData = filterData;
        this.seriesHistoryDataPoints = seriesHistoryDataPoints;
        this.intArraySize = 6; //24-bit default
        this.lineAndPointFormatter = new LineAndPointFormatter(lineAndPointFormatterColor, null, null, null);
        setPointWidth(5); //Def value:
        //Initialize arrays:
        this.unfilteredSignal = new double[seriesHistoryDataPoints];
        // Initialize series
        this.series = new SimpleXYSeries(XYSeriesTitle);
        if(useImplicitXVals) this.series.useImplicitXVals();
        //Don't plot data until explicitly told to do so:
        this.plotData = false;
    }

    public void setPointWidth(float width) {
        this.lineAndPointFormatter.getLinePaint().setStrokeWidth(width);
    }

    // Manipulation Methods
    //Call - addDataPoints(rawData[], 24);
    public void addDataPoints(byte[] newDataPoints, int bytesPerInt, int packetNumber) {
        int byteLength = newDataPoints.length;
        intArraySize = byteLength/bytesPerInt;
        int[] dataArrInts = new int[byteLength/bytesPerInt];
        lastTimeValues = new double[byteLength/bytesPerInt];
        lastDataValues = new double[byteLength/bytesPerInt];
        int startIndex = this.unfilteredSignal.length-intArraySize;
        //shift old data backwards:
        System.arraycopy(unfilteredSignal, intArraySize, unfilteredSignal, 0, startIndex);
        // Parse new data to ints:
        switch (bytesPerInt) {
            case 2: //16-bit
                for (int i = 0; i < byteLength/bytesPerInt; i++) {
                    dataArrInts[i] = unsignedToSigned(unsignedBytesToInt(newDataPoints[2*i],newDataPoints[2*i+1]),16);
                }
                //Call Plot
                break;
            case 3: //24-bit
                for (int i = 0; i < byteLength/bytesPerInt; i++) {
                    dataArrInts[i] = unsignedToSigned(unsignedBytesToInt(newDataPoints[3*i],newDataPoints[3*i+1],newDataPoints[3*i+2]),24);
                    //Last Values (for plotting):
                    lastTimeValues[i] = packetNumber*(0.024) + i*0.004;
                    lastDataValues[i] = convert24bitInt(dataArrInts[i]);
                    unfilteredSignal[994+i] = lastDataValues[i];
                }
                //Call Plot:
                if(this.plotData) updateGraph();
                break;
            default:
                break;
        }
    }

    //Graph Stuff:
    private void clearPlot() {
        if(this.series!=null) {
            DeviceControlActivity.redrawer.pause();
            while(this.series.size()>0) {
                this.series.removeFirst();
            }
//            DeviceControlActivity.mPlotAdapter.adjustPlot(this);
            DeviceControlActivity.redrawer.start();
        }
    }

    private void updateGraph() {
        if(!filterData) {
            for (int i = 0; i < intArraySize; i++) {
                plot(lastTimeValues[i], lastDataValues[i]);
            }
        } else {
            //FILTER AND CALL PLOT (SOMEHOW)
        }
    }

    private void plot(double x, double y) {
        if(series.size()>seriesHistoryDataPoints-1) {
            series.removeFirst();
        }
        series.addLast(x,y);
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
