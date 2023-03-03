#include <graphics.h>
#include <winuser.h>
#include <windows.h>
#include <iostream>
#include <thread>
using namespace std;

const int ScreenX = GetSystemMetrics(SM_CXSCREEN) / 2;
const int ScreenY = GetSystemMetrics(SM_CYSCREEN) / 2;
const double percentToEdge = 0.35;
const int resMultiplier = 10;
const int xInResPixels = ((int)(ScreenX - ((ScreenX * percentToEdge) * 2)) / resMultiplier) * resMultiplier;
const int yInResPixels = ((int)(ScreenY - ((ScreenY * percentToEdge) * 2)) / resMultiplier) * resMultiplier;
const int pixelToEdgeX = (ScreenX - xInResPixels) / 2;
const int pixelToEdgeY = (ScreenY - yInResPixels) / 2;

const int columns = (ScreenX - pixelToEdgeX * 2) / resMultiplier;
const int rows = (ScreenY - pixelToEdgeY * 2) / resMultiplier;

int fps = 30;

int ** board = new int*[columns];
int white = 0;
int black = 0;

int initBoard(){
    for (int i = 0; i < columns; i++){
        board[i] = new int[rows];
    }

    for (int i = 0; i < columns; i++){
        for (int j = 0; j < rows; j++){
            black++;
            board[i][j] = BLACK;
        }
    }
    return 0;
}

int drawPixel(int x, int y, int color){
    x = x * resMultiplier;
    y = y * resMultiplier;
    int actualX = x + pixelToEdgeX;
    int actualY = ScreenY - pixelToEdgeY - y;
    int points[] = {actualX, actualY, actualX, actualY - resMultiplier, actualX + resMultiplier, actualY - resMultiplier, actualX + resMultiplier, actualY};       
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    fillpoly(4, points);     
    return 0;
}

int drawBoard(){
    thread threads[columns][rows];
    for (int i = 0; i < columns; i++){
        for (int j = 0; j < rows; j++){
            int pixel = board[i][j];
            //threads[i][j] = thread(drawPixel, i, j, pixel);
            drawPixel(i, j, pixel);
        }
    }

   /*for (int i = 0; i < columns; i++){
        for (int j = 0; j < rows; j++){
            threads[i][j].join();
        }
    }*/
    
    return 0;
}

int drawGameWindow(int x, int y){

    int offSet = 1;
    //top line
    line(pixelToEdgeX - offSet, pixelToEdgeY - offSet, x - pixelToEdgeX + offSet, pixelToEdgeY - offSet);

    //left line
    line(pixelToEdgeX - offSet, pixelToEdgeY - offSet, pixelToEdgeX - offSet, y - pixelToEdgeY + offSet);
    

    //right line
    line(x - pixelToEdgeX + offSet, pixelToEdgeY - offSet, x - pixelToEdgeX + offSet, y - pixelToEdgeY + offSet);

    //bottom line
    line(pixelToEdgeX - offSet, y - pixelToEdgeY + offSet, x - pixelToEdgeX + offSet, y - pixelToEdgeY + offSet);

    return 0;
}

int adjustColorCount(int subtract, int add){
    if(subtract == WHITE){
        white--;
    } else if(subtract == BLACK){
        black--;
    }

    if(add == WHITE){
        white++;
    } else if(add == BLACK){
        black++;
    }
    return 0;
}

int addPixel(int color){
    POINT cursorPosition;
    long clickX, clickY, gameX, gameY;
    int formerColor;
    GetCursorPos(&cursorPosition);
    clickX=mousex();
    clickY=mousey();
    gameX = (clickX - pixelToEdgeX) / resMultiplier;
    gameY = (ScreenY - pixelToEdgeY - clickY) / resMultiplier;
    formerColor = board[gameX][gameY];
    board[gameX][gameY] = color;
    adjustColorCount(formerColor, color);

    return 0;
}

int main(int argc, char const *argv[])
{
    
    int logs = 1;
    int gd = DETECT, gm;
    initwindow(ScreenX,ScreenY,"pixel engine",0,0);
    setactivepage(0);
    if (logs){
        cout << "PIXELS TO EDGE: " << pixelToEdgeX << ", " << pixelToEdgeY << endl;
        cout << "IN RECT PIXELS: " << xInResPixels << ", " << yInResPixels << endl;
        cout << "SCREEN PIXELS: " << ScreenX << ", " << ScreenY << endl;
    }
    initBoard();
    drawGameWindow(ScreenX, ScreenY);
    
    while(1){     
       if (GetAsyncKeyState(VK_LBUTTON)){
            addPixel(BLACK);
       } 
       if (GetAsyncKeyState(VK_RBUTTON)){
            addPixel(WHITE);
       }    
        drawBoard();
        delay(1000/fps);
    }
    closegraph();
    delete board;
    return 0;
}

