#pragma once

#include "ofMain.h"

#include "ofxMidi.h"


class testApp : public ofBaseApp, public ofxMidiListener {
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
	void newMidiMessage(ofxMidiMessage& eventArgs);
	
	stringstream text;
	
	ofxMidiIn midiIn;
    ofxMidiOut midiOut;
	ofxMidiMessage midiMessage;
    
    ofTrueTypeFont		font;
    ofTrueTypeFont      smallFont;
    
////////////////////[Z]////////////////////////
    
    //for midi inputs
    int channel;
	
	unsigned int currentPgm;
	int note, velocity;
    int pan, bend, touch, polytouch;
    
    //void buttonPressed(int buttonStatus);
    //void faderMoved(int faderStatus);
    
    //arduino serial setups

    ofSerial    serial;
    ofArduino	ard;
	bool		bSetupArduino;			// flag variable for setting up arduino once
    
private:

    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
    void updateArduino();

    int buttonVal;
    int faderVal;
};
