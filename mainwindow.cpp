#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "header.h"

#define W 150
#define H 150

/*
 * Carrega todas as imagens, de todas as classes. Cada posicao do vetor possui outro vetor (de Mat) com as imagens de cada classe.
*/
vector<vector<Mat> > history;

/*
 * Retorna uma lista com nomes dos diretorios.
*/
vector<string> dirs;

/*
 * Possui o caminho total de uma classe.
 * ex: /home/guilherme/ALOI/7/
*/
vector<string> full_paths;

/*
 * Cada posiçao do vetor possui outro vetor (de string) que guarda os nomes dos arquivos de cada pasta separadamente.
 * Para acessar os nomes das imgs do primeiro diretorio carregado, faça um vector<string> = files.at(0), por ex.
*/
vector<vector<string> > files;

QProgressDialog *progressDialog;

string classesDir;
int number_of_samples, gap, kernel;
vector<vector<Mat> > temp;

clock_t tInicio, tFim;
double tDecorrido;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::on_actionOpen_directory_triggered()
{
    QString directory_classes = QFileDialog::getExistingDirectory(0, tr("Select the directory"), QDir::currentPath());
    if(directory_classes.isEmpty()){
        return;
    }

    IO io;
    io.setDirectoryClasses(directory_classes);
    io.load();

    gap = io.getNumberOfSamples()/9;
    clearMosaic();
    mosaic(io.getHistory(), gap);
    if(io.getHistory().size() > 0){
        ui->actionSave_images->setEnabled(true);
        ui->toolBox->setEnabled(true);
    }

    classesDir = io.getClassesDir();
    history = io.getHistory();
    dirs = io.getDirs();
}

void MainWindow::progressBarStatus(int n){

    int numFiles = n;
    QProgressDialog progress("Working", "Abort Copy", 0, numFiles, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setCancelButton(0);

    for (int i = 0; i < numFiles; i++) {
        progress.setValue(i);

        if (progress.wasCanceled())
            break;

    }
    progress.setValue(numFiles);

}

void MainWindow::clearMosaic(){
    ui->img_1->clear();    ui->img_2->clear();    ui->img_3->clear();    ui->img_4->clear();
    ui->img_5->clear();    ui->img_6->clear();    ui->img_7->clear();    ui->img_8->clear();
    ui->img_9->clear();
}

void MainWindow::mosaic(vector<vector<Mat> > matrix, unsigned int gap){
    vector<Mat> allImages;
    for (int var = 0; var < matrix.size(); var++) {
        vector<Mat> aux = matrix.at(var);
        for (int j = 0; j < aux.size(); j++) {
            allImages.push_back(aux.at(j));
        }
    }

    for (int n = 0; n < allImages.size(); n++) {
        Mat dest;
        cvtColor(allImages.at(n), dest,CV_BGR2RGB);
        allImages.at(n) = dest;
        QPixmap  pix = QPixmap::fromImage(QImage((unsigned char*) allImages.at(n).data, allImages.at(n).cols, allImages.at(n).rows, QImage::Format_RGB888));

        if(n == 0*gap){
            ui->img_1->setAlignment(Qt::AlignCenter);
            ui->img_1->setPixmap(pix.scaled(W,H,Qt::KeepAspectRatio));
        }
        if(n == 1*gap){
            ui->img_2->setAlignment(Qt::AlignCenter);
            ui->img_2->setPixmap(pix.scaled(W,H,Qt::KeepAspectRatio));
        }
        if(n == 2*gap){
            ui->img_3->setAlignment(Qt::AlignCenter);
            ui->img_3->setPixmap(pix.scaled(W,H,Qt::KeepAspectRatio));
        }
        if(n == 3*gap){
            ui->img_4->setAlignment(Qt::AlignCenter);
            ui->img_4->setPixmap(pix.scaled(W,H,Qt::KeepAspectRatio));
        }
        if(n == 4*gap){
            ui->img_5->setAlignment(Qt::AlignCenter);
            ui->img_5->setPixmap(pix.scaled(W,H,Qt::KeepAspectRatio));
        }
        if(n == 5*gap){
            ui->img_6->setAlignment(Qt::AlignCenter);
            ui->img_6->setPixmap(pix.scaled(W,H,Qt::KeepAspectRatio));
        }
        if(n == 6*gap){
            ui->img_7->setAlignment(Qt::AlignCenter);
            ui->img_7->setPixmap(pix.scaled(W,H,Qt::KeepAspectRatio));
        }
        if(n == 7*gap){
            ui->img_8->setAlignment(Qt::AlignCenter);
            ui->img_8->setPixmap(pix.scaled(W,H,Qt::KeepAspectRatio));
        }
        if(n == 8*gap){
            ui->img_9->setAlignment(Qt::AlignCenter);
            ui->img_9->setPixmap(pix.scaled(W,H,Qt::KeepAspectRatio));
        }
    }
}
//////// BEGIN - PREPROCESSING ////////
void MainWindow::on_btn_test_histogram_equalization_clicked()
{
    ui->btn_apply_histogram_equalization->setEnabled(true);
    ui->btn_cancel_histogram_equalization->setEnabled(true);
    PreProc *pp = new PreProc(history);
    temp = pp->histogramEqualization();
    mosaic(temp, gap);
}

void MainWindow::on_btn_cancel_histogram_equalization_clicked(){ mosaic(history, gap);}
void MainWindow::on_btn_cancel_image_filtering_clicked(){ mosaic(history, gap);}
void MainWindow::on_btn_cancel_edge_detection_clicked(){ mosaic(history, gap);}
void MainWindow::on_btn_cancel_thresholding_clicked(){ mosaic(history, gap);}

void MainWindow::on_btn_apply_histogram_equalization_clicked(){
    ui->btn_apply_histogram_equalization->setEnabled(false);
    ui->btn_cancel_histogram_equalization->setEnabled(false);
    history = temp;
}
void MainWindow::on_btn_apply_image_filtering_clicked(){
    ui->btn_apply_image_filtering->setEnabled(false);
    ui->btn_cancel_image_filtering->setEnabled(false);
    history = temp;
}
void MainWindow::on_btn_apply_thresholding_clicked(){
    ui->btn_apply_thresholding->setEnabled(false);
    ui->btn_cancel_thresholding->setEnabled(false);
    history = temp;
}
void MainWindow::on_btn_apply_edge_detection_clicked(){
    ui->btn_apply_edge_detection->setEnabled(false);
    ui->btn_cancel_edge_detection->setEnabled(false);
    history = temp;
}

void MainWindow::on_cbox_img_filtering_mask_currentIndexChanged(int index)
{
    switch(index){
    case 0:
        ui->cbox_img_filtering_method->setEnabled(false);
        ui->btn_apply_image_filtering->setEnabled(false);
        ui->btn_cancel_image_filtering->setEnabled(false);
        break;
    case 1:
        ui->cbox_img_filtering_method->setEnabled(true);
        kernel = 3;
        break;
    case 2:
        ui->cbox_img_filtering_method->setEnabled(true);
        kernel = 5;
        break;
    case 3:
        ui->cbox_img_filtering_method->setEnabled(true);
        kernel = 7;
        break;
    case 4:
        ui->cbox_img_filtering_method->setEnabled(true);
        kernel = 9;
        break;
    case 5:
        ui->cbox_img_filtering_method->setEnabled(true);
        kernel = 11;
        break;
    }
}

void MainWindow::on_cbox_img_filtering_method_currentIndexChanged(int index)
{
    PreProc *pp = new PreProc(history);
    switch(index){
    case 0:
        ui->btn_apply_image_filtering->setEnabled(false);
        ui->btn_cancel_image_filtering->setEnabled(false);
        break;
    case 1://average filter
        ui->btn_apply_image_filtering->setEnabled(true);
        ui->btn_cancel_image_filtering->setEnabled(true);
        temp = pp->imageFiltering(kernel, pp->ift_AVERAGE);
        break;
    case 2://median filter
        ui->btn_apply_image_filtering->setEnabled(true);
        ui->btn_cancel_image_filtering->setEnabled(true);
        temp = pp->imageFiltering(kernel, pp->ift_MEDIAN);
        break;
    case 3://gaussian filter
        ui->btn_apply_image_filtering->setEnabled(true);
        ui->btn_cancel_image_filtering->setEnabled(true);
        temp = pp->imageFiltering(kernel, pp->ift_GAUSSIAN);
        break;
    }
    mosaic(temp, gap);
}

void MainWindow::on_sld_thresholding_manual_sliderReleased()
{
    int value = ui->lbl_threshold_value->text().toInt();
    PreProc *pp = new PreProc(history);
    temp = pp->thresholding(value, pp->tt_MANUALLY);
    mosaic(temp, gap);
}

void MainWindow::on_cbox_thresholding_currentIndexChanged(int index)
{
    PreProc *pp = new PreProc(history);
    switch (index) {
    case 0: //white space
        ui->sld_thresholding_manual->setEnabled(false);
        ui->btn_apply_thresholding->setEnabled(false);
        ui->btn_cancel_thresholding->setEnabled(false);
        break;
    case 1: //manually threshould
        ui->sld_thresholding_manual->setEnabled(true);
        ui->btn_apply_thresholding->setEnabled(true);
        ui->btn_cancel_thresholding->setEnabled(true);
        break;
    case 2://to zero
        ui->sld_thresholding_manual->setEnabled(false);
        ui->cbox_thresholding->setCurrentIndex(0);
        ui->btn_apply_thresholding->setEnabled(true);
        ui->btn_cancel_thresholding->setEnabled(true);
        temp = pp->thresholding(0, pp->tt_MANUALLY);
        mosaic(temp, gap);
        break;
    case 3: //binary
        ui->sld_thresholding_manual->setEnabled(false);
        ui->cbox_thresholding->setCurrentIndex(0);
        ui->btn_apply_thresholding->setEnabled(true);
        ui->btn_cancel_thresholding->setEnabled(true);
        temp = pp->thresholding(0, pp->tt_BINARY);
        mosaic(temp, gap);
        break;
    case 4: //truncated
        ui->sld_thresholding_manual->setEnabled(false);
        ui->cbox_thresholding->setCurrentIndex(0);
        ui->btn_apply_thresholding->setEnabled(true);
        ui->btn_cancel_thresholding->setEnabled(true);
        temp = pp->thresholding(0, pp->tt_TRUNCATED);
        mosaic(temp, gap);
        break;
    case 5: //otsu
        ui->sld_thresholding_manual->setEnabled(false);
        ui->cbox_thresholding->setCurrentIndex(0);
        ui->btn_apply_thresholding->setEnabled(true);
        ui->btn_cancel_thresholding->setEnabled(true);
        temp = pp->thresholding(0, pp->tt_OTSU);
        mosaic(temp, gap);
        break;
    default:
        break;
    }
}

void MainWindow::on_cbox_edge_detection_currentIndexChanged(int index)
{
    PreProc *pp = new PreProc(history);
    switch (index) {
    case 0:
        ui->btn_apply_edge_detection->setEnabled(false);
        ui->btn_cancel_edge_detection->setEnabled(false);
        break;
    case 1: //roberts
        ui->btn_apply_edge_detection->setEnabled(true);
        ui->btn_cancel_edge_detection->setEnabled(true);
        temp = pp->edgeDetection(pp->et_ROBERTS);
        mosaic(temp, gap);
        break;
    case 2: //prewitt
        ui->btn_apply_edge_detection->setEnabled(true);
        ui->btn_cancel_edge_detection->setEnabled(true);
        temp = pp->edgeDetection(pp->et_PREWITT);
        mosaic(temp, gap);
        break;
    case 3: //sobel
        ui->btn_apply_edge_detection->setEnabled(true);
        ui->btn_cancel_edge_detection->setEnabled(true);
        temp = pp->edgeDetection(pp->et_SOBEL);
        mosaic(temp, gap);
        break;
    default:
        break;
    }
}
//////// END - PREPROCESSING ////////
//////// BEGIN - EXTRACTORS ////////
void MainWindow::on_box_gch_clicked(){    checkExtractorsEnabled();}
void MainWindow::on_box_cgch_clicked(){    checkExtractorsEnabled();}
void MainWindow::on_box_bic_clicked(){    checkExtractorsEnabled();}
void MainWindow::on_box_haralick_clicked(){    checkExtractorsEnabled();}
void MainWindow::on_box_lbp_clicked(){    checkExtractorsEnabled();}
void MainWindow::on_box_lch_clicked(bool checked)
{
    checkExtractorsEnabled();
    ui->label_3->setEnabled(checked);
    ui->spin_lch_width->setEnabled(checked);
    ui->label_4->setEnabled(checked);
    ui->spin_lch_height->setEnabled(checked);
}

void MainWindow::checkExtractorsEnabled(){

    if(ui->box_gch->isChecked() || ui->box_cgch->isChecked() || ui->box_bic->isChecked() || ui->box_lch->isChecked())  ui->btn_calc_descriptors_color->setEnabled(true);
    else  ui->btn_calc_descriptors_color->setEnabled(false);

    if(ui->box_haralick->isChecked()){
        ui->cbox_haralick_angle->setEnabled(true);
        ui->label_17->setEnabled(true);
        ui->spin_haralick_distance->setEnabled(true);
    }else{
        ui->cbox_haralick_angle->setEnabled(false);
        ui->label_17->setEnabled(false);
        ui->spin_haralick_distance->setEnabled(false);
    }

    if(ui->box_lbp->isChecked()) ui->cbox_lbp->setEnabled(true);
    else ui->cbox_lbp->setEnabled(false);
}

void MainWindow::on_cbox_haralick_angle_currentIndexChanged(int index)
{
    if(index != 0) ui->btn_calc_descriptors_texture->setEnabled(true);
    else ui->btn_calc_descriptors_texture->setEnabled(false);
}

void MainWindow::on_cbox_lbp_currentIndexChanged(int index)
{
    if(index != 0) ui->btn_calc_descriptors_texture->setEnabled(true);
    else ui->btn_calc_descriptors_texture->setEnabled(false);
}


void MainWindow::on_btn_calc_descriptors_color_clicked()
{
    if(ui->box_lch->isChecked()){
        tInicio = clock();
        Extractors e;
        bool saved = e.lch(history, dirs, classesDir, ui->spin_lch_width->value(), ui->spin_lch_height->value());
        tFim = clock();
        tDecorrido = ((double)(tFim - tInicio) / CLOCKS_PER_SEC ); //resultados

        string file_name = classesDir;
        file_name.append(PATH_SEPARATOR).append("txt_dat_files").
                append(PATH_SEPARATOR).append("lch").append(".time");

        tDecorrido = ((double)(tFim - tInicio) / CLOCKS_PER_SEC ); //resultados

        IO::saveTime(tDecorrido, file_name);

        if(saved){
            QMessageBox::information(this,tr(""),"The file with LCH descriptors was saved!");
        }else{
            QMessageBox::information(this,tr(""),"Error to save the file with LCH descriptors.");
        }
    }

    if(ui->box_gch->isChecked()){
        tInicio = clock();
        Extractors e;
        bool saved = e.gch(history, dirs, classesDir);
        tFim = clock();
        tDecorrido = ((double)(tFim - tInicio) / CLOCKS_PER_SEC ); //resultados

        string file_name = classesDir;
        file_name.append(PATH_SEPARATOR).append("txt_dat_files").
                append(PATH_SEPARATOR).append("gch").append(".time");

        IO::saveTime(tDecorrido, file_name);


        if(saved){
            QMessageBox::information(this,tr(""),"The file with GCH descriptors was saved!");
        }else{
            QMessageBox::information(this,tr(""),"Error to save the file with GCH descriptors.");
        }
    }

    if(ui->box_cgch->isChecked()){
        tInicio = clock();
        Extractors e;
        bool saved = e.cgch(history, dirs, classesDir);
        tFim = clock();
        tDecorrido = ((double)(tFim - tInicio) / CLOCKS_PER_SEC ); //resultados

        string file_name = classesDir;
        file_name.append(PATH_SEPARATOR).append("txt_dat_files").
                append(PATH_SEPARATOR).append("cgch").append(".time");

        tDecorrido = ((double)(tFim - tInicio) / CLOCKS_PER_SEC ); //resultados
        IO::saveTime(tDecorrido, file_name);

        if(saved){
            QMessageBox::information(this,tr(""),"The file with CGCH descriptors was saved!");
        }else{
            QMessageBox::information(this,tr(""),"Error to save the file with CGCH descriptors.");
        }
    }


    if(ui->box_bic->isChecked()){
        tInicio = clock();
        Extractors e;
        bool saved = e.bic(history, dirs, classesDir);
        tFim = clock();

        string file_name = classesDir;
        file_name.append(PATH_SEPARATOR).append("txt_dat_files").
                append(PATH_SEPARATOR).append("bic").append(".time");

        tDecorrido = ((double)(tFim - tInicio) / CLOCKS_PER_SEC ); //resultados
        IO::saveTime(tDecorrido, file_name);

        if(saved){
            QMessageBox::information(this,tr(""),"The file with BIC descriptors was saved!");
        }else{
            QMessageBox::information(this,tr(""),"Error to save the file with BIC descriptors.");
        }
    }
}

void MainWindow::on_btn_calc_descriptors_texture_clicked()
{

    if(ui->box_lbp->isChecked()){
        tInicio = clock();
        Extractors e;
        bool saved = e.lbp(history, dirs, classesDir, ui->cbox_lbp->currentIndex());
        tFim = clock();
        tDecorrido = ((double)(tFim - tInicio) / CLOCKS_PER_SEC ); //resultados

        string file_name = classesDir;
        file_name.append(PATH_SEPARATOR).append("txt_dat_files").
                append(PATH_SEPARATOR).append("lbp").append(".time");

        tDecorrido = ((double)(tFim - tInicio) / CLOCKS_PER_SEC ); //resultados
        IO::saveTime(tDecorrido, file_name);

        if(saved){
            QMessageBox::information(this,tr(""),"The file with LBP descriptors was saved!");
        }else{
            QMessageBox::information(this,tr(""),"Error to save the file with LBP descriptors.");
        }
    }

    if(ui->box_haralick->isChecked()){
        tInicio = clock();
        Extractors e;

        int x, y;
        switch (ui->cbox_haralick_angle->currentIndex()) {
        case 1://0°
            x = 1 * ui->spin_haralick_distance->value();
            y = 0;
            break;
        case 2://45°
            x = 1 * ui->spin_haralick_distance->value();
            y = -1 * ui->spin_haralick_distance->value();
            break;
        case 3://90°
            x = 0;
            y = -1 * ui->spin_haralick_distance->value();
            break;
        case 4://135°
            x = -1 * ui->spin_haralick_distance->value();
            y = -1 * ui->spin_haralick_distance->value();
        default:
            break;
        }
        bool saved = e.haralick(history, dirs, classesDir, x, y);
        tFim = clock();
        tDecorrido = ((double)(tFim - tInicio) / CLOCKS_PER_SEC ); //resultados

        string file_name = classesDir;
        file_name.append(PATH_SEPARATOR).append("txt_dat_files").
                append(PATH_SEPARATOR).append("haralick").append(".time");

        tDecorrido = ((double)(tFim - tInicio) / CLOCKS_PER_SEC ); //resultados
        IO::saveTime(tDecorrido, file_name);

        if(saved){
            QMessageBox::information(this,tr(""),"The file with Haralick\'s descriptors was saved!");
        }else{
            QMessageBox::information(this,tr(""),"Error to save the file with Haralick\'s descriptors.");
        }
    }
}
//////// END - EXTRACTORS ////////
//////// BEGIN - CLASSIFIERS ////////
void MainWindow::on_btn_opf_select_path_clicked()
{
    opf_path = QFileDialog::getExistingDirectory(this, tr("Select the OPF directory"), QDir::currentPath());
    if(opf_path.isEmpty()) return;

    ui->lbl_opf_path_opf->setText("");
    ui->lbl_opf_path_opf->setText(opf_path);

    ui->btn_opf_select_dat->setEnabled(true);
    ui->btn_opf_select_txt->setEnabled(true);
}

void MainWindow::on_btn_opf_select_dat_clicked()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(trUtf8("Binaries Files (*.dat)"));
    opf_files_path.clear();
    if (dialog.exec()){
        opf_files_path = dialog.selectedFiles();
    }

    if(opf_files_path.empty()) return;

    ui->btn_opf_run_classifier->setEnabled(true);

    for (int var = 0; var < opf_files_path.size(); var++) {
        string aux = opf_files_path.at(var).toUtf8().data();
        stringstream ss(aux);
        string item;
        vector<string> elems;
        while (std::getline(ss, item, '.')) {
            elems.push_back(item);
        }
        string path_txt = "";
        for (int i = 0; i < elems.size()-1; i++) {
            path_txt.append(elems.at(i));
            if(i < elems.size() - 2){
                path_txt.append(".");
            }
        }
        QString path(path_txt.c_str());
        txt_files_path.push_back(path);
    }
}

void MainWindow::on_btn_opf_select_txt_clicked()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(trUtf8("OPF Text Files (*.txt)"));
    opf_files_path.clear();
    txt_files_path.clear();
    if (dialog.exec()){
        opf_files_path = dialog.selectedFiles();
    }

    if(opf_files_path.empty()) return;

    ui->btn_opf_run_classifier->setEnabled(true);

    txt_files_path = opf_files_path;

    Utils utils;
    vector<string> aux = utils.txt2OPF(opf_files_path, opf_path);
    opf_files_path.clear();

    for (int var = 0; var < aux.size(); var++) {
        QString dat_file = QString::fromStdString(aux.at(var));
        opf_files_path.push_back(dat_file);
    }
}

void MainWindow::on_btn_opf_run_classifier_clicked()
{
    for (int var = 0; var < ui->spin_opf_times->value(); var++) {
        Utils utils;
        vector<vector<vector<vector<int> > > > indices = utils.myShuffle(txt_files_path, ui->spin_opf_training->value());
        utils.myOPFSplit(opf_files_path, opf_path, indices);
        utils.myOPFTrain(opf_files_path, opf_path);
        utils.myOPFClassify(opf_files_path, opf_path);
        utils.myOPFAccuracy(opf_files_path, opf_path);
        utils.myOPFSaveIterations(opf_files_path, var);
    }
    Utils u;
    iterations_paths_opf = u.iterationsPaths(opf_files_path, ui->spin_opf_times->value());
    QMessageBox::information(this,tr(""),"Process was completed!");
    ui->btn_opf_show_results->setEnabled(true);
}


void MainWindow::on_btn_opf_show_results_clicked()
{
    Utils utils;
    utils.setNumberIterations(ui->spin_opf_times->value());
    vector<vector<float> > result_vector = utils.calcResults(opf_files_path,"opf");

    QStringList rowNames;
    for (int var = 0; var < opf_files_path.size(); var++) {
        string aux = opf_files_path.at(var).toUtf8().data();
        aux.append("_opf");
        QString qs(aux.c_str());
        rowNames.push_back(qs);
    }

    //matriz de confusao
    DialogResults d_results;
    d_results.setModal(true);
    d_results.setResultsVector(result_vector,
                               opf_files_path.size(),
                               rowNames);
    d_results.n_iterations = ui->spin_opf_times->value();
    d_results.opf_files_path = opf_files_path;
    d_results.iterations_paths_opf = iterations_paths_opf;
    d_results.exec();
}

void MainWindow::on_btn_svm_select_path_clicked()
{
    svm_path = QFileDialog::getExistingDirectory(this, tr("Select the directory"), QDir::currentPath());
    if(svm_path.isEmpty()) return;

    ui->lbl_svm_path_svm->setText("");
    ui->lbl_svm_path_svm->setText(svm_path);
    ui->btn_svm_select_svm->setEnabled(true);
    ui->btn_svm_select_txt->setEnabled(true);
}

void MainWindow::on_btn_svm_select_svm_clicked()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(trUtf8("SVM Files (*.svm)"));
    svm_files_path.clear();
    if (dialog.exec()){
        svm_files_path = dialog.selectedFiles();
        ui->btn_svm_run_classifier->setEnabled(true);
    }
}

void MainWindow::on_btn_svm_select_txt_clicked()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(trUtf8("SVM Text Files (*.txt)"));
    svm_files_path.clear();
    if (dialog.exec()){
        svm_files_path = dialog.selectedFiles();
        ui->btn_svm_run_classifier->setEnabled(true);
    }

    QStringList aux;
    for (int var = 0; var < svm_files_path.size(); var++) {
        string out = svm_files_path.at(var).toUtf8().data();
        out.append(".svm");
        string in = svm_files_path.at(var).toUtf8().data();
        const char * c_out = out.c_str();
        const char * c_in = in.c_str();
        Utils u;
        u.txt2SVM(c_in, c_out);
        QString _out(out.c_str());
        aux.push_back(_out);
    }
    svm_files_path = aux;
}

void MainWindow::on_btn_svm_run_classifier_clicked()
{
    iterations_paths_svm.clear();
    for (int i = 0; i < ui->spin_svm_times->value(); i++) {
        //shuffle
        Utils utils;
        vector<string> c_and_g;// Grid search function (future)
        for (int var = 0; var < svm_files_path.size(); var++) {
            //svm split
            utils.mySVMSplit(svm_path, svm_files_path.at(var), ui->spin_svm_training->value());
            //svm train
            utils.mySVMTrain(svm_path, svm_files_path.at(var), c_and_g, false /*grid search is false*/);
            //svm classify
            utils.mySVMClassify(svm_path, svm_files_path.at(var));
            //svm accuracy
            utils.mySVMAccuracy(svm_files_path.at(var));

            //copia todo o conteudo para uma pasta apos cada iteracao
            string from = svm_files_path.at(var).toUtf8().data();
            from.append("_files").append(PATH_SEPARATOR);
            char number[5];
            sprintf(number, "%d", i);
            string it = number;
            string to = svm_files_path.at(var).toUtf8().data();
            to.append("_iterations").append(PATH_SEPARATOR);
            QDir().mkdir(to.c_str());
            to.append(it.c_str());
            QDir().mkdir(to.c_str());
            utils.cpFolders(from, to);
            //salva os caminhos dos paths com iteracoes
            iterations_paths_svm.push_back(to);
        }
    }
    QMessageBox::information(this,tr(""),"Process was completed!");
    ui->btn_svm_show_results->setEnabled(true);
}

void MainWindow::on_btn_svm_show_results_clicked()
{
    Utils utils;
    utils.setNumberIterations(ui->spin_svm_times->value());
    vector<vector<float> > result_vector = utils.calcResults(svm_files_path,"svm");

    QStringList rowNames;
    for (int var = 0; var < svm_files_path.size(); var++) {
        string aux = svm_files_path.at(var).toUtf8().data();
        aux.append("_svm");
        QString qs(aux.c_str());
        rowNames.push_back(qs);
    }

    //matriz de confusao
    DialogResults d_results;
    d_results.setModal(true);
    d_results.setResultsVector(result_vector,
                               svm_files_path.size(),
                               rowNames);
    d_results.n_iterations = ui->spin_svm_times->value();
    d_results.svm_files_path = svm_files_path;
    d_results.iterations_paths_svm = iterations_paths_svm;
    d_results.exec();
}

void MainWindow::on_btn_comparison_opf_path_clicked()
{
    opf_path = QFileDialog::getExistingDirectory(this, tr("Select the OPF directory"), QDir::currentPath());
    if(opf_path.isEmpty()) return;
}

void MainWindow::on_btn_comparison_svm_path_clicked()
{
    svm_path = QFileDialog::getExistingDirectory(this, tr("Select the directory"), QDir::currentPath());
    if(svm_path.isEmpty()) return;
}

void MainWindow::on_btn_comparison_select_txt_clicked()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(trUtf8("Text Files (*.txt)"));
    txt_files_path.clear();
    if (dialog.exec()){
        txt_files_path = dialog.selectedFiles();
    }
    ui->btn_comparison_run_classifier->setEnabled(true);
}

void MainWindow::on_btn_comparison_run_classifier_clicked()
{
    //converte os arquivos txt
    //opf
    Utils utils;
    vector<string> aux_opf = utils.txt2OPF(txt_files_path, opf_path);
    opf_files_path.clear();
    svm_files_path.clear();
    string path_txt;
    for (int var = 0; var < aux_opf.size(); var++) {
        QString dat_file = QString::fromStdString(aux_opf.at(var));
        opf_files_path.push_back(dat_file);

        path_txt = txt_files_path.at(var).toUtf8().data();
        path_txt.append(".svm");
        QString p(path_txt.c_str());
        svm_files_path.push_back(p);
    }
    //zera o vetor com os paths de iteracoes
    iterations_paths_opf.clear();
    iterations_paths_svm.clear();

    for (int i = 0; i < ui->spin_comparison_times->value(); i++) {
        //shuffle
        vector<vector<vector<vector<int> > > > indices = utils.myShuffle(txt_files_path,ui->spin_comparison_training->value());
        //opf split
        utils.myOPFSplit(opf_files_path, opf_path, indices);
        //opf to svm
        utils.dat2SVM(opf_files_path, svm_files_path, opf_path);
        //opf train
        utils.myOPFTrain(opf_files_path, opf_path);
        //opf classify
        utils.myOPFClassify(opf_files_path, opf_path);
        //opf accuracy
        utils.myOPFAccuracy(opf_files_path, opf_path);
        vector<string> c_and_g;
        for (int var = 0; var < svm_files_path.size(); var++) {
            //faz grid search
            //if(ui->grid_comparison->isChecked()){
            //retorna 'c' na posicao 0 e 'g' na posicao 1
            c_and_g.clear();
            //  c_and_g = my_svm_grid(svm_path, svm_files_path.at(var));
            //}
            //svm train
            utils.mySVMTrain(svm_path, svm_files_path.at(var), c_and_g, /*future implementation*/false);
            //svm classify
            utils.mySVMClassify(svm_path, svm_files_path.at(var));
            //svm accuracy
            utils.mySVMAccuracy(svm_files_path.at(var));

            //copia todo o conteudo para uma pasta apos cada iteracao
            string from = svm_files_path.at(var).toUtf8().data();
            from.append("_files").append(PATH_SEPARATOR);
            char number[5];
            sprintf(number, "%d", i);
            string it = number;
            string to = svm_files_path.at(var).toUtf8().data();
            to.append("_iterations").append(PATH_SEPARATOR);
            QDir().mkdir(to.c_str());
            to.append(it.c_str());
            QDir().mkdir(to.c_str());
            utils.cpFolders(from, to);
            //salva os caminhos dos paths com iteracoes
            iterations_paths_svm.push_back(to);
        }

        for (int var = 0; var < opf_files_path.size(); var++) {
            //copia todo o conteudo para uma pasta apos cada iteracao
            string from = opf_files_path.at(var).toUtf8().data();
            from.append("_files").append(PATH_SEPARATOR);
            char number[5];
            sprintf(number, "%d", i);
            string it = number;
            string to = opf_files_path.at(var).toUtf8().data();
            to.append("_iterations").append(PATH_SEPARATOR);
            QDir().mkdir(to.c_str());
            to.append(it.c_str());
            QDir().mkdir(to.c_str());
            utils.cpFolders(from, to);
            iterations_paths_opf.push_back(to);
        }
    }
    QMessageBox::information(this,tr(""),"Process was completed!");
    ui->btn_comparison_show_results->setEnabled(true);
}

void MainWindow::on_btn_comparison_show_results_clicked()
{

    Utils utils;
    utils.setNumberIterations(ui->spin_comparison_times->value());
    vector<vector<float> > result_vector = utils.calcResults(svm_files_path,"svm");
    vector<vector<float> > aux = utils.calcResults(opf_files_path,"opf");

    result_vector.insert(result_vector.end(), aux.begin(), aux.end());

    QStringList rowNames;
    for (int var = 0; var < svm_files_path.size(); var++) {
        string aux = svm_files_path.at(var).toUtf8().data();
        aux.append("_svm");
        QString qs(aux.c_str());
        rowNames.push_back(qs);
    }
    for (int var = 0; var < svm_files_path.size(); var++) {
        string aux = opf_files_path.at(var).toUtf8().data();
        aux.append("_opf");
        QString qs(aux.c_str());
        rowNames.push_back(qs);
    }

    //matriz de confusao
    DialogResults d_results;
    d_results.setModal(true);
    d_results.setResultsVector(result_vector,
                               opf_files_path.size()+svm_files_path.size(),
                               rowNames);
    d_results.n_iterations = ui->spin_comparison_times->value();
    d_results.svm_files_path = svm_files_path;
    d_results.opf_files_path = opf_files_path;
    d_results.iterations_paths_opf = iterations_paths_opf;
    d_results.iterations_paths_svm = iterations_paths_svm;
    d_results.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    About a;
    a.setModal(true);
    a.exec();
}

void MainWindow::on_spin_opf_training_valueChanged(int arg1){ ui->spin_opf_testing->setValue(abs(100-arg1));}
void MainWindow::on_spin_opf_testing_valueChanged(int arg1){ ui->spin_opf_training->setValue(abs(100-arg1));}
void MainWindow::on_spin_svm_training_valueChanged(int arg1){ ui->spin_svm_testing->setValue(abs(100-arg1));}
void MainWindow::on_spin_svm_testing_valueChanged(int arg1){ ui->spin_svm_training->setValue(abs(100-arg1));}
void MainWindow::on_spin_comparison_training_valueChanged(int arg1){ ui->spin_comparison_testing->setValue(abs(100-arg1));}
void MainWindow::on_spin_comparison_testing_valueChanged(int arg1){ ui->spin_comparison_training->setValue(abs(100-arg1));}


void MainWindow::on_pushButton_clicked()
{
    int numTasks = 9999999;
       QProgressDialog progress("Task in progress...", "Cancel", 0, numTasks, this);
       progress.setWindowModality(Qt::WindowModal);
       progress.setRange(0,0);
       progress.setMinimumDuration(0);
       progress.show();


       for (int i = 0; i < numTasks; i++) {
           progress.setValue(i);


           if (progress.wasCanceled())
               break;
       }
       progress.setValue(numTasks);
}
