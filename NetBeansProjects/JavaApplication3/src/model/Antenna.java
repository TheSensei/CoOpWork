/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package model;

import java.util.Observable;

/**
 *
 * @author agula
 */
public class Antenna extends Observable {
    private AntennaType type = AntennaType.AlienPatch;
    private boolean powered_on = false;
    private int power = 239; // tenths of a dBm, i.e. 239 = 23.9 dBm = 245 mW
    private int occupancyThreshold = 100;
    private int timeout = 30;

    public int getPower() {
        return power;
    }

    public void setPower(int power) {
        this.power = power;
        setChanged();
        notifyObservers();
    }

    public AntennaPower getDiscretePower() {
        if (power <= 239)
            return AntennaPower.Power250MilliWatt;
        else
            return AntennaPower.Power1Watt;
    }

    public void setDiscretePower(AntennaPower power) {
        switch (power) {
            case Power250MilliWatt:
                this.power = 239;
                break;
            case Power1Watt:
                this.power = 315;
                break;
        }

        setChanged();
        notifyObservers();
    }
    
    public void turnOn() {
        this.powered_on = true;
        setChanged();
        notifyObservers();
    }
    
    public void turnOff() {
        this.powered_on = false;
        setChanged();
        notifyObservers();
    }

    public void setPoweredOn(boolean state) {
        powered_on = state;
        setChanged();
        notifyObservers();
    }
    
    public boolean isPoweredOn() {
        return this.powered_on;
    }
    
    public void setType(AntennaType type) {
        this.type = type;
        setChanged();
        notifyObservers();
    }
    
    public AntennaType getType() {
        return type;
    }
    
    public void setOccupancyThreshold(int threshold) {
        occupancyThreshold = threshold;
        setChanged();
        notifyObservers();
    }
    
    public int getOccupancyThreshold() {
        return occupancyThreshold;
    }
    
    public void setTimeout(int newTimeout) {
        this.timeout = newTimeout;
        setChanged();
        notifyObservers();
    }
    
    public int getTimeout() {
        return this.timeout;
    }
}
