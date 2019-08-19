#ifndef EXTRACTORS_H
#define EXTRACTORS_H

#include "header.h"

using namespace std;
using namespace cv;

class Extractors
{
public:
    Extractors();
    ~Extractors();
    bool gch(vector<vector<Mat> > imgs, vector<string> dirs, string directory);
    bool lch(vector<vector<Mat> > imgs, vector<string> dirs, string directory, int width, int height);
    bool cgch(vector<vector<Mat> > imgs, vector<string> dirs, string directory);
    bool bic(vector<vector<Mat> > imgs, vector<string> dirs, string directory);
    bool haralick(vector<vector<Mat> > imgs, vector<string> dirs, string directory, int x, int y);
    bool lbp(vector<vector<Mat> > imgs, vector<string> dirs, string directory, int index);
};

#endif // EXTRACTORS_H
