/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package view;

import java.util.HashSet;
import java.util.Set;
import java.util.Date;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
//import javax.swing.table.TableModel;
//import javax.swing.table.DefaultTableModel;

import controller.Controller;


/**
 *
 * @author mark
 */
public class AntDataUI extends javax.swing.JFrame {

    private static int max_table_rows = 100;
    private static int max_table_colmns = 6;
    public final int max_antNo = 4;
    private final int max_ant_threshold = 200;
    private static final String ON_STRING = "On";
    private static final String OFF_STRING = "Off";
    // Occupancy values for number of tags compared t threshold level
    private static final String ZERO_STRING = "Zero";
    private static final String UNDER_STRING = "Under";
    private static final String OVER_STRING = "Over";
    // Reader Read Rates Used by GUI
    private static final String READ_RATE_250_MS_STRING = "250";
    private static final String READ_RATE_500_MS_STRING = "500";
    private static final String READ_RATE_1000_MS_STRING = "1000";
    
    private static Controller myController;
    
    /**
     * Creates new form AntDataUI
     */
    public AntDataUI(Controller c) {
        myController = c;
        initComponents();
        myController.addAntDataView(this);

        antTable.getColumnModel().getColumn(0).setPreferredWidth(160);
        antTable.getColumnModel().getColumn(1).setPreferredWidth(80);
        antTable.getColumnModel().getColumn(2).setPreferredWidth(40);
        antTable.getColumnModel().getColumn(3).setPreferredWidth(140);
        antTable.getColumnModel().getColumn(4).setPreferredWidth(20);
        antTable.getColumnModel().getColumn(5).setPreferredWidth(20);
        antTable.getColumnModel().getColumn(6).setPreferredWidth(20);
        antTable.getColumnModel().getColumn(7).setPreferredWidth(20);
        antTable.getColumnModel().getColumn(8).setPreferredWidth(15);
    }
   
    public String getDate() {
        return(this.dateText.getText());
    }
    
    public void setDate() {
        DateFormat dateFormat = new SimpleDateFormat("MM/dd/yy");
        Date date = new Date();
        this.dateText.setText(dateFormat.format(date));
    }

    public String getTime() {
        return(this.timeText.getText());
    }
        
    public void setTime() {
        DateFormat dateFormat = new SimpleDateFormat("HH:mm:ss");
        Date date = new Date();
        this.timeText.setText(dateFormat.format(date));
    }    
            
    public int getAntNo() {
        if (this.antNo.getText() == "")
            return (-1);        

        return Integer.valueOf(this.antNo.getText());
    }

    public int setAntNo(int myInt ) {
        if (myInt < 0)
            myInt = 0;
        if (myInt > max_antNo)
            myInt = max_antNo;
        this.antNo.setText((String) "" + myInt);
        return myInt;
    }
    
    public int getZoneNo() {
        if (this.zoneNo.getText() == "")
            return (-1);        
        else        
            return Integer.valueOf(this.zoneNo.getText());
    }

    public int setZoneNo(int myInt ) {
        if (myInt < 0)
            myInt = 0;
        if (myInt > max_antNo)
            myInt = max_antNo;
        this.zoneNo.setText((String) "" + myInt);
        return myInt;        
    }
    
    public int getTempInC() {
        if (this.tempInC.getText() == "")
            return (-1);
        else
            return (Integer.valueOf(this.tempInC.getText()));
    }

    public void setTempInC(int myInt) {
        if (myInt < 0)
            this.tempInC.setText("");
        else 
            this.tempInC.setText((String) "" + myInt);
    }

    public int getHumiInRh() {
        if (this.humiInRh.getText() == "")
            return (-1);
        else        
            return (Integer.valueOf(this.humiInRh.getText()));
    }

    public void setHumiInRh(int myInt) {
        if (myInt < 0)
            this.humiInRh.setText("");
        else
            this.humiInRh.setText((String) "" + myInt);
    }
    
    public int getLightInLux() {
        if (this.lightInLux.getText() == "")
            return (-1);
        else        
            return (Integer.valueOf(this.lightInLux.getText()));
    }

    public void setLightInLux(int myInt) {
        if (myInt < 0)
            this.lightInLux.setText("");
        else
            this.lightInLux.setText((String) "" + myInt);
    }
    
    public int getTheshold() {
        if (this.theshold.getText() == "")
            return (-1);        
        else
            return (Integer.valueOf(this.theshold.getText()));
    }

    public int setTheshold(int myInt) {
        if (myInt < 0)
            myInt = 0;
        if (myInt > max_ant_threshold)
            myInt = max_ant_threshold;
        this.theshold.setText((String) "" + myInt);
        return (myInt);
    }

    public static AntOccupancyState intToEnumAntOccState(int myInt) {
        switch (myInt) {
            case 0: return (AntOccupancyState.Zero_Tags);       // zero to enum 1 (starting enum value in java)
            case 1: return (AntOccupancyState.Under_Threshold);
            case 2: return (AntOccupancyState.Over_Threshold);
            default: return (AntOccupancyState.Zero_Tags);
        }
    }
    
    public AntOccupancyState getAntOccupancyState() {
        String OccStg = this.Occupancy.getText();
        if (OccStg == ZERO_STRING)
            return (AntOccupancyState.Zero_Tags);
        else if (OccStg == UNDER_STRING)
            return (AntOccupancyState.Under_Threshold);
        else // OVER_STRING
            return (AntOccupancyState.Over_Threshold);
    }

    public void setAntOccupancyState(AntOccupancyState myEnum) {
        if (myEnum == AntOccupancyState.Zero_Tags)
            this.Occupancy.setText(ZERO_STRING);
        else if (myEnum == AntOccupancyState.Under_Threshold)
            this.Occupancy.setText(UNDER_STRING);
        else if (myEnum == AntOccupancyState.Over_Threshold)
            this.Occupancy.setText(OVER_STRING);
    }    

    public ReaderReadRates getReadRate() {
        String ReadRateStg = this.readRate.getText();
        if (ReadRateStg == READ_RATE_250_MS_STRING)
            return (ReaderReadRates.readerRate250ms);
        else if (ReadRateStg == READ_RATE_500_MS_STRING)
            return (ReaderReadRates.readerRate500ms);
        else // READ_RATE_1000_MS_STRING
            return (ReaderReadRates.readerRate1sec);        
    }

    public void setReadRate(ReaderReadRates myEnum) {
        if (myEnum == ReaderReadRates.readerRate250ms)
            this.readRate.setText(READ_RATE_250_MS_STRING);
        else if (myEnum == ReaderReadRates.readerRate500ms)
            this.readRate.setText(READ_RATE_500_MS_STRING);
        else if (myEnum == ReaderReadRates.readerRate1sec)
            this.readRate.setText(READ_RATE_1000_MS_STRING);        
    }


    public boolean getLocAlgoOnState() {
        String myStg = this.locAlgoState.getText();
        if (myStg.equals(ON_STRING)) {
            return (true);  // power on
        }
        else {
            return (false);  // power off
        }
        
    }

    public void setLocAlgoOnState(boolean bState) {
        if (bState == true) {
            this.locAlgoState.setText(ON_STRING);
        }
        else {
            this.locAlgoState.setText(OFF_STRING);
        }
    }
    
    public boolean getAntPwrOnState() {
        String myStg = this.antPwrState.getText();        
        if (myStg.equals(ON_STRING)) {
            return (true);  // power on
        }
        else {
            return (false);  // power off
        }
        
    }

    public void setAntPwrOnState(boolean bState) {
        if (bState == true) {
            this.antPwrState.setText(ON_STRING);
        }
        else {
            this.antPwrState.setText(OFF_STRING);
        }
    }
    
    public void clearAntDataTableContents()
    {
        for (int i = 0; i < this.antTable.getRowCount(); i++) {
            for (int j = 0; j < this.antTable.getColumnCount(); j++) {
                this.antTable.setValueAt("", i, j);
            }
        }
    }  

    public void setTable(Object[][] array) {
        int row = 0;

        if (array != null)
            for (; row < array.length; row++)
                setTableRow(row, array[row]);

        for (; row < 100; row++)
            setTableRow(row, new Object[] {null, null, null, null, null, null, null, null, null});
    }

    // Takes an Object array of Strings: [ id, name, confidence, entry, stay, alarm ]
    private void setTableRow(int row, Object[] array) {
        for (int column = 0; column < 9; column++)
            antTable.setValueAt(array[column], row, column);
    }
       
    /**
     * Creates new form testUI
     */
    /*
    public AntDataUI(String antNoStg) {
        initComponents();
        antNo.setText((String) antNoStg);
        ZoneNo.setText((String) antNoStg);
        //TableModel tm;
        
        //String [][] dataValues = new String[1][8];
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 6; j++) {
                if (j == 0) {
                    //dataValues[i][j] = (String) "" + (i+1);
                    antTable.setValueAt(((String) "" + (i+1)), i , j);
                } else {
                    //dataValues[i][j] = "...";
                    antTable.setValueAt(((String) "..."), i , j);
                }
            }
        }       
    }
    */

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jScrollPane2 = new javax.swing.JScrollPane();
        jComboBox1 = new javax.swing.JComboBox<>();
        buttonPanel = new javax.swing.JPanel();
        resetButton = new javax.swing.JButton();
        antPwrButton = new javax.swing.JButton();
        setupButton = new javax.swing.JButton();
        locAlgoButton = new javax.swing.JButton();
        createFileButton = new javax.swing.JButton();
        antPwrState = new javax.swing.JTextField();
        locAlgoState = new javax.swing.JTextField();
        dateText = new javax.swing.JTextField();
        timeLabel = new javax.swing.JLabel();
        dateLabel = new javax.swing.JLabel();
        timeText = new javax.swing.JTextField();
        readRatePreLabel = new javax.swing.JLabel();
        readRate = new javax.swing.JTextField();
        readRatePostLabel = new javax.swing.JLabel();
        zonePanel = new javax.swing.JPanel();
        antPanel = new javax.swing.JPanel();
        jScrollPane1 = new javax.swing.JScrollPane();
        antTable = new javax.swing.JTable();
        antNoLabel = new javax.swing.JLabel();
        antNo = new javax.swing.JTextField();
        humiPreLabel = new javax.swing.JLabel();
        tempInC = new javax.swing.JTextField();
        tempPostLabel = new javax.swing.JLabel();
        tempPreLabel = new javax.swing.JLabel();
        lightInLux = new javax.swing.JTextField();
        humiPostLabel = new javax.swing.JLabel();
        lightPreLabel = new javax.swing.JLabel();
        humiInRh = new javax.swing.JTextField();
        lightPostLabel = new javax.swing.JLabel();
        occupancyLabel = new javax.swing.JLabel();
        theshold = new javax.swing.JTextField();
        Occupancy = new javax.swing.JTextField();
        thresholdLabel = new javax.swing.JLabel();
        backButton = new javax.swing.JButton();
        zoneNo = new javax.swing.JTextField();
        ZoneNoLabel = new javax.swing.JLabel();
        readerNoLabel = new javax.swing.JLabel();

        jComboBox1.setModel(new javax.swing.DefaultComboBoxModel<>(new String[] { "Item 1", "Item 2", "Item 3", "Item 4" }));

        setTitle("Azbil GUI: Antenna Data Screen");

        buttonPanel.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));

        resetButton.setFont(new java.awt.Font("Tahoma", 0, 14)); // NOI18N
        resetButton.setText("Reset");
        resetButton.setActionCommand("ButtonReset");
        resetButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                resetButtonActionPerformed(evt);
            }
        });

        antPwrButton.setFont(new java.awt.Font("Tahoma", 0, 14)); // NOI18N
        antPwrButton.setText("Ant Power On/Off");
        antPwrButton.setActionCommand("ButtonAntPowerToggle");
        antPwrButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                antPwrButtonActionPerformed(evt);
            }
        });

        setupButton.setFont(new java.awt.Font("Tahoma", 0, 14)); // NOI18N
        setupButton.setText("Setup/Update");
        setupButton.setActionCommand("ButtonSetup");
        setupButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                setupButtonActionPerformed(evt);
            }
        });

        locAlgoButton.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        locAlgoButton.setText("Cross-Read Algorithm");
        locAlgoButton.setActionCommand("ButtonLocAlgoToggle");
        locAlgoButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                locAlgoButtonActionPerformed(evt);
            }
        });

        createFileButton.setFont(new java.awt.Font("Tahoma", 0, 14)); // NOI18N
        createFileButton.setText("Create CSV Tag Files");
        createFileButton.setActionCommand("ButtonCreateCsvFiles");
        createFileButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                createFileButtonActionPerformed(evt);
            }
        });

        antPwrState.setEditable(false);
        antPwrState.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        antPwrState.setText("Off");
        antPwrState.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                antPwrStateActionPerformed(evt);
            }
        });

        locAlgoState.setEditable(false);
        locAlgoState.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        locAlgoState.setText("On");
        locAlgoState.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                locAlgoStateActionPerformed(evt);
            }
        });

        dateText.setEditable(false);
        dateText.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        dateText.setText("08/02/16");
        dateText.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                dateTextActionPerformed(evt);
            }
        });

        timeLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        timeLabel.setText("Time");

        dateLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        dateLabel.setText("Date");

        timeText.setEditable(false);
        timeText.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        timeText.setText("12:30:00");
        timeText.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                timeTextActionPerformed(evt);
            }
        });

        readRatePreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        readRatePreLabel.setText("Read Rate");

        readRate.setEditable(false);
        readRate.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        readRate.setText("500");
        readRate.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                readRateActionPerformed(evt);
            }
        });

        readRatePostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        readRatePostLabel.setText("ms");

        javax.swing.GroupLayout buttonPanelLayout = new javax.swing.GroupLayout(buttonPanel);
        buttonPanel.setLayout(buttonPanelLayout);
        buttonPanelLayout.setHorizontalGroup(
            buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, buttonPanelLayout.createSequentialGroup()
                .addGap(68, 68, 68)
                .addGroup(buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(buttonPanelLayout.createSequentialGroup()
                        .addGroup(buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(locAlgoButton)
                            .addGroup(buttonPanelLayout.createSequentialGroup()
                                .addComponent(dateLabel)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(dateText, javax.swing.GroupLayout.PREFERRED_SIZE, 112, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(buttonPanelLayout.createSequentialGroup()
                                .addComponent(timeLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 66, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(timeText, javax.swing.GroupLayout.PREFERRED_SIZE, 110, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addComponent(locAlgoState, javax.swing.GroupLayout.PREFERRED_SIZE, 46, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(buttonPanelLayout.createSequentialGroup()
                        .addComponent(readRatePreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 123, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(readRate, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(readRatePostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 49, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 177, Short.MAX_VALUE)
                .addGroup(buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(antPwrButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(resetButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(setupButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addGroup(buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(buttonPanelLayout.createSequentialGroup()
                        .addGap(103, 103, 103)
                        .addComponent(createFileButton))
                    .addGroup(buttonPanelLayout.createSequentialGroup()
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(antPwrState, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addGap(95, 95, 95))
        );
        buttonPanelLayout.setVerticalGroup(
            buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(buttonPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(buttonPanelLayout.createSequentialGroup()
                        .addGroup(buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(dateLabel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(dateText, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(timeLabel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(timeText, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(18, 18, 18)
                        .addGroup(buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(locAlgoButton, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 34, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGroup(buttonPanelLayout.createSequentialGroup()
                                .addComponent(locAlgoState, javax.swing.GroupLayout.PREFERRED_SIZE, 34, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(0, 0, Short.MAX_VALUE))))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, buttonPanelLayout.createSequentialGroup()
                        .addGroup(buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(createFileButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(resetButton, javax.swing.GroupLayout.DEFAULT_SIZE, 34, Short.MAX_VALUE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 19, Short.MAX_VALUE)
                        .addGroup(buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(antPwrButton, javax.swing.GroupLayout.PREFERRED_SIZE, 35, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(antPwrState, javax.swing.GroupLayout.PREFERRED_SIZE, 34, javax.swing.GroupLayout.PREFERRED_SIZE))))
                .addGap(18, 18, 18)
                .addGroup(buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(buttonPanelLayout.createSequentialGroup()
                        .addComponent(setupButton, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(13, 13, 13))
                    .addGroup(buttonPanelLayout.createSequentialGroup()
                        .addGroup(buttonPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(readRatePreLabel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(readRate, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(readRatePostLabel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                        .addContainerGap())))
        );

        zonePanel.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));

        antPanel.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));

        antTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null},
                {null, null, null, null, null, null, null, null, null}
            },
            new String [] {
                "ID", "Name", "Confidence", "Entry", "Stay", "Stale", "Remain", "Intent", "Alert"
            }
        ) {
            boolean[] canEdit = new boolean [] {
                false, false, false, false, false, false, false, false, false
            };

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        jScrollPane1.setViewportView(antTable);

        antNoLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        antNoLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        antNoLabel.setText("ANT");

        antNo.setEditable(false);
        antNo.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        antNo.setText("0");
        antNo.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                antNoActionPerformed(evt);
            }
        });

        humiPreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        humiPreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        humiPreLabel.setText("Humidity");

        tempInC.setEditable(false);
        tempInC.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        tempInC.setText("26");
        tempInC.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                tempInCActionPerformed(evt);
            }
        });

        tempPostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        tempPostLabel.setText("C");

        tempPreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        tempPreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        tempPreLabel.setText("Temperature");

        lightInLux.setEditable(false);
        lightInLux.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        lightInLux.setText("10");
        lightInLux.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                lightInLuxActionPerformed(evt);
            }
        });

        humiPostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        humiPostLabel.setText("RH");

        lightPreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        lightPreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        lightPreLabel.setText("Light");

        humiInRh.setEditable(false);
        humiInRh.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        humiInRh.setText("36");
        humiInRh.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                humiInRhActionPerformed(evt);
            }
        });

        lightPostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        lightPostLabel.setText("Lux");

        occupancyLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        occupancyLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        occupancyLabel.setText("Occupancy");

        theshold.setEditable(false);
        theshold.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        theshold.setText("100");
        theshold.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                thesholdActionPerformed(evt);
            }
        });

        Occupancy.setEditable(false);
        Occupancy.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        Occupancy.setText("Under");
        Occupancy.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                OccupancyActionPerformed(evt);
            }
        });

        thresholdLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        thresholdLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        thresholdLabel.setText("Tag Threshold");

        backButton.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        backButton.setText("Back");
        backButton.setActionCommand("ButtonAntDataBack");
        backButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                backButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout antPanelLayout = new javax.swing.GroupLayout(antPanel);
        antPanel.setLayout(antPanelLayout);
        antPanelLayout.setHorizontalGroup(
            antPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, antPanelLayout.createSequentialGroup()
                .addGroup(antPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(antPanelLayout.createSequentialGroup()
                        .addGap(48, 48, 48)
                        .addGroup(antPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(antPanelLayout.createSequentialGroup()
                                .addComponent(antNoLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 113, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(antNo, javax.swing.GroupLayout.PREFERRED_SIZE, 35, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(antPanelLayout.createSequentialGroup()
                                .addGap(5, 5, 5)
                                .addComponent(humiPreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(humiInRh, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(humiPostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(antPanelLayout.createSequentialGroup()
                                .addComponent(lightPreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(lightInLux, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(lightPostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE))))
                    .addGroup(antPanelLayout.createSequentialGroup()
                        .addGap(12, 12, 12)
                        .addGroup(antPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(occupancyLabel)
                            .addComponent(thresholdLabel))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(antPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(Occupancy, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(theshold, javax.swing.GroupLayout.PREFERRED_SIZE, 56, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(antPanelLayout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(backButton, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(antPanelLayout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(tempPreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 148, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(tempInC, javax.swing.GroupLayout.PREFERRED_SIZE, 41, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(10, 10, 10)
                        .addComponent(tempPostLabel)))
                .addGap(18, 18, 18)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 863, Short.MAX_VALUE)
                .addContainerGap())
        );
        antPanelLayout.setVerticalGroup(
            antPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, antPanelLayout.createSequentialGroup()
                .addGap(8, 8, 8)
                .addGroup(antPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(antNoLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 45, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(antNo, javax.swing.GroupLayout.PREFERRED_SIZE, 45, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(60, 60, 60)
                .addGroup(antPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(antPanelLayout.createSequentialGroup()
                        .addGap(50, 50, 50)
                        .addGroup(antPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(humiPreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(humiPostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(humiInRh, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(antPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(tempInC, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(tempPreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(tempPostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(antPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lightPreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lightInLux, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lightPostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(43, 43, 43)
                .addGroup(antPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(theshold, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(thresholdLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(antPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(Occupancy, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(occupancyLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(backButton, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
            .addGroup(antPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(22, Short.MAX_VALUE))
        );

        zoneNo.setEditable(false);
        zoneNo.setFont(new java.awt.Font("Tahoma", 0, 36)); // NOI18N
        zoneNo.setText("0");
        zoneNo.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                zoneNoActionPerformed(evt);
            }
        });

        ZoneNoLabel.setFont(new java.awt.Font("Tahoma", 0, 36)); // NOI18N
        ZoneNoLabel.setText("Zone");

        readerNoLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        readerNoLabel.setText("Reader 1:");

        javax.swing.GroupLayout zonePanelLayout = new javax.swing.GroupLayout(zonePanel);
        zonePanel.setLayout(zonePanelLayout);
        zonePanelLayout.setHorizontalGroup(
            zonePanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, zonePanelLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addComponent(readerNoLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 117, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(ZoneNoLabel)
                .addGap(18, 18, 18)
                .addComponent(zoneNo, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(552, 552, 552))
            .addGroup(zonePanelLayout.createSequentialGroup()
                .addComponent(antPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );
        zonePanelLayout.setVerticalGroup(
            zonePanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, zonePanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(zonePanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(readerNoLabel, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(zonePanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(zoneNo, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(ZoneNoLabel)))
                .addGap(20, 20, 20)
                .addComponent(antPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(buttonPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(zonePanel, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(buttonPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(zonePanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void zoneNoActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_zoneNoActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_zoneNoActionPerformed

    private void antNoActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_antNoActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_antNoActionPerformed

    private void tempInCActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_tempInCActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_tempInCActionPerformed

    private void lightInLuxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_lightInLuxActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_lightInLuxActionPerformed

    private void humiInRhActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_humiInRhActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_humiInRhActionPerformed

    private void thesholdActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_thesholdActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_thesholdActionPerformed

    private void OccupancyActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_OccupancyActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_OccupancyActionPerformed

    private void antPwrButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_antPwrButtonActionPerformed
        // TODO add your handling code here:
        myController.guiActionPerformed(evt);
    }//GEN-LAST:event_antPwrButtonActionPerformed

    private void locAlgoButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_locAlgoButtonActionPerformed
        // TODO add your handling code here:
        myController.guiActionPerformed(evt);        
    }//GEN-LAST:event_locAlgoButtonActionPerformed

    private void createFileButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_createFileButtonActionPerformed
        // TODO add your handling code here:
        myController.guiActionPerformed(evt);
    }//GEN-LAST:event_createFileButtonActionPerformed

    private void locAlgoStateActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_locAlgoStateActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_locAlgoStateActionPerformed

    private void dateTextActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_dateTextActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_dateTextActionPerformed

    private void resetButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_resetButtonActionPerformed
        // TODO add your handling code here:
        myController.guiActionPerformed(evt);
    }//GEN-LAST:event_resetButtonActionPerformed

    private void setupButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_setupButtonActionPerformed
        // TODO add your handling code here:
        myController.guiActionPerformed(evt);        
        //new SetupSelectUI().setVisible(true);
    }//GEN-LAST:event_setupButtonActionPerformed

    private void timeTextActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_timeTextActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_timeTextActionPerformed

    private void antPwrStateActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_antPwrStateActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_antPwrStateActionPerformed

    private void readRateActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_readRateActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_readRateActionPerformed

    private void backButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_backButtonActionPerformed
        // TODO add your handling code here:
        myController.guiActionPerformed(evt);
        //setVisible(false);
    }//GEN-LAST:event_backButtonActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(AntDataUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(AntDataUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(AntDataUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(AntDataUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new AntDataUI(myController).setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JTextField Occupancy;
    private javax.swing.JLabel ZoneNoLabel;
    private javax.swing.JTextField antNo;
    private javax.swing.JLabel antNoLabel;
    private javax.swing.JPanel antPanel;
    private javax.swing.JButton antPwrButton;
    private javax.swing.JTextField antPwrState;
    private javax.swing.JTable antTable;
    private javax.swing.JButton backButton;
    private javax.swing.JPanel buttonPanel;
    private javax.swing.JButton createFileButton;
    private javax.swing.JLabel dateLabel;
    private javax.swing.JTextField dateText;
    private javax.swing.JTextField humiInRh;
    private javax.swing.JLabel humiPostLabel;
    private javax.swing.JLabel humiPreLabel;
    private javax.swing.JComboBox<String> jComboBox1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JTextField lightInLux;
    private javax.swing.JLabel lightPostLabel;
    private javax.swing.JLabel lightPreLabel;
    private javax.swing.JButton locAlgoButton;
    private javax.swing.JTextField locAlgoState;
    private javax.swing.JLabel occupancyLabel;
    private javax.swing.JTextField readRate;
    private javax.swing.JLabel readRatePostLabel;
    private javax.swing.JLabel readRatePreLabel;
    private javax.swing.JLabel readerNoLabel;
    private javax.swing.JButton resetButton;
    private javax.swing.JButton setupButton;
    private javax.swing.JTextField tempInC;
    private javax.swing.JLabel tempPostLabel;
    private javax.swing.JLabel tempPreLabel;
    private javax.swing.JTextField theshold;
    private javax.swing.JLabel thresholdLabel;
    private javax.swing.JLabel timeLabel;
    private javax.swing.JTextField timeText;
    private javax.swing.JTextField zoneNo;
    private javax.swing.JPanel zonePanel;
    // End of variables declaration//GEN-END:variables
}
