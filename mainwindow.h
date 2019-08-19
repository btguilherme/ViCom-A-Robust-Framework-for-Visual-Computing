#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <highgui.h>
#include "header.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
    void on_actionOpen_directory_triggered();
    void clearMosaic();
    void mosaic(vector<vector<Mat> > matrix, unsigned int gap);
    void progressBarStatus(int n);

    void checkExtractorsEnabled();

    void on_btn_test_histogram_equalization_clicked();

    void on_btn_cancel_histogram_equalization_clicked();

    void on_btn_apply_histogram_equalization_clicked();

    void on_cbox_img_filtering_mask_currentIndexChanged(int index);

    void on_cbox_img_filtering_method_currentIndexChanged(int index);

    void on_btn_apply_image_filtering_clicked();

    void on_btn_cancel_image_filtering_clicked();

    void on_btn_apply_thresholding_clicked();

    void on_btn_cancel_thresholding_clicked();

    void on_btn_apply_edge_detection_clicked();

    void on_btn_cancel_edge_detection_clicked();

    void on_cbox_thresholding_currentIndexChanged(int index);

    void on_sld_thresholding_manual_sliderReleased();

    void on_cbox_edge_detection_currentIndexChanged(int index);

    void on_box_gch_clicked();

    void on_box_lch_clicked(bool checked);

    void on_box_cgch_clicked();

    void on_box_bic_clicked();

    void on_box_haralick_clicked();

    void on_box_lbp_clicked();

    void on_cbox_haralick_angle_currentIndexChanged(int index);

    void on_cbox_lbp_currentIndexChanged(int index);

    void on_btn_opf_select_path_clicked();

    void on_btn_opf_select_dat_clicked();

    void on_btn_opf_select_txt_clicked();

    void on_btn_opf_run_classifier_clicked();

    void on_btn_opf_show_results_clicked();

    void on_btn_svm_select_path_clicked();

    void on_btn_svm_select_svm_clicked();

    void on_btn_svm_select_txt_clicked();

    void on_btn_svm_run_classifier_clicked();

    void on_btn_svm_show_results_clicked();

    void on_btn_comparison_opf_path_clicked();

    void on_btn_comparison_svm_path_clicked();

    void on_btn_comparison_select_txt_clicked();

    void on_btn_comparison_run_classifier_clicked();

    void on_btn_comparison_show_results_clicked();

    void on_actionAbout_triggered();

    void on_spin_opf_training_valueChanged(int arg1);

    void on_spin_opf_testing_valueChanged(int arg1);

    void on_spin_svm_training_valueChanged(int arg1);

    void on_spin_svm_testing_valueChanged(int arg1);

    void on_spin_comparison_training_valueChanged(int arg1);

    void on_spin_comparison_testing_valueChanged(int arg1);

    void on_btn_calc_descriptors_color_clicked();

    void on_btn_calc_descriptors_texture_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    //variables
    QString opf_path, svm_path;
    QStringList opf_files_path, svm_files_path, txt_files_path;
    vector<string> iterations_paths_svm, iterations_paths_opf;

};

#endif // MAINWINDOW_H
