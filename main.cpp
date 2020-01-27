#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
#include <ctime>
#include "Serial.h"

#define STARTSTOPBYTE 0x13
#define STUFFBYTE 0x11

#define TIMEOUT 50

using namespace std;
const std::string window_detection_name = "HSV Settings";
const float CLOCKS_PER_MSEC = CLOCKS_PER_SEC * 1000; 

const int max_value_H = 360/2, max_value = 255;

int     low_H = 0, high_H = 28, 
        low_S = 63, high_S = 255, 
        low_V = 138, high_V = 255;

int prepareFrameForSending(ComProtocol&, const std::string&);
std::string byteStuffString(const std::string&);
uint8_t calculateChecksum(const uint8_t, const std::string&);
std::string string_to_hex(const std::string& input);

static void on_low_H_thresh_trackbar(int, void *)
{
    low_H = min(high_H-1, low_H);
    cv::setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
    high_H = max(high_H, low_H+1);
    cv::setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
    low_S = min(high_S-1, low_S);
    cv::setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
    high_S = max(high_S, low_S+1);
    cv::setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
    low_V = min(high_V-1, low_V);
    cv::setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
    high_V = max(high_V, low_V+1);
    cv::setTrackbarPos("High V", window_detection_name, high_V);
}

int main(void)
{
    Serial arduinoSerial;
	char buffer[4];
    bool dbg = false;
    
	if (arduinoSerial.open() == -1)
	{
		std::cout << "Error starting serial Com" << '\n';
		return -1;
	}
    

    cv::VideoCapture cap;
    int device_ID = 0;
    int apID = cv::CAP_ANY;
    cap.open(device_ID + apID);
    //cap.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);
    //cap.set(cv::CAP_PROP_EXPOSURE, 0.1);

    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }


    else{
        cout << "Your webcam is active" << endl;
        cout << "Press SPACEBAR to exit the program" << endl;

    }

    // HSV Thresholding Parameters
    //int low_H = 10; //0;  //0;      //25;     //11;     //25;     //40;     //20 - 0
    //int low_S = 60; //8;  //9;      //50;     //0;      //50;     //40;     //27 - 6
    //int low_V = 70; //162;    //0;      //0;      //116;    //58;     //70;    //100 - 42

    //int high_H = 60;    //180;   //30;    //40;    //65;    //56;    //88;    //32 - 29
    //int high_S = 255;   //57;    //255;   //255;   //255;   //255;   //255;   //255
    //int high_V = 255;   //224;   //255;   //255;   //255;   //255;   //255;   //255

    clock_t lastsampletime = clock();

    for (;;) {
        cv::Mat frame1, frame_HSV, frame_threshold, frameCanny, edges, drawCont;

        cap.grab();
        cap.retrieve(frame1, 0);

        if(frame1.empty()){
            cerr << "Error! Frame is empty." << endl;
            break;
        }

        //cv::resize(frame1, frame1, cv::Size(-1,-1), 0.5, 0.5, cv::INTER_LINEAR);            // Down size to increase speed

        cv::Rect roi(164, 90, 301, 301);
        frame1 = frame1(roi);
        
        cv::Mat dst = frame1.clone();

        // Pre-processing
 //       cv::blur(frame1, frame1, cv::Size(7,7));       // Gaussian Blur

        // HSV Thresholding

        cv::cvtColor(frame1, frame_HSV, cv::COLOR_BGR2HSV);                                                             // Convert from BGR to HSV colorspace
        cv::inRange(frame_HSV, cv::Scalar(low_H, low_S, low_V), cv::Scalar(high_H, high_S, high_V), frame_threshold);   // Detect the object based on HSV Range Values



        // Opening WHEN NOISE IS HIGH
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5), cv::Point(-1,-1));
        cv::morphologyEx(frame_threshold, frame_threshold, cv::MORPH_OPEN, kernel, cv::Point(-1,-1), 1, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());

        //cv::namedWindow("Threshold Open");              // Show the frame
        //cv::imshow("Threshold Open", frame_threshold);


        // Canny Edge Detector
        //cv::Canny(frame_threshold, frameCanny, 150, 50, 5);         // Edge detection
        
        if(dbg)
        {
            cv::namedWindow(window_detection_name);

            // Trackbars to set thresholds for HSV values
            cv::createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
            cv::createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
            cv::createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
            cv::createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
            cv::createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
            cv::createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);

            //cv::namedWindow("Webcam Live", cv::WINDOW_AUTOSIZE);                                // Show frame
            //cv::imshow("Webcam Live", frame1);
            //cv::moveWindow("Webcam Live", 0, 0);

            cv::namedWindow("HSV Threshold");                                                                               // Show the frame
            cv::imshow("HSV Threshold", frame_threshold);

            //cv::namedWindow("Canny");                                   // Show frame
           // cv::imshow("Canny", frameCanny);
        }

        // Find contours
        vector<vector<cv::Point> > contours;
        cv::findContours(frame_threshold.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        int x = 150;    // Standard position = center of image/platform
        int y = 150;    // Standard position = center of image/platform
        bool ballFound = false;

            for (size_t i = 0; i < contours.size(); ++i)
            {
                // Approximate contour with accuracy proportional
                // to the contour perimeter

                // Skip small or non-convex objects
                if (std::fabs(cv::contourArea(contours[i])) > 300)//std::fabs(cv::contourArea(contours[i])) > 700 && std::fabs(cv::contourArea(contours[i])) < 1600)
                   {
                    // Detect and label circles
                    double area = cv::contourArea(contours[i]);
                    cv::Rect r = cv::boundingRect(contours[i]);

                    x = r.x + (float)r.width/2;
                    y = r.y + (float)r.height/2;
                    

                    int radius = r.width / 2;

                    if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
                        std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2 && radius >= 16)
                        {
                        cv::circle(dst, cv::Point(x,y), radius, cv::Scalar(0,0,255), 2);
                        cv::circle(dst, cv::Point(x,y), 1, cv::Scalar(0,0,255), 2);
                        
                        x = x*(-1)+300; // Invert y coordinate for correct console output.                   
                        
                        cout << "Found a circle at " << endl << "[" << std::dec << x << ", " << std::dec << y << "]" << endl;
                        ballFound = true;
                        }
                    }
            }

            cv::namedWindow("Circles", cv::WINDOW_AUTOSIZE);             // Show frame
            cv::imshow("Circles", dst);
        
        clock_t timenow = clock();
        
        //std::cout << "Lastsampletime: " << (float)lastsampletime/CLOCKS_PER_SEC*1000 << '\n';
        //std::cout << "timenow: " << (float)timenow/CLOCKS_PER_SEC*1000 << '\n';
        if(((float)lastsampletime/CLOCKS_PER_SEC*1000 + TIMEOUT) < ((float)timenow/CLOCKS_PER_SEC*1000) && ballFound)
        {
		
        printf("Sending\n");
    
		buffer[0] = x & 0xFF;
		buffer[1] = (x >> 8) & 0xFF;
		buffer[2] = y & 0xFF;
		buffer[3] = (y >> 8) & 0xFF;

		prepareFrameForSending(arduinoSerial, std::string(buffer, 4));
        lastsampletime = clock();
        
        }

        if(cv::waitKey(5) == 32){                   // Press SPACEBAR to make a snapshot from Frame.
            cv::destroyAllWindows();
            break;
        }
        
    
        //usleep(300000);
    }



    return 0;


}

int prepareFrameForSending(ComProtocol &com, const std::string& string)
{
	std::string NewDataString;
	uint16_t StringLength = string.length();
	if (StringLength > 255) return -1;

	uint8_t Checksum = calculateChecksum((uint8_t)StringLength, string);

	NewDataString.push_back(STARTSTOPBYTE);
	NewDataString.push_back(StringLength);
	NewDataString.append(byteStuffString(string));
	NewDataString.push_back(Checksum);
	NewDataString.push_back(STARTSTOPBYTE);

	std::cout << "Original data string size: " << StringLength << "\n";
	std::cout << "Frame size: " << NewDataString.length() << "\n";
	std::cout << "String checksum: " << std::hex << static_cast<int>(Checksum) << "\n";
	std::cout << "String content: " << string_to_hex(NewDataString) << "\n";

	com.write(NewDataString);

	return 0;
}

std::string byteStuffString(const std::string& s)
{
	std::string NewString;
	for (std::string::const_iterator it = s.begin(); it != s.end(); it++)
	{
		if (*it == STARTSTOPBYTE)
		{
			NewString.push_back(0x11);
			NewString.push_back(0x09);
		}
		else if (*it == STUFFBYTE)
		{
			NewString.push_back(0x11);
			NewString.push_back(0x07);
		}
		else
		{
			NewString.push_back(*it);
		}
	}
	return NewString;
}

uint8_t calculateChecksum(const uint8_t DataLength, const std::string& Data)
{
	uint32_t Checksum = DataLength;
	for (std::string::const_iterator it = Data.begin(); it != Data.end(); ++it)
		Checksum += *it;

	return (uint8_t)Checksum;
}

std::string string_to_hex(const std::string& input)
{
	static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i)
	{
		const unsigned char c = input[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
	}
	return output;
}
