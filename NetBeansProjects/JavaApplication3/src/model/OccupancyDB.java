package model;

import java.util.Hashtable;
import java.util.Enumeration;
import java.util.ArrayList;

import java.io.File;
import java.io.PrintWriter;

import model.Antenna;
import model.TagData;
import model.TagDataDB;
import model.TagRead;
import model.Occupancy;
import model.DB;

public class OccupancyDB extends DB {
    private Hashtable<String, Occupancy> occupancies;
    private TagDataDB tagDataDB;
    private Antenna[] antennas;
    private boolean byAntenna;

    public OccupancyDB(Antenna[] antennas) {
        occupancies = new Hashtable<String, Occupancy>();

        // NOTE: Using a TagDataDB in here could have awful side
        // effects. Careful.
        tagDataDB = new TagDataDB();

        // NOTE: Originally this class was designed so that it didn't
        // need to know about antennas. But easiest way to add in
        // "zone averaging" slash "intent to stay" functionality was
        // to pass the antennas in so we can access timeouts. This
        // severely limits us to exactly one reader, four antennas,
        // but it's the quickest solution for Phase 1.
        this.antennas = antennas;
    }

    public boolean getByAntenna() {
        return byAntenna;
    }

    public void setByAntenna(boolean state) {
        if (state != byAntenna)
            clear();

        byAntenna = state;
    }

    public void updateForRead(int reader, TagRead read) {
        String key;
        if (byAntenna)
            key = read.getId() + "@" + reader + "@" + read.getAntenna();
        else
            key = read.getId();

        Occupancy occupancy = occupancies.get(key);

        if (occupancy == null) {
            String id = read.getId();

            TagData tag = tagDataDB.loadTagReadOnly(id);

            if (tag == null)
                return;

            occupancy = new Occupancy(reader, tag, read, antennas);
            occupancies.put(key, occupancy);
        }

        occupancy.updateForRead(reader, read);
    }

    public void timeoutOccupancies() {
        String[] ids = getOccupancyIds();

        if (ids == null || ids.length == 0)
            return;

        for (String id : ids)
            if (occupancies.get(id).isTimedOut())
                occupancies.remove(id);
    }

    public void killOccupancy(String id) {
        Occupancy occupancy = occupancies.get(id);

        if (occupancy != null)
            occupancies.remove(id);
    }

    public String[] getOccupancyIds() {
        String[] ids = occupancies.keySet().toArray(new String[0]);

        if (ids.length == 0)
            return null;

        return ids;
    }

    //public Occupancy[] getOccupancies() {
    //    String[] ids = getOccupancyIds();

    //    if (ids == null)
    //        return null;

    //    Occupancy[] occupancies = new Occupancy[ids.length];

    //    for (int i = 0; i < occupancies.length; i++)
    //        occupancies[i] = this.occupancies.get(ids[i]);

    //    return occupancies;
    //}

    public Occupancy getOccupancy(String id) {
        return occupancies.get(id);
    }

    public String toString() {
        Enumeration<String> enumeration = occupancies.keys();

        String string = "";

        while (enumeration.hasMoreElements()) {
            String element = enumeration.nextElement();
            string += element + " " + occupancies.get(element) + "\n";
        }

        return string;
    }

    public void clear() {
        occupancies = new Hashtable<String, Occupancy>();
    }

    public String toCSV() {
        Enumeration<String> enumeration = occupancies.keys();

        String string = "";

        while (enumeration.hasMoreElements()) {
            String element = enumeration.nextElement();
            string += element + "," + occupancies.get(element).toCSV() + "\n";
        }

        return string;
    }

    public void storeCSVFile() {
        String path = basePath + "/output/registered_tags.csv";

        File file = new File(path);
        File parent = file.getParentFile();

        if (!parent.exists()) {
            boolean status = parent.mkdirs();

            if (!status)
                throw new RuntimeException("store registered tags failed");
        }

        try {
            PrintWriter writer = new PrintWriter(file);
            writer.print(toCSV());
            writer.close();
        }
        catch (Exception e) {
            throw new RuntimeException("store registered tags failed");
        }
    }
}
