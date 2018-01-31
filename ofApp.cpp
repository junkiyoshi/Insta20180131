#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofBackground(255);
	ofSetWindowTitle("Insta");

	ofSetColor(0);
	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 130, true, false, true);

	this->box2d.init();
	this->box2d.setGravity(0, 50);
	this->box2d.createBounds();
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();

	this->eage_1 = ofxBox2dEdge();
	this->eage_1.addVertex(ofVec2f(ofGetWidth() / 2 - 150, ofGetHeight() / 2 + 100));
	this->eage_1.addVertex(ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2));
	this->eage_1.create(this->box2d.getWorld());

	this->eage_2 = ofxBox2dEdge();
	this->eage_2.addVertex(ofVec2f(ofGetWidth() / 2 + 150, ofGetHeight() / 2 + 100));
	this->eage_2.addVertex(ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2));
	this->eage_2.create(this->box2d.getWorld());
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = this->polygons.size() - 1; i >= 0; i--) {
		
		this->polygons_life[i] -= 1;

		if (this->polygons_life[i] < 20) {

			this->polygons[i].get()->destroy();
			this->polygons.erase(this->polygons.begin() + i);
			this->polygons_life.erase(this->polygons_life.begin() + i);
			this->polygons_color.erase(this->polygons_color.begin() + i);
		}
	}

	this->box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->polygons.size(); i++) {

		ofSetColor(this->polygons_color[i], this->polygons_life[i]);
		this->polygons[i].get()->draw();
	}

	ofSetColor(32);
	this->eage_1.draw();
	this->eage_2.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key >= 32 && key < 127) {

		char key_c = (char)key;
		string key_s{ key_c };
		ofTTFCharacter path = this->font.getCharacterAsPoints(key_c);
		ofPoint c_size = ofPoint(this->font.stringWidth(key_s), this->font.stringHeight(key_s));
		ofPoint adjust(ofGetWidth() / 2 - c_size.x / 2, c_size.y);
		ofColor color;
		color.setHsb(ofRandom(255), 255, 225);

		vector<ofPolyline> outline = path.getOutline();

		for (int o_index = 0; o_index < outline.size(); o_index++) {

			outline[o_index] = outline[o_index].getResampledBySpacing(ofRandom(15, 40));
		}

		ofMesh mesh;
		ofTessellator t;
		t.tessellateToMesh(outline, ofPolyWindingMode::OF_POLY_WINDING_ODD, mesh);

		vector<ofMeshFace> triangles = mesh.getUniqueFaces();
		for (int i = 0; i < triangles.size(); i++) {

			shared_ptr<ofxBox2dPolygon> polygon = shared_ptr<ofxBox2dPolygon>(new ofxBox2dPolygon);
			polygon.get()->addTriangle(triangles[i].getVertex(0) + adjust, triangles[i].getVertex(1) + adjust, triangles[i].getVertex(2) + adjust);
			polygon.get()->setPhysics(1.0, 0.3, 0.3);
			polygon.get()->create(box2d.getWorld());

			this->polygons.push_back(polygon);
			this->polygons_life.push_back(255);
			this->polygons_color.push_back(color);
		}

	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}