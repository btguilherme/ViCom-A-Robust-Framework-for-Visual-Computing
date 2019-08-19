#ifndef CONFUSIONMATRIX_H
#define CONFUSIONMATRIX_H

#include <QDialog>

using namespace std;

namespace Ui {
class ConfusionMatrix;
}

class ConfusionMatrix : public QDialog
{
    Q_OBJECT

public:
    explicit ConfusionMatrix(QWidget *parent = 0);
    ~ConfusionMatrix();

private slots:

    void on_combo_iteration_currentIndexChanged(int index);

    void on_make_cm_clicked();

public slots:
    void confusionMatrix(QStringList svm_files_path, QStringList opf_files_path);
    void setComboIterations(int n_iterations);
    void setExtractors();

private:
    Ui::ConfusionMatrix *ui;
};

#endif // CONFUSIONMATRIX_H
