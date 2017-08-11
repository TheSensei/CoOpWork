
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// TO DO: maybe investigate Tag setup formatted text fields (e.g., spaces and tab problems)  >>> not required per CG
// TO DO: make alert cell red on alert
// TO DO: maybe change entry time format to be smaller
// TO DO: consider displaying RRSI in % column when x-read off
// TO DO: maybe Occupancy should only count personnel tags, not asset or Env  >>> CG didn't indicate this was required when questioned, so leave as is
// TO DO - consider making Tag Accept button inactive until verify both name and TID valid and user doesn't change them to invalid
// TO DO - test more loc resolve on/off - saw table stay empty once when turned it off
// TO DO - reader class should fetch state from the reader (not required for functionality)
// TO DO - reader tag processor should stop reading if not contacted in some time
// TO DO - on close, stop reading tags
// TO DO - get Charlie's opinion on the following:
// - No error window if reader not detected, or can't reconnect   >>> Per CG, no change required
// - Evn tags also displayed in table  >>> per CG, no change required
// - qwirks with some data entry fields (formatted text fields auto add spaces) and some display fields (if load env fields with empty string, can't change, so using 0 as init value) >>> Per CG, could change env field to "---" when no data
// - low RSSI filter not added, should it be?  >>> Per CG, will do formal testing first
// - in-house test plan  >>> Per CG, see SOW and create power-point test plan

package controller;

import java.util.Observable;		//for update();
import java.util.ArrayList;

import javax.swing.Timer;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

import glue.AzbilGuiApplication;
import model.ALRF800RFIDReader;

import model.TagData;
import model.TagRead;
import model.RFIDReader;
import model.Antenna;
import model.AntennaDB;
import model.AppConfig;
import model.AppConfigDB;
import model.Occupancy;
import model.OccupancyDB;
import model.UnregisteredTagDB;
import model.TagDataDB;
import model.TagClass;
import model.SecurityClass;

import view.AntOccupancyState;
import view.ReaderReadRates;

import view.AntSelectV2UI;
import view.AntDataUI;
import view.SetupSelectUI;
import view.AntSetupUI;
import view.TagSetupUI;
import view.BusyUI;
import view.WarningUI;
import view.ErrorUI;


/**
 *
 * @author mark
 */
//public class Controller {
public class Controller implements java.util.Observer {

        private ArrayList<TagData> regTags = new ArrayList<TagData>();  // registered tag list
        
	RFIDReader reader;
        byte readerNo = 1;
        int antTimeout = 0;
	
        AzbilGuiApplication view;
        AntSelectV2UI viewAntSel;
        AntDataUI viewAntData;
        SetupSelectUI viewSetupSel;
        AntSetupUI viewAntSetup;
        TagSetupUI viewTagSetup;
        BusyUI viewBusy;
        WarningUI viewWarning;
        ErrorUI viewError;
        
        AppConfigDB appConfigDB;
        AppConfig appConfig;
        AntennaDB antennaDB;
        TagDataDB tagDataDB;
        OccupancyDB occupancyDB;
        UnregisteredTagDB unregisteredTagDB;

        Timer timer;

	public Controller() {	
		//System.out.println ("Controller()");
                //regTags=modelDB.getRegTags();  // copy registered tags from database/DB model
	} //Controller()
        
    
        public int startController() {
            int retVal = 0; // -1 = error

            appConfigDB = new AppConfigDB();
            appConfig = appConfigDB.loadAppConfig();
            antennaDB = new AntennaDB();
            tagDataDB = new TagDataDB();
            unregisteredTagDB = new UnregisteredTagDB();
        
            this.initAntSelectScreen(readerNo);  // initialize screen
            viewAntSel.setVisible(true);  // display 1st, main screen
            
            initAndDisplayBusyScreen("Configuring ext. RFID Reader");  // display busy pop-up
            
            // Start Reader
            reader.addAntennas(antennaDB.loadAntennas(1));
            reader.setEnvironmentalIds(getEnvironmentalIdsFromDB());

            reader.setReadInterval(appConfig.getReaderReadInterval());
            reader.setXReadEnabled(appConfig.getXReadEnabled());

            occupancyDB = new OccupancyDB(reader.getAntennas());
            occupancyDB.setByAntenna(!appConfig.getXReadEnabled());

            startScreenRefreshTimer(1000 * appConfig.getScreenRefreshInterval());

            removeBusyScreen();

            reader.startReadingTags();

            // Wait for:
            // (1) button clicks [via guiActionPerformed() calls]
            // (2) Screen Refresh timeouts [via toBeDefined() calls]
            // (3) Busy/Warning/Errors from Reader Model [via update() calls]
            return (retVal);
        }

        // This Controller is an Observer.
        // The Model/RFIDReader is an Observable, when Model calls these two functions this Controller update() fnc is called:
        // (1) setChanged()
        // (2) noticeObserver()
    	public void update(Observable obs, Object obj) {

		//who called us and what did they send?
                // reader not passing object, so next line if uncommented gets a NULL pointer exception - could change reader to always pass an object like a fatal error object, would then need to change Controller to decode it
		//System.out.println ("Controller's update fnc : Observable is " + obs.getClass() + ", object passed is " + obj.getClass());

		//myTextField.setText("" + ((Integer)obj).intValue());	//obj is an Object, need to cast to an Integer

    	} //update()
        
        public void initAndDisplayBusyScreen(String myStg) {
            viewBusy.setBusyText(myStg);
            viewBusy.setVisible(true);
        }
        
        public void removeBusyScreen() {
            viewBusy.clearBusyText();
            viewBusy.setVisible(false);
        }

        public void initAndDisplayWarningScreen(String myStg) {
            viewWarning.setWarningText(myStg);
            viewWarning.setVisible(true);
        }
        
        // Called when user click on "Close" button on Warning pop-up
        public void removeWarningScreen() {
            viewWarning.clearWarningText();
            viewWarning.setVisible(false);
        }
        
        public void initAndDisplayFatalErrorScreen(String myStg) {
            viewError.setFatalErrorText(myStg);
            viewError.setVisible(true);
        }
        
                
        public byte guiTagNameEntered(java.awt.event.ActionEvent e, String tidString){
                // tid may be an empty string, if user didn't enter it yet, or system didn't auto populate it
                byte retVal = 0;  // -1 = error
                tagNameEntered(e.getActionCommand() /* name String entered*/, tidString);
                return (retVal);
        }
        
        public byte guiTagTidEntered(java.awt.event.ActionEvent e, String nameString){
                // name may be an empty string, if user didn't enter it yet, or system didn't auto populate it
                byte retVal = 0;  // -1 = error
                tagTidEntered(e.getActionCommand() /* name String entered*/, nameString);
                return (retVal);
        }        
                
	//invoked when a button is pressed
	public byte guiActionPerformed(java.awt.event.ActionEvent e){
                byte retVal = 0;  // -1 = error value
                int antNo = 0;
                
                switch (e.getActionCommand()) {

                    // ANTENNA SELECTION SCREEN BUTTON EVENTS:
                    
                    case "ButtonZone0":
                        initAntDataScreen(0, true /*make visible too*/);
                        updateAntDataWindow();
                        break;
                        
                    case "ButtonZone1":
                        initAntDataScreen(1, true /*make visible too*/);
                        updateAntDataWindow();
                        break;
                        
                    case "ButtonZone2":
                        initAntDataScreen(2, true /*make visible too*/);
                        updateAntDataWindow();
                        break;
                        
                    case "ButtonZone3":
                        initAntDataScreen(3, true /*make visible too*/);
                        updateAntDataWindow();
                        break;
                        
                    // ANTENNA DATA SCREEN BUTTON EVENTS:
                        
                    case "ButtonLocAlgoToggle":
                        newCrossReadAlgoState();
                        break;
                        
                    case "ButtonReset":
                        performReset();
                        break;
                        
                    case "ButtonCreateCsvFiles":
                        createCsvTagFiles();
                        break;

                    case "ButtonAntPowerToggle":
                        toggleAntPower();
                        break;
                        
                    case "ButtonSetup":
                        initSetupSelScreen();
                        viewSetupSel.setVisible(true);
                        break;
                        
                    case "ButtonAntDataBack":
                        viewAntData.setVisible(false);
                        break;
                                                
                    // SETUP SELECT SCREEN BUTTON/TEXT EVENTS:
                        
                    case "ButtonTagSetup":
                        initTagSetupScreen();
                        viewTagSetup.setVisible(true);
                        break;
                        
                    case "ButtonAntSetup":
                        antNo = viewAntData.getAntNo();  // use Antenna Number that was on AntData screen before setup select screen displayed                      
                        initAntSetupScreen(antNo);
                        viewAntSetup.setVisible(true);
                        break;
                        
                    case "ButtonSetupSelBack":
                        viewSetupSel.setVisible(false);
                        break;
                        
                    case "screenRefreshComboBoxChanged":
                        newScreenRefreshRate();
                        break;

                    case "readerReadRateComboBoxChanged":
                        newReaderReadRate();
                        initAntDataScreen(viewAntData.getAntNo(), false);
                        updateAntDataWindow();
                        break;
                                                
                        
                    // SETUP ANTENNA SCREEN BUTTON/TEXT EVENTS:
                    
                    case "AntNocomboBoxChanged":
                        antNo = viewAntSetup.getAntNo();  // init screen with AntNo user just selected from combo box                    
                        initAntSetupScreen(antNo);  // re-initialize AntSetup screen when user changes the AntNo
                        break;
                        
                    case "ButtonAntSetupBack":
                        viewAntSetup.setVisible(false);
                        break;                        
                        
                    case "ButtonAntSetupAccept":
                        newAntConfig();
                        break;
                                                      
                    // SETUP TAG SCREEN BUTTON EVENTS:
                        
                    case "ButtonTagEnable":
                        toggleTagEnable();
                        break;
                        
                    case "ButtonTagSetupBack":
                        viewTagSetup.setVisible(false);
                        break;
                        
                    case "ButtonTagSetupAccept":
                        newTagConfig();
                        break;
                        
                    // WARNING SCREEN BUTTON/TEXT EVENTS:
                    case "ButtonCloseWarning":
                        removeWarningScreen();
                        break;
                        
                    default:
                        retVal = -1;
                        break;
                }
               
		//uncomment to see what action happened at view
                
		//System.out.println ("Controller: The " + e.getActionCommand() 
		//	+ " button is clicked at " + new java.util.Date(e.getWhen())
		//	+ " with e.paramString " + e.paramString() );
                
		//System.out.println("Controller: acting on Model");
		//model.incrementValue();
                return (retVal);
	} //guiActionPerformed()

    public void closePerformed() {
        reader.stopReadingTags();
        System.exit(0);
    }

        
        public void initAntSelectScreen (int readerNo) {
            
            // These fields will update when process 1st read list if it contains tags
            viewAntSel.setTempInC_RxA0(-1);  // negative value sets to empty string
            viewAntSel.setTempInC_RxA1(-1);
            viewAntSel.setTempInC_RxA2(-1);
            viewAntSel.setTempInC_RxA3(-1);
            
            viewAntSel.setHumiInRh_RxA0(-1);
            viewAntSel.setHumiInRh_RxA1(-1);
            viewAntSel.setHumiInRh_RxA2(-1);
            viewAntSel.setHumiInRh_RxA3(-1);
            
            viewAntSel.setLightInLux_RxA0(-1);
            viewAntSel.setLightInLux_RxA1(-1);
            viewAntSel.setLightInLux_RxA2(-1);
            viewAntSel.setLightInLux_RxA3(-1);
            
            viewAntSel.setAntOccupancyState_RxA0(AntOccupancyState.Zero_Tags);
            viewAntSel.setAntOccupancyState_RxA1(AntOccupancyState.Zero_Tags);
            viewAntSel.setAntOccupancyState_RxA2(AntOccupancyState.Zero_Tags);
            viewAntSel.setAntOccupancyState_RxA3(AntOccupancyState.Zero_Tags);
            
            viewAntSel.clearAntSelectTableContents();
        }
        
        
        private void initAntDataScreen (int antNo, boolean bMakeVisible) {
            int readerNo = 1;
            ReaderReadRates enumRate;
            
            // Set current date and time
            viewAntData.setDate();
            viewAntData.setTime();
            
            // Set these fields with antenna number passed into this fnc
            viewAntData.setAntNo(antNo);
            viewAntData.setZoneNo(antNo);  // for phase I antNo = zoneNo
            
            // These fields will update when read list contains appr. env tag data
            viewAntData.setTempInC(0);  // negative value sets to empty string
            viewAntData.setHumiInRh(0);
            viewAntData.setLightInLux(0);
            viewAntData.setAntOccupancyState(AntOccupancyState.Zero_Tags);

            // Load values from App Config File:
            int rate = appConfig.getReaderReadInterval();
            if (rate <=250) enumRate = ReaderReadRates.readerRate250ms;
            else if (rate <=500) enumRate = ReaderReadRates.readerRate500ms;
            else enumRate = ReaderReadRates.readerRate1sec;
            viewAntData.setReadRate(enumRate);
            viewAntData.setLocAlgoOnState(reader.getXReadEnabled());
            viewAntData.clearAntDataTableContents();  // clear incase displaying another antenna's data            
            
            Antenna antenna = reader.getAntenna(antNo);
            if (antenna == null) {
                // Display an error window
                initAndDisplayFatalErrorScreen("in call to reader.getAntenna()");
                return;  // don't update config data and don't make visible even if boolean set
            }
            
            viewAntData.setTheshold(antenna.getOccupancyThreshold());
            viewAntData.setAntPwrOnState(antenna.isPoweredOn());
            antTimeout = antenna.getTimeout();

            if (bMakeVisible)
                viewAntData.setVisible(true);
        }

        private void initSetupSelScreen() {
            viewSetupSel.setScreenRefreshRateInSecs(appConfig.getScreenRefreshInterval());

            int intReadInterval = appConfig.getReaderReadInterval();
            ReaderReadRates readInterval = intToReaderReadRates(intReadInterval);

            viewSetupSel.setReaderReadRate(readInterval);
        }

        private void startScreenRefreshTimer(int interval) {
            if (timer != null && timer.isRunning())
                timer.stop();

            ActionListener taskPerformer = new ActionListener() {
                public void actionPerformed(ActionEvent event) {
                    refreshScreen();
                }
            };

            timer = new Timer(interval, taskPerformer);
            timer.start();
        }

        private void stopScreenRefreshTimer() {
            if (timer != null && timer.isRunning())
                timer.stop();
        }

        private String[] getEnvironmentalIdsFromDB() {
            String[] ids = tagDataDB.getTagIds();


            ArrayList<String> list = new ArrayList<String>();

            if (ids != null) {
                for (int i = 0; i < ids.length; i++) {
                    TagData tag = tagDataDB.loadTagReadOnly(ids[i]);

                    if (tag.getTagClass() == TagClass.Environmental)
                        list.add(ids[i]);
                }
            }

            if (list.size() == 0)
                return null;
            else
                return list.toArray(new String[0]);
        }

        private void refreshScreen() {
            processNewTagReads();
            occupancyDB.timeoutOccupancies();
            updateAntSelWindow();
            updateAntDataWindow();
        }

        private void processNewTagReads() {
            TagRead[] reads = reader.getNewTagReads();

            if (reads == null)
                return;

            for (int i = 0; i < reads.length; i++) {
                TagData tag = tagDataDB.loadTagReadOnly(reads[i].getId());

                if (tag != null && tag.getEnabled())
                    occupancyDB.updateForRead(1, reads[i]);

                if (tag == null)
                    unregisteredTagDB.addId(reads[i].getId());
                else
                    unregisteredTagDB.removeId(reads[i].getId());
            }
        }

        private void updateAntSelWindow() {
            updateAntSelTable();
            updateAntSelEnvironmentData();
        }

        private void updateAntDataWindow() {
            viewAntData.setDate();
            viewAntData.setTime();
            updateAntDataTable();
            updateAntDataEnvironmentData();
        }

        private void updateAntSelEnvironmentData() {
            String[] ids = occupancyDB.getOccupancyIds();

            for (int antenna = 0; antenna < 4; antenna++) {
                double temperature = 0.0;
                int humidity = 0;
                int light = 0;

                int temperatureCount = 0, humidityCount = 0, lightCount = 0;

                if (ids != null) {
                    for (int i = 0; i < ids.length; i++) {
                        Occupancy occupancy = occupancyDB.getOccupancy(ids[i]);

                        if (occupancy.getAntenna() != antenna)
                            continue;

                        if (occupancy.hasTemperature()) {
                            temperature += occupancy.getTemperature();
                            temperatureCount++;
                        }
                        if (occupancy.hasHumidity()) {
                            humidity += occupancy.getHumidity();
                            humidityCount++;
                        }
                        if (occupancy.hasLight()) {
                            light += occupancy.getLight();
                            lightCount++;
                        }
                    }
                }

                if (temperatureCount != 0) {
                    temperature /= temperatureCount;
                    
                    // Convert F to C.
                    if (temperature != 0.0)
                        temperature = 5.0 / 9.0 * (temperature - 32.0);
                }
                else 
                    temperature = -1;  // negative value will load a space character into field

                switch (antenna) {
                    case 0:
                        viewAntSel.setTempInC_RxA0((int) temperature);
                        break;
                    case 1:
                        viewAntSel.setTempInC_RxA1((int) temperature);
                        break;
                    case 2:
                        viewAntSel.setTempInC_RxA2((int) temperature);
                        break;
                    case 3:
                        viewAntSel.setTempInC_RxA3((int) temperature);
                        break;
                }

                if (humidityCount != 0)
                    humidity /= humidityCount;
                else
                    humidity = -1; // negative value will load a space character into field

                switch (antenna) {
                    case 0:
                        viewAntSel.setHumiInRh_RxA0((int) humidity);
                        break;
                    case 1:
                        viewAntSel.setHumiInRh_RxA1((int) humidity);
                        break;
                    case 2:
                        viewAntSel.setHumiInRh_RxA2((int) humidity);
                        break;
                    case 3:
                        viewAntSel.setHumiInRh_RxA3((int) humidity);
                        break;
                }

                if (lightCount != 0)
                    light /= lightCount;
                else
                    light = -1;  // negative value will load a space character into field
                
                switch (antenna) {
                    case 0:
                        viewAntSel.setLightInLux_RxA0((int) light);
                        break;
                    case 1:
                        viewAntSel.setLightInLux_RxA1((int) light);
                        break;
                    case 2:
                        viewAntSel.setLightInLux_RxA2((int) light);
                        break;
                    case 3:
                        viewAntSel.setLightInLux_RxA3((int) light);
                        break;
                }
            }
        }

        private void updateAntDataEnvironmentData() {
            String[] ids = occupancyDB.getOccupancyIds();

            int antenna = viewAntData.getAntNo();

            double temperature = 0.0;
            int humidity = 0;
            int light = 0;

            int temperatureCount = 0, humidityCount = 0, lightCount = 0;

            if (ids != null) {
                for (int i = 0; i < ids.length; i++) {
                    Occupancy occupancy = occupancyDB.getOccupancy(ids[i]);

                    if (occupancy.getAntenna() != antenna)
                        continue;

                    if (occupancy.hasTemperature()) {
                        temperature += occupancy.getTemperature();
                        temperatureCount++;
                    }
                    if (occupancy.hasHumidity()) {
                        humidity += occupancy.getHumidity();
                        humidityCount++;
                    }
                    if (occupancy.hasLight()) {
                        light += occupancy.getLight();
                        lightCount++;
                    }
                }
            }

            if (temperatureCount != 0.0) {
                temperature /= temperatureCount;
                // Convert F to C.
                if (temperature != 0.0)
                    temperature = 5.0 / 9.0 * (temperature - 32.0);
            }
            else
                temperature = -1; // negative value will load a space character into field
            
            viewAntData.setTempInC((int) temperature);
            
            if (humidityCount != 0) 
                humidity /= humidityCount;
            else
                humidity = -1;  // negative value will load a space character into field
            
            viewAntData.setHumiInRh(humidity);            
            
            if (lightCount != 0)
                light /= lightCount;
            else light = -1;  // negative value will load a space character into field

            viewAntData.setLightInLux(light);
        }

        private void updateAntSelTable() {
            String[] ids = occupancyDB.getOccupancyIds();

            Object[][] table = new Object[100][5];
            int row = 0;
            int[] antennaOccupancyCounts = new int[4];

            if (ids != null) {
                for (int i = 0; i < ids.length; i++) {
                    Occupancy occupancy = occupancyDB.getOccupancy(ids[i]);

                    table[row][0] = occupancy.getName();
                    table[row][1] = Integer.toString(occupancy.getReader());
                    table[row][2] = Integer.toString(occupancy.getAntenna());
                    table[row][3] = Integer.toString(occupancy.getConfidence());
                    table[row][4] = Boolean.toString(occupancy.hasIntentToStay());

                    row++;

                    if (occupancy.hasIntentToStay())
                        antennaOccupancyCounts[occupancy.getAntenna()]++;
                }
            }

            viewAntSel.setTable(table);

            for (int antNo = 0; antNo < 4; antNo++) {
                AntOccupancyState occupancyState;

                if (antennaOccupancyCounts[antNo] == 0)
                    occupancyState = AntOccupancyState.Zero_Tags;
                else if (antennaOccupancyCounts[antNo] < reader.getAntenna(antNo).getOccupancyThreshold())
                    occupancyState = AntOccupancyState.Under_Threshold;
                else
                    occupancyState = AntOccupancyState.Over_Threshold;

                switch (antNo) {
                    case 0:
                        viewAntSel.setAntOccupancyState_RxA0(occupancyState);
                        break;
                    case 1:
                        viewAntSel.setAntOccupancyState_RxA1(occupancyState);
                        break;
                    case 2:
                        viewAntSel.setAntOccupancyState_RxA2(occupancyState);
                        break;
                    case 3:
                        viewAntSel.setAntOccupancyState_RxA3(occupancyState);
                        break;
                }
            }
        }

        private void updateAntDataTable() {
            String[] ids = occupancyDB.getOccupancyIds();

            Object[][] table = new Object[100][9];
            int rowsWithIntentToStay = 0;

            if (ids != null) {
                int row = 0;

                for (int i = 0; i < ids.length; i++) {
                    Occupancy occupancy = occupancyDB.getOccupancy(ids[i]);

                    // Filter for this antenna.
                    if (occupancy.getAntenna() != viewAntData.getAntNo())
                        continue;

                    Object[] array = new Object[9];

                    table[row][0] = ids[i].split("@")[0];
                    table[row][1] = occupancy.getName();
                    table[row][2] = Integer.toString(occupancy.getConfidence());
                    table[row][3] = occupancy.getEntryDateString();
                    table[row][4] = occupancy.getStayTimeString();
                    table[row][5] = occupancy.getStaleTimeString();
                    table[row][6] = occupancy.getRemainingTimeString();
                    table[row][7] = Boolean.toString(occupancy.hasIntentToStay());
                    table[row][8] = Boolean.toString(occupancy.getAlarmTriggered());

                    row++;

                    if (occupancy.hasIntentToStay())
                        rowsWithIntentToStay++;
                }
            }

            viewAntData.setTable(table);

            AntOccupancyState occupancyState;
            int antNo = viewAntData.getAntNo();

            if (rowsWithIntentToStay == 0)
                occupancyState = AntOccupancyState.Zero_Tags;
            else if (rowsWithIntentToStay < reader.getAntenna(antNo).getOccupancyThreshold())
                occupancyState = AntOccupancyState.Under_Threshold;
            else
                occupancyState = AntOccupancyState.Over_Threshold;

            viewAntData.setAntOccupancyState(occupancyState);
        }

        private void newScreenRefreshRate() {
            // Save App config data & change timer
            int interval = viewSetupSel.getScreenRefreshRateInSecs();
            appConfig.setScreenRefreshInterval(interval);
            startScreenRefreshTimer(1000 * interval);
        }
        
        private void newReaderReadRate() {
            // Save system config data and wait for Reader to reconfigure ext reader
            // Update Reader Read Rate text on AntData screen
            ReaderReadRates myEnum = viewSetupSel.getReaderReadRate();
            int myInt = 0;
            
            if (myEnum == ReaderReadRates.readerRate250ms) myInt = 250;
            else if (myEnum == ReaderReadRates.readerRate500ms) myInt = 500;
            else myInt = 1000;  // ReaderReadRates.readerRate1sec;
            
            reader.setReadInterval(myInt);
            appConfig.setReaderReadInterval(myInt);
        }        
        
        private void newCrossReadAlgoState() {
            boolean oldScreenState = viewAntData.getLocAlgoOnState();
            boolean newState = (oldScreenState == true) ? false : true;             
            if (newState != reader.getXReadEnabled()) {
                // Save system config data change
                if (newState == true) {
                    reader.setXReadEnabled(true);
                    occupancyDB.setByAntenna(false);
                    appConfig.setXReadEnabled(true);
                    // FYI, new Reader Tag Lists will now start to re-populate the "Antenna Confidence %" row with new percentages
                }
                else {
                    // Ext Reader doesn't need to perform Cross-Read Algo
                    reader.setXReadEnabled(false);
                    occupancyDB.setByAntenna(true);
                    appConfig.setXReadEnabled(false);
                }
                
                // Update text on screen to match new state
                viewAntData.setLocAlgoOnState(newState);
                
                // Optionally, display Busy screen until Model indicates operation completed and verified             
            }
            // else user picked same value, or maybe DB and screen were out of sync
        }
        
        private void performReset() {
            // Tell Model to reconfigure the external RFID reader
            reader.reset();

            occupancyDB.clear();
            
            // Clear tag data fields on screen too
            this.initAntSelectScreen(readerNo);
            viewAntData.clearAntDataTableContents();
            
            // Initialize AntData screen with same Antenna number, if valid
            int antNo = viewAntData.getAntNo();
            if ((antNo < 0) || (antNo > viewAntData.max_antNo))
                    antNo = 0;
            initAntDataScreen(antNo, false /* already visible*/);
            
            // Optionally, display Busy screen until model indicates operation completed and verified            
        }
        
        private void createCsvTagFiles() {
            // Call model methods to write reg & unreg Controller Tag Objects as CSV files
            occupancyDB.storeCSVFile();
            unregisteredTagDB.storeCSVFile();
            // Optionally, display Busy screen until Model indicates operation completed and verified
        }
        
        private void toggleAntPower() {
            // Get user entered state from screen
            boolean oldScreenState = viewAntData.getAntPwrOnState();
            boolean newState = (oldScreenState == true) ? false : true;
            
            // Get and validate antenna number of screen
            int antNo = viewAntData.getAntNo();
            if ((antNo < 0) || (antNo > viewAntData.max_antNo)) {
                    // if Screen has bad data, correct it
                    antNo = 0;
                    initAntDataScreen(antNo, false /* already visible*/);
            }
            
            Antenna antenna = reader.getAntenna(antNo);

            if (antenna == null) {
                // Display an error window
                initAndDisplayFatalErrorScreen("in call to reader.getAntenna()");
                return;  // don't update config data and don't make visible even if boolean set
            }
                
            // If file state changed, update DB
            if (newState != antenna.isPoweredOn()) {
                 if (newState == true) {
                     initAndDisplayWarningScreen("<html>Make sure antenna cable<br>" + "is attached to port " + antNo + " to prevent<br>" + "damaging ext. reader h/w<html>");
                     antenna.turnOn();
                 }
                 else
                     antenna.turnOff();
                 
                // Update text on screen to match new state
                viewAntData.setAntPwrOnState(newState);
                
                 // Optionally, display Busy screen until Model indicates operation completed and verified
            }
            // else user selected same value, or maybe DB and screen were out of sync
        }
        
        private void initAntSetupScreen(int antNo) {
            if ((antNo < 0) || (antNo > viewAntData.max_antNo))
                    antNo = 0;            
            
            // Load values from Antenna Config file for above antNo
            Antenna antenna = reader.getAntenna(antNo);
            if (antenna == null) {
                // Display an error window
                initAndDisplayFatalErrorScreen("in call to reader.getAntenna()");
                return;  // don't update config data and don't make visible even if boolean set
            }
            
            // Copy current config data for this antenna number, if available, to appr. Ant Setup screen fields
            viewAntSetup.setAntNo(antNo);
            viewAntData.setAntPwrOnState(antenna.isPoweredOn());
            viewAntSetup.setAntType(antenna.getType());
            viewAntSetup.setAntPower(antenna.getDiscretePower());
            viewAntSetup.setAntThreshold(antenna.getOccupancyThreshold());
            viewAntSetup.setAntTagTimeout(antenna.getTimeout());
            
            viewAntSetup.set_status_label(" ");

            viewAntSetup.setVisible(true);
        }
        
        private void newAntConfig() {
            // Screen text fields updated by user don't have to be validated because
            // screen uses combo and spin boxes with only valid options.
            
            // Get antenna number from GUI screen
            int antNo = viewAntSetup.getAntNo();
            if ((antNo < 0) || (antNo > viewAntData.max_antNo)) {  // sanity check
                    antNo = 0;
                    viewAntSetup.setAntNo(antNo);
                    return;  // change antNo on GUI screen and let user set fields again and hit "Accept" button
            }
            
            Antenna antenna = reader.getAntenna(antNo);
            if (antenna == null) {
                // Display an error window
                initAndDisplayFatalErrorScreen("in call to reader.getAntenna()");
                return;  // don't update config data and don't make visible even if boolean set
            }
                        
            // Set AntConfig DB fields using GUI screen's current user selected values
            antenna.setType(viewAntSetup.getAntType());
            antenna.setDiscretePower(viewAntSetup.getAntPower());
            antenna.setOccupancyThreshold(viewAntSetup.getAntThreshold());
            antenna.setTimeout(viewAntSetup.getAntTagTimeout());
            
            viewAntSetup.set_status_label("Saved Antenna: " + antNo);
            
            // Leave Antenna Setup screen visible, so user can setup another antenna or hit "Back" button
        }
        
        private void initTagSetupScreen() {
            // When reenter TagSetup screen, clear screen's data entry fields of previous tag's data
            viewTagSetup.setTid("");
            viewTagSetup.setTagName("");
            viewTagSetup.setCellNo("");
            viewTagSetup.setFirm("");
            viewTagSetup.setNote("");
            viewTagSetup.clearAlarms();
           
            viewTagSetup.setSection("");
            viewTagSetup.setTagClass(TagClass.Personnel);
            viewTagSetup.setSecClass(SecurityClass.Low);
            viewTagSetup.set_tag_enable(true);
            
            viewTagSetup.set_status_label(" ");  // could comment this line out and status label would display last TID written to DB for quick cut-n-paste for updating TID setup
        }
        
        
        private void tagNameEntered(String nameEntered, String tidUiFieldContents) {
            // future development phase could check here that Name is unique too (only checking TID unique now)
            viewTagSetup.set_status_label(" ");  // clear status label when name or TID entered
        } // tagNameEntered
        
        
        private void tagTidEntered(String tidEntered, String nameUiFieldContents) {
            // User entered Tag TID on Tag Setup Screen
            // Check if TID already exists
            // If TID new - return
            // If TID exists - either user wants to edit an existing Tag Setup, or user entered a duplicate TID by accident
            // If nameUiFieldContents is an empty string - assume user wants to edit an existing tag setup, so
            // auto populate the other screens text fields for user to edit.
            // If nameUiFieldContents are not an empty string, and TIDs don't match - display Warning screen with text like:
            // "TID already exists for Name <name string>" and return.  User can exit Warning
            // screen and change TID and/or Name.
            viewTagSetup.set_status_label(" ");  // clear status label when name or TID entered
            
            TagData userTag = tagDataDB.loadTagReadOnly(viewTagSetup.getTid());
            if (userTag == null)
                return;  // TID new, so return
            
            // TID already registered
            String tagName = viewTagSetup.getTagName();
            String tagNameTrimmed = tagName.trim();
            String tagNameTrimmed2 = (userTag.getName()).trim();
            if (!tagName.trim().isEmpty()) {  // check string not all white space (e.g., showing up as multiple spaces)
                // Screen's Name field not empty, so check if name the same
                if (!tagNameTrimmed.equals(tagNameTrimmed2)) {  // FYI, <br> forces a carriage return line feed
                    initAndDisplayWarningScreen("<html>TID registered with different name:<br><" + tagName + "><br>" + "Continue to rename registered TID,<br>" + "or change the TID number<html>");  // display warning pop-up - user can continue to rename TID, or change TID
                }
                // return even if name the same, don't auto populate - this feature allows user to re-use gUI text fields from previous
                // tag setup for existing register TID (e.g., user enters name before TID to prevent auto populate of GUI data fields)
                return;
            }
            
            // Auto populate Screen's other Tag fields from Tag DB
            viewTagSetup.setTagName(userTag.getName());
            
            viewTagSetup.setCellNo(userTag.getCellNumber());
            viewTagSetup.setFirm(userTag.getFirm());
            viewTagSetup.setNote(userTag.getNote());
            
            // Don't need to clear selections.  Deselected at startup.  User can select and deselect using Widgit which auto updates
            // the database, so DB would never have deselected alarm that is not already deselected on GUI.  However, clearAlarms() clears all.
            if (userTag.getAlarmsR1A0() == true)
                viewTagSetup.set_r1A0_alarm_enable();
            if (userTag.getAlarmsR1A1() == true)            
                viewTagSetup.set_r1A1_alarm_enable();
            if (userTag.getAlarmsR1A2() == true)            
                viewTagSetup.set_r1A2_alarm_enable();
            if (userTag.getAlarmsR1A3() == true)            
                viewTagSetup.set_r1A3_alarm_enable();
            
            viewTagSetup.setSection(userTag.getSection());
            viewTagSetup.setTagClass(userTag.getTagClass());
            viewTagSetup.setSecClass(userTag.getSecurityClass());
            viewTagSetup.set_tag_enable(userTag.getEnabled());

        }  // tagTidEntered
        
        // On tag Setup screen, when user clicks tag eanble/disable - need to change the screen's text to enable or disable too
        private void toggleTagEnable() {
            // Get user entered state from screen
            boolean oldScreenState = viewTagSetup.get_tag_enable();
            boolean newState = (oldScreenState == true) ? false : true;
            viewTagSetup.set_tag_enable(newState);
        }        
        
        private void newTagConfig() {
            // Screen text fields updated by user and user pressed "Accept" button
            // Screen's text fields don't have to be validated because
            // screen uses widgets that only allow valid options, with the following exceptions:
            // Exceptions:
            // (1) The Name field must not be an empty string
            // (2) The TID field must not be an empty string
            // (3) The Name and/or TID field, may already be in the system and
            //     if Name, or TID, is already in system, and one field, either Name or TID,
            //     has changed (e.g., both of them not the same values as before):
            //     - If TID changed, system will have duplicates names on two TIDs, so
            //       display warning to user and ask them to change the name (no option in Phase I to deleted old tag)
            //       return from event handler, don't accept data, wait for user to
            //      hit exit button on warning screen, and to change the Name field and/or hit "back" or "accept".
            //    - If name changed, assume user is re-assigning the tag to a new user and continue
            
            String screenName = viewTagSetup.getTagName();
            String screenTid = viewTagSetup.getTid();
            if ((screenTid.trim().isEmpty()) || (screenName.trim().isEmpty())) {  // check strings not all whitespace
                initAndDisplayWarningScreen("<html>Enter TID and Name<br>Or use the Back button<html>");
                return;  // ignore accept button until name and tid valid
            }
                        
            // Else get user data from screen and add/update Tag to Tag DB
            boolean bNewTagId = false;  // assume false (e.g., updating an existing, registered TID)
            
            TagData userTag = tagDataDB.loadTagReadOnly(screenTid);
            if (userTag == null) {
                bNewTagId = true;
                userTag = new TagData();
            }
            
            // Get data from screen's fields and load into Tag structure:
            userTag.setName(viewTagSetup.getTagName().trim());
            //userTag.setTid(viewTagSetup.getTid()); // This line commented out by Ryan during merge. - TagData does not have a TID field
            userTag.setCellNumber(viewTagSetup.getCellNo().trim());  // trim off leading and trailing spaces
            userTag.setFirm(viewTagSetup.getFirm().trim());
            userTag.setNote(viewTagSetup.getNote().trim());
            userTag.setAlarmsR1A0(viewTagSetup.get_r1A0_alarm_enable());
            userTag.setAlarmsR1A1(viewTagSetup.get_r1A1_alarm_enable());
            userTag.setAlarmsR1A2(viewTagSetup.get_r1A2_alarm_enable());
            userTag.setAlarmsR1A3(viewTagSetup.get_r1A3_alarm_enable());
            userTag.setSection(viewTagSetup.getSection().trim());
            userTag.setTagClass(viewTagSetup.getTagClass());
            userTag.setSecurityClass(viewTagSetup.getSecClass());
            userTag.setEnabled(viewTagSetup.get_tag_enable());
            
            tagDataDB.storeTag(userTag, screenTid);

            // Kill the occupancy with old data.
            occupancyDB.killOccupancy(screenTid);

            // Tell the reader about any environmental ID changes.
            reader.setEnvironmentalIds(getEnvironmentalIdsFromDB());
            
            viewTagSetup.set_status_label("Saved TID: " + screenTid);

            // Leave Tag Setup screen visible, so user can enter/change tag data or hit "Back" button
            
        }  // newTagConfig
       

        private int readerReadRatesToInt(ReaderReadRates value) {
            switch (value) {
                case readerRate250ms:
                    return 250;
                case readerRate500ms:
                    return 500;
                default:
                case readerRate1sec:
                    return 1000;
            }
        }

        private ReaderReadRates intToReaderReadRates(int value) {
            if (value <= 250)
                return ReaderReadRates.readerRate250ms;
            if (value <= 500)
                return ReaderReadRates.readerRate500ms;
            else
                return ReaderReadRates.readerRate1sec;
        }
       
	public void addReaderModel(RFIDReader m){
		//System.out.println("Controller: adding model");
		this.reader = m;
                //model.configReader(readerNo); // This line commented out by Ryan during merge.
	} //addReaderModel()


	public void addView(AzbilGuiApplication v){
		//System.out.println("Controller: adding AzbilGuiApplication view");
		this.view = v;
	} //addView()
        
	public void addAntSelectView(AntSelectV2UI v){
		//System.out.println("Controller: adding AntSelectV2UI view");
		this.viewAntSel = v;
	} //addAntSelectView()

	public void addAntDataView(AntDataUI v){
		//System.out.println("Controller: adding addAntDataView view");
		this.viewAntData = v;
	} //addAntDataView()
        
	public void addSetupSelView(SetupSelectUI v){
		//System.out.println("Controller: adding addSetupSelView view");
		this.viewSetupSel = v;
	} //addSetupSelView()
        
	public void addAntSetupView(AntSetupUI v){
		//System.out.println("Controller: adding addAntSetupView view");
		this.viewAntSetup = v;
	} //addSetUpView()
        
	public void addTagSetupView(TagSetupUI v){
		//System.out.println("Controller: adding addTagSetupView view");
		this.viewTagSetup = v;
	} //addTagSetupView()        

	public void addBusyView(BusyUI v){
		//System.out.println("Controller: adding addBusyView view");
		this.viewBusy = v;
	} //addBusyView()        
        
	public void addWarningView(WarningUI v){
		//System.out.println("Controller: adding addWarningView view");
		this.viewWarning = v;
	} //addWarningView()

	public void addErrorView(ErrorUI v){
		//System.out.println("Controller: adding addErrorView view");
		this.viewError = v;
	} //addErrorView()        

} //Controller
