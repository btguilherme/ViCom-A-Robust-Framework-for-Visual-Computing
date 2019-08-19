#include "avgaccperclass.h"
#include "ui_avgaccperclass.h"
#include "header.h"


vector<string> values_testing, values_out;

AVGAccPerClass::AVGAccPerClass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AVGAccPerClass)
{
    ui->setupUi(this);
}

AVGAccPerClass::~AVGAccPerClass()
{
    delete ui;
}

void AVGAccPerClass::calcAVGperClass(){
    QStringList classes;
    QStringList full_classes;

    classes.clear();
    full_classes.clear();

    //nomes das colunas
    ui->matrix->setColumnCount(extractors_opf.size() + extractors_svm.size());
    QStringList aux = extractors_opf;
    aux += extractors_svm;
    ui->matrix->setHorizontalHeaderLabels(aux);

    if(iterations_paths_opf.size() != 0) {
        classes = classesNames(false);
        for (int var = 0; var < extractors_opf.size(); var++) {
            for (int i = 0; i < classes.size(); i++) {
                string aclass = classes.at(i).toUtf8().data();
                //view
                if(var == 0){
                    QString c = "Class ";
                    QString ac(aclass.c_str());
                    c.append(ac);
                    full_classes.append(c); // exemplo: Class 1
                    ui->matrix->setRowCount(full_classes.size());
                    ui->matrix->setVerticalHeaderLabels(full_classes);
                }
                //end view
                int match = 0;
                int miss = 0;
                for (int k = var; k < iterations_paths_opf.size(); k = k + extractors_opf.size()) {
                    getData(false, iterations_paths_opf.at(k));
                    for (int j = 0; j < values_out.size(); j++) {
                        string a1 = values_testing.at(j); //correct
                        if(strcmp(aclass.c_str(), a1.c_str()) == 0){
                            string a2 = values_out.at(j);     //predicted
                            if(strcmp(a1.c_str(), a2.c_str()) == 0){
                                match = match + 1;
                            } else {
                                miss = miss + 1;
                            }
                        }
                    }
                }
                float acuracy = (float)((float)match/(float)(match + miss));
                acuracy = 100*(acuracy);
                QString ac = QString::number(acuracy);
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setText(ac);
                ui->matrix->setItem(i, var, item);

                //cout << ac.toUtf8().data() << endl;

            }
        }
    }

    classes.clear();
    full_classes.clear();
    if(iterations_paths_svm.size() != 0) {
        classes = classesNames(true);
        for (int var = 0; var < extractors_svm.size(); var++) {
            for (int i = 0; i < classes.size(); i++) {
                string aclass = classes.at(i).toUtf8().data();
                //view
                if(var == 0 && iterations_paths_opf.size() == 0){
                    QString c = "Class ";
                    QString ac(aclass.c_str());
                    c.append(ac);
                    full_classes.append(c); // exemplo: Class 1
                    ui->matrix->setRowCount(full_classes.size());
                    ui->matrix->setVerticalHeaderLabels(full_classes);
                }
                //end view
                int match = 0;
                int miss = 0;
                for (int k = var; k < iterations_paths_svm.size(); k = k + extractors_svm.size()) {
                    getData(true, iterations_paths_svm.at(k));
                    for (int j = 0; j < values_out.size(); j++) {
                        string a1 = values_testing.at(j); //correct
                        if(strcmp(aclass.c_str(), a1.c_str()) == 0){
                            string a2 = values_out.at(j);     //predicted
                            if(strcmp(a1.c_str(), a2.c_str()) == 0){
                                match = match + 1;
                            } else {
                                miss = miss + 1;
                            }
                        }
                    }
                }
                float acuracy = (float)((float)match/(float)(match + miss));
                acuracy = 100*(acuracy);
                QString ac = QString::number(acuracy);
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setText(ac);
                ui->matrix->setItem(i, (var + extractors_opf.size()), item);
                //cout << ac.toUtf8().data() << endl;
            }
        }
    }
}

void AVGAccPerClass::getData(bool isSvm, string path){
    string testing_path_aux, testing_path_aux_out, testing_path, open_file_test, open_file_out;
    vector<string> testing_file, out_file;
    int param;

    values_testing.clear();
    values_out.clear();

    testing_path = path;
    testing_path.append(PATH_SEPARATOR);

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
    testing_path_aux_out = testing_path_aux;

    testing_path_aux_out.append(open_file_out);
    testing_path_aux.append(open_file_test);

    testing_file = IO::openFile(testing_path_aux);
    out_file = IO::openFile(testing_path_aux_out);

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
}


QStringList AVGAccPerClass::classesNames(bool isSvm){
    //string testing_path_aux, testing_path_aux_out, testing_path, open_file_test, open_file_out;
    vector<string> header_labels;

    if(isSvm){
        getData(isSvm, iterations_paths_svm.at(0));
    } else {
        getData(isSvm, iterations_paths_opf.at(0));
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
    //fim nomes das colunas e linhas

    return qsl;
}
