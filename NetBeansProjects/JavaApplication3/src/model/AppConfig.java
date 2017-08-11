package model;

import java.util.Observable;

public class AppConfig extends Observable {
    private int screenRefreshInterval = 10;
    private int readerReadInterval = 250;
    private boolean xReadEnabled = true;
    private String readerIpAddress = "";

    public int getScreenRefreshInterval() {
        return screenRefreshInterval;
    }

    public void setScreenRefreshInterval(int interval) {
        screenRefreshInterval = interval;
        setChanged();
        notifyObservers();
    }

    public int getReaderReadInterval() {
        return readerReadInterval;
    }

    public void setReaderReadInterval(int interval) {
        readerReadInterval = interval;
        setChanged();
        notifyObservers();
    }

    public boolean getXReadEnabled() {
        return xReadEnabled;
    }

    public void setXReadEnabled(boolean state) {
        xReadEnabled = state;
        setChanged();
        notifyObservers();
    }

    public String getReaderIpAddress() {
        return readerIpAddress;
    }

    public void setReaderIpAddress(String ipAddress) {
        readerIpAddress = ipAddress;
    }
}
