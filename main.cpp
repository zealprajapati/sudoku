//
//  main.cpp
//  Sudoku
//
//  Created by cpsfaculty on 02/08/18.
//  Copyright (c) 2018 ___Ju Shen___. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "VisualDisplay.h"

using namespace cv;
using namespace std;

int main(int argc, const char * argv[]) {
	// insert code here...
	VisualDisplay display;
	display.load((char *)"imgs/sudoku.png");
	display.run();
	return 0;
}