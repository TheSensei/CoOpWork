/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package glue;

import model.AppConfig;
import model.AppConfigDB;
import model.RFIDReader;
import model.ALRF800RFIDReader;

import controller.Controller;

/**
 *
 * @author mark
 */
public class Glue {

	private int start_value = 10;	//initialise model, which in turn initialises view

	public Glue() {

		//create Model, Controller and View:
                
		AppConfigDB appConfigDB = new AppConfigDB();
		AppConfig appConfig = appConfigDB.loadAppConfig();
		RFIDReader myModel = new ALRF800RFIDReader(appConfig.getReaderIpAddress());

		//create Controller. tell it about Model and View, initialise model
		Controller myController = new Controller();
                
                myModel.addObserver(myController);
                
                
                AzbilGuiApplication myView = new AzbilGuiApplication();  // new View();
                myView.addController(myController);
		myController.addReaderModel(myModel);
		myController.addView(myView);
                
                myView.createScreensAndTellController(myController);
                
	} //Glue()

} //Glue
