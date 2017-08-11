package model;

import java.util.Observable;
import java.util.Observer;

public abstract class RFIDReader extends Observable implements Observer {
    private Antenna[] antennas;
    private final int numberOfPorts = 4;

    public RFIDReader() {
        antennas = new Antenna[numberOfPorts];
    }

    public abstract int getReadInterval();
    public abstract void setReadInterval(int time);

    public abstract boolean getXReadEnabled();
    public abstract void setXReadEnabled(boolean enabled);

    public abstract void reset();

    public abstract void startReadingTags();
    public abstract void stopReadingTags();

    public abstract String[] getEnvironmentalIds();
    public abstract void setEnvironmentalIds(String ids[]);

    public abstract TagRead[] getNewTagReads();

    public int getNumberOfPorts() {
        return numberOfPorts;
    }

    public Antenna getAntenna(int port) {
        if (!isValidAntennaPortNumber(port))
            throw new IllegalArgumentException("invalid port");

        return antennas[port];
    }

    public Antenna[] getAntennas() {
        return antennas;
    }

    public void addAntenna(Antenna antenna, int port) {
        if (!isValidAntennaPortNumber(port))
            throw new IllegalArgumentException("invalid port");

        if (antennas[port] != null)
            throw new RuntimeException("port occupied");

        antennas[port] = antenna;
        antenna.addObserver(this);
        configureAntennas();

        setChanged();
        notifyObservers();
    }

    public void addAntennas(Antenna[] antennas) {
        if (antennas.length != getNumberOfPorts())
            throw new IllegalArgumentException();

        for (int port = 0; port < getNumberOfPorts(); port++)
            if (this.antennas[port] != null)
                throw new RuntimeException("port occupied");

        for (int port = 0; port < antennas.length; port++)
            if (antennas[port] != null)
                addAntenna(antennas[port], port);
    }

    public Antenna removeAntenna(int port) {
        if (!isValidAntennaPortNumber(port))
            throw new IllegalArgumentException("invalid port");

        if (antennas[port] == null)
            throw new RuntimeException("no antenna in port");

        Antenna antenna = antennas[port];
        antennas[port] = null;
        antenna.deleteObserver(this);
        configureAntennas();

        setChanged();
        notifyObservers();

        return antenna;
    }

    public void update(Observable o, Object arg) {
        // if an antenna has changed
        if (o.getClass().equals(Antenna.class)) {
            configureAntennas();
        }
    }

    // Configures the reader's antennas to match the objects in our antennas array.
    protected abstract void configureAntennas();

    private boolean isValidAntennaPortNumber(int port) {
        return port >= 0 && port < numberOfPorts;
    }
}
