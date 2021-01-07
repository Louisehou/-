#include <stdio.h>
#include <string>
#include <iostream>
#include <io.h>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void fillHole(const Mat srcBw, Mat &dstBw);

int main()
{
	VideoCapture capture;
	capture.open("G://test.avi");
	namedWindow("ruby", CV_WINDOW_AUTOSIZE);
	//namedWindow("lab", CV_WINDOW_AUTOSIZE);
	int h_low = 0;
	int h_upper = 9;
	int sub_low = 100;
	int sub_upper = 255;
	const int h_max = 180;
	const int sub_max = 255;

	createTrackbar("h_low_thresh", "ruby", &h_low, h_max);
	createTrackbar("h_upper_thresh", "ruby", &h_upper, h_max);
	createTrackbar("sub_low_thresh", "ruby", &sub_low, sub_max);
	createTrackbar("sub_upper_thresh", "ruby", &sub_upper, sub_max);
	//createTrackbar("c_low_thresh", "lab", &c_low, c_max);
	//createTrackbar("c_upper_thresh", "lab", &c_upper, c_max);

	while (1)
	{
		Mat org_img;
		Mat roi1, roi2, roi3;
		Mat hsv_channels1[3];
		Mat endimg, hsv_img;
		Mat h1;
		Mat fillimg;
		capture >> org_img;
		if (org_img.empty())break;

		Mat dst_img = org_img.clone();
		Mat hsv_img1;
		cvtColor(org_img, hsv_img, CV_BGR2HSV);
		cv::Mat hsv_channels[3];
		split(hsv_img, hsv_channels);
		cv::Mat h, s, v;
		h = hsv_channels[0];
		s = hsv_channels[1];
		v = hsv_channels[2];
		cv::Mat h_bin_img;
		inRange(h, h_low, h_upper, h_bin_img);
		cv::Mat h_seg_img;
		org_img.copyTo(h_seg_img, h_bin_img);
		Mat s_bin_img;
		inRange(s, sub_low, sub_upper, s_bin_img);
		cv::Mat h_seg_img1;
		h_seg_img.copyTo(h_seg_img1, s_bin_img);
		Mat redimage;
		h_bin_img.copyTo(redimage, s_bin_img);
		//Mat element = getStructuringElement(MORPH_RECT,Size(2, 2));
		//Mat open_result;
		//morphologyEx(h_seg_img1, open_result, MORPH_OPEN, element);
		Mat midimg;
		cvtColor(h_seg_img1, midimg, CV_BGR2GRAY);
		vector<vector<Point>> contours;
		vector<Vec4i> hierarcy;
		findContours(midimg, contours, hierarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		vector<vector<Point>> contours1;
		for (int i = 0; i < contours.size(); ++i)
		{
			if (contours[i].size() < 1000 && contours[i].size() > 250)//½«±È½ÏÐ¡µÄÂÖÀªÌÞ³ýµô  
			{
				contours1.push_back(contours[i]);
			}
		}
		vector<Rect> boundRect(contours1.size());
		//drawContours(h_seg_img1, contours1, -1, Scalar(255, 255, 255), 2, 8);
		vector<RotatedRect> box(contours1.size());
		Point2f rect[4];
		for (int i = 0; i<contours1.size(); i++)
		{
			box[i] = minAreaRect(Mat(contours1[i]));
			boundRect[i] = boundingRect(Mat(contours1[i]));
			//circle(dst_img, Point(box[i].center.x, box[i].center.y), 5, Scalar(0, 255, 0), -1, 8);
			box[i].points(rect);

			/* rectangle(dst_img, Point(boundRect[i].x, boundRect[i].y), Point(boundRect[i].x + boundRect[i].width, boundRect[i].y + boundRect[i].height), Scalar(0, 255, 0), 2);*/
			/*for (int j = 0; j<4; j++)
			{
			line(dst_img, rect[j], rect[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);
			}*/
			Point2f center;
			float radius;
			minEnclosingCircle(contours1[i], center, radius);
			int radius1 = (int)radius;
			rectangle(h_seg_img1, Point(boundRect[i].x - 50, boundRect[i].y - 50), Point(boundRect[i].x + boundRect[i].width + 50, boundRect[i].y + boundRect[i].height + 50), Scalar(0, 255, 0), 2);

			Mat midimg2;
			cvtColor(dst_img, midimg2, CV_RGB2GRAY);
			Rect roimid;
			roimid.x = boundRect[i].x - 50;
			roimid.y = boundRect[i].y - 50;
			roimid.height = boundRect[i].height + 100;
			roimid.width = boundRect[i].width + 100;
			roimid &= Rect(0, 0, org_img.cols, org_img.rows);
			roi1 = org_img(Rect(roimid));
			medianBlur(roi1, roi2, 9);
			/*Rect maskimage;
			maskimage.height= boundRect[i].height + 102;
			maskimage.width = boundRect[i].width + 102;
			floodFill(roi1, Point(box[i].center.x, box[i].center.y), Scalar(20, 20, 20), &maskimage, 4);*/


			/*roi1 = midimg2(Rect(roimid));
			inRange(roi1, 11, 60, seg_img3);
			bitwise_not(seg_img3, seg_img3);
			roi3 = dst_img(Rect(roimid));
			roi2 = h_seg_img1(Rect(roimid));
			addWeighted(seg_img3, 1, roi2, 1, 0, roi2);*/
			cvtColor(roi2, hsv_img1,CV_RGB2HSV);
			split(hsv_img1, hsv_channels1);
			h1 = hsv_channels1[0];
			inRange(h1, 0, 60, endimg);
			Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
			Mat close_result;
			//Mat close_result1;
			morphologyEx(endimg, close_result, MORPH_CLOSE, element);
			//morphologyEx(endimg, close_result, MORPH_CLOSE, element);
			roi2 = redimage(Rect(roimid));
			addWeighted(close_result, 1, roi2, 1, 0, roi2);
			roi3 = redimage(Rect(roimid));
			//fillHole(roi3, fillimg);

		}
		
		imshow("ruby", h_seg_img1);
		waitKey(500);
	}

	return 0;
}
void fillHole(const Mat srcBw, Mat &dstBw)
{
	Size m_Size = srcBw.size();
	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//ÑÓÕ¹Í¼Ïñ
	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	Mat cutImg;//²Ã¼ôÑÓÕ¹µÄÍ¼Ïñ
	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	dstBw = srcBw | (~cutImg);
}