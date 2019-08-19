#ifndef AVGACCPERCLASS_H
#define AVGACCPERCLASS_H

#include <QDialog>

using namespace std;

namespace Ui {
class AVGAccPerClass;
}

class AVGAccPerClass : public QDialog
{
    Q_OBJECT

public:
    explicit AVGAccPerClass(QWidget *parent = 0);
    ~AVGAccPerClass();
    vector<string> iterations_paths_svm;
    vector<string> iterations_paths_opf;
    QStringList extractors_opf;
    QStringList extractors_svm;

private:
    Ui::AVGAccPerClass *ui;

public slots:
    void calcAVGperClass();

private slots:
    QStringList classesNames(bool isSvm);
    void getData(bool isSvm, string path);

};

#endif // AVGACCPERCLASS_H
