/**
    CS-368/718 Computer Graphics
    Professor: Dr.Zhigang Xiang
    colormixer.cpp
    Purpose: interactive color mixer that allows a user to experiment with
    different combinations of RGB primaries.
    @author Dilande Roger Jr.
    @version 1.0 7/28/14
*/

#include <sstream>
#include <iostream>
#include <string>
#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>

int width = 500, height = 500, vertRed[1000000][4],vertGreen[1000000][4],vertBlue[1000000][4],n = 0, type = GL_POLYGON,
x1 = 40, x2 = 90,Y1 = 50,y2 = 65,v,mousePos, previousY;
const int verticalBarHeight = 320;
int ry1 = Y1,ry2 = y2;// initial red bar y-coordinates
int gx1 = 110 , gx2 = 160;// green bar x-coordinates
int gy1 = Y1, gy2 = y2;// initial green bar y-coordinates
int bx1 = 180, bx2 = 230;// blue bar x-coordinates
int by1 = Y1, by2 = y2;// initial blue bar y-coordinates
int gV;// green vertex index variable
int bV;// blue vertex index variable
int gN = 0;// green vertex n variable
int bN = 0;// blue vertex n variable
const int MAX_HEIGHT = verticalBarHeight;
const int MIN_HEIGHT = Y1;
double colorX = 0.0, colorY = 0.0, colorZ = 0.0;// x,y,z coordinates of the square
double lgradColorX = 1, lgradColorY = 1, lgradColorZ = 1;
double rgradColorX = 1, rgradColorY = 1, rgradColorZ = 1;
bool rubberbanding = false, antialiasing = false, leftButton = false, rightButton = false;
bool redVert = false,greenVert = false,blueVert = false;
bool leftSideGrade = false, rightSideGrade = false;
int previousRedN = 0, previousGreenN = 0, previousBlueN = 0;// variables used to keep track of the buttons in the for loop within display3
int slideBarValRed = 0;
// first x-coordiantes of the slide bar for each color
int slideBarValRedX = 60;
int slideBarValGreenX = 130;
int slideBarValBlueX = 200;

char redBuffer[1000];// buffer to hold redScale values
int slideBarValGreen = 0;
char greenBuffer[1000];// buffer to hold greenScale values
int slideBarValBlue = 0;
char blueBuffer[1000];// buffer to hold blueScale values
int base = y2;
std::ostringstream number;// temp tring value used to convert int scale values to char
std::string word = "G r a d i e n t";
std::ostringstream gradient;
char gradBuffer[1000];// buffer used to hold the characters of the string "word"

void display3(void){

// create a gray box which is the same size as the display window
glClear(GL_COLOR_BUFFER_BIT);
glColor3d(.3,.3,.4);
glBegin(GL_POLYGON);
glVertex3d(0.0,0.0, 0.0);
glVertex3d(0.0, 499, 0.0);
glVertex3d(499, 499, 0.0);
glVertex3d(499, 0.0, 0.0);
glEnd();
// Initial red button
if(n == 0 && type == GL_POLYGON)
{
// red button
glColor3d(1,0,0);
glBegin(GL_POLYGON);
// bottom left x1, y1
glVertex3d(x1,Y1,0.0);
// bottom rigt x2,y1
glVertex3d(x2,Y1,0.0);
// top right x2,y2
glVertex3d(x2,y2,0.0);
// top left x1,y2
glVertex3d(x1,y2,0.0);
glEnd();
}
// Initial green button
if(gN == 0 && type == GL_POLYGON)
{
// green button
glColor3d(0,1,0);
glBegin(GL_POLYGON);
// bottom left x1, y1
glVertex3d(gx1,Y1,0.0);
// bottom rigt x2,y1
glVertex3d(gx2,Y1,0.0);
// top right x2,y2
glVertex3d(gx2,y2,0.0);
// top left x1,y2
glVertex3d(gx1,y2,0.0);
glEnd();
}
// Initial blue button
if(bN == 0 && type == GL_POLYGON)
{
// blue button
glColor3d(0,0,1);
glBegin(GL_POLYGON);
// bottom left x1, y1
glVertex3d(bx1,Y1,0.0);
// bottom rigt x2,y1
glVertex3d(bx2,Y1,0.0);
// top right x2,y2
glVertex3d(bx2,y2,0.0);
// top left x1,y2
glVertex3d(bx1,y2,0.0);
glEnd();
}

// red meter bar
glBegin(type);
for(int i = previousRedN; i < n; i++)
{
// if the mouse cursor is under the max vertical limit
if(vertRed[i][2] <= MAX_HEIGHT && vertRed[i][1] >= MIN_HEIGHT)
{
glColor3d(1,0,0);
// bottom left x1, y1
glVertex3d(x1,vertRed[i][1],0.0);
// bottom rigt x2,y1
glVertex3d(x2,vertRed[i][1],0.0);
// top right x2,y2
glVertex3d(x2,vertRed[i][2],0.0);
// top left x1,y2
glVertex3d(x1,vertRed[i][2],0.0);

}else
if(vertRed[i][2] > MAX_HEIGHT)
{
// red meter bar
glColor3d(1,0,0);
// bottom left x1, y1
glVertex3d(x1,verticalBarHeight-15,0.0);
// bottom rigt x2,y1
glVertex3d(x2,verticalBarHeight-15,0.0);
// top right x2,y2
glVertex3d(x2,verticalBarHeight,0.0);
// top left x1,y2
glVertex3d(x1,verticalBarHeight,0.0);
}else
if(vertRed[i][1] < MIN_HEIGHT)// bar is below the vertical min
{
// red meter bar
glColor3d(1,0,0);
// bottom left x1, y1
glVertex3d(x1,50,0.0);
// bottom rigt x2,y1
glVertex3d(x2,50,0.0);
// top right x2,y2
glVertex3d(x2,65,0.0);
// top left x1,y2
glVertex3d(x1,65,0.0);
}
}// end for
glEnd();

// Display the bar in proper place if the cursor is moved away from the main window
if(previousRedN == n)
    {
        int tempY1 = vertRed[previousRedN-1][1], tempY2 = vertRed[previousRedN-1][2];
        if(tempY2 > MAX_HEIGHT)
        {
            tempY1 = verticalBarHeight-15;
            tempY2 = verticalBarHeight;
        }else
            if(tempY1 < MIN_HEIGHT)
            {
                tempY1 = 50;
                tempY2 = 65;
            }

        glColor3d(1,0,0);
        glBegin(GL_POLYGON);
        // bottom left x1, y1
        glVertex3d(x1,tempY1,0.0);
        // bottom rigt x2,y1
        glVertex3d(x2,tempY1,0.0);
        // top right x2,y2
        glVertex3d(x2,tempY2,0.0);
        // top left x1,y2
        glVertex3d(x1,tempY2,0.0);
        glEnd();
    }// end if


// green meter bar
glBegin(type);
for(int i = previousGreenN; i < gN; i++)
{
// if the mouse cursor is under the max vertical limit
if(vertGreen[i][2] <= MAX_HEIGHT && vertGreen[i][1] >= MIN_HEIGHT)
{
glColor3d(0,1,0);
// bottom left x1, y1
glVertex3d(gx1,vertGreen[i][1],0.0);
// bottom rigt x2,y1
glVertex3d(gx2,vertGreen[i][1],0.0);
// top right x2,y2
glVertex3d(gx2,vertGreen[i][2],0.0);
// top left x1,y2
glVertex3d(gx1,vertGreen[i][2],0.0);
}else
if(vertGreen[i][2] > MAX_HEIGHT)
{
// green meter bar
glColor3d(0,1,0);
// bottom left x1, y1
glVertex3d(gx1,verticalBarHeight-15,0.0);
// bottom rigt x2,y1
glVertex3d(gx2,verticalBarHeight-15,0.0);
// top right x2,y2
glVertex3d(gx2,verticalBarHeight,0.0);
// top left x1,y2
glVertex3d(gx1,verticalBarHeight,0.0);
}else
if(vertGreen[i][1] < MIN_HEIGHT)// bar is below the vertical min
{
// green meter bar
glColor3d(0,1,0);
// bottom left x1, y1
glVertex3d(gx1,50,0.0);
// bottom rigt x2,y1
glVertex3d(gx2,50,0.0);
// top right x2,y2
glVertex3d(gx2,65,0.0);
// top left x1,y2　
glVertex3d(gx1,65,0.0);
}//end if
}// end for
glEnd();

// Display the bar in proper place if the cursor is moved away from the main window
if(previousGreenN == gN)
    {
        int tempY1 = vertGreen[previousGreenN-1][1], tempY2 = vertGreen[previousGreenN-1][2];
        if(tempY2 > MAX_HEIGHT)
        {
            tempY1 = verticalBarHeight-15;
            tempY2 = verticalBarHeight;
        }else
            if(tempY1 < MIN_HEIGHT)
            {
                tempY1 = 50;
                tempY2 = 65;
            }

        glColor3d(0,1,0);
        glBegin(GL_POLYGON);
        // bottom left x1, y1
        glVertex3d(gx1,tempY1,0.0);
        // bottom rigt x2,y1
        glVertex3d(gx2,tempY1,0.0);
        // top right x2,y2
        glVertex3d(gx2,tempY2,0.0);
        // top left x1,y2
        glVertex3d(gx1,tempY2,0.0);
        glEnd();
    }// end if

// blue meter bar
glBegin(type);
for(int i = previousBlueN; i < bN; i++)
{
// if the mouse cursor is under the max vertical limit
if(vertBlue[i][2] <= MAX_HEIGHT && vertBlue[i][1] >= MIN_HEIGHT)
{
glColor3d(0,0,1);
// bottom left x1, y1
glVertex3d(bx1,vertBlue[i][1],0.0);
// bottom rigt x2,y1
glVertex3d(bx2,vertBlue[i][1],0.0);
// top right x2,y2
glVertex3d(bx2,vertBlue[i][2],0.0);
// top left x1,y2
glVertex3d(bx1,vertBlue[i][2],0.0);
}else
if(vertBlue[i][2] > MAX_HEIGHT)
{
// blue meter bar
glColor3d(0,0,1);
// bottom left x1, y1
glVertex3d(bx1,verticalBarHeight-15,0.0);
// bottom rigt x2,y1
glVertex3d(bx2,verticalBarHeight-15,0.0);
// top right x2,y2
glVertex3d(bx2,verticalBarHeight,0.0);
// top left x1,y2
glVertex3d(bx1,verticalBarHeight,0.0);
}else
if(vertBlue[i][1] < MIN_HEIGHT)// bar is below the vertical min
{
// blue meter bar
glColor3d(0,0,1);
// bottom left x1, y1
glVertex3d(bx1,50,0.0);
// bottom rigt x2,y1
glVertex3d(bx2,50,0.0);
// top right x2,y2
glVertex3d(bx2,65,0.0);
// top left x1,y2
glVertex3d(bx1,65,0.0);
}//end if
}// end for
glEnd();

// Display the bar in proper place if the cursor is moved away from the main window
if(previousBlueN == bN)
    {
        int tempY1 = vertBlue[previousBlueN-1][1], tempY2 = vertBlue[previousBlueN-1][2];
        if(tempY2 > MAX_HEIGHT)
        {
            tempY1 = verticalBarHeight-15;
            tempY2 = verticalBarHeight;
        }else
            if(tempY1 < MIN_HEIGHT)
            {
                tempY1 = 50;
                tempY2 = 65;
            }

        glColor3d(0,0,1);
        glBegin(GL_POLYGON);
        // bottom left x1, y1
        glVertex3d(bx1,tempY1,0.0);
        // bottom rigt x2,y1
        glVertex3d(bx2,tempY1,0.0);
        // top right x2,y2
        glVertex3d(bx2,tempY2,0.0);
        // top left x1,y2
        glVertex3d(bx1,tempY2,0.0);
        glEnd();
    }// end if

// color of the color square
glColor3d(colorX,colorY,colorZ);
glBegin(GL_POLYGON);
// bottom left x1, y1
glVertex3d(230,350,0.0);
// bottom rigt x2,y1
glVertex3d(480,350,0.0);
// top right x2,y2
glVertex3d(480,490,0.0);
// top left x1,y2
glVertex3d(230,490,0.0);
glEnd();

        // gradient square
        glBegin(GL_POLYGON);
        glColor3d(lgradColorX, lgradColorY,lgradColorZ);
        glVertex2d(270,Y1);
        glColor3d(rgradColorX,rgradColorY,rgradColorZ);
        glVertex2d(480,Y1);
        glColor3d(rgradColorX, rgradColorY, rgradColorZ);
        glVertex2d(480,150);
        glColor3d(lgradColorX, lgradColorY, lgradColorZ);
        glVertex2d(270,150);
        glEnd();

// red vertical limit bar
glColor3d(1,0,0);
glBegin(GL_POLYGON);
// bottom left x1, y1
glVertex3d(60,Y1,0.0);
// bottom rigt x2,y1
glVertex3d(70,Y1,0.0);
// top right x2,y2
glVertex3d(70,verticalBarHeight,0.0);
// top left x1,y2
glVertex3d(60,verticalBarHeight,0.0);
glEnd();
// green vertical limit bar
glColor3d(0,1,0);
glBegin(GL_POLYGON);
// bottom left x1, y1
glVertex3d(130,Y1,0.0);
// bottom rigt x2,y1
glVertex3d(140,Y1,0.0);
// top right x2,y2
glVertex3d(140,verticalBarHeight,0.0);
// top left x1,y2
glVertex3d(130,verticalBarHeight,0.0);
glEnd();
// blue vertical limit bar
glColor3d(0,0,1);
glBegin(GL_POLYGON);
// bottom left x1, y1
glVertex3d(200,Y1,0.0);
// bottom rigt x2,y1
glVertex3d(210,Y1,0.0);
// top right x2,y2
glVertex3d(210,verticalBarHeight,0.0);
// top left x1,y2
glVertex3d(200,verticalBarHeight,0.0);
glEnd();

// Draw the RGB characters
glColor3d(1,0,0);
glRasterPos2i(60,30);
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'R');
glEnd();

glColor3d(0,1,0);
glRasterPos2i(130,30);
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'G');
glEnd();

glColor3d(0,0,1);
glRasterPos2i(200,30);
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'B');
glEnd();

    number << slideBarValRed;
    // convert int val to char val
    for(unsigned int i = 0; i < number.str().length(); i++)
        redBuffer[i] = number.str().at(i);

// write char val to the screen
glColor3d(0,0,0);
for(unsigned int i = 0; i < number.str().length(); i++)
{
glRasterPos2i(slideBarValRedX,verticalBarHeight + 15);
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,redBuffer[i]);
slideBarValRedX += 10;
}
glEnd();

slideBarValRedX = 60;
number.str("");

number << slideBarValGreen;
    // convert int val to char val
    for(unsigned int i = 0; i < number.str().length(); i++)
        greenBuffer[i] = number.str().at(i);

// write char val to the screen
glColor3d(0,0,0);
for(unsigned int i = 0; i < number.str().length(); i++)
{
glRasterPos2i(slideBarValGreenX,verticalBarHeight + 15);
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,greenBuffer[i]);
slideBarValGreenX += 10;
}
glEnd();
slideBarValGreenX = 130;
number.str("");

number << slideBarValBlue;
    // convert int val to char val
    for(unsigned int i = 0; i < number.str().length(); i++)
        blueBuffer[i] = number.str().at(i);

// write char val to the screen
glColor3d(0,0,0);
for(unsigned int i = 0; i < number.str().length(); i++)
{
glRasterPos2i(slideBarValBlueX,verticalBarHeight+15);
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,blueBuffer[i]);
slideBarValBlueX += 10;
}
glEnd();
slideBarValBlueX = 200;
number.str("");

// write the word "Gradient" to the screen
int tempX = 270;
gradient << word;
for(unsigned int i = 0; i < gradient.str().length(); i++)
        gradBuffer[i] = gradient.str().at(i);

glColor3d(0,0,0);
for(unsigned int i = 0; i < gradient.str().length(); i++)
{
glRasterPos2i(tempX,170);
glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,gradBuffer[i]);
 
     tempX += 8;
}
glEnd();
gradient.str("");

glutSwapBuffers();
glFlush();

}

void mouse(int button, int state,int x, int y)
{
switch(button)
{
case GLUT_LEFT_BUTTON:

// if the mouse is within the red button
if((height - 1 - y)>= ry1 && (height - 1 - y) <= ry2 && x >= x1 && x <= x2)
{
// test if the left button was pressed again before releasing
if(state == GLUT_DOWN && redVert != true && greenVert != true && blueVert != true)
{
redVert = true;// red button selected

// reset leftButton,rightButton and index variable for the red vertex array
if(leftButton == true || rightButton == true)
{
    previousRedN = n;
    leftButton = false;
    rightButton = false;
}

previousGreenN = gN-1;// continue to display the green bar
previousBlueN = bN - 1;// continue to display the blue bar

// test if the other buttons are still active
if(greenVert == true)
greenVert = false;
if(blueVert == true)
blueVert = false;
v = n++;
vertRed[v][0] = x1;
vertRed[v][1] = ry1;
vertRed[v][2] = ry2;
vertRed[v][3] = height - 1 - y;// current position of the mouse while left button down
mousePos = height - 1 - y;
rubberbanding = true;
glutPostRedisplay();
}else
rubberbanding = false;
}
// if the mouse is within the green button
if((height - 1 - y)>= gy1 && (height - 1 - y) <= gy2 && x >= gx1 && x <= gx2)
{
if(state == GLUT_DOWN && greenVert != true && redVert != true && blueVert != true)
{
greenVert = true;// green button selected
previousRedN = n-1;// continue to display the red bar
previousBlueN = bN - 1;// continue to display the blue bar

// reset leftButton and index variable for the green vertex array
if(leftButton == true || rightButton == true)
{
    previousGreenN = gN;
    leftButton = false;
    rightButton = false;
}
// test if the other buttons are still active
if(redVert == true)
redVert = false;
if(blueVert == true)
blueVert = false;
gV = gN++;

vertGreen[gV][0] = gx1;
vertGreen[gV][1] = gy1;
vertGreen[gV][2] = gy2;
vertGreen[gV][3] = height - 1 - y;// current position of the mouse while button down
mousePos = height - 1 - y;
rubberbanding = true;
glutPostRedisplay();
}else
rubberbanding = false;
}
// if the mouse is within the blue button
if((height - 1 - y)>= by1 && (height - 1 - y) <= by2 && x >= bx1 && x <= bx2)
{
if(state == GLUT_DOWN && blueVert != true && redVert != true && greenVert != true)
{
blueVert = true;// blue button selected
previousRedN = n-1;// continue to display the red bar
previousGreenN = gN - 1;// continue to display the blue bar

// reset leftButton and index variable for the blue vertex array
if(leftButton == true || rightButton == true)
{
    previousBlueN = bN;
    leftButton = false;
    rightButton = false;
}
// test if the other buttons are still active
if(redVert == true)
redVert = false;
if(greenVert == true)
greenVert = false;
bV = bN++;
vertBlue[bV][0] = bx1;
vertBlue[bV][1] = by1;
vertBlue[bV][2] = by2;
vertBlue[bV][3] = height - 1 - y;// current position of the mouse while button down
mousePos = height - 1 - y;
rubberbanding = true;
glutPostRedisplay();
}else
rubberbanding = false;
}
// if the cursor is within the left side of the gradient rectangle
if((height - 1 - y) >= Y1 && (height - 1 - y) <= 150 && x >= 270 && x <= 375)
{
        rubberbanding = false;
        redVert = false;
        greenVert = false;
        blueVert = false;
        leftSideGrade = true;

    double result = 480 - x;
    double percentVal = result/210;
    double lColorX = lgradColorX * percentVal;
    double lColorY = lgradColorY * percentVal;
    double lColorZ = lgradColorZ * percentVal;
    double rColorX = rgradColorX * (1-percentVal);
    double rColorY = rgradColorY * (1-percentVal);
    double rColorZ = rgradColorZ * (1-percentVal);

    //update the square
    colorX = lColorX + rColorX;
    colorY = lColorY + rColorY;
    colorZ = lColorZ + rColorZ;

    //update bars for each color
    slideBarValRed = colorX/.0039215686;
    slideBarValGreen = colorY/.0039215686;
    slideBarValBlue = colorZ/.0039215686;

    previousBlueN = bN + 1;
    previousGreenN = gN + 1;
    previousRedN = n + 1;

    // red bar update
    v = n++;
    vertRed[v][0] = x1;
    ry2 = slideBarValRed + base;
    ry1 = ry2 - 15;
    vertRed[v][1] = ry1;
    vertRed[v][2] = ry2;

    // green bar update
    gV = gN++;
    vertGreen[gV][0] = gx1;
    gy2 = slideBarValGreen + base;
    gy1 = gy2 - 15;
    vertGreen[gV][1] = gy1;
    vertGreen[gV][2] = gy2;

    // blue bar update
    bV = bN++;
    vertBlue[bV][0] = bx1;
    by2 = slideBarValBlue + base;
    by1 = by2 - 15;
    vertBlue[bV][1] = by1;
    vertBlue[bV][2] = by2;
    
    glutPostRedisplay();

}else// if the cursor is within the right side of the gradient rectangle
    if((height - 1 - y) >= Y1 && (height - 1 - y) <= 150 && x > 375 && x <= 480)
    {
        rubberbanding = false;
        redVert = false;
        greenVert = false;
        blueVert = false;
        rightSideGrade = true;

    double result = 480 - x;
    double percentVal = result/210;
    double lColorX = lgradColorX * percentVal;
    double lColorY = lgradColorY * percentVal;
    double lColorZ = lgradColorZ * percentVal;
    double rColorX = rgradColorX * (1-percentVal);
    double rColorY = rgradColorY * (1-percentVal);
    double rColorZ = rgradColorZ * (1-percentVal);

    //update the square
    colorX = lColorX + rColorX;
    colorY = lColorY + rColorY;
    colorZ = lColorZ + rColorZ;

        //update bars for each color
        slideBarValRed = colorX/.0039215686;
        slideBarValGreen = colorY/.0039215686;
        slideBarValBlue = colorZ/.0039215686;
        
        previousBlueN = bN + 1;
        previousGreenN = gN + 1;
        previousRedN = n + 1;
        
        // red bar update
        v = n++;
        vertRed[v][0] = x1;
        ry2 = slideBarValRed + base;
        ry1 = ry2 - 15;
        vertRed[v][1] = ry1;
        vertRed[v][2] = ry2;

        // green bar update
        gV = gN++;
        vertGreen[gV][0] = gx1;
        gy2 = slideBarValGreen + base;
        gy1 = gy2 - 15;
        vertGreen[gV][1] = gy1;
        vertGreen[gV][2] = gy2;

        // blue bar update
        bV = bN++;
        vertBlue[bV][0] = bx1;
        by2 = slideBarValBlue + base;
        by1 = by2 - 15;
        vertBlue[bV][1] = by1;
        vertBlue[bV][2] = by2;
        
        glutPostRedisplay();

    }

break;
case GLUT_RIGHT_BUTTON:

// release the red, green or blue meter bar
rubberbanding = false;
redVert = false;
greenVert = false;
blueVert = false;
leftSideGrade = false;
rightSideGrade = false;

// remember where to continue in the for loop if the button is
// selected again
previousRedN = n;
previousGreenN = gN;
previousBlueN = bN;

// reset y-coordinates if mouse cursor was in the negative y-coordinate field while
// pressing the right button
if(ry1 < 0 && ry2 < 0)
{
    ry1 = Y1;
    ry2 = y2;
}else
    if(gy1 < 0 && gy2 < 0)
    {
        gy1 = Y1;
        gy2 = y2;
    }else
        if(by1 < 0 && by2 < 0)
        {
            by1 = Y1;
            by2 = y2;
        }

break;
}
}
void motion(int x, int y)
{

if(rubberbanding)
{
// red button selected
if(redVert == true)
{

// if mouse cursor is within the limit
if(x <= 105)
{

// test if the other buttons are still active
if(blueVert == true)
blueVert = false;
if(greenVert == true)
greenVert = false;
// if the mouse has been moved to a higher position
if((height - 1 - y) >= vertRed[v][3])
{
// the difference the mouse has moved in the y position
int diff = (height - 1 - y) - vertRed[v][3];
vertRed[v][1] = vertRed[v][1] + diff;
vertRed[v][2] = vertRed[v][2] + diff;
vertRed[v][3] = height - 1 - y;
ry1 = vertRed[v][1];
ry2 = vertRed[v][2];

// increase the color gradually while still under the vertical limit
if(ry2 <= MAX_HEIGHT && height-1-y >= MIN_HEIGHT)
{
    if(ry2 >65)
    {
        slideBarValRed = ry2 - base;
        colorX = .0039215686 * slideBarValRed;
    }

    if(slideBarValRed > 255)
        slideBarValRed = 255;
    else
        if(colorX > 1)
            colorX = 1;

}else// for very fast mouse movements
    if(height-1-y > MAX_HEIGHT)
    {
        slideBarValRed = 255;
        colorX = 1;
    }
// if the mouse passes the vertical limit and is released by
// pressing the right button, keep the bar at the vertical limit
if(height - 1 - y > MAX_HEIGHT)
{
ry1 = MAX_HEIGHT-15;
ry2 = MAX_HEIGHT;
}

}else// the mouse has been moved to a lower position
{

int diff = vertRed[v][3] - (height - 1 - y);
vertRed[v][1] = vertRed[v][1] - diff;
vertRed[v][2] = vertRed[v][2] - diff;
vertRed[v][3] = height - 1 - y;
ry1 = vertRed[v][1];
ry2 = vertRed[v][2];

// if the mouse passes the vertical limit and is released by
// pressing the right button, keep the bar at the vertical limit
if(height - 1 - y < MIN_HEIGHT)
{
ry1 = MIN_HEIGHT;
ry2 = MIN_HEIGHT + 15;
}
// decrease the color gradually while still over the vertical limit
if(height-1-y <= MAX_HEIGHT && ry1 >= MIN_HEIGHT)
{
    if(ry2 < MAX_HEIGHT)
    {
        slideBarValRed = ry2 - base;
        colorX = .0039215686 * slideBarValRed;
    }
    
    if(slideBarValRed < 0)
        slideBarValRed = 0;
    else
        if(colorX < 0)
            colorX = 0;
    
}else// for very fast mouse movements
    if(height-1-y < MIN_HEIGHT)
    {
        slideBarValRed = 0;
        colorX = 0;
    }

}

}else
{
    redVert = false;
    rubberbanding = false;
    previousRedN = n;
    previousGreenN = gN;
    previousBlueN = bN;

}// end if x <= 105

}// end if redVert == true

// green button selected
if(greenVert == true)
{
// if mouse cursor is within the limit
if(x >= gx1-10 && x <= 175)
{

// test if the other buttons are still active
if(redVert == true)
redVert = false;
if(blueVert == true)
blueVert = false;
// if the mouse has been moved to a higher position
if((height - 1 - y) >= vertGreen[gV][3])
{
// the difference the mouse has moved in the y position
int diff = (height - 1 - y) - vertGreen[gV][3];
vertGreen[gV][1] = vertGreen[gV][1] + diff;
vertGreen[gV][2] = vertGreen[gV][2] + diff;
vertGreen[gV][3] = height - 1 - y;
gy1 = vertGreen[gV][1];
gy2 = vertGreen[gV][2];
// if the mouse passes the vertical limit and is released by
// pressing the right button, keep the bar at the vertical limit
if(height - 1 - y > MAX_HEIGHT)
{
gy1 = MAX_HEIGHT-15;
gy2 = MAX_HEIGHT;
}
// increase the color gradually while still under the vertical limit
if(gy2 <= MAX_HEIGHT && height-1-y >= MIN_HEIGHT)
{
    if(gy2 >65)
    {
        slideBarValGreen = gy2 - base;
        colorY = .0039215686 * slideBarValGreen;
    }


    if(slideBarValGreen > 255)
        slideBarValGreen = 255;
    else
        if(colorY > 1)
            colorY = 1;

}else// for very fast mouse movements
    if(height-1-y > MAX_HEIGHT)
    {
        slideBarValGreen = 255;
        colorY = 1;
    }

}else// the mouse has been moved to a lower position
{
int diff = vertGreen[gV][3] - (height - 1 - y);
vertGreen[gV][1] = vertGreen[gV][1] - diff;
vertGreen[gV][2] = vertGreen[gV][2] - diff;
vertGreen[gV][3] = height - 1 - y;
gy1 = vertGreen[gV][1];
gy2 = vertGreen[gV][2];
// if the mouse passes the vertical limit and is released by
// pressing the right button, keep the bar at the vertical limit
if(height - 1 - y < MIN_HEIGHT)
{
gy1 = MIN_HEIGHT;
gy2 = MIN_HEIGHT + 15;
}
// decrease the color gradually while still over the vertical limit
if(height-1-y <= MAX_HEIGHT && gy1 >= MIN_HEIGHT)
{
    if(gy2 < MAX_HEIGHT)
    {
        slideBarValGreen = gy2 - base;
        colorY = .0039215686 * slideBarValGreen;
    }
    if(slideBarValGreen < 0)
        slideBarValGreen = 0;
    else
        if(colorY < 0)
            colorY = 0;
    
}else// for very fast mouse movements
    if(height-1-y < MIN_HEIGHT)
    {
        slideBarValGreen = 0;
        colorY = 0;
    }
}

}else
{
    greenVert = false;
    rubberbanding = false;
    previousRedN = n;
    previousGreenN = gN;
    previousBlueN = bN;

}// end if x >= gx1 && x <= 175

}// end if greenVert == true

// blue button selected
if(blueVert == true)
{

// if mouse cursor is within the limit
if(x >= bx1-10 && x <= bx2 + 10)
{

// test if the other buttons are still active
if(redVert == true)
redVert = false;
if(greenVert == true)
greenVert = false;
// if the mouse has been moved to a higher position
if((height - 1 - y) >= vertBlue[bV][3])
{
// the difference the mouse has moved in the y position
int diff = (height - 1 - y) - vertBlue[bV][3];
vertBlue[bV][1] = vertBlue[bV][1] + diff;
vertBlue[bV][2] = vertBlue[bV][2] + diff;
vertBlue[bV][3] = height - 1 - y;
by1 = vertBlue[bV][1];
by2 = vertBlue[bV][2];
// if the mouse passes the vertical limit and is released by
// pressing the right button, keep the bar at the vertical limit
if(height - 1 - y > MAX_HEIGHT)
{
by1 = MAX_HEIGHT-15;
by2 = MAX_HEIGHT;
}
// increase the color gradually while still under the vertical limit
if(by2 <= MAX_HEIGHT && height-1-y >= MIN_HEIGHT)
{
    if(by2 >65)
    {
        slideBarValBlue = by2 - base;
        colorZ = .0039215686 * slideBarValBlue;
    }

    if(slideBarValBlue > 255)
        slideBarValBlue = 255;
    else
        if(colorZ > 1)
            colorZ = 1;
}else// for very fast mouse movements
    if(height-1-y > MAX_HEIGHT)
    {
        slideBarValBlue = 255;
        colorY = 1;
    }
}else// the mouse has been moved to a lower position
{
int diff = vertBlue[bV][3] - (height - 1 - y);
vertBlue[bV][1] = vertBlue[bV][1] - diff;
vertBlue[bV][2] = vertBlue[bV][2] - diff;
vertBlue[bV][3] = height - 1 - y;
by1 = vertBlue[bV][1];
by2 = vertBlue[bV][2];
// if the mouse passes the vertical limit and is released by
// pressing the right button, keep the bar at the vertical limit
if(height - 1 - y < MIN_HEIGHT)
{
by1 = MIN_HEIGHT;
by2 = MIN_HEIGHT + 15;
}
// decrease the color gradually while still over the vertical limit
if(height-1-y <= MAX_HEIGHT && by1 >= MIN_HEIGHT)
{
    if(by2 < MAX_HEIGHT)
    {
        slideBarValBlue = by2 - base;
        colorZ = .0039215686 * slideBarValBlue;
    }
    if(slideBarValBlue < 0)
        slideBarValBlue = 0;
    else
        if(colorZ < 0)
            colorZ = 0;
    
}else// for very fast mouse movements
    if(height-1-y < MIN_HEIGHT)
    {
        slideBarValBlue = 0;
        colorZ = 0;
    }
}

}else
{
    blueVert = false;
    rubberbanding = false;
    previousRedN = n;
    previousGreenN = gN;
    previousBlueN = bN;

}// end if x >= bx1-10 && x <= bx2 + 10

}// end if blueVert == true

glutPostRedisplay();
}

}
void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'l':
            if(n > 0)
            {            
                previousRedN = n;// continue to display the red bar
                leftButton = true;
                rubberbanding = false;
                redVert = false;
                greenVert = false;
                blueVert = false;
            }
            if(gN > 0)
            {
                previousGreenN = gN;// continue to display the green bar
                leftButton = true;
                rubberbanding = false;
                redVert = false;
                greenVert = false;
                blueVert = false;
            }
            if(bN > 0)
            {
                previousBlueN = bN;// continue to display the blue bar
                leftButton = true;
                rubberbanding = false;
                redVert = false;
                greenVert = false;
                blueVert = false;
            }    
            lgradColorX = colorX;
            lgradColorY = colorY;
            lgradColorZ = colorZ;
            glutPostRedisplay();
            break;

        case 'r':
            if(n > 0)
            {
                previousRedN = n;// continue to display the red bar
                leftButton = false;
                rightButton = true;
                rubberbanding = false;
                redVert = false;
                greenVert = false;
                blueVert = false;
            }
            if(gN > 0)
            {
                previousGreenN = gN;// continue to display the green bar
                leftButton = false;
                rightButton = true;
                rubberbanding = false;
                redVert = false;
                greenVert = false;
                blueVert = false;
            }
            if(bN > 0)
            {
                previousBlueN = bN;// continue to display the blue bar
                leftButton = false;
                rightButton = true;
                rubberbanding = false;
                redVert = false;
                greenVert = false;
                blueVert = false;
            }
            rgradColorX = colorX;
            rgradColorY = colorY;
            rgradColorZ = colorZ;
            glutPostRedisplay();
            break;
    }
    glutPostRedisplay();
}
void main(int argc, char** argv)
{
glutInit(&argc,argv);
glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
glutInitWindowSize(500,500);
glutInitWindowPosition(500,100);
glutCreateWindow("Color Mixer Display");
glClearColor(0.0,0.0,0.0,0.0);
gluOrtho2D(0.0,500.0,0.0,500.0);
glutDisplayFunc(display3);
glutKeyboardFunc(keyboard);
glutMouseFunc(mouse);
glutMotionFunc(motion);
glutMainLoop();
}
