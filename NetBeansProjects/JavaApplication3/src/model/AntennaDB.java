package model;

import java.util.Properties;
import java.util.Hashtable;
import java.util.Enumeration;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

import java.util.Observable;
import java.util.Observer;

import model.DB;
import model.Antenna;
import model.AntennaType;

public class AntennaDB extends DB implements Observer {
    private final int antennas_per_reader = 4;

    // stores the reader number and antenna number for each antenna we're watching
    private Hashtable<Antenna, int[]> watchingAntennas;

    public AntennaDB() {
        watchingAntennas = new Hashtable<Antenna, int[]>();
    }

    public void update(Observable o, Object arg) {
        if (o.getClass().equals(Antenna.class)) {
            Antenna antenna = (Antenna) o;
            int[] numbers = watchingAntennas.get(antenna);

            if (numbers != null) {
                int reader_number = numbers[0];
                int antenna_number = numbers[1];
                storeAntenna(antenna, reader_number, antenna_number);
            }
        }
    }

    public void watchAntennas(Antenna[] antennas, int reader_number) {
        if (antennas.length != antennas_per_reader)
            throw new RuntimeException();

        for (int i = 0; i < antennas.length; i++)
            if (antennas[i] != null)
                watchAntenna(antennas[i], reader_number, i);
    }

    // note: implicitly unwatches an already watched antenna for this reader and port
    public void watchAntenna(Antenna antenna, int reader_number, int antenna_number) {
        Enumeration<Antenna> enumeration = watchingAntennas.keys();

        // if we're already watching an Antenna for this reader/port, unwatch it
        while (enumeration.hasMoreElements()) {
            Antenna key = enumeration.nextElement();
            int[] value = watchingAntennas.get(key);

            if (value[0] == reader_number && value[1] == antenna_number)
                unwatchAntenna(key);
        }

        int[] numbers = new int[2];
        numbers[0] = reader_number;
        numbers[1] = antenna_number;
        watchingAntennas.put(antenna, numbers);
        antenna.addObserver(this);
    }

    public void unwatchAntenna(Antenna antenna) {
        watchingAntennas.remove(antenna);
        antenna.deleteObserver(this);
    }

    // note: implicitly watches the antennas
    public Antenna[] loadAntennas(int reader_number) {
        Antenna[] antennas = new Antenna[antennas_per_reader];

        for (int i = 0; i < antennas.length; i++)
            antennas[i] = loadAntenna(reader_number, i);

        return antennas;
    }

    // note: implicitly watches the antenna
    public Antenna loadAntenna(int reader_number, int antenna_number) {
        String path = basePath + "/readers/" + reader_number
                                        + "/antennas/" + antenna_number;

        Properties properties = new Properties();

        try {
            File file = new File(path);

            if (!file.exists())
                return null;

            FileInputStream stream = new FileInputStream(file);
            properties.load(stream);
            stream.close();
        }
        catch (Exception e) {
            throw new RuntimeException("load antenna failed");
        }

        Antenna antenna = new Antenna();

        if (properties.get("type") == null
                || properties.get("powered_on") == null
                || properties.get("power") == null
                || properties.get("occupancy_threshold") == null
                || properties.get("timeout") == null)
            throw new RuntimeException("antenna config missing field");

        try {
            AntennaType type = AntennaType.valueOf(properties.get("type").toString());
            boolean poweredOn = new Boolean(properties.get("powered_on").toString());
            int power = new Integer(properties.get("power").toString());
            int threshold = new Integer(properties.get("occupancy_threshold").toString());
            int timeout = new Integer(properties.get("timeout").toString());

            antenna.setType(type);
            antenna.setPoweredOn(poweredOn);
            antenna.setPower(power);
            antenna.setOccupancyThreshold(threshold);
            antenna.setTimeout(timeout);
        }
        catch (Exception e) {
            throw new RuntimeException("antenna config invalid value");
        }

        watchAntenna(antenna, reader_number, antenna_number);

        return antenna;
    }

    public void storeAntenna(Antenna antenna, int reader_number, int antenna_number) {
        String path = basePath + "/readers/" + reader_number
                                        + "/antennas/" + antenna_number;

        File parent = new File(path).getParentFile();

        if (!parent.exists()) {
            boolean status = parent.mkdirs();

            if (!status)
                throw new RuntimeException("store antenna failed");
        }

        Properties properties = new Properties();

        properties.put("type", antenna.getType().toString());
        properties.put("powered_on", Boolean.toString(antenna.isPoweredOn()));
        properties.put("power", Integer.toString(antenna.getPower()));
        properties.put("occupancy_threshold", Integer.toString(antenna.getOccupancyThreshold()));
        properties.put("timeout", Integer.toString(antenna.getTimeout()));

        try {
            FileOutputStream stream = new FileOutputStream(new File(path));
            properties.store(stream, "Antenna Configuration");
            stream.close();
        }
        catch (Exception e) {
            throw new RuntimeException("store antenna failed");
        }
    }
}
