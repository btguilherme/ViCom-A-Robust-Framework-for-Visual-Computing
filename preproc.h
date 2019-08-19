#ifndef PREPROC_H
#define PREPROC_H

#include <vector>
#include <highgui.h>
#include <cv.h>

using namespace cv;
using namespace std;

class PreProc
{
public:
    enum img_filtering_technic_t{ift_AVERAGE, ift_MEDIAN, ift_GAUSSIAN};
    enum thresholding_technic_t{tt_TO_ZERO = CV_THRESH_TOZERO, tt_BINARY = CV_THRESH_BINARY, tt_TRUNCATED = CV_THRESH_TRUNC, tt_OTSU = CV_THRESH_OTSU, tt_MANUALLY};
    enum edg_technic_t{et_ROBERTS, et_PREWITT, et_SOBEL};

    vector<vector<Mat> > histogramEqualization();
    vector<vector<Mat> > imageFiltering(int kernel, img_filtering_technic_t technic);
    vector<vector<Mat> > thresholding(int value, thresholding_technic_t technic);
    vector<vector<Mat> > edgeDetection(edg_technic_t technic);

    PreProc(vector<vector<Mat> > src);
    ~PreProc();

private:
    //variable
    vector<vector<Mat> > src;
};

#endif // PREPROC_H
