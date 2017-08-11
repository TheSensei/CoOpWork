package model;

import java.util.Observable;
    
/**
 *
 * @author agula and mark
 */
public class TagData extends Observable {
    private String name = "";
    private boolean enabled = true;
    private TagClass tagClass = TagClass.Asset;
    private SecurityClass securityClass = SecurityClass.Low;
    private String note = "";
    private String section = "";
    private String firm = "";
    private String cellNumber = "";
    private boolean alarmsR1A0 = false;
    private boolean alarmsR1A1 = false;
    private boolean alarmsR1A2 = false;
    private boolean alarmsR1A3 = false;
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
        setChanged();
        notifyObservers();
    }

    public boolean getEnabled() {
        return enabled;
    }
    
    public void setEnabled(boolean state) {
        enabled = state;
        setChanged();
        notifyObservers();
    }

    public TagClass getTagClass() {
        return tagClass;
    }
    
    public void setTagClass(TagClass tagClass) {
        this.tagClass = tagClass;
        setChanged();
        notifyObservers();
    }

    public SecurityClass getSecurityClass() {
        return securityClass;
    }
    
    public void setSecurityClass(SecurityClass securityClass) {
        this.securityClass = securityClass;
        setChanged();
        notifyObservers();
    }

    public String getNote() {
        return note;
    }
    
    public void setNote(String note) {
        this.note = note;
        setChanged();
        notifyObservers();
    }

    public String getSection() {
        return section;
    }
    
    public void setSection(String section) {
        this.section = section;
        setChanged();
        notifyObservers();
    }

    public String getFirm() {
        return firm;
    }
    
    public void setFirm(String firm) {
        this.firm = firm;
        setChanged();
        notifyObservers();
    }

    public String getCellNumber() {
        return cellNumber;
    }
    
    public void setCellNumber(String cellNumber) {
        this.cellNumber = cellNumber;
        setChanged();
        notifyObservers();
    }

    public boolean getAlarmsR1A0() {
        return alarmsR1A0;
    }

    public void setAlarmsR1A0(boolean state) {
        alarmsR1A0 = state;
        setChanged();
        notifyObservers();
    }

    public boolean getAlarmsR1A1() {
        return alarmsR1A1;
    }

    public void setAlarmsR1A1(boolean state) {
        alarmsR1A1 = state;
        setChanged();
        notifyObservers();
    }

    public boolean getAlarmsR1A2() {
        return alarmsR1A2;
    }

    public void setAlarmsR1A2(boolean state) {
        alarmsR1A2 = state;
        setChanged();
        notifyObservers();
    }

    public boolean getAlarmsR1A3() {
        return alarmsR1A3;
    }

    public void setAlarmsR1A3(boolean state) {
        alarmsR1A3 = state;
        setChanged();
        notifyObservers();
    }
}
