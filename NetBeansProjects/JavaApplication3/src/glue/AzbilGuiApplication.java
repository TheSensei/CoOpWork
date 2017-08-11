/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package glue;

// import <libname>  // MRB
/**
 *
 * @author mark
 */
//public class AzbilGuiApplication {
import java.awt.Button;
import java.util.Observable;		//for update();
import java.awt.event.ActionListener;	//for addController()

import view.BusyUI;
import view.WarningUI;
import view.ErrorUI;
import view.TagSetupUI;
import view.AntSetupUI;
import view.SetupSelectUI;
import view.AntDataUI;
import view.AntSelectV2UI;

import controller.Controller;


public class AzbilGuiApplication implements java.util.Observer {    
    //private Button button;
    private static Controller myController;
        //my ErrorUI = new ErrorUI().setVisible(false);
    //myBusyUI = new BusyUI().setVisible(false);
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new Glue();
                //new AntSelectV2UI(myController).setVisible(true);
            }
        });
    }

    public void update(Observable obs, Object obj) {

     } //update()
        
public void addController(Controller c){
		//System.out.println("View      : adding controller");
		//button.addActionListener(controller);	//need instance of controller before can add it as a listener 
                this.myController = c;
	} //addController()    
 
public void createScreensAndTellController(Controller c) {
    new BusyUI(c).setVisible(false);
    new WarningUI(c).setVisible(false);
    new ErrorUI(c).setVisible(false);
    new TagSetupUI(c).setVisible(false);
    new AntSetupUI(c).setVisible(false);
    new SetupSelectUI(c).setVisible(false);
    new AntDataUI(c).setVisible(false);
    new AntSelectV2UI(c).setVisible(false);
    c.startController();
}

}
