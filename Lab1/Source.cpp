#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include "imageloader.h"

using namespace std;

GLuint _textureId;
GLuint _textureId2;
stringstream ss;
string _score = "Score :";
string _gameLost = "You lost, ";
string _gameWon = "You won, ";
float _tileX1 = 0.490; //with of the tiles
float _tileX2 = 0.510;
float _tileMidPoint = (_tileX1 + _tileX2) / 2.0;

float _tileYaxis = 0.0; //incremental Y axis value of tiles

float _boxX1 = 0.7;
float _boxX2 = 0.77;
float _boxY1 = 0.5;
float _boxY2 = 0.55;

int score = 0;

bool boxInRight = true;
bool changeBoxPosition = false;
bool scoreUpdateFlag = false;
bool scoreUpdatePermissionFlag = true;
bool bounceFlag = false;
bool boxReallyShifted = false;
bool gamePaused = false;
bool gameLost = false;
bool gameEnd = false;
bool effectEnable = false;

float effectColor = 0.0;

const int TILESIZE = 200;
const float TILESPEED = 0.015;

float tilesArray[TILESIZE][3]; //[TilesId][Y1,Color]
float totalLength;



int collisionTile()
{
	//When BOx in Left
	if (boxInRight == false){
		if (_boxX2 >= _tileMidPoint && (((_boxX1 + _boxX2) / 2) - _tileMidPoint)>0.00){

			//find The Tile
			for (int i = 0; i<TILESIZE - 1; i++){
				if (_boxY1>tilesArray[i][0] && _boxY2<tilesArray[i + 1][0]){
					if (scoreUpdatePermissionFlag){
						scoreUpdateFlag = true;
						//   printf("Score Update Flag = true\n");
					}
					printf("Box shifted\n");
					boxReallyShifted = true;
					return i;
				}
			}
			printf("COLL\n");
			return -1;
		}
	}
	//WHEN box in RIght
	if (boxInRight == true){
		if (_boxX1 <= _tileMidPoint && (_tileMidPoint - ((_boxX1 + _boxX2) / 2))>0.00){
			//find The Tile
			for (int i = 0; i<TILESIZE - 1; i++){
				if (_boxY1>tilesArray[i][0] && _boxY2<tilesArray[i + 1][0]){
					if (scoreUpdatePermissionFlag){
						scoreUpdateFlag = true;

					}
					printf("Box shifted\n");
					boxReallyShifted = true;
					return i;
				}
			}
			printf("COLL\n");
			return -1;
		}
	}
}

bool checkGameEnd(){
	if (totalLength<0.99){
		gameEnd = true;
		return true;
	}
	if (score<0){
		gameLost = true;
		score = 0;
		return true;
	}
	else
		return false;
}

void scoreUpdate(){
	int tempIndex = -1;
	tempIndex = collisionTile();
	printf("Length- %f\n", totalLength);

	if (tempIndex == -1){
		//  printf("Minus");
	}
	else
	{
		if (tilesArray[tempIndex][1] == 0)
		{ //RED
			if (boxReallyShifted == true){
				effectEnable = true;
				tilesArray[tempIndex][1] = 3;
				--score;
			}
			boxReallyShifted = false;
		}
		if (tilesArray[tempIndex][1] == 1)
		{ //GREEN
			//Checking if really came from the function
			if (boxReallyShifted == true)
			{
				printf("BOUNCE ENABLE\n");
				tilesArray[tempIndex][1] = 0; //Change Color to Red
				bounceFlag = true;
			}
			boxReallyShifted = false;
		}
		if (tilesArray[tempIndex][1] == 2){ //BLUE
			if (boxReallyShifted == true)
			{
				score += 2; //point increasing 2 for each hit
				tilesArray[tempIndex][1] = 3;
				effectEnable = true;
				printf("BLUE\n");
			}
			boxReallyShifted = false;
		}
	}

	scoreUpdatePermissionFlag = false;
	if (checkGameEnd()){
		gamePaused = true;
		//score=0; // Make Score 0 if game over
		printf("GAME PAUSE");
	}
}

void reInitTiles()
{
	score = 0;
	gamePaused = false;
	gameLost = false;
	gameEnd = false;
	//boxInRight=true;
	boxReallyShifted = false;

	srand(time(0));
	float startingValue = 0.00001;
	tilesArray[0][0] = startingValue;
	tilesArray[0][1] = 0;

	float tempCounter = 0.0;
	for (int i = 1; i<TILESIZE; i++){
		tilesArray[i][0] = tilesArray[i - 1][0] + ((rand() % 4) / 10.0);
		tilesArray[i][1] = rand() % 3;
		tempCounter += tilesArray[i][0];
	}
	totalLength = tempCounter;
}


void ChangeBoxPosition(){
	changeBoxPosition = true;
}



void print(int x, int y, char *string)
{
	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y+0.95);
	//get the length of the string to display
	int len = (int)strlen(string);

	//loop to display character by character
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void printScore(){
	/*int p = score;
	ss << p;
	string newst;
	ss >> newst;
	ss.clear();
	//cout<<"Score: "+newst<<endl;
	string temp=newst.c_str();*/

	int number = score;
	stringstream ss;
	if (gameEnd && score > 0){
		ss << _gameWon;
	}
	else if (gameLost){
		ss << _gameLost;
	}
	ss << _score;


	ss << number;
	string temp_str = ss.str();
	char* char_type = (char*)temp_str.c_str();
	print(0.0, 0.0, char_type);
}

void update(int value) {
	if (!gamePaused){
		//Effect Enable
		if (effectEnable){
			effectColor += 0.15;
			if (effectColor >= 0.5){
				effectEnable = false;
				effectColor = 0.0;
			}
		}

		//Tiles Position Changing
		for (int i = 0; i<TILESIZE; i++)
		{
			tilesArray[i][0] -= TILESPEED;
		}
		totalLength = tilesArray[TILESIZE - 1][0];


		//Box Position Changing
		if (changeBoxPosition)
		{
			if (bounceFlag == false)
			{
				if (boxInRight)
				{ // BOx in the Right Side
					//change Code to pull box Left
					_boxX1 -= 0.05;
					_boxX2 -= 0.05;
					//check if properly In left -> make BoxInRight=false & changeBoxPosition=false;
					if (_boxX1<0.2){
						boxInRight = false;
						changeBoxPosition = false;
						scoreUpdatePermissionFlag = true;
					}
				}
				else
				{ //Box in the Left Side
					_boxX1 += 0.05;
					_boxX2 += 0.05;
					//checking if properly in left side
					if (_boxX1>0.7)
					{
						boxInRight = true;
						changeBoxPosition = false;
						scoreUpdatePermissionFlag = true;
					}
				}
			}
			//IF BOUNCE ENABLE
			if (bounceFlag == true){
				if (boxInRight){
					_boxX1 += 0.05;
					_boxX2 += 0.05;
					if (_boxX1>0.7)
					{
						boxInRight = true;
						changeBoxPosition = false;
						bounceFlag = false;
					}
				}
				else{
					_boxX1 -= 0.05;
					_boxX2 -= 0.05;
					if (_boxX1<0.2){
						boxInRight = false;
						changeBoxPosition = false;
						bounceFlag = false;
					}
				}

			}

		}

		scoreUpdate();

	} //End of GamePaused-IF

	glutPostRedisplay(); //Tell GLUT that the display has changed
	//Tell GLUT to call update again in 25 milliseconds
	glutTimerFunc(35, update, 0);
}

bool bal(){

	if (true){
		if (false) {
			return false;
		}
		else{
			if (true){
				return true;
			}
			else{
				return false;
			}
		}
	}
	else{
		return false;
	}
}

void handleMouse(int b, int s, int x, int y)
{      // mouse click callback

	bool hauwa = bal();
	if (s == GLUT_DOWN)
	{
		if (b == GLUT_LEFT_BUTTON)
		{
			//printf("Mouse Pressed\n");
			ChangeBoxPosition();

		}
	}
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 'p':
		if (gamePaused){
			gamePaused = false;
			reInitTiles();
		}
	}
}

void DrawBackground() //This function is to draw water
{
	glBegin(GL_POLYGON);
	glColor4f(0.118, 0.565, 1.000, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(0.0, 2.0, 0.0);
	glEnd();
}

void drawBox(){

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPushMatrix();
	//Square
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(_boxX1, _boxY1, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(_boxX2, _boxY1, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(_boxX2, _boxY2, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(_boxX1, _boxY2, 0.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

void DrawEffect(){
	glPushMatrix();
	glScalef(1.9f, 1.4f, 0.0f);
	glTranslated(0.262, 0.35, 0.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId2);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_POLYGON);
	glColor4f(1.0, 1.0, 1.0, effectColor);
	for (int i = 0; i<200; i++)
	{
		float pi = 3.1416;
		float A = (i * 2 * pi) / 50;
		float r = 0.05;
		float x = r * cos(A);
		float y = r * sin(A);
		float tx = cos(A)*0.5 + 0.5;
		float ty = sin(A)*0.5 + 0.5;
		glTexCoord2f(tx, ty);
		glVertex2f(x, y);
		
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


void DrawTiles()
{
	glPushMatrix();
	//Tile1

	for (int i = 0; i<TILESIZE - 1; i++)
	{
		if (tilesArray[i][1] == 0){ //Minus
			glColor4f(0.709, 0.047, 0.212, 1.0);
		}
		if (tilesArray[i][1] == 1){ //Bounce
			glColor4f(0.913, 0.929, 0.129, 1.0);
		}
		if (tilesArray[i][1] == 2){ //Plus
			glColor4f(0.003, 0.349, 0.647, 1.0);
		}
		if (tilesArray[i][1] == 3){
			glColor4f(0.0, 0.0, 1.0, 0.0);
		}


		glBegin(GL_POLYGON);
		glVertex3f(_tileX1, tilesArray[i][0], 0.0);
		glVertex3f(_tileX2, tilesArray[i][0], 0.0);
		glVertex3f(_tileX2, tilesArray[i + 1][0], 0.0);
		glVertex3f(_tileX1, tilesArray[i + 1][0], 0.0);
		glEnd();
	}


	glPopMatrix();
}



void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	DrawBackground();
	glPopMatrix();

	glPushMatrix();
	drawBox();
	glPopMatrix();

	glPushMatrix();
	DrawTiles();
	glPopMatrix();

	glPushMatrix();
	printScore();
	glPopMatrix();

	glPushMatrix();
	DrawEffect();
	glPopMatrix();

	glFlush();
}

GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
		//as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void init(void)
{
	/* select clearing (background) color */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	/* initialize viewing values */
	glMatrixMode(GL_PROJECTION);
	Image* image = loadBMP("C:\\Users\\Mohammad\\Desktop\\shark.bmp");
	_textureId = loadTexture(image);
	delete image;
	Image* image2 = loadBMP("C:\\Users\\Mohammad\\Desktop\\explosion.bmp");
	_textureId2 = loadTexture(image2);
	delete image2;
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 600);
	glutInitWindowPosition(250, 50);
	glutCreateWindow("Computer Graphics Project");
	init();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	reInitTiles();

	glutDisplayFunc(display);
	glutTimerFunc(35, update, 0); //THIS IS THE UPDATE FUNCTOIN

	glutMouseFunc(handleMouse);
	glutKeyboardFunc(handleKeypress);
	glutMainLoop();
	return 0;
}
