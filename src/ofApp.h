/*
 * Declaration of the SutcliffeFractal app with oF.
 *
 * Diego Montesinos (diegoa.montesinos at gmail.com)
 * January, 2015
 */

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include <vector>

class ofApp : public ofBaseApp{
	public:

		///// METHODS

		void setup ();
		void update ();
		void draw ();

		void keyPressed(int key);

		void generateBasePolygon ();

		void sutcliffe (std::vector<ofVec2f>, ofVec2f, int);
		ofVec2f calcCentroid (std::vector<ofVec2f>);

		///// DATA

		int nSides;
		float radius;
		std::vector<ofVec2f> basePolygon;

		float weight;
		float fScale;
		int depth;

		///// GUI

		void sidesChanged(int &);
		void radiusChanged(float &);

		ofxPanel gui;

		ofxIntSlider sidesCtrl;
		ofxFloatSlider radiusCtrl;

		ofxFloatSlider weightCtrl;
		ofxFloatSlider fScaleCtrl;
		ofxIntSlider depthCtrl;

		ofxToggle constantLW;
		ofxToggle invertColors;
        //El mesh guarda vertices
        ofVboMesh mesh;
    
		bool hGui;
};
