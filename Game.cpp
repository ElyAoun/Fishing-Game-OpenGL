#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <ctime>
#include <iostream>
using namespace std;

const double pi = 3.14159265359;
const int winWidth = 800, winHeight = 500; //WINDOW WIDTH AND HEIGHT

//INITIAL POINTS FOR THE BOAT AND THE FISH
int x0_boat = 30, y0_boat = winHeight / 2; //initial position of the starting point of the boat
int x0_fish, y0_fish; //initial position of the starting point of the fish

//ROTATION
int x_rotation, y_rotation; //rotation point
int x_oldPt, y_oldPt; // old point
int x_newPt, y_newPt; //new point after rotation
double theta = 30 * (pi / 100); // rotation angle : 30 degree in radian

//TO CHECK IF WE CAUGHT THE FISH OR NOT
bool fish_caught = false;

//display this text after getting the fish
char text[18] = { 'y','o','u',' ','c','a','u','g','h','t',' ','a',' ','f','i','s','h','!' };

void init() { //init function
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, winWidth, 0, winHeight);
}

void displayFinalText() { //final text after getting the fish
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2i(500, 400);
	for (int c = 0; c < 18; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[c]);
	}
}

void drawCircle(int x, int y, int r, float R, float G, float B) //specifying starting pt,radius and fill color of the circle 
{
	float angle;
	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * pi * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void makeLine(int x1, int y1, int x2, int y2, float R, float G, float B) { //make line between 2 given points
	glBegin(GL_LINES);
	glColor3f(R, G, B);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
}


void drawSun() {
	drawCircle(750, 500, 150, 1.0, 1.0, 0.0);
}

void drawOcean() {  //COLOR HALF THE SCREEN BLUE USING A QUAD
	glBegin(GL_QUADS);
	glColor3f(0.0, 1.0, 1.0);
	glVertex2i(0, 0);
	glVertex2i(winWidth, 0);
	glVertex2i(winWidth, winHeight / 2);
	glVertex2i(0, winHeight / 2);
	glEnd();
}


//specifying the colors of the wood,sails and fishing rod
void drawBoat(float R1, float G1, float B1, float R2, float G2, float B2, float R3, float G3, float B3) {
	//WOOD
	glBegin(GL_QUADS);
	glColor3f(R1, G1, B1); //RED
	glVertex2i(x0_boat, y0_boat);
	glVertex2i(x0_boat + 150, y0_boat);
	glVertex2i(x0_boat + 170, y0_boat + 50);
	glVertex2i(x0_boat - 20, y0_boat + 50);
	glEnd();

	//WINDOWS
	drawCircle(x0_boat + 20, y0_boat + 30, 15, 1.0, 1.0, 1.0);
	drawCircle(x0_boat + 75, y0_boat + 30, 15, 1.0, 1.0, 1.0);
	drawCircle(x0_boat + 130, y0_boat + 30, 15, 1.0, 1.0, 1.0);

	//LINE IN THE MIDDLE OF THE BOAT TO HOLD THE SAILS
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glColor3f(R1, G1, B1);
	glVertex2i(x0_boat + 75, y0_boat + 50);
	glVertex2i(x0_boat + 75, y0_boat + 200);
	glEnd();

	//SAILS
	glBegin(GL_TRIANGLES);
	glColor3f(R2, G2, B2); //BLUE
	glVertex2i(x0_boat + 75, y0_boat + 200);
	glVertex2i(x0_boat + 10, y0_boat + 75);
	glVertex2i(x0_boat + 75, y0_boat + 75);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(R2, G2, B2);
	glVertex2i(x0_boat + 75, y0_boat + 200);
	glVertex2i(x0_boat + 140, y0_boat + 75);
	glVertex2i(x0_boat + 75, y0_boat + 75);
	glEnd();

	//FISHING ROD 
	glBegin(GL_LINES);
	glColor3f(R3, G3, B3); //BLACK
	glVertex2i(x0_boat + 170, y0_boat + 50);
	glVertex2i(x0_boat + 260, y0_boat + 90);
	glEnd();

	//PULLING ROD
	glBegin(GL_LINES);
	glColor3f(R3, G3, B3);
	glVertex2i(x0_boat + 170, y0_boat + 50);
	glVertex2i(x0_boat + 170, y0_boat + 20);
	glEnd();

	drawCircle(x0_boat + 170, y0_boat + 20, 10, 0.0, 0.0, 0.0);

	glFlush();
}

void clearBoat() { //DRAW BOAT BUT WITH WHITE COLORS
	drawBoat(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
}

void drawFish(int x0, int y0, float R1, float G1, float B1, float R2, float G2, float B2) {
	// Body
	glBegin(GL_QUADS);
	glColor3f(R1, G1, B1);
	glVertex2i(x0, y0);
	glVertex2i(x0 + 50, y0 - 20);
	glVertex2i(x0 + 80, y0);
	glVertex2i(x0 + 50, y0 + 20);
	glEnd();

	//Eye
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glColor3f(R2, G2, B2);
	glVertex2i(x0 + 55, y0 + 10);
	glEnd();

	//TAIL
	glBegin(GL_TRIANGLES);
	glColor3f(R1, G1, B1);
	glVertex2i(x0, y0);
	glVertex2i(x0 - 20, y0 - 20);
	glVertex2i(x0 - 20, y0 + 20);
	glEnd();
	glFlush();
}

void generateFish() { //Draw a fish at random position each time we run the program
	srand(time(NULL));
	x0_fish = 250 + (rand() % ((winWidth - 250) - 250 + 1)); //x in [250,winWidth-250]
	y0_fish = 20 + (rand() % ((winHeight / 2 - 50) - 20 + 1)); // y in [20,winHeight/2-50] so it doesnt intersect the surface
	drawFish(x0_fish, y0_fish, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}


void drawFishingRod(float R1, float G1, float B1, float R2, float G2, float B2) { //colors of the 2 lines are specified
	//draw with white colors if we want to remove the object
	//Here i split the rod into 2 parts , the first is above the surface of the water and the second is under the water
	glBegin(GL_LINES);
	glColor3f(R1, G1, B1);
	glVertex2i(x0_boat + 260, y0_boat + 90);
	glVertex2i(x0_boat + 260, winHeight / 2);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(R2, G2, B2);
	glVertex2i(x0_boat + 260, winHeight / 2);
	glVertex2i(x0_boat + 260, y0_boat - 250);
	glEnd();

	//check if the position of the fishing rod intersects the fish
	if (x0_boat + 260 > x0_fish - 20 && x0_boat + 260 < x0_fish + 80) //check if we caught the fish
		fish_caught = true;
	else {
		fish_caught = false;
	}
	glFlush();
}

void moveBoatForward() { //translate boat forward
	clearBoat();  //remove boat at old position
	drawCircle(x0_boat + 170, y0_boat + 20, 10, 1.0, 1.0, 1.0); //remove circle at old position
	x0_boat += 20;
	drawBoat(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0); //draw boat at new position
}

void moveBoatBackward() { //translate boat backwards
	clearBoat(); //remove boat at old position
	drawCircle(x0_boat + 170, y0_boat + 20, 10, 1.0, 1.0, 1.0); //remove circle at old position
	x0_boat -= 20;
	drawBoat(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0); //draw circle at new position
}


void pullFishUp() {
	if (y0_fish >= winHeight / 2) { //if fish is above the surface
		drawFish(x0_fish, y0_fish, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0); //remove fish at old position
		drawOcean();
	}
	else {
		drawFish(x0_fish, y0_fish, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0); //remove fish at old position
	}

	y0_fish += 10; // pull fish up by increasing y
	if (y0_fish >= y0_boat + 90) { //reached the max level
		displayFinalText(); //display final text
		drawFish(x0_fish, y0_fish, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0); //clear fish (draw with all white colors)
	}
	else {
		drawFish(x0_fish, y0_fish, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}
}

void rotateCircle() { //rotate while pulling the fish up
	if (fish_caught) {
		x_newPt = x_rotation + (x_oldPt - x_rotation) * cos(theta) - (y_oldPt - y_rotation) * sin(theta); //rotation formula
		y_newPt = y_rotation + (x_oldPt - x_rotation) * sin(theta) + (y_oldPt - y_rotation) * cos(theta); //rotation formula

		makeLine(x_rotation, y_rotation, x_oldPt, y_oldPt, 1.0, 1.0, 1.0); //remove line at old position
		drawCircle(x_oldPt, y_oldPt, 10, 1.0, 1.0, 1.0); //remove circle at old position

		makeLine(x_rotation, y_rotation, x_newPt, y_newPt, 0.0, 0.0, 0.0); //draw line at new position
		drawCircle(x_newPt, y_newPt, 10, 0.0, 0.0, 0.0); //draw new circle

		x_oldPt = x_newPt; //set the new rotation point as old point for the next rotation
		y_oldPt = y_newPt; //set the new rotation point as old point for the next rotation

		glBegin(GL_QUADS); //i did this to fill the body of the boat again after each rotation so that it doesnt have empty areas
		glColor3f(1.0, 0.0, 0.0);
		glVertex2i(x0_boat, y0_boat);
		glVertex2i(x0_boat + 150, y0_boat);
		glVertex2i(x0_boat + 170, y0_boat + 50);
		glVertex2i(x0_boat - 20, y0_boat + 50);
		glEnd();
		//WINDOWS
		drawCircle(x0_boat + 20, y0_boat + 30, 15, 1.0, 1.0, 1.0);
		drawCircle(x0_boat + 75, y0_boat + 30, 15, 1.0, 1.0, 1.0);
		drawCircle(x0_boat + 130, y0_boat + 30, 15, 1.0, 1.0, 1.0);
	}
}


void fishCaught() {
	if (!fish_caught) { //didn't catch a fish => fishing rod reaches the x-axis
		drawFishingRod(1.0, 1.0, 1.0, 0.0, 1.0, 1.0);
	}
	else { //caught a fish
		glBegin(GL_LINES);
		//change color depending on fish position
		if (y0_fish >= winHeight / 2) { //check if the fish is above the surface
			glColor3f(1.0, 1.0, 1.0);
		}
		else { //fish is still in the water
			glColor3f(0.0, 1.0, 1.0);
		}
		glVertex2i(x0_boat + 260, y0_fish);
		glVertex2i(x0_boat + 260, 0); //here we removed the line between the fish and the x axis (y=0)
		glEnd();

		pullFishUp(); //pull fish up

	}
	//reset the fishing rod to its initial appearance after getting the fish
	makeLine(x0_boat + 170, y0_boat + 50, x0_boat + 260, y0_boat + 90, 0.0, 0.0, 0.0);
}

void updateRotationPoints() { //update points after each rotation to be able to make the next rotation
	x_rotation = x0_boat + 170, y_rotation = y0_boat + 50; //rotation pt
	x_oldPt = x0_boat + 170, y_oldPt = y0_boat + 20; //original pt
}

void game() { //final game after combining everything
	glClear(GL_COLOR_BUFFER_BIT);
	drawSun();
	drawOcean();
	generateFish();
	drawBoat(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0);
	glFlush();
}

void keyboardSpecialInput(int key, int x, int y) { //keyboard listener
	switch (key) {
	case GLUT_KEY_RIGHT: //right key
		moveBoatForward();
		updateRotationPoints(); // update rotation points after changing x0_boat 
		break;
	case GLUT_KEY_LEFT: // left key
		moveBoatBackward();
		updateRotationPoints(); // update rotation points after changing x0_boat 
		break;
	case GLUT_KEY_DOWN: // down key
		drawFishingRod(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		break;
	case GLUT_KEY_UP: // up key
		fishCaught();
		rotateCircle();
		break;
	default:
		break;
	}
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(winWidth, winHeight);

	glutCreateWindow("FISHING GAME");
	init();

	glutDisplayFunc(game);
	glutSpecialFunc(keyboardSpecialInput);
	glutMainLoop();
}