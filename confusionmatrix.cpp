#include "confusionmatrix.h"
#include "ui_confusionmatrix.h"
#include "header.h"

int it;
QStringList svm, opf;

ConfusionMatrix::ConfusionMatrix(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfusionMatrix)
{
    ui->setupUi(this);
}

ConfusionMatrix::~ConfusionMatrix()
{
    delete ui;
}

void ConfusionMatrix::setComboIterations(int n_iterations){
    ui->combo_iteration->addItem("");
    for (int var = 0; var < n_iterations; ++var) {
        QString text = QString::number(var);
        ui->combo_iteration->addItem(text);
    }
}

void ConfusionMatrix::setExtractors(){

    ui->combo_extrator->addItem("");
    for (int var = 0; var < opf.size(); var++) {
        ui->combo_extrator->addItem(opf.at(var));
    }
    for (int var = 0; var < svm.size(); var++) {
        ui->combo_extrator->addItem(svm.at(var));
    }
}

void ConfusionMatrix::confusionMatrix(QStringList svm_files_path, QStringList opf_files_path){
    svm = svm_files_path;
    opf = opf_files_path;
}

void ConfusionMatrix::on_combo_iteration_currentIndexChanged(int index)
{
    it = index - 1;
}

void ConfusionMatrix::on_make_cm_clicked()
{
    QString path = ui->combo_extrator->currentText();
    bool isSvm = false;

    //verifica se eh svm
    for (int var = 0; var < svm.size(); var++) {
        string p1 = svm.at(var).toUtf8().data();
        string p2 = path.toUtf8().data();
        if(strcmp(p1.c_str(), p2.c_str()) == 0){
            isSvm = true;
            break;
        }
    }

    string testing_path_aux, testing_path_aux_out;
    char number[5];
    vector<string> testing_file;
    vector<string> out_file;

    string testing_path = path.toUtf8().data();
    testing_path.append("_iterations").append(PATH_SEPARATOR);

    sprintf(number, "%d", it);
    string iteration = number;

    string open_file_test, open_file_out;

    int param;
    if(isSvm){
        open_file_test = "testing.svm";
        open_file_out = "output_file.txt";
        param = 0;
    }else{
        open_file_test = "testing.txt";
        open_file_out = "testing.dat.out";
        param = 1;
    }
    testing_path_aux = testing_path;
    testing_path_aux.append(iteration).append(PATH_SEPARATOR);
    testing_path_aux_out = testing_path_aux;
    testing_path_aux_out.append(open_file_out);
    testing_path_aux.append(open_file_test);

    testing_file = IO::openFile(testing_path_aux);
    out_file = IO::openFile(testing_path_aux_out);


    vector<string> values_testing, values_out;
    vector<string> header_labels;

    for (size_t j = param; j < testing_file.size(); j++) {
        stringstream ss(testing_file.at(j));
        string item;
        vector<string> elems;
        while (std::getline(ss, item, ' ') && elems.size() < 2) {
            elems.push_back(item);
        }
        values_testing.push_back(elems.at(param));
    }

    for (size_t j = 0; j < out_file.size(); j++) {
        stringstream ss(out_file.at(j));
        string item;
        vector<string> elems;
        while (std::getline(ss, item, '\n')) {
            elems.push_back(item);
        }
        values_out.push_back(elems.at(0));
    }

    //nomes das colunas e linhas
    header_labels = values_testing;
    sort(header_labels.begin(), header_labels.end());
    header_labels.erase(unique(header_labels.begin(), header_labels.end()), header_labels.end());

    QStringList qsl;
    vector<int> labels_int;
    for (int j = 0; j < header_labels.size(); j++) {
        string aux = header_labels.at(j);
        int value_int = atoi(aux.c_str());
        labels_int.push_back(value_int);
    }
    sort(labels_int.begin(), labels_int.end());

    for (int j = 0; j < labels_int.size(); j++) {
        char number[10];
        sprintf(number, "%d", labels_int.at(j));
        string v = number;
        QString value(v.c_str());
        qsl.push_back(value);
    }
    ui->confusion_matrix->setRowCount(qsl.size());
    ui->confusion_matrix->setColumnCount(qsl.size());
    ui->confusion_matrix->setVerticalHeaderLabels(qsl);
    ui->confusion_matrix->setHorizontalHeaderLabels(qsl);
    //fim nomes das colunas e linhas

    //zera a matriz
    for (int i = 0; i < qsl.size(); i++) {
        for (int j = 0; j < qsl.size(); j++) {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText("0");
            ui->confusion_matrix->setItem(i, j, item);
        }
    }

    //conta e atualiza valores
    for (int j = 0; j < values_out.size(); j++) {
        string a1 = values_testing.at(j); //correct - linha
        string a2 = values_out.at(j);     //predicted - coluna

        //linha e coluna correspondentes a a1 e a2
        int l, c;
        for (int k = 0; k < qsl.size(); k++) {
            string header = qsl.at(k).toUtf8().data();
            if(strcmp(a1.c_str(),header.c_str()) == 0){
                l = k;
            }
            if(strcmp(a2.c_str(),header.c_str()) == 0){
                c = k;
            }
        }


        QString value = ui->confusion_matrix->item(l, c)->text();
        string value_s = value.toUtf8().data();

        int value_int = atoi(value_s.c_str());
        value_int = value_int + 1;


        QTableWidgetItem* item = new QTableWidgetItem();
        QString num_str = QString::number(value_int);
        item->setText(num_str);
        ui->confusion_matrix->setItem(l, c, item);
    }
}
