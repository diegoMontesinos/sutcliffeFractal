/*
 * Implementation of the SutcliffeFractal app with oF.
 *
 * Diego Montesinos (diegoa.montesinos at gmail.com)
 * January, 2015
 */

#include "ofApp.h"
#include <math.h>

void ofApp::setup () {

	// Set the sides and the radius
	nSides = 5;
	radius = 300.0;

	// Set weight, the factor scale and depth
	weight = 0.5;
	fScale = 0.35;
	depth = 4;

	// Generate the polygon base
	generateBasePolygon();

	// Set the of params
	ofSetVerticalSync(true);
	ofNoFill();
	ofSetHexColor(0x000000);
	ofBackgroundHex(0xffffff);
	ofEnableAntiAliasing();

	// Set the GUI
	sidesCtrl.addListener(this, &ofApp::sidesChanged);
	radiusCtrl.addListener(this, &ofApp::radiusChanged);

	gui.setup();
	gui.add(sidesCtrl.setup("Polygon sides", 5, 3, 10));
	gui.add(radiusCtrl.setup("Polygon radius", 300.0, 200.0, 500.0));

	gui.add(weightCtrl.setup("Middle proportion", 0.5, 0.0, 1.0));
	gui.add(fScaleCtrl.setup("Factor scale", 0.35, 0.1, 0.5));
	gui.add(depthCtrl.setup("Recursion depth", 4, 1, 6));

	gui.add(constantLW.setup("Constant lineWidth", true));
	gui.add(invertColors.setup("Invert colors", false));
}

void ofApp::update () {

	// Get the vars from the gui
	weight = weightCtrl;
	fScale = fScaleCtrl;
	depth = depthCtrl;

	if(invertColors) {
		ofBackgroundHex(0x000000);
		ofSetHexColor(0xffffff);
	} else {
		ofBackgroundHex(0xffffff);
		ofSetHexColor(0x000000);
	}
}

void ofApp::draw () {

	// Draw the GUI
	if(!hGui) {
		gui.draw();
	}

	// Get the center and render sutcliffe fractal
	ofVec2f center (ofGetWidth() / 2.0, ofGetHeight() / 2.0);
	this->sutcliffe(basePolygon, center, depth);
}

void ofApp::keyPressed(int key) {
	if(key == 'h') {
		hGui = !hGui;
	}
	if(key == 's') {
		ofSaveFrame();
	}
}

void ofApp::sidesChanged(int & newSides) {
	nSides = newSides;
	generateBasePolygon();
}

void ofApp::radiusChanged(float & newRadius) {
	radius = newRadius;
	generateBasePolygon();
}

void ofApp::generateBasePolygon () {

	// Clear the base polygon
	basePolygon.clear();

	// Calc the angle amount
	float angleAmt = (2.0 * M_PI) / ((float) nSides);

	// Set the angle
	float angle = 0.0;
	while ( angle <= (2.0 * M_PI) ) {

		// Create a point
		float x = (cos(angle) * radius) + (ofGetWidth() / 2.0);
		float y = (sin(angle) * radius) + (ofGetHeight() / 2.0);
		ofVec2f point (x, y);

		// Store in
		basePolygon.push_back(point);

		// Increase the angle
		angle += angleAmt;
	}
}

void ofApp::sutcliffe (std::vector<ofVec2f> polygon, ofVec2f center, int n) {

	// Get the polygon size
	int pSize = polygon.size();

	// Set the line width if its necessary
	if(constantLW) {
		ofSetLineWidth(1.0);
	} else {
		ofSetLineWidth(ofMap(n, 0, depth, 1.1, 3.5));
	}

	// Draw the polygon
	for (int i = 0; i < pSize; i++) {
		ofVec2f point = polygon[i];
		ofVec2f nextPoint = polygon[(i + 1) % pSize];

		ofLine(point.x, point.y, nextPoint.x, nextPoint.y);
	}

	// If there are more steps
	if(n > 0) {

		// Firts create the inner and middle points 
		std::vector<ofVec2f> innerPoints;
		std::vector<ofVec2f> middlePoints;
		for (int i = 0; i < pSize; i++) {

			// Get the point and next point
			ofVec2f point = polygon[i];
			ofVec2f nextPoint = polygon[(i + 1) % pSize];

			// Get the middle point in proportion
			ofVec2f middlePoint = (point * weight) + (nextPoint * (1.0 - weight));
			middlePoints.push_back(middlePoint);

			// Get the direction vector
			ofVec2f direction;
			if(pSize == 5) {
				direction = polygon[(i + 3) % 5] - middlePoint;
			} else {
				direction = center - middlePoint;
			}

			// Get the radius
			float r = (center - middlePoint).length();

			// Normalize and scale the direction
			direction.normalize();
			direction.scale(r * fScale);

			// Create and store the inner point
			ofVec2f innerPoint = middlePoint + direction;
			innerPoints.push_back(innerPoint);
		}

		// Create the inner pentagons
		for (int i = 0; i < pSize; ++i) {
			std::vector<ofVec2f> pentagon;

			// Create a sutfliffe pentagon
			pentagon.push_back(middlePoints[i]);
			pentagon.push_back(innerPoints[i]);
			pentagon.push_back(innerPoints[(i + 1) % pSize]);
			pentagon.push_back(middlePoints[(i + 1) % pSize]);
			pentagon.push_back(polygon[(i + 1) % pSize]);

			// Call recursively with the pentagon
			this->sutcliffe (pentagon, calcCentroid(pentagon), n - 1);
		}

		// Call recursively with the inner points
		this->sutcliffe (innerPoints, center, n - 1);
	}
}

ofVec2f ofApp::calcCentroid (std::vector<ofVec2f> polygon) {

	// Get the sum of positions
	float x = 0.0, y = 0.0;
	for (int i = 0; i < polygon.size(); i++) {
		x += polygon[i].x;
		y += polygon[i].y;
	}

	// Get the average (centroid)
	x = x / (float) polygon.size();
	y = y / (float) polygon.size();

	ofVec2f centroid (x, y);
	return centroid;
}
