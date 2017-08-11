/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package view;

import controller.Controller;

/**
 *
 * @author mark
 */
public class AntSelectV2UI extends javax.swing.JFrame {
    private static Controller myController;
   
    private static final String ZERO_STRING = "Zero";
    private static final String UNDER_STRING = "Under";
    private static final String OVER_STRING = "Over";    
    
    /**
     * Creates new form AntSelectV2UI
     */
    public AntSelectV2UI(Controller c) {
        myController = c;
        initComponents();
        myController.addAntSelectView(this);

        antSelScrollPane.getVerticalScrollBar().setUnitIncrement(10);
    }
    
    // Reader x Antenna 0 Methods (RxA0)
    public int getTempInC_RxA0() {
        if (this.temp0InC.getText() == "")
            return (-1);
        else
            return (Integer.valueOf(this.temp0InC.getText()));
    }

    public void setTempInC_RxA0(int myInt) {
        if (myInt < 0)
            this.temp0InC.setText(" ");
        else 
            this.temp0InC.setText((String) "" + myInt);
    }

    public int getHumiInRh_RxA0() {
        if (this.humi0InRh.getText() == "")
            return (-1);
        else        
            return (Integer.valueOf(this.humi0InRh.getText()));
    }

    public void setHumiInRh_RxA0(int myInt) {
        if (myInt < 0)
            this.humi0InRh.setText(" ");
        else
            this.humi0InRh.setText((String) "" + myInt);
    }
    
    public int getLightInLux_RxA0() {
        if (this.light0InLux.getText() == "")
            return (-1);
        else        
            return (Integer.valueOf(this.light0InLux.getText()));
    }

    public void setLightInLux_RxA0(int myInt) {
        if (myInt < 0)
            this.light0InLux.setText(" ");
        else
            this.light0InLux.setText((String) "" + myInt);
    }

    public AntOccupancyState getAntOccupancyState_RxA0() {
        String OccStg = this.Occupancy0.getText();
        if (OccStg == ZERO_STRING)
            return (AntOccupancyState.Zero_Tags);
        else if (OccStg == UNDER_STRING)
            return (AntOccupancyState.Under_Threshold);
        else // OVER_STRING
            return (AntOccupancyState.Over_Threshold);
    }

    public void setAntOccupancyState_RxA0(AntOccupancyState myEnum) {
        if (myEnum == AntOccupancyState.Zero_Tags)
            this.Occupancy0.setText(ZERO_STRING);
        else if (myEnum == AntOccupancyState.Under_Threshold)
            this.Occupancy0.setText(UNDER_STRING);
        else if (myEnum == AntOccupancyState.Over_Threshold)
            this.Occupancy0.setText(OVER_STRING);
    }    

    // Reader x Antenna 1 Methods (RxA1)
    public int getTempInC_RxA1() {
        if (this.temp1InC.getText() == "")
            return (-1);
        else
            return (Integer.valueOf(this.temp1InC.getText()));
    }

    public void setTempInC_RxA1(int myInt) {
        if (myInt < 0)
            this.temp1InC.setText(" ");
        else 
            this.temp1InC.setText((String) "" + myInt);
    }

    public int getHumiInRh_RxA1() {
        if (this.humi1InRh.getText() == "")
            return (-1);
        else        
            return (Integer.valueOf(this.humi1InRh.getText()));
    }

    public void setHumiInRh_RxA1(int myInt) {
        if (myInt < 0)
            this.humi1InRh.setText(" ");
        else
            this.humi1InRh.setText((String) "" + myInt);
    }
    
    public int getLightInLux_RxA1() {
        if (this.light1InLux.getText() == "")
            return (-1);
        else        
            return (Integer.valueOf(this.light1InLux.getText()));
    }

    public void setLightInLux_RxA1(int myInt) {
        if (myInt < 0)
            this.light1InLux.setText(" ");
        else
            this.light1InLux.setText((String) "" + myInt);
    }

    public AntOccupancyState getAntOccupancyState_RxA1() {
        String OccStg = this.Occupancy1.getText();
        if (OccStg == ZERO_STRING)
            return (AntOccupancyState.Zero_Tags);
        else if (OccStg == UNDER_STRING)
            return (AntOccupancyState.Under_Threshold);
        else // OVER_STRING
            return (AntOccupancyState.Over_Threshold);
    }

    public void setAntOccupancyState_RxA1(AntOccupancyState myEnum) {
        if (myEnum == AntOccupancyState.Zero_Tags)
            this.Occupancy1.setText(ZERO_STRING);
        else if (myEnum == AntOccupancyState.Under_Threshold)
            this.Occupancy1.setText(UNDER_STRING);
        else if (myEnum == AntOccupancyState.Over_Threshold)
            this.Occupancy1.setText(OVER_STRING);
    }    

    // Reader x Antenna 2 Methods (RxA2)
    public int getTempInC_RxA2() {
        if (this.temp2InC.getText() == "")
            return (-1);
        else
            return (Integer.valueOf(this.temp2InC.getText()));
    }

    public void setTempInC_RxA2(int myInt) {
        if (myInt < 0)
            this.temp2InC.setText(" ");
        else 
            this.temp2InC.setText((String) "" + myInt);
    }

    public int getHumiInRh_RxA2() {
        if (this.humi2InRh.getText() == "")
            return (-1);
        else        
            return (Integer.valueOf(this.humi2InRh.getText()));
    }

    public void setHumiInRh_RxA2(int myInt) {
        if (myInt < 0)
            this.humi2InRh.setText(" ");
        else
            this.humi2InRh.setText((String) "" + myInt);
    }
    
    public int getLightInLux_RxA2() {
        if (this.light2InLux.getText() == "")
            return (-1);
        else        
            return (Integer.valueOf(this.light2InLux.getText()));
    }

    public void setLightInLux_RxA2(int myInt) {
        if (myInt < 0)
            this.light2InLux.setText(" ");
        else
            this.light2InLux.setText((String) "" + myInt);
    }

    public AntOccupancyState getAntOccupancyState_RxA2() {
        String OccStg = this.Occupancy2.getText();
        if (OccStg == ZERO_STRING)
            return (AntOccupancyState.Zero_Tags);
        else if (OccStg == UNDER_STRING)
            return (AntOccupancyState.Under_Threshold);
        else // OVER_STRING
            return (AntOccupancyState.Over_Threshold);
    }

    public void setAntOccupancyState_RxA2(AntOccupancyState myEnum) {
        if (myEnum == AntOccupancyState.Zero_Tags)
            this.Occupancy2.setText(ZERO_STRING);
        else if (myEnum == AntOccupancyState.Under_Threshold)
            this.Occupancy2.setText(UNDER_STRING);
        else if (myEnum == AntOccupancyState.Over_Threshold)
            this.Occupancy2.setText(OVER_STRING);
    }    

    // Reader x Antenna 3 Methods (RxA3)
    public int getTempInC_R1A3() {
        if (this.temp3InC.getText() == "")
            return (-1);
        else
            return (Integer.valueOf(this.temp3InC.getText()));
    }

    public void setTempInC_RxA3(int myInt) {
        if (myInt < 0)
            this.temp3InC.setText(" ");
        else 
            this.temp3InC.setText((String) "" + myInt);
    }

    public int getHumiInRh_RxA3() {
        if (this.humi3InRh.getText() == "")
            return (-1);
        else        
            return (Integer.valueOf(this.humi3InRh.getText()));
    }

    public void setHumiInRh_RxA3(int myInt) {
        if (myInt < 0)
            this.humi3InRh.setText(" ");
        else
            this.humi3InRh.setText((String) "" + myInt);
    }
    
    public int getLightInLux_RxA3() {
        if (this.light3InLux.getText() == "")
            return (-1);
        else        
            return (Integer.valueOf(this.light3InLux.getText()));
    }

    public void setLightInLux_RxA3(int myInt) {
        if (myInt < 0)
            this.light3InLux.setText(" ");
        else
            this.light3InLux.setText((String) "" + myInt);
    }

    public AntOccupancyState getAntOccupancyState_RxA3() {
        String OccStg = this.Occupancy3.getText();
        if (OccStg == ZERO_STRING)
            return (AntOccupancyState.Zero_Tags);
        else if (OccStg == UNDER_STRING)
            return (AntOccupancyState.Under_Threshold);
        else // OVER_STRING
            return (AntOccupancyState.Over_Threshold);
    }

    public void setAntOccupancyState_RxA3(AntOccupancyState myEnum) {
        if (myEnum == AntOccupancyState.Zero_Tags)
            this.Occupancy3.setText(ZERO_STRING);
        else if (myEnum == AntOccupancyState.Under_Threshold)
            this.Occupancy3.setText(UNDER_STRING);
        else if (myEnum == AntOccupancyState.Over_Threshold)
            this.Occupancy3.setText(OVER_STRING);
    }    

    public void clearAntSelectTableContents()
    {
        for (int i = 0; i < this.nameAndLocTable.getRowCount(); i++) {
            for (int j = 0; j < this.nameAndLocTable.getColumnCount(); j++) {
                this.nameAndLocTable.setValueAt("", i, j);
            }
        }
    }

    public void setTable(Object[][] array) {
        int row = 0;

        if (array != null)
            for (; row < array.length; row++)
                setTableRow(row, array[row]);

        for (; row < 100; row++)
            setTableRow(row, new Object[] {null, null, null, null, null});
    }

    // Takes an Object array of Strings: [ name, antenna, reader, confidence]
    private void setTableRow(int row, Object[] array) {
        for (int column = 0; column < 5; column++)
            nameAndLocTable.setValueAt(array[column], row, column);
    }
        
    
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        tablePanel = new javax.swing.JPanel();
        jScrollPane1 = new javax.swing.JScrollPane();
        nameAndLocTable = new javax.swing.JTable();
        antselectPanel = new javax.swing.JPanel();
        readerNoLabel = new javax.swing.JLabel();
        readerNoComboBox = new javax.swing.JComboBox<>();
        antSelScrollPane = new javax.swing.JScrollPane();
        jPanel1 = new javax.swing.JPanel();
        zone1Panel = new javax.swing.JPanel();
        temp1PreLabel = new javax.swing.JLabel();
        temp1InC = new javax.swing.JTextField();
        temp1PostLabel = new javax.swing.JLabel();
        humi1PreLabel = new javax.swing.JLabel();
        humi1InRh = new javax.swing.JTextField();
        humi1PostLabel = new javax.swing.JLabel();
        light1PreLabel = new javax.swing.JLabel();
        light1InLux = new javax.swing.JTextField();
        light1PostLabel = new javax.swing.JLabel();
        occupancy1Label = new javax.swing.JLabel();
        Occupancy1 = new javax.swing.JTextField();
        zone1Button = new javax.swing.JButton();
        zone0Panel = new javax.swing.JPanel();
        temp0PreLabel = new javax.swing.JLabel();
        temp0InC = new javax.swing.JTextField();
        temp0PostLabel = new javax.swing.JLabel();
        hum0PreLabel = new javax.swing.JLabel();
        humi0InRh = new javax.swing.JTextField();
        hum0iPostLabel = new javax.swing.JLabel();
        light0PreLabel = new javax.swing.JLabel();
        light0InLux = new javax.swing.JTextField();
        light0PostLabel = new javax.swing.JLabel();
        occupancy0Label = new javax.swing.JLabel();
        Occupancy0 = new javax.swing.JTextField();
        zone0Button = new javax.swing.JButton();
        zone2Panel = new javax.swing.JPanel();
        temp2PreLabel = new javax.swing.JLabel();
        temp2InC = new javax.swing.JTextField();
        temp2PostLabel = new javax.swing.JLabel();
        humi2PreLabel = new javax.swing.JLabel();
        humi2InRh = new javax.swing.JTextField();
        humi2PostLabel = new javax.swing.JLabel();
        light2PreLabel = new javax.swing.JLabel();
        light2InLux = new javax.swing.JTextField();
        light2PostLabel = new javax.swing.JLabel();
        occupancy2Label = new javax.swing.JLabel();
        Occupancy2 = new javax.swing.JTextField();
        zone2Button = new javax.swing.JButton();
        zone3Panel = new javax.swing.JPanel();
        temp3PreLabel = new javax.swing.JLabel();
        temp3InC = new javax.swing.JTextField();
        temp3PostLabel = new javax.swing.JLabel();
        humi3PreLabel = new javax.swing.JLabel();
        humi3InRh = new javax.swing.JTextField();
        humi3PostLabel = new javax.swing.JLabel();
        light3PreLabel = new javax.swing.JLabel();
        light3InLux = new javax.swing.JTextField();
        light3PostLabel = new javax.swing.JLabel();
        occupancy3Label = new javax.swing.JLabel();
        Occupancy3 = new javax.swing.JTextField();
        zone3Button = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.DO_NOTHING_ON_CLOSE);
        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosing(java.awt.event.WindowEvent evt) {
                antSelClosing(evt);
            }
        });

        nameAndLocTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null},
                {null, null, null, null, null}
            },
            new String [] {
                "Name", "Reader", "Zone", "Confidence", "Intent"
            }
        ) {
            boolean[] canEdit = new boolean [] {
                false, false, false, false, false
            };

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        jScrollPane1.setViewportView(nameAndLocTable);

        javax.swing.GroupLayout tablePanelLayout = new javax.swing.GroupLayout(tablePanel);
        tablePanel.setLayout(tablePanelLayout);
        tablePanelLayout.setHorizontalGroup(
            tablePanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(tablePanelLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 592, Short.MAX_VALUE)
                .addContainerGap())
        );
        tablePanelLayout.setVerticalGroup(
            tablePanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(tablePanelLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 695, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        readerNoLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        readerNoLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        readerNoLabel.setText("Reader:");

        readerNoComboBox.setFont(new java.awt.Font("Tahoma", 0, 48)); // NOI18N
        readerNoComboBox.setModel(new javax.swing.DefaultComboBoxModel<>(new String[] { "1" }));
        readerNoComboBox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                readerNoComboBoxActionPerformed(evt);
            }
        });

        antSelScrollPane.setHorizontalScrollBar(null);

        zone1Panel.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));

        temp1PreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        temp1PreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        temp1PreLabel.setText("Temperature");

        temp1InC.setEditable(false);
        temp1InC.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        temp1InC.setText("25");
        temp1InC.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                temp1InCActionPerformed(evt);
            }
        });

        temp1PostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        temp1PostLabel.setText("C");

        humi1PreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        humi1PreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        humi1PreLabel.setText("Humidity");

        humi1InRh.setEditable(false);
        humi1InRh.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        humi1InRh.setText("35");
        humi1InRh.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                humi1InRhActionPerformed(evt);
            }
        });

        humi1PostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        humi1PostLabel.setText("RH");

        light1PreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        light1PreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        light1PreLabel.setText("Light");

        light1InLux.setEditable(false);
        light1InLux.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        light1InLux.setText("9");
        light1InLux.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                light1InLuxActionPerformed(evt);
            }
        });

        light1PostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        light1PostLabel.setText("Lux");

        occupancy1Label.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        occupancy1Label.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        occupancy1Label.setText("Occupancy");

        Occupancy1.setEditable(false);
        Occupancy1.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        Occupancy1.setText("Over");
        Occupancy1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                Occupancy1ActionPerformed(evt);
            }
        });

        zone1Button.setFont(new java.awt.Font("Tahoma", 1, 24)); // NOI18N
        zone1Button.setText("Go to Reader's Zone #1 Screen");
        zone1Button.setActionCommand("ButtonZone1");
        zone1Button.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                zone1ButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout zone1PanelLayout = new javax.swing.GroupLayout(zone1Panel);
        zone1Panel.setLayout(zone1PanelLayout);
        zone1PanelLayout.setHorizontalGroup(
            zone1PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, zone1PanelLayout.createSequentialGroup()
                .addGroup(zone1PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(temp1PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 148, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(zone1PanelLayout.createSequentialGroup()
                        .addComponent(humi1PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(10, 10, 10)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(zone1PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(zone1PanelLayout.createSequentialGroup()
                        .addComponent(temp1InC, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(temp1PostLabel))
                    .addGroup(zone1PanelLayout.createSequentialGroup()
                        .addComponent(humi1InRh, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(humi1PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(zone1PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(occupancy1Label)
                    .addComponent(light1PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(zone1PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(zone1PanelLayout.createSequentialGroup()
                        .addComponent(light1InLux, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(light1PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(Occupancy1, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(41, 41, 41))
            .addGroup(zone1PanelLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(zone1Button, javax.swing.GroupLayout.PREFERRED_SIZE, 512, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        zone1PanelLayout.setVerticalGroup(
            zone1PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(zone1PanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(zone1PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(zone1PanelLayout.createSequentialGroup()
                        .addGroup(zone1PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(temp1PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(temp1InC, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(temp1PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(7, 7, 7)
                        .addGroup(zone1PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(humi1PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(humi1InRh, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(humi1PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(zone1PanelLayout.createSequentialGroup()
                        .addGroup(zone1PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(light1InLux, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(light1PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(light1PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(7, 7, 7)
                        .addGroup(zone1PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(occupancy1Label, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(Occupancy1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(zone1Button, javax.swing.GroupLayout.DEFAULT_SIZE, 47, Short.MAX_VALUE)
                .addContainerGap())
        );

        zone0Panel.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        zone0Panel.setPreferredSize(new java.awt.Dimension(569, 174));

        temp0PreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        temp0PreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        temp0PreLabel.setText("Temperature");

        temp0InC.setEditable(false);
        temp0InC.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        temp0InC.setText("24");
        temp0InC.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                temp0InCActionPerformed(evt);
            }
        });

        temp0PostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        temp0PostLabel.setText("C");

        hum0PreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        hum0PreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        hum0PreLabel.setText("Humidity");

        humi0InRh.setEditable(false);
        humi0InRh.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        humi0InRh.setText("34");
        humi0InRh.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                humi0InRhActionPerformed(evt);
            }
        });

        hum0iPostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        hum0iPostLabel.setText("RH");

        light0PreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        light0PreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        light0PreLabel.setText("Light");

        light0InLux.setEditable(false);
        light0InLux.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        light0InLux.setText("8");
        light0InLux.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                light0InLuxActionPerformed(evt);
            }
        });

        light0PostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        light0PostLabel.setText("Lux");

        occupancy0Label.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        occupancy0Label.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        occupancy0Label.setText("Occupancy");

        Occupancy0.setEditable(false);
        Occupancy0.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        Occupancy0.setText("Under");
        Occupancy0.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                Occupancy0ActionPerformed(evt);
            }
        });

        zone0Button.setFont(new java.awt.Font("Tahoma", 1, 24)); // NOI18N
        zone0Button.setText("Go to Reader's Zone #0 Screen");
        zone0Button.setActionCommand("ButtonZone0");
        zone0Button.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                zone0ButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout zone0PanelLayout = new javax.swing.GroupLayout(zone0Panel);
        zone0Panel.setLayout(zone0PanelLayout);
        zone0PanelLayout.setHorizontalGroup(
            zone0PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, zone0PanelLayout.createSequentialGroup()
                .addGroup(zone0PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, zone0PanelLayout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(zone0Button, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, zone0PanelLayout.createSequentialGroup()
                        .addGroup(zone0PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(temp0PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 148, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGroup(zone0PanelLayout.createSequentialGroup()
                                .addComponent(hum0PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(10, 10, 10)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(zone0PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(zone0PanelLayout.createSequentialGroup()
                                .addComponent(temp0InC, javax.swing.GroupLayout.PREFERRED_SIZE, 41, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(temp0PostLabel))
                            .addGroup(zone0PanelLayout.createSequentialGroup()
                                .addComponent(humi0InRh, javax.swing.GroupLayout.PREFERRED_SIZE, 41, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(hum0iPostLabel)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addGroup(zone0PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(occupancy0Label)
                            .addComponent(light0PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(zone0PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(zone0PanelLayout.createSequentialGroup()
                                .addComponent(light0InLux, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(light0PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addComponent(Occupancy0, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE))))
                .addGap(21, 21, 21))
        );
        zone0PanelLayout.setVerticalGroup(
            zone0PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(zone0PanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(zone0PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(zone0PanelLayout.createSequentialGroup()
                        .addGroup(zone0PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(temp0PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(temp0InC, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(temp0PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(7, 7, 7)
                        .addGroup(zone0PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(hum0PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(humi0InRh, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(hum0iPostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(zone0PanelLayout.createSequentialGroup()
                        .addGroup(zone0PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(light0InLux, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(light0PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(light0PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(7, 7, 7)
                        .addGroup(zone0PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(occupancy0Label, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(Occupancy0, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(zone0Button, javax.swing.GroupLayout.DEFAULT_SIZE, 47, Short.MAX_VALUE)
                .addContainerGap())
        );

        zone2Panel.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));

        temp2PreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        temp2PreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        temp2PreLabel.setText("Temperature");

        temp2InC.setEditable(false);
        temp2InC.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        temp2InC.setText("26");
        temp2InC.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                temp2InCActionPerformed(evt);
            }
        });

        temp2PostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        temp2PostLabel.setText("C");

        humi2PreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        humi2PreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        humi2PreLabel.setText("Humidity");

        humi2InRh.setEditable(false);
        humi2InRh.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        humi2InRh.setText("36");
        humi2InRh.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                humi2InRhActionPerformed(evt);
            }
        });

        humi2PostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        humi2PostLabel.setText("RH");

        light2PreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        light2PreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        light2PreLabel.setText("Light");

        light2InLux.setEditable(false);
        light2InLux.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        light2InLux.setText("10");
        light2InLux.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                light2InLuxActionPerformed(evt);
            }
        });

        light2PostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        light2PostLabel.setText("Lux");

        occupancy2Label.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        occupancy2Label.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        occupancy2Label.setText("Occupancy");

        Occupancy2.setEditable(false);
        Occupancy2.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        Occupancy2.setText("Zero");
        Occupancy2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                Occupancy2ActionPerformed(evt);
            }
        });

        zone2Button.setFont(new java.awt.Font("Tahoma", 1, 24)); // NOI18N
        zone2Button.setText("Go to Reader's Zone #2 Screen");
        zone2Button.setActionCommand("ButtonZone2");
        zone2Button.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                zone2ButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout zone2PanelLayout = new javax.swing.GroupLayout(zone2Panel);
        zone2Panel.setLayout(zone2PanelLayout);
        zone2PanelLayout.setHorizontalGroup(
            zone2PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, zone2PanelLayout.createSequentialGroup()
                .addGroup(zone2PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(temp2PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 148, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(zone2PanelLayout.createSequentialGroup()
                        .addComponent(humi2PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(10, 10, 10)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(zone2PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(zone2PanelLayout.createSequentialGroup()
                        .addComponent(temp2InC, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(temp2PostLabel))
                    .addGroup(zone2PanelLayout.createSequentialGroup()
                        .addComponent(humi2InRh, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(humi2PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(zone2PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(occupancy2Label)
                    .addComponent(light2PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(zone2PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(zone2PanelLayout.createSequentialGroup()
                        .addComponent(light2InLux, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(light2PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(Occupancy2, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(43, 43, 43))
            .addGroup(zone2PanelLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(zone2Button, javax.swing.GroupLayout.PREFERRED_SIZE, 512, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        zone2PanelLayout.setVerticalGroup(
            zone2PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(zone2PanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(zone2PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(zone2PanelLayout.createSequentialGroup()
                        .addGroup(zone2PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(temp2PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(temp2InC, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(temp2PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(7, 7, 7)
                        .addGroup(zone2PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(humi2PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(humi2InRh, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(humi2PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(zone2PanelLayout.createSequentialGroup()
                        .addGroup(zone2PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(light2InLux, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(light2PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(light2PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(7, 7, 7)
                        .addGroup(zone2PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(occupancy2Label, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(Occupancy2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(zone2Button, javax.swing.GroupLayout.DEFAULT_SIZE, 47, Short.MAX_VALUE)
                .addContainerGap())
        );

        zone3Panel.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));

        temp3PreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        temp3PreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        temp3PreLabel.setText("Temperature");

        temp3InC.setEditable(false);
        temp3InC.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        temp3InC.setText("27");
        temp3InC.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                temp3InCActionPerformed(evt);
            }
        });

        temp3PostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        temp3PostLabel.setText("C");

        humi3PreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        humi3PreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        humi3PreLabel.setText("Humidity");

        humi3InRh.setEditable(false);
        humi3InRh.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        humi3InRh.setText("37");
        humi3InRh.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                humi3InRhActionPerformed(evt);
            }
        });

        humi3PostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        humi3PostLabel.setText("RH");

        light3PreLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        light3PreLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        light3PreLabel.setText("Light");

        light3InLux.setEditable(false);
        light3InLux.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        light3InLux.setText("11");
        light3InLux.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                light3InLuxActionPerformed(evt);
            }
        });

        light3PostLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        light3PostLabel.setText("Lux");

        occupancy3Label.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        occupancy3Label.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        occupancy3Label.setText("Occupancy");

        Occupancy3.setEditable(false);
        Occupancy3.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        Occupancy3.setText("Under");
        Occupancy3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                Occupancy3ActionPerformed(evt);
            }
        });

        zone3Button.setFont(new java.awt.Font("Tahoma", 1, 24)); // NOI18N
        zone3Button.setText("Go to Reader's Zone #3 Screen");
        zone3Button.setActionCommand("ButtonZone3");
        zone3Button.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                zone3ButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout zone3PanelLayout = new javax.swing.GroupLayout(zone3Panel);
        zone3Panel.setLayout(zone3PanelLayout);
        zone3PanelLayout.setHorizontalGroup(
            zone3PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, zone3PanelLayout.createSequentialGroup()
                .addGroup(zone3PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(temp3PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 148, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(zone3PanelLayout.createSequentialGroup()
                        .addComponent(humi3PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(10, 10, 10)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(zone3PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(zone3PanelLayout.createSequentialGroup()
                        .addComponent(temp3InC, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(temp3PostLabel))
                    .addGroup(zone3PanelLayout.createSequentialGroup()
                        .addComponent(humi3InRh, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(humi3PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(zone3PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(occupancy3Label)
                    .addComponent(light3PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(zone3PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(zone3PanelLayout.createSequentialGroup()
                        .addComponent(light3InLux, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(light3PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(Occupancy3, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(41, 41, 41))
            .addGroup(zone3PanelLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(zone3Button, javax.swing.GroupLayout.PREFERRED_SIZE, 512, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        zone3PanelLayout.setVerticalGroup(
            zone3PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(zone3PanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(zone3PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(zone3PanelLayout.createSequentialGroup()
                        .addGroup(zone3PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(temp3PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(temp3InC, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(temp3PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(7, 7, 7)
                        .addGroup(zone3PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(humi3PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(humi3InRh, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(humi3PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(zone3PanelLayout.createSequentialGroup()
                        .addGroup(zone3PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(light3InLux, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(light3PostLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(light3PreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(7, 7, 7)
                        .addGroup(zone3PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(occupancy3Label, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(Occupancy3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(zone3Button, javax.swing.GroupLayout.DEFAULT_SIZE, 47, Short.MAX_VALUE)
                .addContainerGap())
        );

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(zone1Panel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(zone2Panel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(zone3Panel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(zone0Panel, javax.swing.GroupLayout.PREFERRED_SIZE, 548, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(28, Short.MAX_VALUE))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(zone0Panel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(zone1Panel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(zone2Panel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(zone3Panel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        antSelScrollPane.setViewportView(jPanel1);

        javax.swing.GroupLayout antselectPanelLayout = new javax.swing.GroupLayout(antselectPanel);
        antselectPanel.setLayout(antselectPanelLayout);
        antselectPanelLayout.setHorizontalGroup(
            antselectPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(antselectPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(antselectPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(antselectPanelLayout.createSequentialGroup()
                        .addComponent(readerNoLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 101, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(readerNoComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, 71, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addComponent(antSelScrollPane, javax.swing.GroupLayout.Alignment.TRAILING))
                .addContainerGap())
        );
        antselectPanelLayout.setVerticalGroup(
            antselectPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(antselectPanelLayout.createSequentialGroup()
                .addGroup(antselectPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(readerNoLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 64, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(readerNoComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(17, 17, 17)
                .addComponent(antSelScrollPane, javax.swing.GroupLayout.PREFERRED_SIZE, 610, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(antselectPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(tablePanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(tablePanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(antselectPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void antSelClosing(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_antSelClosing
        myController.closePerformed();
    }//GEN-LAST:event_antSelClosing

    private void zone3ButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_zone3ButtonActionPerformed
        // TODO add your handling code here:
        myController.guiActionPerformed(evt);
    }//GEN-LAST:event_zone3ButtonActionPerformed

    private void Occupancy3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_Occupancy3ActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_Occupancy3ActionPerformed

    private void light3InLuxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_light3InLuxActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_light3InLuxActionPerformed

    private void humi3InRhActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_humi3InRhActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_humi3InRhActionPerformed

    private void temp3InCActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_temp3InCActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_temp3InCActionPerformed

    private void zone2ButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_zone2ButtonActionPerformed
        // TODO add your handling code here:
        myController.guiActionPerformed(evt);
        //new AntDataUI("2").setVisible(true);
    }//GEN-LAST:event_zone2ButtonActionPerformed

    private void Occupancy2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_Occupancy2ActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_Occupancy2ActionPerformed

    private void light2InLuxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_light2InLuxActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_light2InLuxActionPerformed

    private void humi2InRhActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_humi2InRhActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_humi2InRhActionPerformed

    private void temp2InCActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_temp2InCActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_temp2InCActionPerformed

    private void zone0ButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_zone0ButtonActionPerformed
        // TODO add your handling code here:
        myController.guiActionPerformed(evt);
    }//GEN-LAST:event_zone0ButtonActionPerformed

    private void Occupancy0ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_Occupancy0ActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_Occupancy0ActionPerformed

    private void light0InLuxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_light0InLuxActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_light0InLuxActionPerformed

    private void humi0InRhActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_humi0InRhActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_humi0InRhActionPerformed

    private void temp0InCActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_temp0InCActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_temp0InCActionPerformed

    private void readerNoComboBoxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_readerNoComboBoxActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_readerNoComboBoxActionPerformed

    private void zone1ButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_zone1ButtonActionPerformed
        // TODO add your handling code here:
        myController.guiActionPerformed(evt);
    }//GEN-LAST:event_zone1ButtonActionPerformed

    private void Occupancy1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_Occupancy1ActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_Occupancy1ActionPerformed

    private void light1InLuxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_light1InLuxActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_light1InLuxActionPerformed

    private void humi1InRhActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_humi1InRhActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_humi1InRhActionPerformed

    private void temp1InCActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_temp1InCActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_temp1InCActionPerformed

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
            java.util.logging.Logger.getLogger(AntSelectV2UI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(AntSelectV2UI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(AntSelectV2UI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(AntSelectV2UI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new AntSelectV2UI(myController).setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JTextField Occupancy0;
    private javax.swing.JTextField Occupancy1;
    private javax.swing.JTextField Occupancy2;
    private javax.swing.JTextField Occupancy3;
    private javax.swing.JScrollPane antSelScrollPane;
    private javax.swing.JPanel antselectPanel;
    private javax.swing.JLabel hum0PreLabel;
    private javax.swing.JLabel hum0iPostLabel;
    private javax.swing.JTextField humi0InRh;
    private javax.swing.JTextField humi1InRh;
    private javax.swing.JLabel humi1PostLabel;
    private javax.swing.JLabel humi1PreLabel;
    private javax.swing.JTextField humi2InRh;
    private javax.swing.JLabel humi2PostLabel;
    private javax.swing.JLabel humi2PreLabel;
    private javax.swing.JTextField humi3InRh;
    private javax.swing.JLabel humi3PostLabel;
    private javax.swing.JLabel humi3PreLabel;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JTextField light0InLux;
    private javax.swing.JLabel light0PostLabel;
    private javax.swing.JLabel light0PreLabel;
    private javax.swing.JTextField light1InLux;
    private javax.swing.JLabel light1PostLabel;
    private javax.swing.JLabel light1PreLabel;
    private javax.swing.JTextField light2InLux;
    private javax.swing.JLabel light2PostLabel;
    private javax.swing.JLabel light2PreLabel;
    private javax.swing.JTextField light3InLux;
    private javax.swing.JLabel light3PostLabel;
    private javax.swing.JLabel light3PreLabel;
    private javax.swing.JTable nameAndLocTable;
    private javax.swing.JLabel occupancy0Label;
    private javax.swing.JLabel occupancy1Label;
    private javax.swing.JLabel occupancy2Label;
    private javax.swing.JLabel occupancy3Label;
    private javax.swing.JComboBox<String> readerNoComboBox;
    private javax.swing.JLabel readerNoLabel;
    private javax.swing.JPanel tablePanel;
    private javax.swing.JTextField temp0InC;
    private javax.swing.JLabel temp0PostLabel;
    private javax.swing.JLabel temp0PreLabel;
    private javax.swing.JTextField temp1InC;
    private javax.swing.JLabel temp1PostLabel;
    private javax.swing.JLabel temp1PreLabel;
    private javax.swing.JTextField temp2InC;
    private javax.swing.JLabel temp2PostLabel;
    private javax.swing.JLabel temp2PreLabel;
    private javax.swing.JTextField temp3InC;
    private javax.swing.JLabel temp3PostLabel;
    private javax.swing.JLabel temp3PreLabel;
    private javax.swing.JButton zone0Button;
    private javax.swing.JPanel zone0Panel;
    private javax.swing.JButton zone1Button;
    private javax.swing.JPanel zone1Panel;
    private javax.swing.JButton zone2Button;
    private javax.swing.JPanel zone2Panel;
    private javax.swing.JButton zone3Button;
    private javax.swing.JPanel zone3Panel;
    // End of variables declaration//GEN-END:variables
}
