#include "header.h"
#include "ui_dialogresults.h"
#include "header.h"

vector<vector<string> > results;
QStringList extractors_opf, extractors_svm;

DialogResults::DialogResults(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogResults)
{
    ui->setupUi(this);
}

DialogResults::~DialogResults()
{
    delete ui;
}

void DialogResults::setResultsVector(vector<vector<float> > results, int number_of_rows, QStringList rowNames)
{
    ui->results_table->setRowCount(number_of_rows);
    QStringList qsl = getRowNames(rowNames);

    ui->results_table->setVerticalHeaderLabels(qsl);
    for (int i = 0; i < results.size(); i++) {
        vector<float> result = results.at(i);
        for (int j = 0; j < result.size(); j++) {
            QTableWidgetItem* item = new QTableWidgetItem();
            QString aux;
            aux.setNum(result.at(j));
            item->setText(aux);
            ui->results_table->setItem(i, j, item);

            //cout << aux.toUtf8().data() << endl;

        }
    }
}

QStringList DialogResults::getRowNames(QStringList rowNames){
    QStringList ret;
    char delim = '/';
    char delim2 = '.';

    extractors_opf.clear();
    extractors_svm.clear();

    for (int var = 0; var < rowNames.size(); var++) {
        QString aux(rowNames.at(var));

        vector<string> elemsSepDelim1;
        vector<string> elemsSepDelim2;
        stringstream ss(aux.toUtf8().data());
        string item;
        while (std::getline(ss, item, delim)) {
            elemsSepDelim1.push_back(item);
        }

        string sent = elemsSepDelim1.at(elemsSepDelim1.size() - 1);

        stringstream ss2(elemsSepDelim1.at(elemsSepDelim1.size() - 1));
        while (std::getline(ss2, item, delim2)) {
            elemsSepDelim2.push_back(item);
        }

        string name = elemsSepDelim2.at(0);

        if(strstr(sent.c_str(), "opf") != NULL) {
            name.append("_opf");
            QString a(name.c_str());
            extractors_opf.push_back(a);
        }else if(strstr(sent.c_str(), "svm") != NULL) {
            name.append("_svm");
            QString a(name.c_str());
            extractors_svm.push_back(a);
        }

        QString s(name.c_str());
        ret.push_back(s);
    }
    return ret;
}


void DialogResults::on_btn_confusion_matrix_clicked()
{
    ConfusionMatrix cf;
    cf.setModal(true);
    cf.setComboIterations(n_iterations);
    cf.confusionMatrix(svm_files_path, opf_files_path);
    cf.setExtractors();
    cf.exec();
}

void DialogResults::on_btn_avg_acc_per_class_clicked()
{
    AVGAccPerClass avg;
    avg.setModal(true);
    avg.iterations_paths_opf = iterations_paths_opf;
    avg.iterations_paths_svm = iterations_paths_svm;
    avg.extractors_opf = extractors_opf;
    avg.extractors_svm = extractors_svm;
    avg.calcAVGperClass();
    avg.exec();
}
