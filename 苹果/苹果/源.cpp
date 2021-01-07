#include <stdio.h>
#include <string>
#include <iostream>
#include <io.h>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main()
{

	int h_low = 0;
	int h_upper = 9;
	int sub_low = 100;
	int sub_upper = 255;
	const int h_max = 180;
	const int sub_max = 255;

	namedWindow("ruby", CV_WINDOW_AUTOSIZE);
	createTrackbar("h_low_thresh", "ruby", &h_low, h_max);
	createTrackbar("h_upper_thresh", "ruby", &h_upper, h_max);
	createTrackbar("sub_low_thresh", "ruby", &sub_low, sub_max);
	createTrackbar("sub_upper_thresh", "ruby", &sub_upper, sub_max);
	

	Mat frame;
	frame = imread("G://5.png");
	//imshow("ss", frame);
	Mat rgb_channels[3];
	Mat r, g, b;
	split(frame, rgb_channels);
	r = rgb_channels[2];
	g = rgb_channels[1];
	b = rgb_channels[0];
	Mat r_img;
	inRange(r, sub_low, sub_upper, r_img);
	//imshow("ruby", r_img);
	Mat i;
	frame.copyTo(i,r_img);
	imshow("ruby", i);

	waitKey();
	return 0;
}