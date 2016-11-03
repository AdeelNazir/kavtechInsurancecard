//g++ -std=c++11 `pkg-config --cflags opencv tesseract` insurance_ocr.cpp `pkg-config --libs opencv tesseract` -o insurance_ocr
// insurance_ocr.cpp : main project file.
//SURF
#include "opencv2/opencv.hpp"
#include <vector>  
#include <iostream>  
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <fstream>
#include <string>
////
//OCR
# include "opencv2/features2d/features2d.hpp"
# include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "opencv2/calib3d/calib3d.hpp"
///
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "tesseract/baseapi.h"
#include "leptonica/allheaders.h"
#include <iostream>


using namespace cv;
using namespace std;

class DOOCR
{
	// TODO: Add your methods for this class here.
public:
	//System::String^ OCRstring;
	void GetfieldOCR(Rect rect, Mat FrontImage,int index, tesseract::TessBaseAPI *tess,string company);
	Mat NormlizImage(Mat oTexture, Point topLeft, Point topRight, Point bottomRight, Point bottomLeft);
	void InitializeOCR();
};

int CardIndex = 0;
int erosion_elem = 1;
int erosion_size = 1;
int dilation_elem = 1;
int dilation_size = 1;
int threshold_value = 160;
int threshold_type = 0;
int const max_BINARY_value = 255;
double DrawingFont = 1;
int count = 0;
struct PointMap
{
public:
	Point From;
	Point To;
	PointMap(Point oFrom, Point oTo)
	{
		From = oFrom;
		To = oTo;
	}
	PointMap()
	{}
};
//for sorting rectangle using x position
struct byXposition {
	bool operator () (const Rect & a, const Rect & b) {
		return (a.x) < (b.x);
	}
};
//for sorting rectangle using y position
struct byYposition {
	bool operator () (const Rect & a, const Rect & b) {
		return (a.y) < (b.y);
	}
};
//for sorting rectangle using area
struct byHeight {
	bool operator () (const Rect & a, const Rect & b) {
		return (a.height) > (b.height);
	}
};
struct byWidth {
	bool operator () (const Rect & a, const Rect & b) {
		return (a.width) > (b.width);
	}
};
std::vector<Rect> FindBloobs(Mat imag)
{
	std::vector<Rect> rects_list;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<Vec4i> hierarchy;
	findContours(imag, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	int MinWidth = (int)(imag.cols * 0.004f);
	int MinHeight = (int)(imag.cols * 0.009f);

	int MaxWidth = (int)(imag.cols * 0.04f);
	int MaxHeight = (int)(imag.cols * 0.04f);

	for (size_t i = 0; i < contours.size(); i++)
	{
		cv::Point p(boundingRect(contours[i]).x, boundingRect(contours[i]).y);
		Rect r = boundingRect(contours[i]);
		if (r.width > MinWidth && r.width < MaxWidth && r.height > MinHeight && r.height < MaxHeight)
		{
			rects_list.push_back(r);
		}
	}

	return rects_list;
}
bool valueInRange(int value, int min, int max, float range)
{
	return (value >= min - range) && (value <= max + range);
}
cv::Rect  GettUnion(cv::Rect A, cv::Rect B)
{
	int x = 0, y = 0, width = 0, height = 0;
	if (A.x < B.x)
		x = A.x;
	else
		x = B.x;
	if (A.y < B.y)
		y = A.y;
	else
		y = B.y;

	if ((A.x + A.width) < (B.x + B.width))
		width = (B.x + B.width) - x;
	else
		width = (A.x + A.width) - x;
	if ((A.y + A.height) < (B.y + B.height))
		height = (B.y + B.height) - y;
	else
		height = (A.y + A.height) - y;
	//cout << A << B << Rect(x, y, width, height);
	return Rect(x, y, width, height);
}
bool AreOverlapingSameLine(Rect A, Rect B, Rect* rect)
{
	float factor = 0.6;

	float range;
	if (A.height / 2 < B.height / 2)
		range = A.height * factor;
	else
		range = B.height * factor;
	//range = 2;
	bool xOverlap = valueInRange(A.x, B.x, B.x + B.width, range) ||
		valueInRange(B.x, A.x, A.x + A.width, range);
	range = 0;
	bool yOverlap = valueInRange(A.y, B.y, B.y + B.height, range) ||
		valueInRange(B.y, A.y, A.y + A.height, range);
	/*if (xOverlap && yOverlap)
		*rect = GettUnion(A, B);*/
	bool horizontal = abs(B.y - A.y) < A.height*0.6 && (B.height < A.height * 1.5 && B.height > A.height * 0.5);
	
	return xOverlap && yOverlap && horizontal;
}
bool AreOverlaping(Rect A, Rect B, Rect* rect)
{
	float factor = 0.2;

	float range;
	if (A.height / 2 < B.height / 2)
		range = A.height * factor;
	else
		range = B.height * factor;
	//range = 0;
	bool xOverlap = valueInRange(A.x, B.x, B.x + B.width, range) ||
		valueInRange(B.x, A.x, A.x + A.width, range);
	range = 0;
	bool yOverlap = valueInRange(A.y, B.y, B.y + B.height, range) ||
		valueInRange(B.y, A.y, A.y + A.height, range);

	bool horizontal = abs(B.y - A.y) < A.height*0.6 && (B.height < A.height * 1.5 && B.height > A.height * 0.5);

	/*if (xOverlap && yOverlap)
	*rect = GettUnion(A, B);*/
	return xOverlap && yOverlap;
}
bool IsParenthesesOrDash(char c)
{
	switch (c)
	{
	case '?':
		return true;
	default:
		return false;
	}
}
Mat Erosion(Mat src)
{
	Mat erosion_dst;
	int erosion_type;
	if (erosion_elem == 0){ erosion_type = MORPH_RECT; }
	else if (erosion_elem == 1){ erosion_type = MORPH_CROSS; }
	else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(erosion_type,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		cv::Point(erosion_size, erosion_size));

	/// Apply the erosion operation
	erode(src, erosion_dst, element);
	return erosion_dst;
}
Mat Dilation(Mat src)
{
	Mat dilation_dst;
	int dilation_type;
	if (dilation_elem == 0){ dilation_type = MORPH_RECT; }
	else if (dilation_elem == 1){ dilation_type = MORPH_CROSS; }
	else if (dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(dilation_type,
		cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		cv::Point(dilation_size, dilation_size));
	/// Apply the dilation operation
	dilate(src, dilation_dst, element);
	return dilation_dst;
}
Mat Threshode(Mat src)
{
	Mat thImage;
	int size = 0;
	threshold_value = 200;
	do{
		threshold(src, thImage, threshold_value, max_BINARY_value, threshold_type);
		IplImage image = thImage;
		threshold_value -= 5;
		size = cvCountNonZero(&image);
	} while (size < (thImage.cols * thImage.rows) * 0.6 && threshold_value > 0);
	//threshold(src, thImage, threshold_value, max_BINARY_value, threshold_type);
	return thImage;
}
void DOOCR::InitializeOCR()
{
	
}


std::vector<Rect> RemoveOverlaping(std::vector<Rect> rects_list)
{
	//Removing Overlaping
	for (int i = 0; i < rects_list.size(); i++)
	{
		for (int j = i + 1; j < rects_list.size(); j++)
		{
			Rect RECT = Rect();
			bool isOverlapping = AreOverlaping(rects_list[i], rects_list[j], &RECT);
			if (isOverlapping)
			{
				int size1 = rects_list[i].width * rects_list[i].height;
				int size2 = rects_list[j].width * rects_list[j].height;
				if (size1 > size2)
				{
					//cout << GettUnion(rects_list[i], rects_list[j]) << endl;
					rects_list[i] = GettUnion(rects_list[i], rects_list[j]);
					rects_list.erase(rects_list.begin() + j);
					j--;
				}
				else
				{
					//cout << GettUnion(rects_list[i], rects_list[j]) << endl;
					rects_list[j] = GettUnion(rects_list[i], rects_list[j]);
					rects_list.erase(rects_list.begin() + i);
					i--;
					break;
				}
			}
		}
	}

	return rects_list;
}
std::vector<Rect> RemoveOverlapingSameLine(std::vector<Rect> rects_list)
{
	//Removing Overlaping
	for (int i = 0; i < rects_list.size(); i++)
	{
		for (int j = i + 1; j < rects_list.size(); j++)
		{
			Rect RECT = Rect();
			bool isOverlapping = AreOverlapingSameLine(rects_list[i], rects_list[j], &RECT);
			if (isOverlapping)
			{
				int size1 = rects_list[i].width * rects_list[i].height;
				int size2 = rects_list[j].width * rects_list[j].height;
				if (size1 > size2)
				{
					//cout << GettUnion(rects_list[i], rects_list[j]) << endl;
					rects_list[i] = GettUnion(rects_list[i], rects_list[j]);
					rects_list.erase(rects_list.begin() + j);
					j--;
				}
				else
				{
					//cout << GettUnion(rects_list[i], rects_list[j]) << endl;
					rects_list[j] = GettUnion(rects_list[i], rects_list[j]);
					rects_list.erase(rects_list.begin() + i);
					i--;
					break;
				}
			}
		}
	}

	return rects_list;
}

std::vector<Rect> getBloobs(Mat imag)
{
	std::vector<Rect> rects_list;
	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;
	findContours(imag, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	int MinWidth = (int)(imag.cols * 0.002f);
	int MinHeight = (int)(imag.cols * 0.006f);

	int MaxWidth = (int)(imag.cols * 0.08f);
	int MaxHeight = (int)(imag.cols * 0.1f);

	for (size_t i = 0; i < contours.size(); i++)
	{
		cv::Point p(boundingRect(contours[i]).x, boundingRect(contours[i]).y);
		Rect r = boundingRect(contours[i]);
		if (r.width > MinWidth && r.width < MaxWidth && r.height > MinHeight && r.height < MaxHeight)
		{
			rects_list.push_back(r);
		}
	}
	std::sort(rects_list.begin(), rects_list.end(), byXposition());
	//rects_list = RemoveOverlapingSameLine(rects_list);
	//rects_list = RemoveOverlaping(rects_list);
	return rects_list;
}
std::vector<Rect> ReGetBloobs(Mat imag)
{
	std::vector<Rect> rects_list;
	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;
	findContours(imag, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	int MinWidth = (int)(imag.rows * 0.3f);
	int MinHeight = (int)(imag.rows * 0.4f);

	int MaxWidth = (int)(imag.cols * 1.0f);
	int MaxHeight = (int)(imag.rows * 1.0f);

	for (size_t i = 0; i < contours.size(); i++)
	{
		cv::Point p(boundingRect(contours[i]).x, boundingRect(contours[i]).y);
		Rect r = boundingRect(contours[i]);
		//if (r.width > MinWidth && r.width < MaxWidth && r.height > MinHeight && r.height < MaxHeight)
		{
			rects_list.push_back(r);
		}
	}
	std::sort(rects_list.begin(), rects_list.end(), byXposition());
	//rects_list = RemoveOverlapingSameLine(rects_list);
	//rects_list = RemoveOverlaping(rects_list);
	return rects_list;
}
void split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
}
vector<string> split(const string &s, char delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}
string removeOnlyAlpha(string data)
{
	vector<string> words = split(data, ' ');
	for (int i = 0; i < words.size(); i++){
		bool isnum = false;
		for (int j = 0; j < words[i].size(); j++){
			if (isalpha(words[i][j]))
				isnum = true;
			else
			{
				isnum = false;
				break;
			}
		}
		if (isnum){
			words.erase(words.begin() + i);
			i--;
		}
	}
	string newData = "";
	for (int i = 0; i < words.size(); i++)
		newData += words[i] + " ";
	return newData;
}
//std::vector<Rect> GetOverlaping(Rect rect, std::vector<Rect> rects_list, std::vector<int> indexs, std::vector<String> ocrs, string *text)
//{
//	std::vector<Rect> Over_rects_list;
//	//Removing Overlaping
//	for (int i = 0; i < rects_list.size(); i++)
//	{
//			Rect RECT = Rect();
//			Rect A = rect, B = rects_list[i];
//			int range = 0;
//			bool xOverlap = valueInRange(A.x, B.x, B.x + B.width, range) ||
//				valueInRange(B.x, A.x, A.x + A.width, range);
//			range = 0;
//			bool yOverlap = valueInRange(A.y, B.y, B.y + B.height, range) ||
//				valueInRange(B.y, A.y, A.y + A.height, range);
//
//			//bool horizontal = abs(B.y - A.y) < A.height*0.5 && (B.height < A.height * 1.5 && B.height > A.height * 0.5);
//			bool IsHeading = false;
//			for (int l = 0; l < indexs.size();l++)
//			if (i == indexs[l])
//				IsHeading = true;
//
//			if (xOverlap && yOverlap && !IsHeading )
//			{
//				*text += ocrs[i] + " ";
//				Over_rects_list.push_back(rects_list[i]);
//			}
//			
//	}
//
//	vector<string> strings = split(*text, ' ');
//	*text = "";
//	bool done = false;
//	for (int l = strings.size()-1; l > -1;l--){
//		if(strings[l] != "")
//		if(strings[l].size() < 2 && !done)
//		{
//			cout<< " R :"<<strings[l];
//			strings.erase(strings.begin() + l);
//			
//		}
//		else
//		{
//			done = true;	
//		}
//	}
//	for (int l = 0;l<strings.size();l++)
//	*text += strings[l]+" ";		
//	return Over_rects_list;
//}
string GetOcr(Rect recs, Mat gimage, tesseract::TessBaseAPI *tess)
{
	Rect local(recs.x - 1, recs.y,
		recs.width + 2, recs.height);
	
	Mat sub = Threshode(gimage(local));
	//imshow("sub", sub);
	//waitKey(1000);
	Mat imageToPaste = Mat(sub.rows * 3, sub.cols * 3, gimage.type(), cvScalar(255, 255, 255));
	sub.copyTo(imageToPaste.rowRange(sub.rows, sub.rows + sub.rows).colRange(sub.cols, sub.cols + sub.cols));

	(*tess).SetImage((uchar*)imageToPaste.data, imageToPaste.size().width,
		imageToPaste.size().height, imageToPaste.channels(), imageToPaste.step1());
	(*tess).Recognize(0);
	const char* out = (*tess).GetUTF8Text();

	string OCR = out;
	//if (OCR.size() == 0)
	//	OCR = "I\n\n" + OCR;
	int size = OCR.size();
	if (size > 2)
	for (size_t k = size - 1; k > size - 3; k--){
		if (k > 0)
			OCR.erase(OCR.begin() + k);
		else
			break;
	}
	return OCR;
}
std::vector<Rect> GetOverlaping(string name, Mat gimage, tesseract::TessBaseAPI *tess, Rect rect, std::vector<Rect> rects_list, std::vector<int> indexs, std::vector<String> ocrs, string *text)
{
	//std::sort(rects_list.begin(), rects_list.end(), byWidth());
	std::vector<Rect> Over_rects_list;
	Mat PrevBlob = Mat();
	vector<int> existing;
	int lastx = 0;
	//Removing Overlaping
	for (int i = 0; i < rects_list.size(); i++)
	{
		Rect RECT = Rect();
		Rect A = rect, B = rects_list[i];
		int range = 0;
		bool xOverlap = valueInRange(A.x, B.x, B.x + B.width, range) ||
			valueInRange(B.x, A.x, A.x + A.width, range);
		range = 0;
		bool yOverlap = valueInRange(A.y, B.y, B.y + B.height, range) ||
			valueInRange(B.y, A.y, A.y + A.height, range);

		//bool horizontal = abs(B.y - A.y) < A.height*0.5 && (B.height < A.height * 1.5 && B.height > A.height * 0.5);
		bool IsHeading = false;
		for (int l = 0; l < indexs.size(); l++)
		if (i == indexs[l])
			IsHeading = true;

		

		if (xOverlap && yOverlap && !IsHeading)
		{
			////////////////////////////  removing small blobs
			bool xxOverlap= false, yyOverlap=false, big = false;
			for (int j = 0; j < existing.size(); j++)
			{
				A = rects_list[existing[j]], B = rects_list[i];
				range = 0;
				xxOverlap = valueInRange(A.x, B.x, B.x + B.width, range) ||
					valueInRange(B.x, A.x, A.x + A.width, range);
				range = 0;
				yyOverlap = valueInRange(A.y, B.y, B.y + B.height, range) ||
					valueInRange(B.y, A.y, A.y + A.height, range);
				big = B.width < A.width;
				if(xxOverlap && yyOverlap )
				break;
			}
			bool isovelappingExisting = (xxOverlap && yyOverlap);
			bool inrange = true;
			if(lastx != 0){
			if(rects_list[i].x - lastx < gimage.cols*0.2)
			 inrange = true;
			else
			inrange = false;
			}
			///////////////////////////////////////////////////////////////////
			if (!isovelappingExisting && inrange)
			{
				if (name == "Name"){
					Mat blob = gimage(Rect(rects_list[i].x - 2, rects_list[i].y - 2, rects_list[i].width + 4, rects_list[i].height + 4));

					if (PrevBlob.cols > 0 && blob.rows < PrevBlob.rows*0.7)
						continue;
					PrevBlob = blob;
					Mat canny = blob.clone();
					int lowThreshold = 160;
					int heighThreshold = 255;
					Canny(canny, canny, lowThreshold, heighThreshold, 3);
					//imshow("blob", blob);
					//imshow("canny", canny);
					//waitKey(2000);

					vector<Rect> blobs = ReGetBloobs(canny.clone());
					blobs = RemoveOverlaping(blobs);
					for (int l = 0; l < blobs.size(); l++)
						*text += GetOcr(blobs[l], blob, tess) + " ";
				}
				else
				{
					*text += ocrs[i] + " ";
				}
				Over_rects_list.push_back(rects_list[i]);
				existing.push_back(i);
				lastx = rects_list[i].x + rects_list[i].width;
			}
		}

	}

	/*vector<string> strings = split(*text, ' ');
	*text = "";
	bool done = false;
	for (int l = strings.size() - 1; l > -1; l--){
		if (strings[l] != "")
		if (strings[l].size() < 2 && !done)
		{
			cout << " R :" << strings[l];
			strings.erase(strings.begin() + l);

		}
		else
		{
			done = true;
		}
	}
	for (int l = 0; l<strings.size(); l++)
		*text += strings[l] + " ";*/
	return Over_rects_list;
}

void DOOCR::GetfieldOCR(Rect rect,Mat fieldImage,int index, tesseract::TessBaseAPI *tess,string Company)
{
	CardIndex = index;
	Mat gimage;
	cvtColor(fieldImage, gimage, CV_RGB2GRAY);
	int lowThreshold = 160;
	int heighThreshold = 255;
	Canny(gimage, gimage, lowThreshold, heighThreshold, 3);
	
	//imshow("gimage",gimage);
	//waitKey(1000);
	vector<Rect> blobs = getBloobs(gimage.clone());
	blobs = RemoveOverlapingSameLine(blobs);
	vector<vector<Rect> > AsRect;
	//if (blobs.size() == 0)
	//	return "No blob";
	std::sort(blobs.begin(), blobs.end(), byXposition());
	////..............................Removing garbage rectangless and combine overlaping
	vector<Rect> recs = blobs;
	for (int i = 0; i<recs.size(); i++)
	{
		if ((recs[i].width > fieldImage.cols*0.6 || recs[i].width < fieldImage.cols*0.01 || recs[i].height > fieldImage.cols*0.1)){
			recs.erase(recs.begin() + i);
			i--;
		}
	}
	recs = RemoveOverlapingSameLine(recs);
	
	string str = "\n";
	///////////////////////////////////////////////////////////////////////////////
	////Initialing OCR
	cvtColor(fieldImage, gimage, CV_RGB2GRAY);
	Mat OCRimg(gimage.rows, gimage.cols, CV_8UC3, Scalar(255, 255, 255));
	int endx = 0;
	int percent = recs.size() / 100;
	int inc = 1;

	std::sort(recs.begin(), recs.end(), byXposition());

	vector<int> indexs;
	vector<string> Headings;
	vector<string> ocrs;
	/////////////////////////////////////////
	//for (int j = 0; j < AsRect.size(); j++)
	{
		//recs = AsRect[j];
		for (int i = 0; i < recs.size(); i++)
		{
			int xfactor = 0;// Blobs[i].width / 10;
			int yfactor = 0;// Blobs[i].height / 10;
			Rect local(recs[i].x, recs[i].y,
				recs[i].width, recs[i].height);


			//imshow("gimage", gimage);
			Mat sub = Threshode(gimage(local));

			Mat imageToPaste = Mat(sub.rows * 3, sub.cols * 3, gimage.type(), cvScalar(255, 255, 255));
			sub.copyTo(imageToPaste.rowRange(sub.rows, sub.rows + sub.rows).colRange(sub.cols, sub.cols + sub.cols));

			/*if (index == 5)
			dilate(imageToPaste, imageToPaste, getStructuringElement(MORPH_CROSS, Size(3, 3)));*/

			//imshow("blob", sub);
			//waitKey(1000);
			(*tess).SetImage((uchar*)imageToPaste.data, imageToPaste.size().width,
				imageToPaste.size().height, imageToPaste.channels(), imageToPaste.step1());
			(*tess).Recognize(0);
			const char* out = (*tess).GetUTF8Text();

			string OCR = out;
			//if (OCR.size() == 0)
			//	OCR = "I\n\n" + OCR;
			int size = OCR.size();
			if (size > 2)
			for (size_t k = size - 1; k > size - 3; k--){
				if (k > 0)
					OCR.erase(OCR.begin() + k);
				else
					break;
			}
			OCR.erase(remove_if(OCR.begin(), OCR.end(), [](char c) { return !isalpha(c) && !isalnum(c); }), OCR.end());
			//cout << OCR << " ";
			str += OCR;
			endx = recs[i].x + recs[i].width;
			//str += " "+OCR;
			rectangle(fieldImage, Point(local.x, local.y), Point((local.x + local.width), (local.y + local.height)), Scalar(0, 0, 255), 4, 8, 0);
			putText(fieldImage, OCR, cvPoint(local.x, local.y),
				FONT_HERSHEY_COMPLEX_SMALL, fieldImage.cols*0.001, cvScalar(0, 0, 250), fieldImage.cols*0.0015, CV_AA);
			bool isMateched = false;

			//Group
			//read group file and find
			bool wordfound = false;
			std::ifstream myfileg("ocrTxtFiles/group.txt");
			std::string lineg;
			if (myfileg.is_open())
			{
			  while (std::getline (myfileg,lineg))
			  {
			     if (lineg != " " && lineg != "" && (OCR.find(lineg)) != string::npos)
			     {
			     	Headings.push_back("Group");
			     	indexs.push_back(i);
			     	wordfound = true;
			     	break;
			     }
			  }
			  myfileg.close();
			}
			else
				cout << "can not open group file" << endl;

			if (!wordfound)
			{
				if ((OCR.find("Gro")) != string::npos || (OCR.find("oup")) != string::npos || (OCR.find("Grau")) != string::npos
				|| (OCR.find("GRO")) != string::npos || (OCR.find("OUP")) != string::npos || (OCR.find("Gmup")) != string::npos)
				{
					if (std::find(Headings.begin(), Headings.end(), "Group") != Headings.end())
						indexs[find(Headings.begin(), Headings.end(), "Group") - Headings.begin()] = i;
					else{
						indexs.push_back(i);
						Headings.push_back("Group");
					}
					// rectangle(fieldImage, Point(local.x, local.y), Point((local.x + local.width), (local.y + local.height)), Scalar(255, 0, 0), 3, 8, 0);
					
				}
				else
				{

					if ((OCR.find("G")) != string::npos && ((OCR.find("p")) != string::npos || (OCR.find("P")) != string::npos)
						&& OCR.size() < 6 && OCR.size() > 3)
					{
						if (std::find(Headings.begin(), Headings.end(), "Group") == Headings.end())
						{
							Headings.push_back("Group");
							indexs.push_back(i);
						}
						// rectangle(fieldImage, Point(local.x, local.y), Point((local.x + local.width), (local.y + local.height)), Scalar(255, 0, 0), 3, 8, 0);
					}
					else
					if (((OCR.find("RP")) != string::npos || (OCR.find("GRP")) != string::npos || (OCR.find("Grp")) != string::npos) && OCR.size() < 4)
					{
						if (std::find(Headings.begin(), Headings.end(), "Group") == Headings.end())
						{
							Headings.push_back("Group");
							indexs.push_back(i);
						}
						// rectangle(fieldImage, Point(local.x, local.y), Point((local.x + local.width), (local.y + local.height)), Scalar(255, 0, 0), 3, 8, 0);
					}
					else
					if (((OCR.find("G")) != string::npos && (OCR.find("No")) != string::npos) && OCR.size() < 7)
					{
						if (std::find(Headings.begin(), Headings.end(), "Group") == Headings.end())
						{
							Headings.push_back("Group");
							indexs.push_back(i);
						}
						// rectangle(fieldImage, Point(local.x, local.y), Point((local.x + local.width), (local.y + local.height)), Scalar(255, 0, 0), 3, 8, 0);
					}
					else
					if ((OCR.find("mu")) != string::npos && OCR.size() < 6)
					{
						if (std::find(Headings.begin(), Headings.end(), "Group") == Headings.end())
						{
							Headings.push_back("Group");
							indexs.push_back(i);
						}
						// rectangle(fieldImage, Point(local.x, local.y), Point((local.x + local.width), (local.y + local.height)), Scalar(255, 0, 0), 3, 8, 0);
					}
				}
			}
			

			//ID
			wordfound = false;
			// ID file handing 
			//Adeel
			std::ifstream myfileid("ocrTxtFiles/id.txt");
			std::string lineid;
			if (myfileid.is_open())
			{
			  while (std::getline (myfileid,lineid))
			  {
			     if (lineid != " " && lineid != "" && (OCR.find(lineid)) != string::npos)
			     {
			     	Headings.push_back("ID");
			     	indexs.push_back(i);
			     	wordfound = true;
			     	break;
			     }
			  }
			  myfileid.close();
			}
			else
				cout << "can not open id file" << endl;
			// Zain
			if (!wordfound)
			{
				if (((OCR.find("1d")) != string::npos || (OCR.find("Id")) != string::npos || (OCR.find("ld")) != string::npos) && ((OCR.find("Num")) != string::npos))
				{
					Headings.push_back("ID");
					rectangle(fieldImage, Point(local.x, local.y), Point((local.x + local.width), (local.y + local.height)), Scalar(255, 0, 0), 3, 8, 0);
					indexs.push_back(i);
				}
				else
				if ((OCR.find("ID")) != string::npos)
				{
					if (OCR.size() > 2){
						if ((OCR.find("ID")) != string::npos && (OCR.find("Me")) != string::npos)
						{
							Headings.push_back("ID");
						rectangle(fieldImage, Point(local.x, local.y), Point((local.x + local.width), (local.y + local.height)), Scalar(255, 0, 0), 3, 8, 0);
						indexs.push_back(i);
						}
					}
					else
					{
						Headings.push_back("ID");
						rectangle(fieldImage, Point(local.x, local.y), Point((local.x + local.width), (local.y + local.height)), Scalar(255, 0, 0), 3, 8, 0);
						indexs.push_back(i);
					}
				}
			}
				
			//Name
			wordfound = false;
			std::ifstream myfile("ocrTxtFiles/name.txt");
			std::string line;
			if (myfile.is_open())
			{
			  while (std::getline (myfile,line))
			  {
			     if (line != " " && line != "" && (OCR.find(line)) != string::npos)
			     {
			     	Headings.push_back("Name");
			     	indexs.push_back(i);
			     	wordfound = true;
			     	break;
			     }
			  }
			  myfile.close();
			}
			else
				cout << "can not open name file" << endl;

			if (!wordfound)
			{
				if (((OCR.find("Name") != string::npos || OCR.find("NAME") != string::npos || OCR.find("NAHE") != string::npos)
				&& (OCR.find("G") == string::npos))
				|| (OCR.find("Member") != string::npos) && OCR.size() < 8)
				{
					// rectangle(fieldImage, Point(local.x, local.y), Point((local.x + local.width), (local.y + local.height)), Scalar(255, 0, 0), 3, 8, 0);
					indexs.push_back(i);
					Headings.push_back("Name");
				}
			}
			


			/*if (index == 0 && ((OCR.find("Gro")) != string::npos || (OCR.find("roup")) != string::npos))
				j = AsRect.size();*/
			ocrs.push_back(OCR);
		}
		//cout << endl;
	}
	string json = "{\"Company\":\""+Company+"\",";
	for (int k = 0; k < indexs.size(); k++)
	{
		string text = "";
		Rect rect = recs[indexs[k]];
		string ocr = ocrs[indexs[k]];
		int sizeOf1 = rect.width / ocr.size();
		vector<Rect> r = GetOverlaping(Headings[k], gimage, tess, Rect(rect.x + (sizeOf1 * 8), rect.y + rect.height / 1.9, (sizeOf1 * 14), rect.height / 10), recs, indexs, ocrs, &text);
rectangle(fieldImage, Rect(rect.x + (sizeOf1 * 8), rect.y + rect.height / 1.9, (sizeOf1 * 14), rect.height / 10), Scalar(0, 255, 0), 3, 8, 0);
		std::sort(r.begin(), r.end(), byXposition());

		if (r.size() > 0)
		for (int j = 0; j < r.size(); j++){
			rectangle(fieldImage, Point(r[j].x, r[j].y), Point((r[j].x + r[j].width), (r[j].y + r[j].height)), Scalar(0, 255, 0), 3, 8, 0);
		}
		else
		{
			r = GetOverlaping(Headings[k], gimage, tess, Rect(rect.x, rect.y + rect.height*1.2, (sizeOf1 * 14), rect.height/2), recs, indexs, ocrs, &text);
rectangle(fieldImage, Rect(rect.x, rect.y + rect.height*1.2, (sizeOf1 * 14), rect.height/2), Scalar(0, 255, 0), 3, 8, 0);
			std::sort(r.begin(), r.end(), byXposition());

			for (int j = 0; j < r.size(); j++)
				rectangle(fieldImage, Point(r[j].x, r[j].y), Point((r[j].x + r[j].width), (r[j].y + r[j].height)), Scalar(0, 255, 0), 3, 8, 0);
		}
		if (Headings[k] == "Name")
			text.erase(remove_if(text.begin(), text.end(), [](char c) { return !isalpha(c) && c != ' '; }), text.end());
		if (Headings[k] == "Group" || Headings[k] == "ID")
			text = removeOnlyAlpha(text);
		if (Headings[k] == "ID")
		for (int j = 0; j < text.size(); j++)
		if (text[j] == 'H' || text[j] == 'N')
			text[j] = 'W';
		json += "\"" + Headings[k] + "\":\"" + text + "\",";
		//cout << Headings[k] << ": " << text << endl;
	}
	//Finding name if didnot have heading
	if (indexs.size() == 2)
	{
		string text = "";
		Rect rect;
		if (recs[indexs[0]].y < recs[indexs[1]].y)
		rect = recs[indexs[0]];
		else
			rect = recs[indexs[1]];
		string ocr = ocrs[indexs[0]];
		int sizeOf1 = rect.width / ocr.size();
		vector<Rect> r = GetOverlaping("Name", gimage, tess, Rect(rect.x, rect.y - rect.height * 2, (sizeOf1 * 8), rect.height), recs, indexs, ocrs, &text);

		std::sort(r.begin(), r.end(), byXposition());

		for (int j = 0; j < r.size(); j++)
			rectangle(fieldImage, Point(r[j].x, r[j].y), Point((r[j].x + r[j].width), (r[j].y + r[j].height)), Scalar(0, 255, 0), 3, 8, 0);
		text.erase(remove_if(text.begin(), text.end(), [](char c) { return !isalpha(c); }), text.end());
		
		json += "\"Name\":\"" + text + "\"}";
		//cout << "Name: " << text << endl;
	}
	else
	{
		json[json.size() - 1] = '}';
	}
	cout << json << endl;
	//rectangle(fieldImage, Point(xrange, 20), Point(xrange +10, 40), Scalar(0, 0, 255), 1, 8, 0);
	//rectangle(fieldImage, Point(xrange, 20), Point(xrange +10, 40), Scalar(0, 0, 255), 1, 8, 0);
	//namedWindow("field",WINDOW_NORMAL);
	//cv::imshow("field", fieldImage);
	//cv::waitKey(50000);
	//char *data = new char[str.length() + 1];
	//std::strcpy(data, str.c_str());
	//return data;

}

////////////////////////////////////////////    Surf     //////////////////
class Surf
{
public:
	vector<DMatch> DetectCard(Mat SearchImage, Mat ObservedIMage, std::vector<Point2f>* scene_corners);
	Surf();
	~Surf();
};

Mat src, frameImg;
int width;
int height;
vector<Point> srcCorner(4);
vector<Point> dstCorner(4);

bool createDetectorDescriptorMatcher(const string& detectorType, const string& descriptorType, const string& matcherType,
	Ptr<FeatureDetector>& featureDetector,
	Ptr<DescriptorExtractor>& descriptorExtractor,
	Ptr<DescriptorMatcher>& descriptorMatcher)
{
	//cout << "<Creating feature detector, descriptor extractor and descriptor matcher ..." << endl;
	if (detectorType == "SIFT" || detectorType == "SURF")
		cv::initModule_nonfree();
	featureDetector = FeatureDetector::create(detectorType);
	descriptorExtractor = DescriptorExtractor::create(descriptorType);
	descriptorMatcher = DescriptorMatcher::create(matcherType);
	//cout << ">" << endl;
	bool isCreated = !(featureDetector.empty() || descriptorExtractor.empty() || descriptorMatcher.empty());
	//if (!isCreated)
	//	cout << "Can not create feature detector or descriptor extractor or descriptor matcher of given types." << endl << ">" << endl;
	return isCreated;
}


bool refineMatchesWithHomography(const std::vector<cv::KeyPoint>& queryKeypoints,
	const std::vector<cv::KeyPoint>& trainKeypoints,
	float reprojectionThreshold,
	std::vector<cv::DMatch>& matches,
	cv::Mat& homography)
{
	const int minNumberMatchesAllowed = 4;
	if (matches.size() <minNumberMatchesAllowed)
		return false;
	// Prepare data for cv::findHomography    
	std::vector<cv::Point2f> queryPoints(matches.size());
	std::vector<cv::Point2f> trainPoints(matches.size());
	for (size_t i = 0; i <matches.size(); i++)
	{
		queryPoints[i] = queryKeypoints[matches[i].queryIdx].pt;
		trainPoints[i] = trainKeypoints[matches[i].trainIdx].pt;
	}
	// Find homography matrix and get inliers mask    
	std::vector<unsigned char> inliersMask(matches.size());
	homography = cv::findHomography(queryPoints,
		trainPoints,
		CV_FM_RANSAC,
		reprojectionThreshold,
		inliersMask);
	std::vector<cv::DMatch> inliers;
	for (size_t i = 0; i<inliersMask.size(); i++)
	{
		if (inliersMask[i])
			inliers.push_back(matches[i]);
	}
	matches.swap(inliers);
	Mat homoShow;
	drawMatches(src, queryKeypoints, frameImg, trainKeypoints, matches, homoShow, Scalar::all(-1), CV_RGB(255, 255, 255), Mat(), 2);
	//imshow("homoShow", homoShow);
	return matches.size() > minNumberMatchesAllowed;

}


std::vector<Point2f> matchingDescriptor(const vector<KeyPoint>& queryKeyPoints, const vector<KeyPoint>& trainKeyPoints,
	const Mat& queryDescriptors, const Mat& trainDescriptors,
	Ptr<DescriptorMatcher>& descriptorMatcher, vector<DMatch> *m_Matches,
	bool enableRatioTest = true)
{
	vector<vector<DMatch> > m_knnMatches;
	//vector<DMatch>m_Matches;
	std::vector<Point2f> scene_corners(4);
	if (enableRatioTest)
	{
		//cout << "KNN Matching" << endl;
		const float minRatio = 1.f / 1.5f;
		descriptorMatcher->knnMatch(queryDescriptors, trainDescriptors, m_knnMatches, 2);
		for (size_t i = 0; i<m_knnMatches.size(); i++)
		{
			const cv::DMatch& bestMatch = m_knnMatches[i][0];
			const cv::DMatch& betterMatch = m_knnMatches[i][1];
			float distanceRatio = bestMatch.distance / betterMatch.distance;
			if (distanceRatio <minRatio)
			{
				(*m_Matches).push_back(bestMatch);
			}
		}
	}
	else
	{
		//cout << "Cross-Check" << endl;
		Ptr<cv::DescriptorMatcher> BFMatcher(new cv::BFMatcher(cv::NORM_HAMMING, true));
		BFMatcher->match(queryDescriptors, trainDescriptors, (*m_Matches));
	}
	Mat homo;
	float homographyReprojectionThreshold = 1.0;
	bool homographyFound = refineMatchesWithHomography(
		queryKeyPoints, trainKeyPoints, homographyReprojectionThreshold, (*m_Matches), homo);

	if (!homographyFound)
		return scene_corners;
	else
	{
		if ((*m_Matches).size()>10)
		{
			std::vector<Point2f> obj_corners(4);
			obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(src.cols, 0);
			obj_corners[2] = cvPoint(src.cols, src.rows); obj_corners[3] = cvPoint(0, src.rows);
			
			perspectiveTransform(obj_corners, scene_corners, homo);
			
			return scene_corners;
		}
		return scene_corners;
	}
}
vector<DMatch> Surf::DetectCard(Mat SearchImage, Mat ObservedIMage, std::vector<Point2f>* scene_corners)
{
	src = SearchImage;
	width = src.cols;
	height = src.rows;
	string detectorType = "SIFT";
	string descriptorType = "SIFT";
	string matcherType = "FlannBased";
	vector<DMatch>m_Matches;

	Ptr<FeatureDetector> featureDetector;
	Ptr<DescriptorExtractor> descriptorExtractor;
	Ptr<DescriptorMatcher> descriptorMatcher;
	if (!createDetectorDescriptorMatcher(detectorType, descriptorType, matcherType, featureDetector, descriptorExtractor, descriptorMatcher))
	{
		//cout << "Creat Detector Descriptor Matcher False!" << endl;
		return m_Matches;
	}
	//Intial: read the pattern img keyPoint  
	vector<KeyPoint> queryKeypoints;
	Mat queryDescriptor;
	featureDetector->detect(src, queryKeypoints);
	descriptorExtractor->compute(src, queryKeypoints, queryDescriptor);

	srcCorner[0] = Point(0, 0);
	srcCorner[1] = Point(width, 0);
	srcCorner[2] = Point(width, height);
	srcCorner[3] = Point(0, height);

	vector<KeyPoint> trainKeypoints;
	Mat trainDescriptor;

	Mat frame, grayFrame;
	char key = 0;
	frame = ObservedIMage;
		if (!frame.empty())
		{
			int owidth = frame.cols, oheight = frame.rows;
			int Width = 800;
			int Height = frame.rows * (800.0 / frame.cols);
			resize(frame, frame, Size(Width, Height));

			frame.copyTo(frameImg);
			//printf("%d,%d\n", frame.depth(), frame.channels());
			grayFrame.zeros(frame.rows, frame.cols, CV_8UC1);
			cvtColor(frame, grayFrame, CV_BGR2GRAY);
			trainKeypoints.clear();
			trainDescriptor.setTo(0);
			featureDetector->detect(grayFrame, trainKeypoints);

			if (trainKeypoints.size() != 0)
			{
				descriptorExtractor->compute(grayFrame, trainKeypoints, trainDescriptor);

				(*scene_corners) = matchingDescriptor(queryKeypoints, trainKeypoints, queryDescriptor, trainDescriptor, descriptorMatcher, &m_Matches);
				line(frameImg, (*scene_corners)[0], (*scene_corners)[1], CV_RGB(255, 0, 0), 2);
				line(frameImg, (*scene_corners)[1], (*scene_corners)[2], CV_RGB(255, 0, 0), 2);
				line(frameImg, (*scene_corners)[2], (*scene_corners)[3], CV_RGB(255, 0, 0), 2);
				line(frameImg, (*scene_corners)[3], (*scene_corners)[0], CV_RGB(255, 0, 0), 2);
				//imshow("foundImg", frameImg);

				for (int i = 0; i < 4; i++)
				{
					(*scene_corners)[i].x = ((*scene_corners)[i].x / (frameImg.cols*1.0))*owidth;
					(*scene_corners)[i].y = ((*scene_corners)[i].y / (frameImg.rows*1.0))*oheight;
				}
				
			}
		}
		key = waitKey(1000);
		return m_Matches;
}

Surf::Surf()
{
}


Surf::~Surf()
{
}





/////////////////////////////////////////////////////////////////////////////////////////


float angleBetween(const Point &v1, const Point &v2)
{
	float len1 = sqrt(v1.x * v1.x + v1.y * v1.y);
	float len2 = sqrt(v2.x * v2.x + v2.y * v2.y);

	float dot = v1.x * v2.x + v1.y * v2.y;

	float a = dot / (len1 * len2);

	if (a >= 1.0)
		return 0.0;
	else if (a <= -1.0)
		return 3.14;
	else
		return acos(a); // 0..PI
}
Rect detectCard(Mat image, vector<Point2f> scene_corners)
{
	Rect rect;
	float minx = 10000.0, miny = 10000.0, maxx = 0.0, maxy = 0.0;
	for (int i = 0; i < 4; i++)
	{
		if (scene_corners[i].x < 0)
			scene_corners[i].x = 1;
		if (scene_corners[i].x > image.cols)
			scene_corners[i].x = image.cols;

		if (scene_corners[i].y < 0)
			scene_corners[i].y = 1;
		if (scene_corners[i].y > image.rows)
			scene_corners[i].y = image.rows;

		if (scene_corners[i].x < minx)
			minx = scene_corners[i].x;
		if (scene_corners[i].y < miny)
			miny = scene_corners[i].y;

		if (scene_corners[i].x > maxx)
			maxx = scene_corners[i].x;
		if (scene_corners[i].y > maxy)
			maxy = scene_corners[i].y;
	}
	for (int i = 0; i < 4; i++)
	{
		scene_corners[i].x = scene_corners[i].x - minx;
		scene_corners[i].y = scene_corners[i].y - miny;
	}
	rect = Rect(minx, miny, maxx - minx, maxy - miny);
	return rect;
}
Mat normalizeRotation(Mat image,vector<Point2f> scene_corners)
{
	cv::Mat src = image;
	Point p1 = Point(scene_corners[1].x - scene_corners[0].x, scene_corners[1].y - scene_corners[0].y);
	Point p2 = Point(scene_corners[1].x, 0);
	double angle = -angleBetween(p1, p2) * 40;

	// get rotation matrix for rotating the image around its center
	cv::Point2f center(src.cols / 2.0, src.rows / 2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	// determine bounding rectangle
	cv::Rect bbox = cv::RotatedRect(center, src.size(), angle).boundingRect();
	// adjust transformation matrix
	rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

	cv::Mat dst;
	cv::warpAffine(src, dst, rot, bbox.size());
	return dst;
}
cv::Rect  GettUnionMain(cv::Rect A, cv::Rect B)
{
	int x = 0, y = 0, width = 0, height = 0;
	if (A.x < B.x)
		x = A.x;
	else
		x = B.x;
	if (A.y < B.y)
		y = A.y;
	else
		y = B.y;

	if ((A.x + A.width) < (B.x + B.width))
		width = (B.x + B.width) - x;
	else
		width = (A.x + A.width) - x;
	if ((A.y + A.height) < (B.y + B.height))
		height = (B.y + B.height) - y;
	else
		height = (A.y + A.height) - y;
	//cout << A << B << Rect(x, y, width, height);
	return Rect(x, y, width, height);
}
Mat Rotate(Mat src, int angle)
{
	// get rotation matrix for rotating the image around its center
	cv::Point2f center(src.cols / 2.0, src.rows / 2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	// determine bounding rectangle
	cv::Rect bbox = cv::RotatedRect(center, src.size(), angle).boundingRect();
	// adjust transformation matrix
	rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

	cv::Mat dst;
	cv::warpAffine(src, dst, rot, bbox.size());

	return dst;
}
int main(int argc, char* argv[])
{
	Surf detecter = Surf();
	DOOCR doocr = DOOCR();
	vector<vector<Point2f> > scene_corners;
	vector<vector<DMatch> > m_Matches;

	IplImage* ipimage;
	// if (argc > 1)
	ipimage = cvLoadImage("card.png", 1);
	// else{
	// 	cout << "Provide path to image for processing";
	// 	//getch();
	// 	//return 0;
	// 	ipimage = cvLoadImage("CARDS/card_3.png", 1);
	// }
		//ipimage = cvLoadImage("image.png", 1);
	
	Mat image = Mat(ipimage);
	Mat resizedImage;
	int Width = 900;
	int Height = image.rows * (900.0 / image.cols);
	resize(image, resizedImage, Size(Width, Height));

	Mat grad;
	int lowThreshold = 160;
	int heighThreshold = 255;
	Canny(resizedImage, grad, lowThreshold, heighThreshold, 3);

	
	std::vector<Rect> rects_list;
	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;

	//imshow("grad", grad);
	//waitKey(1000);

	findContours(grad, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	int MinWidth = (int)(grad.cols * 0.1f);
	int MinHeight = (int)(grad.cols * 0.1f);

	int MaxWidth = (int)(grad.cols * 1);
	int MaxHeight = (int)(grad.cols * 1);

	
	for (size_t i = 0; i < contours.size(); i++)
	{
		cv::Point p(boundingRect(contours[i]).x, boundingRect(contours[i]).y);
		Rect r = boundingRect(contours[i]);
		if (r.width > MinWidth && r.width <= MaxWidth && r.height > MinHeight && r.height <= MaxHeight)
		{
			rectangle(image, r, Scalar(0, 0, 255), 1, 8, 0);
			rects_list.push_back(r);
		}
	}
	Rect full = rects_list[0];
	for (size_t i = 1; i < rects_list.size(); i++)
		full = GettUnionMain(rects_list[i], full);
	//Locating card rectangle on the full image
	float fx = (1.0f*image.cols) / resizedImage.cols;
	float fy = (1.0f*image.rows) / resizedImage.rows;
	full = Rect(full.x*fx, full.y*fy, full.width*fx, full.height*fy);
	//rectangle(image, full, Scalar(0, 0, 255), 3, 8, 0);
	//////////////////////////////
	
	vector<cv::String> fn;
	glob("images/templates/*.png", fn, false);

	// vector<Mat> images;
	// size_t count = fn.size(); //number of png files in images folder
	// for (size_t i=0; i<count; i++)
	//     images.push_back(imread(fn[i]));


	int count = fn.size();
	int index = -1;
	std::string path;

	vector<Point2f> corners(4);
	#pragma omp parallel for
	for (int i = 0; i < count; i++)
	{
		Mat temImage = imread(fn[i]);//"template" + to_string(i) + ".png");
		vector<DMatch> Matches = detecter.DetectCard(temImage, image, &corners);
		//scene_corners.push_back(corners);
		//m_Matches.push_back(Matches);
		if (Matches.size() > 15)
		{
			index = i;
			path = fn[i];
			break;
		}
		// else
		// {
		// 	// add image in pending
		// 	// path : images/pendings/

		// }
	}
	if (index == -1)
	{
		vector<cv::String> fnPending;
		glob("images/pendings/*.png", fnPending, false);

		std::string imagesStartPath("images/pendings/");
		
		int a = fnPending.size()+1;
		std::string imageFileName  = std::to_string(a);
		std::string imageExt(".png");
		std::string imagePath = imagesStartPath+imageFileName+imageExt;
		imwrite(imagePath,image);
		cout << "\n\n Could not Detect Card \n Image Saved into Pending Directory";
		return 0;
	}
	image = image(full);
	

	if (abs(corners[0].y - corners[1].y) > abs(corners[2].y - corners[1].y))
		image = Rotate(image, -90);
	else
	if ((corners[0].y > corners[3].y))
		image = Rotate(image, 180);

	Width = 900;
	Height = image.rows * (900.0 / image.cols);
	resize(image, image, Size(Width, Height));

		
	vector<string> Companies;
	// glob("","")

	// std::ifstream input("file.log");
	
	// ofstream myfile;
	// string text;
	// int indexCounter=0;
	// std::ifstream myfile("images/templates/companyNames.txt");
	// if (myfile.is_open())
	// {
	// 	// while(getline(text,size,file)))
	//   while (std::getline (myfile,text))
	//   {
	//     cout << text << '\n';
	//     if (indexCounter == index)
	//     {
	//     	break;
	//     }
	//     indexCounter =indexCounter +1;
	//   }
	//   myfile.close();
	// }
	// cout << "index =" << index << endl;


	// for (int i = 0; i < count; ++i)
	// {
	    
	    // input >> path;
		std::string name;
	    size_t sep = path.find_last_of("\\/");
	    if (sep != std::string::npos)
	        path = path.substr(sep + 1, path.size() - sep - 1);

	    size_t dot = path.find_last_of(".");
	    if (dot != std::string::npos)
	    {
	        name = path.substr(0, dot);
	        std::string ext  = path.substr(dot, path.size() - dot);
	    }
	    else
	    {
	        name = path;
	        std::string ext  = "";
	    }
	    // Companies.push_back(name);
	// }

	// Companies.push_back("BlueCross BlueShield");
	// Companies.push_back("Administered By Signa Health and Life Insurance Co");
	// Companies.push_back("Aetna");
	// Companies.push_back("UMR");
	// Companies.push_back("Aetna");
	// Companies.push_back("UnitedHealthcare");
	// Companies.push_back("Humana");
	// Companies.push_back("A UnitedHealthcare company");


	//cout << "Card: " << Companies[index] << endl;
	Rect rect;
	
	//rectangle(image, rect, Scalar(0, 0, 255), 1, 8, 0);
	tesseract::TessBaseAPI tess;
	cout << endl;
	tess.Init("tesseract", "eng", tesseract::OEM_DEFAULT);
	// doocr.GetfieldOCR(rect, image.clone(), index, &tess,Companies[index]);
	doocr.GetfieldOCR(rect, image.clone(), index, &tess,name);
	
	//cout << "Done";
	//imshow("image", image);
	//waitKey(100000);
	//getch();
    return 0;
}
