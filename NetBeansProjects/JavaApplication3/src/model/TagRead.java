package model;

import java.util.Date;
import java.text.SimpleDateFormat;
import java.text.ParsePosition;

public class TagRead {
    private String id;
    private int antenna;
    private int xRead;
    private double[] decisionRSSIs;
    private Date date;

    private boolean hasTemperature;
    private boolean hasHumidity;
    private boolean hasLight;

    private double temperature;
    private int humidity;
    private int light;

    public TagRead(String rawData) {
        try {
            String[] fields = rawData.split("\\s+");

            id = fields[0];
            antenna = Integer.parseInt(fields[1]);
            xRead = Integer.parseInt(fields[2]);

            String[] decisionFactors = fields[3].replaceAll("\\[", "").replaceAll("\\]", "").split(",");
            decisionRSSIs = new double[4];
            for (int i = 0; i < 4; i++) {
                if (decisionFactors[i].equals("xxxxx"))
                    decisionRSSIs[i] = 0.0;
                else
                    decisionRSSIs[i] = Double.parseDouble(decisionFactors[i]);
            }

            SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ssXXX");
            ParsePosition position = new ParsePosition(0);
            date = format.parse(fields[4], position);

            hasTemperature = !fields[5].equals("xxxxx");
            if (hasTemperature) {
                temperature = Double.parseDouble(fields[5]);
            }
            else {
                temperature = -1.0;
            }

            hasHumidity = !fields[6].equals("xx");
            if (hasHumidity) {
                humidity = (int) Double.parseDouble(fields[6]);
            }
            else {
                humidity = -1;
            }

            hasLight = !fields[7].equals("xxxxx");
            if (hasLight) {
                light = (int) Double.parseDouble(fields[7]);
            }
            else {
                light = -1;
            }
        }
        catch (Exception e) {
            throw new IllegalArgumentException();
        }
    }

    public String getId() {
        return id;
    }

    public int getAntenna() {
        return antenna;
    }

    public int getXRead() {
        return xRead;
    }

    public double[] getDecisionRSSIs() {
        return decisionRSSIs;
    }

    public Date getDate() {
        return date;
    }

    public boolean hasTemperature() {
        return hasTemperature;
    }

    public double getTemperature() {
        return temperature;
    }

    public boolean hasHumidity() {
        return hasHumidity;
    }

    public int getHumidity() {
        return humidity;
    }

    public boolean hasLight() {
        return hasLight;
    }

    public int getLight() {
        return light;
    }

    public String toString() {
        String string = getId() + " " + getAntenna() + " " + getXRead();

        string += " [";
        for (int i = 0; i < 4; i++) {
            if (i != 0)
                string += ",";

            if (getDecisionRSSIs()[i] == 0.0)
                string += "xxxxx";
            else
                string += String.format("%5.1f", getDecisionRSSIs()[i]);
        }
        string += "]";
       
        string += " " + String.join("-", getDate().toString().split(" "));

        string += " ";
        if (getTemperature() == -1.0)
            string += "xxxxx";
        else
            string += String.format("%5.1f", getTemperature());

        string += " ";
        if (getHumidity() == -1)
            string += "xx";
        else
            string += String.format("%2d", getHumidity());

        string += " ";
        if (getLight() == -1)
            string += "xxxxx";
        else
            string += String.format("%5d", getLight());

        return string;
    }
}
