#ifndef DIALOGRESULTS_H
#define DIALOGRESULTS_H

#include <QDialog>
#include <vector>
#include <string>

using namespace std;

namespace Ui {
class DialogResults;
}

class DialogResults : public QDialog
{
    Q_OBJECT

public:
    explicit DialogResults(QWidget *parent = 0);
    ~DialogResults();
    int n_iterations;
    QStringList svm_files_path, opf_files_path;
    vector<string> iterations_paths_opf;
    vector<string> iterations_paths_svm;

public slots:
    void setResultsVector(vector<vector<float> > results, int number_of_rows, QStringList rowNames);

private slots:
    void on_btn_confusion_matrix_clicked();

    void on_btn_avg_acc_per_class_clicked();

private:
    QStringList getRowNames(QStringList rowNames);
    Ui::DialogResults *ui;
};

#endif // DIALOGRESULTS_H
