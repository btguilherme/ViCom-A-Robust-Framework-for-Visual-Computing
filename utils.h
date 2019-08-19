#ifndef UTILS_H
#define UTILS_H

#include "header.h"

class Utils
{
public:
    Utils();
    ~Utils();

    //getters
    vector<vector<float> > getInteriorHistograms();
    vector<vector<float> > getBorderHistograms();

    //setter
    void setNumberIterations(int n);

    vector<Mat> colorReduce(vector<Mat> input, size_t div);
    vector<vector<float> > colorHistogram(vector<Mat> imgs, char option);
    vector<string> createOPFBody(int n_samples, string label, vector<vector<float> > histograms);
    vector<string> createOPFBody(int n_samples, string label, vector<string> haralick); // For Haralick features only!
    vector<string> correctsNumbering(vector<string> src);
    string createOPFHeader(int number_of_samples, int number_of_labels, int number_of_features);
    vector<vector<Mat> > splitter(vector<Mat> src, int w, int h);
    void classifyBorderInteriorPixels (vector<Mat> src);
    vector<string> haralickDescriptors(int x, int y, vector<Mat> src);
    vector<vector<float> > lbpHistograms(vector<Mat> src, int index);
    vector<string> txt2OPF(QStringList opf_files_path, QString opf_path);
    void mySVMSplit(QString svm_path, QString svm_file_path, int training_percentage);

    vector<vector<vector<vector<int> > > > myShuffle(QStringList opf_files_path, int training_percentage);
    void myOPFSplit(QStringList opf_files_path, QString opf_path, vector<vector<vector<vector<int> > > > indices);
    void myOPFTrain(QStringList opf_files_path, QString opf_path);
    void myOPFClassify(QStringList opf_files_path, QString opf_path);
    void myOPFAccuracy(QStringList opf_files_path, QString opf_path);
    void myOPFSaveIterations(QStringList opf_files_path, int i);
    vector<string> iterationsPaths(QStringList files_path, int n_of_iterations);
    vector<vector<float> > calcResults(QStringList opf_files_path, string classifier);
    void txt2SVM(const char *filenamein, const char *filenameout) ;
    void dat2SVM(QStringList opf_files_path, QStringList svm_files_path, QString opf_path);
    void mySVMTrain(QString svm_path, QString svm_file_path, vector<string> c_and_g, bool grid);
    void mySVMClassify(QString svm_path, QString svm_file_path);
    void mySVMAccuracy(QString svm_file_path);
    void cpFolders(string from, string to);

private:
    //variables
    vector<vector<float> > borderHistograms;
    vector<vector<float> > interiorHistograms;
    float normalized_matrix[TAM][TAM];
    int r, p; // LBP radius and points
    vector<vector<string> > classes_separadas;
    vector<vector<vector<string> > > todas_classes_separadas;
    int n_iterations;

    //functions
    void bicColorHistogram(Mat src, int **points);
    vector<Mat> splitImage(Mat src, int w, int h);
    vector<float> normalizeHist(vector<float> src);
    void coocurrenceMatrix(int x, int y, Mat img);
    string descriptors();
    string toString(float num);
    float sam();
    float entropy();
    float contrast();
    float homogeneity();
    float correlation();
    void setRandP(int index);
    int verifyS(int gc, int gp);
    int sum8(Mat aux, int i, int j);
    int sum12(Mat aux, int i, int j);
    int sum16(Mat aux, int i, int j);
    void mvDAT(string from, string to, string folder, char type);
    void del(string fileName, string file);
    void cp(string from, string to);
    float calcAVG(vector<string> src);
    float calcDP(vector<string> src, float avg);
    bool saveAccuracySVM(string svm_file_path, string line);
    void del(string filePath);
};

#endif // UTILS_H
