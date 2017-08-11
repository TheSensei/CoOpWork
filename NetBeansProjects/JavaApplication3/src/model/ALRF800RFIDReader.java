package model;

import java.net.ServerSocket;
import java.net.Socket;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.PrintWriter;

import java.util.ArrayList;

import java.util.Date;
import java.util.TimeZone;
import java.text.SimpleDateFormat;

import model.RFIDReader;
import model.Antenna;
import model.TagRead;

public class ALRF800RFIDReader extends RFIDReader {
    private boolean readingTags = false;
    private boolean allAntennasAreOff = true;
    private String ipAddress;
    private Communication com;

    private boolean reading = false;
    private int readInterval = 1000;
    private String[] environmentalIds = null;
    private boolean xReadEnabled = true;

    private final int timeUpdateInterval = 100;
    private int timeUpdateCount = 0;

    public ALRF800RFIDReader(String ipAddress) {
        this.ipAddress = ipAddress;
        com = new Communication(ipAddress);
        if (!com.ping()) {
            System.err.println("Debug: tag processor "
                                                + ipAddress + " ping failed");
            throw new RuntimeException();
        }

        com.setTime(new Date());
    }

    public int getReadInterval() {
        return readInterval;
    }

    public void setReadInterval(int time) {
        readInterval = time;
        com.setReadInterval(time);

        setChanged();
        notifyObservers();
    }

    public boolean getXReadEnabled() {
        return xReadEnabled;
    }

    public void setXReadEnabled(boolean enabled) {
        xReadEnabled = enabled;
        com.setXRead(enabled);

        setChanged();
        notifyObservers();
    }

    public void reset() {
        // Do nothing, really
        com.ping();
    }

    public boolean isReadingTags() {
        return reading;
    }

    public void startReadingTags() {
        reading = true;
        com.setReading(true);

        setChanged();
        notifyObservers();
    }

    public void stopReadingTags() {
        reading = false;
        com.setReading(false);

        setChanged();
        notifyObservers();
    }

    public String[] getEnvironmentalIds() {
        return environmentalIds;
    }

    public void setEnvironmentalIds(String ids[]) {
        environmentalIds = ids;
        com.setEnvironmentalIds(String.join(" ", ids));

        setChanged();
        notifyObservers();
    }

    public TagRead[] getNewTagReads() {
        timeUpdateCount++;
        timeUpdateCount %= timeUpdateInterval;

        if (timeUpdateCount == 0)
            com.setTime(new Date());

        String[] strings = com.getReads();

        if (strings == null)
            return null;

        ArrayList<TagRead> reads = new ArrayList<TagRead>();

        for (int i = 0; i < strings.length; i++) {
            try {
                reads.add(new TagRead(strings[i]));
            }
            catch (IllegalArgumentException e) {
                System.err.println("Warning: Malformed tag data");
            }
        }

        if (reads.size() != 0)
            return reads.toArray(new TagRead[0]);
        else
            return null;
    }

    protected void configureAntennas() {
        String antennas = "";

        for (int i = 0; i < getNumberOfPorts(); i++) {
            Antenna antenna = getAntenna(i);

            if (antenna == null) {
                antennas += " 315off";
                continue;
            }

            String string = " ";
            string += Integer.toString(antenna.getPower());
            string += antenna.isPoweredOn() ? "on" : "off";

            antennas += string;
        }

        com.setAntennas(antennas);
    }

    // for network communication with the tag read processor
    private class Communication {
        private final int port = 4000;
        private String ipAddress;

        private Socket socket;
        private PrintWriter out;
        private BufferedReader in;

        public Communication(String ipAddress) {
            this.ipAddress = ipAddress;
        }

        boolean ping() {
            boolean answer = false;

            try {
                open();

                send("ping");

                if (!status("ack"))
                    throw new RuntimeException();

                close();

                answer = true;
            }
            catch (Exception e) {
            }

            return answer;
        }

        String getState() {
            String answer = "";

            try {
                open();

                send("get state");

                String line;
                while ((line = recv()) != "end") {
                    answer += line;
                }

                close();
            }
            catch (Exception e) {
                System.err.println("Debug: getState failed");
            }

            return answer;
        }

        String[] getReads() {
            ArrayList<String> list = new ArrayList<String>();

            try {
                open();

                send("get reads");

                String line;
                while (!(line = recv()).equals("end")) {
                    if (line == null)
                        break;

                    list.add(line);
                }

                close();
            }
            catch (Exception e) {
            }

            if (list.size() != 0) {
                String[] answer = list.toArray(new String[0]);
                return answer;
            }

            return null;
        }

        boolean setReading(boolean state) {
            boolean answer = false;

            try {
                open();

                send("set reading");
                send(Boolean.toString(state));

                if (!status("ok"))
                    throw new RuntimeException();

                close();

                answer = true;
            }
            catch (Exception e) {
                System.err.println("Debug: setReading failed");
            }

            return answer;
        }

        boolean setReadInterval(int interval) {
            boolean answer = false;

            try {
                open();

                send("set read interval");
                send(Integer.toString(interval));

                if (!status("ok"))
                    throw new RuntimeException();

                close();

                answer = true;
            }
            catch (Exception e) {
                System.err.println("Debug: setReadInterval failed");
            }

            return answer;
        }

        // "<power0><on/off0> <power1><on/off1> <power2><on/off2> <power3><on/off3>"
        // ex: "315on 315off 315off 315off"
        boolean setAntennas(String string) {
            boolean answer = false;

            try {
                open();

                send("set antennas");
                send(string);

                if (!status("ok"))
                    throw new RuntimeException();

                close();

                answer = true;
            }
            catch (Exception e) {
                System.err.println("Debug: setAntennas failed");
            }

            return answer;
        }

        // "<id> <id> <id>..."
        // ex: "000000000000000000000001 000000000000000000000002"
        boolean setEnvironmentalIds(String string) {
            boolean answer = false;

            try {
                open();

                send("set environmental ids");
                send(string);

                if (!status("ok"))
                    throw new RuntimeException();

                close();

                answer = true;
            }
            catch (Exception e) {
                System.err.println("Debug: setEnvironmentalIds failed");
            }

            return answer;
        }

        boolean setXRead(boolean state) {
            boolean answer = false;

            try {
                open();

                send("set xread");
                send(Boolean.toString(state));

                if (!status("ok"))
                    throw new RuntimeException();

                close();

                answer = true;
            }
            catch (Exception e) {
                System.err.println("Debug: setXRead failed");
            }

            return answer;
        }

        boolean setTime(Date time) {
            boolean answer = false;

            try {
                open();

                SimpleDateFormat format = new SimpleDateFormat("YYYY/MM/dd HH:mm:ss");
                format.setTimeZone(TimeZone.getTimeZone("UTC"));

                send("set time");
                send(format.format(time));

                if (!status("ok"))
                    throw new RuntimeException();

                close();

                answer = true;
            }
            catch (Exception e) {
                System.err.println("Debug: setTime failed");
            }

            return answer;
        }

        private void open() throws Exception {
            socket = new Socket(ipAddress, port);

            out = new PrintWriter(socket.getOutputStream());
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        }

        private void close() throws Exception {
            in.close();
            socket.close();
        }

        private void send(String line) throws Exception {
            out.println(line);
            out.flush();
        }

        private String recv() throws Exception {
            return in.readLine();
        }

        private boolean status(String line) throws Exception {
            return line.equals(recv());
        }
    }
}
