package model;

import java.util.Properties;
// need 1 or 2 of these 3, or maybe none
import java.util.Hashtable;
import java.util.Enumeration;
import java.util.ArrayList;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

import java.util.Observable;
import java.util.Observer;

import model.DB;
import model.TagData;

public class TagDataDB extends DB implements Observer {
    private Hashtable<TagData, String> watchingTags;

    public TagDataDB() {
        watchingTags = new Hashtable<TagData, String>();
    }

    public void update(Observable o, Object arg) {
        if (o.getClass().equals(TagData.class)) {
            TagData tag = (TagData) o;

            String id = watchingTags.get(tag);

            if (id != null) {
                storeTag(tag, id);
            }
        }
    }

    // note: implicitly unwatches an already watched tag for this id
    public void watchTag(TagData tag, String id) {
        Enumeration<TagData> enumeration = watchingTags.keys();

        // if we're already watching a tag for this id, unwatch it
        while (enumeration.hasMoreElements()) {
            TagData key = enumeration.nextElement();
            String value = watchingTags.get(key);

            if (value.equals(id))
                unwatchTag(key);
        }

        watchingTags.put(tag, id);
        tag.addObserver(this);
    }

    public void unwatchTag(TagData tag) {
        watchingTags.remove(tag);
        tag.deleteObserver(this);
    }

    public String[] getTagIds() {
        String path = basePath + "/tags";

        File[] files = new File(path).listFiles();

        String[] names = new String[files.length];

        for (int i = 0; i < files.length; i++)
            names[i] = files[i].getName();

        return names;
    }

    // note: implicitly watches the tag
    public TagData loadTag(String id) {
        return loadTag(id, true);
    }

    public TagData loadTagReadOnly(String id) {
        return loadTag(id, false);
    }

    private TagData loadTag(String id, boolean implicitlyWatch) {
        String path = basePath + "/tags/" + id;

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
            throw new RuntimeException("load tag failed");
        }

        TagData tag = new TagData();

        if (properties.get("name") == null
                || properties.get("enabled") == null
                || properties.get("tag_class") == null
                || properties.get("security_class") == null
                || properties.get("note") == null
                || properties.get("section") == null
                || properties.get("firm") == null
                || properties.get("cell_number") == null
                || properties.get("alarms_r1_a0") == null
                || properties.get("alarms_r1_a1") == null
                || properties.get("alarms_r1_a2") == null
                || properties.get("alarms_r1_a3") == null)
            throw new RuntimeException("tag config missing field");

        try {
            String name = properties.get("name").toString();
            boolean enabled = new Boolean(properties.get("enabled").toString());
            TagClass tagClass = TagClass.valueOf(properties.get("tag_class").toString());
            SecurityClass securityClass = SecurityClass.valueOf(properties.get("security_class").toString());
            String note = properties.get("note").toString();
            String section = properties.get("section").toString();
            String firm = properties.get("firm").toString();
            String cellNumber = properties.get("cell_number").toString();
            boolean alarmsR1A0 = new Boolean(properties.get("alarms_r1_a0").toString());
            boolean alarmsR1A1 = new Boolean(properties.get("alarms_r1_a1").toString());
            boolean alarmsR1A2 = new Boolean(properties.get("alarms_r1_a2").toString());
            boolean alarmsR1A3 = new Boolean(properties.get("alarms_r1_a3").toString());

            tag.setName(name);
            tag.setEnabled(enabled);
            tag.setTagClass(tagClass);
            tag.setSecurityClass(securityClass);
            tag.setNote(note);
            tag.setSection(section);
            tag.setFirm(firm);
            tag.setCellNumber(cellNumber);
            tag.setAlarmsR1A0(alarmsR1A0);
            tag.setAlarmsR1A1(alarmsR1A1);
            tag.setAlarmsR1A2(alarmsR1A2);
            tag.setAlarmsR1A3(alarmsR1A3);
        }
        catch (Exception e) {
            throw new RuntimeException("tag config invalid value");
        }

        if (implicitlyWatch)
            watchTag(tag, id);

        return tag;
    }

    public void storeTag(TagData tag, String id) {
        String path = basePath + "/tags/" + id;

        File parent = new File(path).getParentFile();

        if (!parent.exists()) {
            boolean status = parent.mkdirs();

            if (!status)
                throw new RuntimeException("store tag failed");
        }

        Properties properties = new Properties();

        properties.put("name", tag.getName());
        properties.put("enabled", Boolean.toString(tag.getEnabled()));
        properties.put("tag_class", tag.getTagClass().toString());
        properties.put("security_class", tag.getSecurityClass().toString());
        properties.put("note", tag.getNote());
        properties.put("section", tag.getSection());
        properties.put("firm", tag.getFirm());
        properties.put("cell_number", tag.getCellNumber());
        properties.put("alarms_r1_a0", Boolean.toString(tag.getAlarmsR1A0()));
        properties.put("alarms_r1_a1", Boolean.toString(tag.getAlarmsR1A1()));
        properties.put("alarms_r1_a2", Boolean.toString(tag.getAlarmsR1A2()));
        properties.put("alarms_r1_a3", Boolean.toString(tag.getAlarmsR1A3()));

        try {
            FileOutputStream stream = new FileOutputStream(new File(path));
            properties.store(stream, "Tag Data Configuration");
            stream.close();
        }
        catch (Exception e) {
            throw new RuntimeException("store tag failed");
        }
    }
}

