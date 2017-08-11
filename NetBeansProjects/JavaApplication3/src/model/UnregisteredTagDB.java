package model;

import java.util.ArrayList;

import java.io.File;
import java.io.PrintWriter;

import model.DB;

public class UnregisteredTagDB extends DB {
    private ArrayList<String> ids;

    public UnregisteredTagDB() {
        ids = new ArrayList<String>();
    }

    public void addId(String id) {
        if (!ids.contains(id))
            ids.add(id);
    }

    public void removeId(String id) {
        if (ids.contains(id))
            ids.remove(id);
    }

    public void clear() {
        ids = new ArrayList<String>();
    }

    public String toCSV() {
        String[] ids = this.ids.toArray(new String[0]);

        if (ids == null || ids.length == 0)
            return "";

        return String.join("\n", ids) + "\n";
    }

    public void storeCSVFile() {
        String path = basePath + "/output/unregistered_tags.csv";

        File file = new File(path);
        File parent = file.getParentFile();

        if (!parent.exists()) {
            boolean status = parent.mkdirs();

            if (!status)
                throw new RuntimeException("store unregistered tags failed");
        }

        try {
            PrintWriter writer = new PrintWriter(file);
            writer.print(toCSV());
            writer.close();
        }
        catch (Exception e) {
            throw new RuntimeException("store unregistered tags failed");
        }
    }
}
