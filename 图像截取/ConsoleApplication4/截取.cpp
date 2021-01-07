#include <opencv2/opencv.hpp>
#include <tchar.h> 
#include <stdio.h>  
#include <iostream>  
#include <math.h>  
#include <ostream> 

using namespace std;
using namespace cv;

void main()

{
	VideoCapture capture("F://test(6).avi");
    if (!capture.isOpened())
	{
		return;
	}
	int imgIndex = 0;
	for (;;)
	{
		Mat frame;
		capture >> frame;
		if (frame.empty())
		{
			break;
		}
     	char* imageSaveName = new char[2000];
		sprintf(imageSaveName, "G:\\pic\\%d.jpg", imgIndex);
		imwrite(imageSaveName, frame);
		delete[] imageSaveName;
		imgIndex++;
	}
	system("pause");
}