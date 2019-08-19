#ifndef HEADER
#define HEADER

#define PATH_SEPARATOR "/"
#define TAM 256

// Q includes
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QProgressDialog>

// System includes
#include <iostream>
#include <thread>
#include <vector>
#include <ctime>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <opencv/cvwimage.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// Project headers includes
#include "ui_mainwindow.h"
#include "classification.h"
#include "extractors.h"
#include "io.h"
#include "mainwindow.h"
#include "preproc.h"
#include "progressdialog.h"
#include "utils.h"
#include "dialogresults.h"
#include "confusionmatrix.h"
#include "avgaccperclass.h"
#include "about.h"


using namespace std;
using namespace cv;

#endif // HEADER

