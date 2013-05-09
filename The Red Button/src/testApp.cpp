#include "testApp.h"
int comMu;


/*////////////////////////////////////////////////////////////////
//////////////////// THE RED BUTTON PROJECT //////////////////////
//////////////////////////////////////////////////////////////////

//  Button + Fader [ARDUINO] ---Serial---> [OF] ---MIDI---> [DAW]
//  [ARDUINO] button D0, on/off ; fader A0, 0-1023
//  [OF] open up serial port, look for arduino, set up connection
//  [OF] read from ARDUINO, send out values as MIDI message:
//      * Button - Bypass CC 141
//      * Fader  - Mix

///////////////////////////////////////////////////////////////*/



//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
    ofBackground(0);
	ofSetLogLevel(OF_LOG_VERBOSE);
    ofEnableSmoothing();

    font.loadFont("DIN.otf", 40);
    smallFont.loadFont("DIN.otf", 14);

    //////////////////////////////////////////////
    //////////////  MIDI SETUPS  /////////////////
    //////////////////////////////////////////////
    
	// print input ports to console
	midiIn.listPorts(); // via instance
	//ofxMidiIn::listPorts(); // via static as well
	
	// open port by number (you may need to change this)
	midiIn.openPort(0);
	//midiIn.openPort("IAC Pure Data In");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input");	// open a virtual port
    midiOut.openVirtualPort("ofxMidiOut");		// open a virtual port

	
	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);
	
	// add testApp as a listener
	midiIn.addListener(this);
	
	// print received messages to the console
	midiIn.setVerbose(true);
    
    serial.listDevices();
    
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();

	int baud = 9600;

	// call setupArduino()
	ofAddListener(ard.EInitialized, this, &testApp::setupArduino);
	bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)
    

}

//--------------------------------------------------------------
void testApp::update() {
    updateArduino();
}

//--------------------------------------------------------------
void testApp::draw() {
    
    //////////////////////////////////////////////
    /////////////  ARDUINO SETUPS  ///////////////
    //////////////////////////////////////////////
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    ofSetColor(255, 255, 255);
    switch (comMu) {
        case 0:
            font.drawString(deviceList[0].getDevicePath(), 50, 100);
            break;
        case 1:
            font.drawString(deviceList[1].getDevicePath(), 50, 100);
            break;
        case 2:
            font.drawString(deviceList[2].getDevicePath(), 50, 100);
            break;
        case 3:
            font.drawString(deviceList[3].getDevicePath(), 50, 100);
            break;
        case 4:
            font.drawString(deviceList[4].getDevicePath(), 50, 100);
            break;
        case 5:
            font.drawString(deviceList[5].getDevicePath(), 50, 100);
            break;
        case 6:
            font.drawString(deviceList[6].getDevicePath(), 50, 100);
            break;
        case 7:
            font.drawString(deviceList[7].getDevicePath(), 50, 100);
            break;
        case 8:
            font.drawString(deviceList[8].getDevicePath(), 50, 100);
            break;
        case 9:
            font.drawString(deviceList[9].getDevicePath(), 50, 100);
            break;
        case 10:
            font.drawString(deviceList[10].getDevicePath(), 50, 100);
            break;
        default:
            break;
    }
    

    
    
    ofSetColor(255, 255, 255);
    if (!bSetupArduino){

	} else {

	}
    
    // THE ON/OFF BUTTON
    smallFont.drawString("The Button = " + ofToString(buttonVal), 20, 172);

    
    if(buttonVal == 1) {
        //The CC for Bypass is 114
        ofSetColor(255,0,0);
        ofRectRounded(320, 142, 40, 40, 10);
        
	}
	else {
        ofSetColor(255);
		ofRectRounded(320, 142, 40, 40, 10);
        
	}
	
//    // THE FADER
//    int faderSendVal = ofMap(faderVal, 0, 1023, 0, 127);
//    
//    midiOut.sendControlChange(10, 10, faderSendVal);
//    midiOut.sendControlChange(10, 11, buttonVal);
    
}

//--------------------------------------------------------------
void testApp::exit() {
	
	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}

//--------------------------------------------------------------
void testApp::newMidiMessage(ofxMidiMessage& msg) {

	// make a copy of the latest message
	midiMessage = msg;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    int baud = 9600;
    serial.setup(comMu, baud);
    
    switch(key) {
            
        case 'l':
			//midiIn.listPorts();
			break;
            
        case OF_KEY_RIGHT:
            comMu = comMu + 1;
            serial.setup(comMu, baud);
            ard.connect(deviceList[comMu].getDevicePath(), 57600);
            break;
            
        case OF_KEY_LEFT:
            comMu = comMu - 1;
            serial.setup(comMu, baud);
            ard.connect(deviceList[comMu].getDevicePath(), 57600);
            break;
        default:
            break;
            
 
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
    }

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {

    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void testApp::mouseReleased() {

}

/*////////////////////////////////////////////////////////////////
/////////////////// ARDUINO setup + dataRead /////////////////////
////////////////////////////////////////////////////////////////*/

//--------------------------------------------------------------
void testApp::setupArduino(const int & version) {
	
	// remove listener because we don't need it anymore
	ofRemoveListener(ard.EInitialized, this, &testApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    cout << ard.getFirmwareName() << endl;
    cout << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion() << endl;
    
    // set pin A0 to analog input
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
    
    // set pin D13 as digital input
    ard.sendDigitalPinMode(13, ARD_INPUT);
    
    ofAddListener(ard.EDigitalPinChanged, this, &testApp::digitalPinChanged);
    ofAddListener(ard.EAnalogPinChanged, this, &testApp::analogPinChanged);
}

//--------------------------------------------------------------
void testApp::updateArduino(){
    
	// update the arduino, get any data or messages.
    // the call to ard.update() is required
	ard.update();
    
}

// digital pin event handler, called whenever a digital pin value has changed
// note: if an analog pin has been set as a digital pin, it will be handled
// by the digitalPinChanged function rather than the analogPinChanged function.
void testApp::digitalPinChanged(const int & pinNum) {
   
    buttonVal = ard.getDigital(13);
    midiOut.sendControlChange(11, 11, 127);

}

// analog pin event handler, called whenever an analog pin value has changed
//--------------------------------------------------------------
void testApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input.
    
    faderVal = ard.getAnalog(0);
    int faderSendVal = ofMap(faderVal, 0, 1023, 0, 127);
    midiOut.sendControlChange(10, 10, faderSendVal);
    
    
}

