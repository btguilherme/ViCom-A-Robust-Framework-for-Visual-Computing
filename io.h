#ifndef IO_H
#define IO_H

#include <vector>
#include <string>
#include <highgui.h>
#include <QtCore>

using namespace std;
using namespace cv;

class IO
{
public:    
    IO();
    ~IO();
    int getNumberOfSamples();
    void load();

    //getters
    vector<string> getFilesNames();
    vector<vector<Mat> > getHistory();
    vector<string> getDirs();
    string getClassesDir();
    vector<string> getFullPaths();
    vector<vector<string> > getFiles();

    //setter
    void setDirectoryClasses(QString directory);

    vector<Mat> openDir(string directory);
    vector<string> listNames(string path, char type);

    static vector<string> openFile(string file);
    static bool saveTXT(QString directory, string head, vector<string> body, string file_name);
    static bool saveTime(double tDecorrido, string path);
    static bool saveTXT_temp(string path, string head, vector<string> body, string file_name);

private:
    //variables
    QString directory_classes;
    vector<string> dirs;
    vector<string> full_paths;
    string classesDir;
    vector<vector<Mat> > history;
    vector<vector<string> > files;
    int number_of_samples;
    vector<string> filesNames;
    //functions
    void openDirectory();
    void saveImages();
};

#endif // IO_H
