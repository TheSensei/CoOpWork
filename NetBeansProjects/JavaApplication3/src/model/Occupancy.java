package model;

import java.util.ArrayList;

import java.util.Date;
import java.util.concurrent.TimeUnit;

import java.text.SimpleDateFormat;

import model.Antenna;
import model.TagRead;

public class Occupancy {
    private TagData initialData;
    private int reader;
    private ArrayList<TagRead> tagReads;
    private Date entryDate;
    private boolean alarmTriggered = false;
    private Antenna[] antennas;
    private int antennaAtLastUpdateForRead;
    private int decidedAntenna;
    private int decidedConfidence;

    private double temperature;
    private int humidity;
    private int light;

    private boolean haveTemperature = false;
    private boolean haveHumidity = false;
    private boolean haveLight = false;

    public Occupancy(int reader, TagData initialData, TagRead initialRead, Antenna[] antennas) {
        this.reader = reader;
        this.initialData = initialData;

        tagReads = new ArrayList<TagRead>();
        tagReads.add(initialRead);

        entryDate = initialRead.getDate();
        antennaAtLastUpdateForRead = initialRead.getAntenna();

        // NOTE: Originally this class was designed so that it didn't
        // need to know about antennas. But easiest way to add in
        // "zone averaging" slash "intent to stay" functionality was
        // to pass the antennas in so we can access timeouts. This
        // severely limits us to exactly one reader, four antennas,
        // but it's the quickest solution for Phase 1.
        this.antennas = antennas;
    }

    public void updateForRead(int reader, TagRead read) {
        this.reader = reader;

        tagReads.add(read);

        tryToTriggerAlarm();
        processTagReads();

        if (getAntenna() != antennaAtLastUpdateForRead)
            entryDate = read.getDate();

        antennaAtLastUpdateForRead = getAntenna();

        if (read.hasTemperature()) {
            haveTemperature = true;
            temperature = read.getTemperature();
        }

        if (read.hasHumidity()) {
            haveHumidity = true;
            humidity = read.getHumidity();
        }

        if (read.hasLight()) {
            haveLight = true;
            light = read.getLight();
        }
    }

    // Patched up for "zone averaging" slash "intent to stay". If this
    // returns true, the object will be in an invalid state and will
    // blow up when you use it.
    public boolean isTimedOut() {
        boolean hasRemainingTime = getRemainingTime() >= 0;

        timeoutTagReads();

        return tagReads.isEmpty() || !hasRemainingTime;
    }

    public String getName() {
        return initialData.getName();
    }

    public int getReader() {
        return reader;
    }

    public int getAntenna() {
        return decidedAntenna;
    }

    public int getZone() {
        return getAntenna();
    }

    public int getConfidence() {
        return decidedConfidence;
    }

    public boolean hasTemperature() {
        return haveTemperature;
    }

    public double getTemperature() {
        return temperature;
    }

    public boolean hasHumidity() {
        return haveHumidity;
    }

    public int getHumidity() {
        return humidity;
    }

    public boolean hasLight() {
        return haveLight;
    }

    public int getLight() {
        return light;
    }

    public Date getEntryDate() {
        return entryDate;
    }

    public String getEntryDateString() {
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss Z");
        return format.format(getEntryDate());
    }

    public Date getLastDate() {
        TagRead lastRead = tagReads.get(tagReads.size() - 1);
        return lastRead.getDate();
    }

    public Date getLastDateForCurrentAntenna() {
        for (int i = tagReads.size() - 1; i >= 0; i--) {
            TagRead read = tagReads.get(i);

            if (read.getAntenna() == getAntenna())
                return read.getDate();
        }

        // else this object is in an invalid state
        throw new RuntimeException();
    }

    // seconds between entryDate and now
    public int getStayTime() {
        return secondsBetweenDates(getEntryDate(), new Date());
    }

    public String getStayTimeString() {
        return secondsToString(getStayTime());
    }

    // seconds between lastDate and now
    public int getStaleTime() {
        return secondsBetweenDates(getLastDate(), new Date());
    }

    public String getStaleTimeString() {
        return secondsToString(getStaleTime());
    }

    public int getStaleTimeForCurrentAntenna() {
        return secondsBetweenDates(getLastDateForCurrentAntenna(), new Date());
    }

    public String getStaleTimeForCurrentAntennaString() {
        return secondsToString(getStaleTimeForCurrentAntenna());
    }

    // seconds until timeout
    public int getRemainingTime() {
        return currentAntennaTimeout() - getStaleTimeForCurrentAntenna();
    }

    public String getRemainingTimeString() {
        String string = secondsToString(getRemainingTime());

        if (getStaleTime() + getRemainingTime() < currentAntennaTimeout())
            string += "*";

        return string;
    }

    public boolean hasIntentToStay() {
        return getStayTime() - getStaleTimeForCurrentAntenna() > longestAntennaTimeout();
    }

    public boolean getAlarmTriggered() {
        return alarmTriggered;
    }

    public void triggerAlarm() {
        alarmTriggered = true;
    }

    public void resetAlarm() {
        alarmTriggered = false;
    }

    public void setAlarmTriggered(boolean state) {
        alarmTriggered = state;
    }

    public String toString() {
        return "" + getReader() + " " + getZone() + " " + getConfidence()
                            + " " + getEntryDate() + " " + getLastDate()
                            + " " + getStayTime() + " " + getAlarmTriggered();
    }

    public String toCSV() {
        return "" + getReader() + "," + getZone() + "," + getConfidence()
                            + "," + getEntryDate() + "," + getLastDate()
                            + "," + getStayTime() + "," + getAlarmTriggered();
    }

    private void tryToTriggerAlarm() {
        boolean r1a0 = initialData.getAlarmsR1A0();
        boolean r1a1 = initialData.getAlarmsR1A1();
        boolean r1a2 = initialData.getAlarmsR1A2();
        boolean r1a3 = initialData.getAlarmsR1A3();

        boolean[] seenOnAntennas = {false, false, false, false};

        for (TagRead read : tagReads)
            seenOnAntennas[read.getAntenna()] = true;

        if (r1a0 && seenOnAntennas[0])
            alarmTriggered = true;
        if (r1a1 && seenOnAntennas[1])
            alarmTriggered = true;
        if (r1a2 && seenOnAntennas[2])
            alarmTriggered = true;
        if (r1a3 && seenOnAntennas[3])
            alarmTriggered = true;
    }

    private int secondsBetweenDates(Date dateFrom, Date dateTo) {
        return (int) (dateTo.getTime() - dateFrom.getTime()) / 1000;
    }

    private void timeoutTagReads() {
        Date now = new Date();

        for (int i = 0; i < tagReads.size(); i++) {
            if (secondsBetweenDates(tagReads.get(i).getDate(), now) > longestAntennaTimeout())
                tagReads.remove(i--);
        }
    }

    private int currentAntennaTimeout() {
        Antenna antenna = antennas[getAntenna()];
        return antenna.getTimeout();
    }

    private int longestAntennaTimeout() {
        int timeout = 0;

        for (TagRead read : tagReads) {
            Antenna antenna = antennas[read.getAntenna()];

            if (antenna.getTimeout() > timeout)
                timeout = antenna.getTimeout();
        }

        return timeout;
    }

    private void processTagReads() {
        if (getMostRecentRead().getXRead() == 0) {
            keepMostRecentReadOnly();
            decidedAntenna = getMostRecentRead().getAntenna();
            decidedConfidence = 0;
            return;
        }

        // indices 0-3 are antennas 0-3
        int[] counts = {0, 0, 0, 0};
        int[] confidenceSums = {0, 0, 0, 0};
        double[] confidenceAverages = {0, 0, 0, 0};

        for (TagRead read : tagReads) {
            counts[read.getAntenna()] += 1;
            confidenceSums[read.getAntenna()] += read.getXRead();
        }

        for (int i = 0; i < 4; i++)
            if (counts[i] != 0)
                confidenceAverages[i] = (double) confidenceSums[i] / counts[i];

        int maxSumIndex = 0;
        int secondMaxSumIndex = 0;

        if (confidenceSums[0] > confidenceSums[1]) {
            maxSumIndex = 0;
            secondMaxSumIndex = 1;
        }
        else {
            maxSumIndex = 1;
            secondMaxSumIndex = 0;
        }

        for (int i = 2; i < 4; i++) {
            if (confidenceSums[i] >= confidenceSums[maxSumIndex]) {
                secondMaxSumIndex = maxSumIndex;
                maxSumIndex = i;
            }
            else if (confidenceSums[i] >= confidenceSums[secondMaxSumIndex]) {
                secondMaxSumIndex = i;
            }
        }

        int maxSum = confidenceSums[maxSumIndex];
        int secondMaxSum = confidenceSums[secondMaxSumIndex];

        int combinedSum = maxSum + secondMaxSum;

        int totalSum = 0;
        for (int i = 0; i < 4; i++)
            totalSum += confidenceSums[i];

        int confidence;

        if (maxSum == totalSum) {
            confidence = (int) confidenceAverages[maxSumIndex];

            if (confidence < 1)
                confidence = 1;
            if (confidence > 100)
                confidence = 100;
        }
        else {
            double average = confidenceAverages[maxSumIndex];

            double proportion = (double) maxSum / combinedSum;

            // Transform the range [0.5, 1.0] into [0.0, 1.0].
            double proportionWeight = 2 * (proportion - 0.5);

            confidence = (int) (proportionWeight * average);

            if (confidence < 1)
                confidence = 1;
            else if (confidence > 99)
                confidence = 99;
        }

        decidedAntenna = maxSumIndex;
        decidedConfidence = confidence;
    }

    private String secondsToString(int seconds) {
        String string = "";

        long days = TimeUnit.SECONDS.toDays(seconds);
        if (days > 0) string += days + "d ";
        seconds -= days * TimeUnit.DAYS.toSeconds(1);

        long hours = TimeUnit.SECONDS.toHours(seconds);
        if (hours > 0) string += String.format("%02d:", hours);
        seconds -= hours * TimeUnit.HOURS.toSeconds(1);

        long minutes = TimeUnit.SECONDS.toMinutes(seconds);
        string += String.format("%02d:", minutes);
        seconds -= minutes * TimeUnit.MINUTES.toSeconds(1);

        string += String.format("%02d", seconds);

        return string;
    }

    private TagRead getMostRecentRead() {
        return tagReads.get(tagReads.size() - 1);
    }

    private void keepMostRecentReadOnly() {
        for (int i = 0; i < tagReads.size() - 1; i++)
            tagReads.remove(0);
    }
}
