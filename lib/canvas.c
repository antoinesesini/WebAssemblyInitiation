#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>
#include <time.h>

//Number of circles to create
#define NUM_CIRCLES 500

//Circle struct
struct Circle {
  int x; //x position
  int y; //y position
  int r; //radius
  int cr; //color red
  int cg; //color green
  int cb; //color blue
};

struct CircleAnimationData {
  int x;
  int y;
  int r;
  int xv; //x velocity
  int yv; //y velocity
  int xd; //x direction (1 = forward)
  int yd; //y direction (1 = forward)
};

//Array of circles
struct Circle circles[NUM_CIRCLES];
struct CircleAnimationData animationData[NUM_CIRCLES];

int getRand(int max) {
  return rand() % max;
}

int main() {

  //Initialize random number generator
  srand(time(NULL));
  //Initialize circles
  for (int i = 0; i < NUM_CIRCLES; i++) {
    //Radius
    int radius = getRand(50);

    //Coordinates
    int x = getRand(1000)+radius;
    int y = getRand(1000)+radius;

    //Fill animation data struct - i
    animationData[i].x = x;
    animationData[i].y = y;
    animationData[i].r = radius;
    animationData[i].xv = getRand(10);
    animationData[i].yv = getRand(10);
    animationData[i].xd = 1;
    animationData[i].yd = 1;


    //Fill circle struct - i
    circles[i].x = x;
    circles[i].y = y;
    circles[i].r = radius;
    circles[i].cr = getRand(255);
    circles[i].cg = getRand(255);
    circles[i].cb = getRand(255);
  }


  EM_ASM({render($0, $1);}, NUM_CIRCLES*6, 6);
}

struct Circle * getCircles(int canvasWidth, int canvasHeight) {

    //Update circle data
    for (int i=0; i<NUM_CIRCLES; i++) {
        //Collision detection - RIGHT => set x direction to 0
        if(animationData[i].x+ animationData[i].r >= canvasWidth) {
            animationData[i].xd = 0;
        }
        //Collision detection - LEFT => set x direction to 1
        if(animationData[i].x- animationData[i].r <= 0) {
            animationData[i].xd = 1;
        }
        //Collision detection - TOP => set y direction to 1
        if(animationData[i].y- animationData[i].r <= 0) {
            animationData[i].yd = 1;
        }
        //Collision detection - BOTTOM => set y direction to 0
        if(animationData[i].y+ animationData[i].r >= canvasHeight) {
            animationData[i].yd = 0;
        }

        //Update x position
        if(animationData[i].xd == 1) {
            animationData[i].x += animationData[i].xv;
        } else {
            animationData[i].x -= animationData[i].xv;
        }
        //Update y position
        if(animationData[i].yd == 1) {
            animationData[i].y += animationData[i].yv;
        } else {
            animationData[i].y -= animationData[i].yv;
        }

        //Update circle struct - i
        circles[i].x = animationData[i].x;
        circles[i].y = animationData[i].y;
    }

  return circles;
}