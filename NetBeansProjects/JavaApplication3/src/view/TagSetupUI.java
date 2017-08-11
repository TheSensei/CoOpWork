/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package view;

import model.TagClass;
import model.SecurityClass;

import controller.Controller;

/**
 *
 * @author mark
 */
public class TagSetupUI extends javax.swing.JFrame {
    private static Controller myController;
    
    private static final String ENABLED_STRING = "Enabled";
    private static final String DISABLED_STRING = "Disabled";
    
    /**
     * Creates new form TagSetupUI
     */
    public TagSetupUI(Controller c) {
        myController = c;
        initComponents();
        myController.addTagSetupView(this);
    }
    
    public String getTagName() {
        return (nameFormattedTextField.getText());
    }

    public void setTagName(String myStg) {
        nameFormattedTextField.setText(myStg);
    }
    
    public String getTid() {
        return (tidFormattedTextField.getText());
    }

    public void setTid(String myStg) {
        tidFormattedTextField.setText(myStg);
    }
    
    public static TagClass intToEnumTagClass(int myInt) {
        switch (myInt) {
            case 0: return TagClass.Personnel;       // zero to enum 1 (starting enum value in java)
            case 1: return TagClass.Asset;
            case 2: return TagClass.Environmental;
            default: return TagClass.Personnel;
        }
    }
    
    public TagClass getTagClass() {
        return intToEnumTagClass(tagClassComboBox.getSelectedIndex());  // convert int to enum 
    }

    public void setTagClass(TagClass myEnum) {
        int myInt = myEnum.ordinal();
        tagClassComboBox.setSelectedIndex(myInt);
    }
    
    public static SecurityClass intToEnumSecClass(int myInt) {
        switch (myInt) {
            case 0: return SecurityClass.Low;       // zero to enum 1 (starting enum value in java)
            case 1: return SecurityClass.Medium;
            case 2: return SecurityClass.High;
            default: return SecurityClass.Low;
        }
    }
    
    public SecurityClass getSecClass() {
        return intToEnumSecClass(securityClassComboBox.getSelectedIndex());  // convert int to enum 
    }

    public void setSecClass(SecurityClass myEnum) {
        int myInt = myEnum.ordinal();
        securityClassComboBox.setSelectedIndex(myInt);
    }
    
    public String getNote() {
        return (notesFormattedTextField.getText());
    }

    public void setNote(String myStg) {
        notesFormattedTextField.setText(myStg);
    }
    
    public String getSection() {
        return (sectionFormattedTextField.getText());
    }

    public void setSection(String myStg) {
        sectionFormattedTextField.setText(myStg);
    }
    
    public String getFirm() {
        return (firmFormattedTextField.getText());
    }

    public void setFirm(String myStg) {
        firmFormattedTextField.setText(myStg);
    }
        
    public String getCellNo() {
        return (cellNoFormattedTextField.getText());
    }

    public void setCellNo(String myStg) {
        cellNoFormattedTextField.setText(myStg);
    }
    

    public boolean get_tag_enable() {
        String myStg = this.tagEnableState.getText();
        if (myStg.equals(ENABLED_STRING)) {
            return (true);  // tag enabled
        }
        else {
            return (false);  // tag disabled
        }
        
    }

    public void set_tag_enable(boolean bState) {
        if (bState == true) {
            this.tagEnableState.setText(ENABLED_STRING);
        }
        else {
            this.tagEnableState.setText(DISABLED_STRING);
        }
    }
    
    public void set_status_label(String myStg)
    {
        this.statusLabel.setText(myStg);
    }
    
    public void clearAlarms() {
        this.enableAlarmList.clearSelection();
    }
    
        
    public boolean get_r1A0_alarm_enable() {
        return (enableAlarmList.isSelectedIndex(0));
    }
    
    public void set_r1A0_alarm_enable() {
        //enableAlarmList.setSelectedIndex(0);  // Don't use, clears other indexes and only sets this index
        enableAlarmList.addSelectionInterval(0,0);        
    }
    
    public boolean get_r1A1_alarm_enable() {
        return (enableAlarmList.isSelectedIndex(1));
    }
    
    public void set_r1A1_alarm_enable() {
        //enableAlarmList.setSelectedIndex(1);
        enableAlarmList.addSelectionInterval(1,1);
    }

    public boolean get_r1A2_alarm_enable() {
        return (enableAlarmList.isSelectedIndex(2));
    }
    
    public void set_r1A2_alarm_enable() {
        //enableAlarmList.setSelectedIndex(2);
        enableAlarmList.addSelectionInterval(2,2);        
    }

    public boolean get_r1A3_alarm_enable() {
        return (enableAlarmList.isSelectedIndex(3));
    }
    
    public void set_r1A3_alarm_enable() {
        //enableAlarmList.setSelectedIndex(3);
        enableAlarmList.addSelectionInterval(3,3);        
    }

    
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jPanel1 = new javax.swing.JPanel();
        tagIdLabel = new javax.swing.JLabel();
        tidFormattedTextField = new javax.swing.JFormattedTextField();
        backButton = new javax.swing.JButton();
        acceptButton = new javax.swing.JButton();
        nameLabel = new javax.swing.JLabel();
        nameFormattedTextField = new javax.swing.JFormattedTextField();
        alarmCellNoLabel = new javax.swing.JLabel();
        firmFormattedTextField = new javax.swing.JFormattedTextField();
        sectionLabel = new javax.swing.JLabel();
        sectionFormattedTextField = new javax.swing.JFormattedTextField();
        firmLabel = new javax.swing.JLabel();
        cellNoFormattedTextField = new javax.swing.JFormattedTextField();
        notesLabel = new javax.swing.JLabel();
        notesFormattedTextField = new javax.swing.JFormattedTextField();
        securityClassComboBox = new javax.swing.JComboBox<>();
        tagClassLabel = new javax.swing.JLabel();
        tagClassComboBox = new javax.swing.JComboBox<>();
        enableAlarmListLabel = new javax.swing.JLabel();
        tagEnableButton = new javax.swing.JButton();
        tagEnableState = new javax.swing.JTextField();
        securityClassLabel = new javax.swing.JLabel();
        jScrollPane1 = new javax.swing.JScrollPane();
        enableAlarmList = new javax.swing.JList<>();
        statusLabel = new javax.swing.JLabel();

        tagIdLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        tagIdLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        tagIdLabel.setText("Tag EPC (exactly 24 hex digits):");

        try {
            tidFormattedTextField.setFormatterFactory(new javax.swing.text.DefaultFormatterFactory(new javax.swing.text.MaskFormatter("HHHHHHHHHHHHHHHHHHHHHHHH")));
        } catch (java.text.ParseException ex) {
            ex.printStackTrace();
        }
        tidFormattedTextField.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        tidFormattedTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                tidFormattedTextFieldActionPerformed(evt);
            }
        });

        backButton.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        backButton.setText("Back");
        backButton.setActionCommand("ButtonTagSetupBack");
        backButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                backButtonActionPerformed(evt);
            }
        });

        acceptButton.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        acceptButton.setText("Accept");
        acceptButton.setActionCommand("ButtonTagSetupAccept");
        acceptButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                acceptButtonActionPerformed(evt);
            }
        });

        nameLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        nameLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        nameLabel.setText("Name (0-32):");

        try {
            nameFormattedTextField.setFormatterFactory(new javax.swing.text.DefaultFormatterFactory(new javax.swing.text.MaskFormatter("?*******************************")));
        } catch (java.text.ParseException ex) {
            ex.printStackTrace();
        }
        nameFormattedTextField.setText("");
        nameFormattedTextField.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        nameFormattedTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                nameFormattedTextFieldActionPerformed(evt);
            }
        });

        alarmCellNoLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        alarmCellNoLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        alarmCellNoLabel.setText("Alarm Cell Phone No. (for future use):");

        try {
            firmFormattedTextField.setFormatterFactory(new javax.swing.text.DefaultFormatterFactory(new javax.swing.text.MaskFormatter("********************")));
        } catch (java.text.ParseException ex) {
            ex.printStackTrace();
        }
        firmFormattedTextField.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        firmFormattedTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                firmFormattedTextFieldActionPerformed(evt);
            }
        });

        sectionLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        sectionLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        sectionLabel.setText("Section (0-20):");

        try {
            sectionFormattedTextField.setFormatterFactory(new javax.swing.text.DefaultFormatterFactory(new javax.swing.text.MaskFormatter("********************")));
        } catch (java.text.ParseException ex) {
            ex.printStackTrace();
        }
        sectionFormattedTextField.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        sectionFormattedTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                sectionFormattedTextFieldActionPerformed(evt);
            }
        });

        firmLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        firmLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        firmLabel.setText("Firm (0-20):");

        try {
            cellNoFormattedTextField.setFormatterFactory(new javax.swing.text.DefaultFormatterFactory(new javax.swing.text.MaskFormatter("****************")));
        } catch (java.text.ParseException ex) {
            ex.printStackTrace();
        }
        cellNoFormattedTextField.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        cellNoFormattedTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cellNoFormattedTextFieldActionPerformed(evt);
            }
        });

        notesLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        notesLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        notesLabel.setText("Notes (0-40):");

        try {
            notesFormattedTextField.setFormatterFactory(new javax.swing.text.DefaultFormatterFactory(new javax.swing.text.MaskFormatter("****************************************")));
        } catch (java.text.ParseException ex) {
            ex.printStackTrace();
        }
        notesFormattedTextField.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        notesFormattedTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                notesFormattedTextFieldActionPerformed(evt);
            }
        });

        securityClassComboBox.setFont(new java.awt.Font("Tahoma", 0, 48)); // NOI18N
        securityClassComboBox.setModel(new javax.swing.DefaultComboBoxModel<>(new String[] { "Low", "Medium", "High" }));
        securityClassComboBox.setActionCommand("securityClassComboBoxChanged");
        securityClassComboBox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                securityClassComboBoxActionPerformed(evt);
            }
        });

        tagClassLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        tagClassLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        tagClassLabel.setText("Tag Class:");

        tagClassComboBox.setFont(new java.awt.Font("Tahoma", 0, 48)); // NOI18N
        tagClassComboBox.setModel(new javax.swing.DefaultComboBoxModel<>(new String[] { "Personnel", "Asset", "Environmental" }));
        tagClassComboBox.setActionCommand("tagClassComboBoxChanged");
        tagClassComboBox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                tagClassComboBoxActionPerformed(evt);
            }
        });

        enableAlarmListLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        enableAlarmListLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        enableAlarmListLabel.setText("Enable Alarm List:");

        tagEnableButton.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        tagEnableButton.setText("Tag Enable/Disable");
        tagEnableButton.setActionCommand("ButtonTagEnable");
        tagEnableButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                tagEnableButtonActionPerformed(evt);
            }
        });

        tagEnableState.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        tagEnableState.setText("Disabled");
        tagEnableState.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                tagEnableStateActionPerformed(evt);
            }
        });

        securityClassLabel.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        securityClassLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        securityClassLabel.setText("Security Class:");

        enableAlarmList.setFont(new java.awt.Font("Tahoma", 0, 24)); // NOI18N
        enableAlarmList.setModel(new javax.swing.AbstractListModel<String>() {
            String[] strings = { "Enable Alarm R1 A0", "Enable Alarm R1 A1", "Enable Alarm R1 A2", "Enable Alarm R1 A3" };
            public int getSize() { return strings.length; }
            public String getElementAt(int i) { return strings[i]; }
        });
        jScrollPane1.setViewportView(enableAlarmList);

        statusLabel.setFont(new java.awt.Font("Tahoma", 1, 24)); // NOI18N
        statusLabel.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        statusLabel.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Status:", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, new java.awt.Font("Tahoma", 0, 14))); // NOI18N

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, jPanel1Layout.createSequentialGroup()
                                        .addComponent(securityClassLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 225, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(securityClassComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, 209, javax.swing.GroupLayout.PREFERRED_SIZE))
                                    .addGroup(jPanel1Layout.createSequentialGroup()
                                        .addComponent(tagClassLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 122, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(tagClassComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, 312, javax.swing.GroupLayout.PREFERRED_SIZE))
                                    .addComponent(alarmCellNoLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 425, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(sectionLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 425, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(firmLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 425, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(notesLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 425, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addComponent(nameLabel, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 425, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addComponent(tagIdLabel, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 425, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(notesFormattedTextField)
                            .addComponent(sectionFormattedTextField)
                            .addComponent(cellNoFormattedTextField)
                            .addComponent(firmFormattedTextField)
                            .addComponent(nameFormattedTextField)
                            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createSequentialGroup()
                                .addGap(0, 150, Short.MAX_VALUE)
                                .addComponent(tagEnableButton)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(tagEnableState, javax.swing.GroupLayout.PREFERRED_SIZE, 194, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addComponent(tidFormattedTextField)))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createSequentialGroup()
                        .addGap(0, 0, Short.MAX_VALUE)
                        .addComponent(enableAlarmListLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 205, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 247, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addComponent(backButton, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(85, 85, 85)
                        .addComponent(statusLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 647, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(acceptButton, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(tagIdLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(tidFormattedTextField, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 64, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(nameLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(nameFormattedTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(sectionLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(sectionFormattedTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(firmLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(firmFormattedTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(notesLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(notesFormattedTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(alarmCellNoLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(cellNoFormattedTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(18, 18, 18)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel1Layout.createSequentialGroup()
                                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(tagClassComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(tagClassLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(16, 16, 16)
                                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                    .addComponent(securityClassLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                        .addComponent(enableAlarmListLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 59, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addComponent(securityClassComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))))
                            .addGroup(jPanel1Layout.createSequentialGroup()
                                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                    .addComponent(tagEnableState, javax.swing.GroupLayout.PREFERRED_SIZE, 56, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(tagEnableButton, javax.swing.GroupLayout.PREFERRED_SIZE, 56, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(18, 18, 18)
                                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 169, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addGap(18, 18, Short.MAX_VALUE)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(statusLabel, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 64, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(acceptButton, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGap(0, 0, Short.MAX_VALUE)
                        .addComponent(backButton, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jPanel1, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void tagEnableStateActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_tagEnableStateActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_tagEnableStateActionPerformed

    private void tagEnableButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_tagEnableButtonActionPerformed
        // TODO add your handling code here:
        myController.guiActionPerformed(evt);
    }//GEN-LAST:event_tagEnableButtonActionPerformed

    private void tagClassComboBoxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_tagClassComboBoxActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_tagClassComboBoxActionPerformed

    private void securityClassComboBoxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_securityClassComboBoxActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_securityClassComboBoxActionPerformed

    private void notesFormattedTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_notesFormattedTextFieldActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_notesFormattedTextFieldActionPerformed

    private void cellNoFormattedTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cellNoFormattedTextFieldActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_cellNoFormattedTextFieldActionPerformed

    private void sectionFormattedTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_sectionFormattedTextFieldActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_sectionFormattedTextFieldActionPerformed

    private void firmFormattedTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_firmFormattedTextFieldActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_firmFormattedTextFieldActionPerformed

    private void nameFormattedTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_nameFormattedTextFieldActionPerformed
        // TODO add your handling code here:
        myController.guiTagNameEntered(evt, nameFormattedTextField.getText());  // Note: evt.getActionCommand() returns the name string provided by the user, not a constant string like other widgets with an actionCommand property
    }//GEN-LAST:event_nameFormattedTextFieldActionPerformed

    private void acceptButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_acceptButtonActionPerformed
        // TODO add your handling code here:
        myController.guiActionPerformed(evt);
    }//GEN-LAST:event_acceptButtonActionPerformed

    private void backButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_backButtonActionPerformed
        // TODO add your handling code here:
        myController.guiActionPerformed(evt);
        //setVisible(false);
    }//GEN-LAST:event_backButtonActionPerformed

    private void tidFormattedTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_tidFormattedTextFieldActionPerformed
        // TODO add your handling code here:
        myController.guiTagTidEntered(evt, nameFormattedTextField.getText());  // Note: evt.getActionCommand() returns the TID string provided by the user, not a constant string like other widgets with an actionCommand property
    }//GEN-LAST:event_tidFormattedTextFieldActionPerformed

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
            java.util.logging.Logger.getLogger(TagSetupUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(TagSetupUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(TagSetupUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(TagSetupUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new TagSetupUI(myController).setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton acceptButton;
    private javax.swing.JLabel alarmCellNoLabel;
    private javax.swing.JButton backButton;
    private javax.swing.JFormattedTextField cellNoFormattedTextField;
    private javax.swing.JList<String> enableAlarmList;
    private javax.swing.JLabel enableAlarmListLabel;
    private javax.swing.JFormattedTextField firmFormattedTextField;
    private javax.swing.JLabel firmLabel;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JFormattedTextField nameFormattedTextField;
    private javax.swing.JLabel nameLabel;
    private javax.swing.JFormattedTextField notesFormattedTextField;
    private javax.swing.JLabel notesLabel;
    private javax.swing.JFormattedTextField sectionFormattedTextField;
    private javax.swing.JLabel sectionLabel;
    private javax.swing.JComboBox<String> securityClassComboBox;
    private javax.swing.JLabel securityClassLabel;
    private javax.swing.JLabel statusLabel;
    private javax.swing.JComboBox<String> tagClassComboBox;
    private javax.swing.JLabel tagClassLabel;
    private javax.swing.JButton tagEnableButton;
    private javax.swing.JTextField tagEnableState;
    private javax.swing.JLabel tagIdLabel;
    private javax.swing.JFormattedTextField tidFormattedTextField;
    // End of variables declaration//GEN-END:variables
}
