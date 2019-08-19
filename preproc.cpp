#include "header.h"

using namespace cv;
using namespace std;

vector<vector<Mat> > src;

PreProc::PreProc(vector<vector<Mat> > src)
{
    this->src = src;
}

PreProc::~PreProc()
{

}

vector<vector<Mat> > PreProc::histogramEqualization(){
    vector<vector<Mat> > histogram_eq;
    for (int  var = 0; var < src.size(); var++) {
        vector<Mat> oneClass = src.at(var);
        vector<Mat> aux;
        for (int  i = 0; i < oneClass.size(); i++) {
            Mat img = oneClass.at(i);
            vector<Mat> channels;
            Mat img_hist_equalized;
            cvtColor(img, img_hist_equalized, CV_BGR2YCrCb);
            split(img_hist_equalized,channels);
            equalizeHist(channels[0], channels[0]);
            merge(channels,img_hist_equalized);
            cvtColor(img_hist_equalized, img_hist_equalized, CV_YCrCb2BGR);
            aux.push_back(img_hist_equalized);
        }
        histogram_eq.push_back(aux);
    }
    return histogram_eq;
}

vector<vector<Mat> > PreProc::imageFiltering(int kernel, img_filtering_technic_t technic){
    vector<vector<Mat> > ret;
    switch (technic) {
    case ift_AVERAGE:
        for (int  var = 0; var < src.size(); var++) {
            vector<Mat> oneClass = src.at(var);
            vector<Mat> aux;
            for (int  i = 0; i < oneClass.size(); i++) {
                Mat temp;
                blur(oneClass.at(i),temp, Size(kernel,kernel), Point(-1,-1), BORDER_DEFAULT);
                aux.push_back(temp);
            }
            ret.push_back(aux);
        }
        break;
    case ift_MEDIAN:
        for (int  var = 0; var < src.size(); var++) {
            vector<Mat> oneClass = src.at(var);
            vector<Mat> aux;
            for (int  i = 0; i < oneClass.size(); i++) {
                Mat temp;
                medianBlur(oneClass.at(i), temp, kernel);
                aux.push_back(temp);
            }
            ret.push_back(aux);
        }
        break;
    case ift_GAUSSIAN:
        for (int  var = 0; var < src.size(); var++) {
            vector<Mat> oneClass = src.at(var);
            vector<Mat> aux;
            for (int  i = 0; i < oneClass.size(); i++) {
                Mat temp;
                GaussianBlur(oneClass.at(i),temp, Size(kernel, kernel),0,0, BORDER_DEFAULT);
                aux.push_back(temp);
            }
            ret.push_back(aux);
        }
        break;
    default:
        break;
    }
    return ret;
}

vector<vector<Mat> > PreProc::thresholding(int value/*only for manual thresholding*/, thresholding_technic_t technic){
    vector<vector<Mat> > thre;
    switch (technic) {
    case tt_MANUALLY:
        for (int  k = 0; k < src.size(); k++) {
            vector<Mat> oneClass = src.at(k);
            vector<Mat> aux;
            for (int  n = 0; n < oneClass.size(); ++n) {
                Mat grayImage, final;
                Mat original = oneClass.at(n);
                Mat result = Mat(original.size(), CV_LOAD_IMAGE_GRAYSCALE);
                Size size_img = original.size();
                cvtColor(original, grayImage, CV_RGB2GRAY);
                for(int i = 0; i < size_img.height; i++){
                    for(int j = 0; j < size_img.width; j++){
                        if(grayImage.at<uchar>(i,j) <= value) result.at<uchar>(i,j) = 0;
                        else result.at<uchar>(i,j) = 255;
                    }
                }
                cvtColor(result, final, CV_GRAY2RGB);
                aux.push_back(final);
            }
            thre.push_back(aux);
        }
        break;
    case tt_TO_ZERO:
    case tt_BINARY:
    case tt_TRUNCATED:
    case tt_OTSU:
        for (int var = 0; var < src.size(); var++) {
            vector<Mat> oneClass = src.at(var);
            vector<Mat> aux;
            for (int i = 0; i < oneClass.size(); i++) {
                Mat greyMat, result, dst;
                cvtColor(oneClass.at(i), greyMat, CV_RGB2GRAY);
                threshold(greyMat,dst, 100, 255, technic);

                cvtColor(dst, result, CV_GRAY2RGB);
                aux.push_back(result);
            }
            thre.push_back(aux);
        }
        break;
    default:
        break;
    }
    return thre;
}

vector<vector<Mat> > PreProc::edgeDetection(edg_technic_t technic){
    vector<vector<Mat> > ret;
    switch (technic) {
    case et_PREWITT:
        for (int var = 0; var < src.size(); var++) {
            vector<Mat> oneClass = src.at(var);
            vector<Mat> aux;
            for (int n = 0; n < oneClass.size(); n++) {
                Mat finalMat;
                Mat grayImage;
                Mat original = oneClass.at(n);
                Mat result = Mat(original.size(), CV_LOAD_IMAGE_GRAYSCALE);
                Size size_img = original.size();

                cvtColor(original, grayImage, CV_RGB2GRAY);
                for(int i = 1; i < size_img.height - 1; i++){
                    for(int j = 1; j < size_img.width - 1; j++){
                        result.at<uchar>(i,j) = abs(grayImage.at<uchar>(i,j+1)+grayImage.at<uchar>(i+1,j+1)+grayImage.at<uchar>(i-1,j+1)
                                                    -grayImage.at<uchar>(i,j-1)-grayImage.at<uchar>(i-1,j-1)-grayImage.at<uchar>(i+1,j-1))
                                +abs(grayImage.at<uchar>(i+1,j)+grayImage.at<uchar>(i+1,j-1)+grayImage.at<uchar>(i+1,j+1)
                                     -grayImage.at<uchar>(i-1,j)-grayImage.at<uchar>(i-1,j-1)-grayImage.at<uchar>(i-1,j+1));
                    }
                }
                cvtColor(result,finalMat, CV_GRAY2RGB);
                aux.push_back(finalMat);
            }
            ret.push_back(aux);
        }
        break;
    case et_ROBERTS:
        for (int var = 0; var < src.size(); var++) {
            vector<Mat> oneClass = src.at(var);
            vector<Mat> aux;
            for (int n = 0; n < oneClass.size(); n++) {
                Mat grayImage;
                Mat finalMat;
                Mat original = oneClass.at(n);
                Mat result = Mat(original.size(), CV_LOAD_IMAGE_GRAYSCALE);
                Size size_img = original.size();
                cvtColor(original, grayImage, CV_RGB2GRAY);
                for(int i = 1; i < size_img.height - 1; i++){
                    for(int j = 1; j < size_img.width - 1; j++){
                        result.at<uchar>(i,j) = abs(grayImage.at<uchar>(i,j) - grayImage.at<uchar>(i+1,j+1))
                                + abs(grayImage.at<uchar>(i,j+1) - grayImage.at<uchar>(i+1,j));
                    }
                }
                cvtColor(result,finalMat, CV_GRAY2RGB);
                aux.push_back(finalMat);
            }
            ret.push_back(aux);
        }
        break;
    case et_SOBEL:
        for (int  var = 0; var < src.size(); var++) {
            vector<Mat> oneClass = src.at(var);
            vector<Mat> aux;
            for (int  n = 0; n < oneClass.size(); n++) {
                Mat grayImage;
                Mat sobelx;
                Mat finalMat;
                Mat original = oneClass.at(n);
                Mat result = Mat(original.size(), CV_LOAD_IMAGE_GRAYSCALE);
                double minVal, maxVal;

                cvtColor(original, grayImage, CV_RGB2GRAY);
                Sobel(grayImage, sobelx, CV_32F, 1, 0);
                minMaxLoc(sobelx, &minVal, &maxVal);
                sobelx.convertTo(result, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
                cvtColor(result,finalMat, CV_GRAY2RGB);
                aux.push_back(finalMat);
            }
            ret.push_back(aux);
        }
        break;
    default:
        break;
    }
    return ret;
}
