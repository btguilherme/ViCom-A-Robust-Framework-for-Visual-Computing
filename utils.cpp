#include "utils.h"
#include "header.h"

Utils::Utils()
{

}

Utils::~Utils()
{

}

vector<vector<float> > Utils::getBorderHistograms(){
    return borderHistograms;
}

vector<vector<float> > Utils::getInteriorHistograms(){
    return interiorHistograms;
}

void Utils::setNumberIterations(int n){
    n_iterations = n;
}

vector<Mat> Utils::colorReduce(vector<Mat> input, size_t div){
    vector<Mat> ret;
    for(int  var = 0; var < input.size(); var++){
        Mat image = input.at(var);
        Size size = image.size();

        vector<Mat> channels, channelsFinal;

        split(image, channels);

        Mat ch0 = channels.at(0);
        Mat dstCh0 = ch0;
        Mat ch1 = channels.at(1);
        Mat dstCh1 = ch1;
        Mat ch2 = channels.at(2);
        Mat dstCh2 = ch2;

        for(int  i = 0; i < size.height; i++){
            for(int  j = 0; j < size.width; j++){
                dstCh0.at<uchar>(i,j) = ch0.at<uchar>(i,j) / div * div + div / 2;
                dstCh1.at<uchar>(i,j) = ch1.at<uchar>(i,j) / div * div + div / 2;
                dstCh2.at<uchar>(i,j) = ch2.at<uchar>(i,j) / div * div + div / 2;
            }
        }

        channelsFinal.push_back(dstCh0);
        channelsFinal.push_back(dstCh1);
        channelsFinal.push_back(dstCh2);

        Mat result = Mat::zeros(image.rows, image.cols, CV_8UC1);
        cv::merge(channelsFinal, result);

        ret.push_back(result);
    }

    return ret;
}

vector<vector<float> > Utils::colorHistogram(vector<Mat> imgs, char option){
    vector<vector<float> > histograms;
    Utils u;
    for (int  var = 0; var < imgs.size(); var++) {
        Mat src = imgs.at(var);
        Size size = src.size();
        vector<Mat> channels;
        split(src, channels);

        vector<float> r_hist(TAM, 0.0), g_hist(TAM, 0.0),b_hist(TAM, 0.0);
        vector<float> r_hist_no_zeros, g_hist_no_zeros,b_hist_no_zeros;

        for(int  i = 0; i < size.height; i++){
            for(int  j = 0; j < size.width; j++){
                Mat aux = channels.at(0);
                int value_b = aux.at<uchar>(i,j);
                aux = channels.at(1);
                int value_g = aux.at<uchar>(i,j);
                aux = channels.at(2);
                int value_r = aux.at<uchar>(i,j);

                r_hist.at(value_r) = r_hist.at(value_r) + 1.0;
                g_hist.at(value_g) = g_hist.at(value_g) + 1.0;
                b_hist.at(value_b) = b_hist.at(value_b) + 1.0;
            }
        }

        //retira os valores 'zeros' dos vetores
        //caso a imagem seja reduzida em um valor diferente de
        //64 cores, os valores deverao ser diferentes para
        //o primeiro e terceiro parametro da funcao 'for' abaixo
        for (unsigned int  k = 2; k < r_hist.size(); k = k + 4) {
            r_hist_no_zeros.push_back(r_hist.at(k));
            g_hist_no_zeros.push_back(g_hist.at(k));
            b_hist_no_zeros.push_back(b_hist.at(k));
        }

        //'c' indica que o histograma sera cumulativo
        //qualquer outra letra nao sera cumulativo (por padrao, a letra
        //'n' representa 'no' (nao cumulativo)
        if(option == 'c'){

            //acumula os valores dos histogramas dos 3 canais
            for (unsigned int  i = 1; i < r_hist_no_zeros.size(); i++) {
                int aux = r_hist_no_zeros.at(i);
                r_hist_no_zeros.at(i) = r_hist_no_zeros.at(i - 1) + aux;

                aux = g_hist_no_zeros.at(i);
                g_hist_no_zeros.at(i) = g_hist_no_zeros.at(i - 1) + aux;

                aux = b_hist_no_zeros.at(i);
                b_hist_no_zeros.at(i) = b_hist_no_zeros.at(i - 1) + aux;
            }
        }

        vector<float> r_hist_no_zeros_f, g_hist_no_zeros_f,b_hist_no_zeros_f;

        //normaliza os histogramas
        Utils u;
        r_hist_no_zeros_f = u.normalizeHist(r_hist_no_zeros);
        g_hist_no_zeros_f = u.normalizeHist(g_hist_no_zeros);
        b_hist_no_zeros_f = u.normalizeHist(b_hist_no_zeros);

        histograms.push_back(r_hist_no_zeros_f);
        histograms.push_back(g_hist_no_zeros_f);
        histograms.push_back(b_hist_no_zeros_f);
    }

    return histograms;
}

vector<string> Utils::createOPFBody(int n_samples, string label, vector<string> haralick){
    vector<string> body;
    string aux;
    for (int i = 0; i < n_samples; i++) {
        aux = "";

        char number[10];
        sprintf(number, "%d", i);
        aux.append(number).append("\t").append(label).append("\t").append(haralick.at(i));

        body.push_back(aux);
    }
    return body;
}

vector<string> Utils::createOPFBody(int n_samples, string label, vector<vector<float> > histograms){
    vector<string> body;
    string aux;
    char number[100];

    //neste caso a imagem nao foi subdividida (eh subdividida no extrator LCH)
    //lbp entra nesse if
    if(3 * n_samples == histograms.size()){

        for (int i = 0; i < n_samples; i++) {
            aux = "";
            sprintf(number, "%d", i);
            aux.append(number).append("\t").append(label).append("\t");

            vector<float> hist_r = histograms.at(i*3);
            vector<float> hist_g = histograms.at((i*3)+1);
            vector<float> hist_b = histograms.at((i*3)+2);

            for (unsigned int j = 0; j < hist_r.size(); j++) {
                sprintf(number, "%.6f", hist_r.at(j));
                aux.append(number);
                aux.append("\t");
            }
            for (unsigned int k = 0; k < hist_g.size(); k++) {
                sprintf(number, "%.6f", hist_g.at(k));
                aux.append(number);
                aux.append("\t");
            }
            for (unsigned int l = 0; l < hist_b.size(); l++) {
                sprintf(number, "%.6f", hist_b.at(l));
                aux.append(number);
                aux.append("\t");
            }
            aux.append("\n");

            body.push_back(aux);
        }

    } else if(6 * n_samples == histograms.size()){

        //neste caso eh a funcao 'bic' escrevendo o arquivo (6 histogramas por imagem, sendo
        //tres (r,g,b) do interior e tres das bordas

        for (int i = 0; i < n_samples; i++) {
            aux = "";

            sprintf(number, "%d", i);
            aux.append(number).append("\t").append(label).append("\t");


            vector<float> hist_r_interior = histograms.at(i*6);
            vector<float> hist_g_interior = histograms.at((i*6)+1);
            vector<float> hist_b_interior = histograms.at((i*6)+2);

            vector<float> hist_r_border = histograms.at((i*6)+3);
            vector<float> hist_g_border = histograms.at((i*6)+4);
            vector<float> hist_b_border = histograms.at((i*6)+5);

            for (int j = 0; j < hist_r_interior.size(); j++) {
                sprintf(number, "%.6f", hist_r_interior.at(j));
                aux.append(number);
                aux.append("\t");
            }
            for (int k = 0; k < hist_g_interior.size(); k++) {
                sprintf(number, "%.6f", hist_g_interior.at(k));
                aux.append(number);
                aux.append("\t");
            }
            for (int l = 0; l < hist_b_interior.size(); l++) {
                sprintf(number, "%.6f", hist_b_interior.at(l));
                aux.append(number);
                aux.append("\t");
            }
            for (int j = 0; j < hist_r_border.size(); j++) {
                sprintf(number, "%.6f", hist_r_border.at(j));
                aux.append(number);
                aux.append("\t");
            }
            for (int k = 0; k < hist_g_border.size(); k++) {
                sprintf(number, "%.6f", hist_g_border.at(k));
                aux.append(number);
                aux.append("\t");
            }
            for (int l = 0; l < hist_b_border.size(); l++) {
                sprintf(number, "%.6f", hist_b_border.at(l));
                aux.append(number);
                aux.append("\t");
            }
            aux.append("\n");
            body.push_back(aux);
        }

    } else {
        //neste caso a imagem eh subdividida em 'x' partes
        for (int  i = 0; i < histograms.size()/3; i++) {
            if (i == 0){
                aux = "";
                sprintf(number, "%d", i);
                aux.append(number).append("\t").append(label).append("\t");
            }

            vector<float> hist_r = histograms.at(i*3);
            vector<float> hist_g = histograms.at((i*3)+1);
            vector<float> hist_b = histograms.at((i*3)+2);

            for (int j = 0; j < hist_r.size(); j++) {
                sprintf(number, "%.6f", hist_r.at(j));
                aux.append(number);
                aux.append("\t");
            }
            for (int k = 0; k < hist_g.size(); k++) {
                sprintf(number, "%.6f", hist_g.at(k));
                aux.append(number);
                aux.append("\t");
            }
            for (int l = 0; l < hist_b.size(); l++) {
                sprintf(number, "%.6f", hist_b.at(l));
                aux.append(number);
                aux.append("\t");
            }
        }
        aux.append("\n");
        body.push_back(aux);
    }

    return body;
}

vector<string> Utils::correctsNumbering(vector<string> src){
    vector<string> ret;
    for (int  var = 0; var < src.size(); var++) {
        string aux = src.at(var);
        int t = aux.find("\t",0);
        if(t == -1) {
            t = aux.find(' ',0);
        }
        char number[6];
        sprintf(number, "%d", var);
        aux.replace(0, t, number);
        ret.push_back(aux);
    }
    return ret;
}

string Utils::createOPFHeader(int number_of_samples, int number_of_labels, int number_of_features){
    char number[100];
    string head = "";

    sprintf(number, "%d", number_of_samples);
    head.append(number);
    head.append("\t");
    sprintf(number, "%d", number_of_labels);
    head.append(number);
    head.append("\t");
    sprintf(number, "%d", number_of_features);
    head.append(number);
    head.append("\n");

    return head;
}

vector<float> Utils::normalizeHist(vector<float> src){
    vector<float> hist;
    float sum = 0.0;
    for (int  var = 0; var < src.size(); var++) {
        sum = sum + src.at(var);
    }
    for (int  var = 0; var < src.size(); var++) {
        hist.push_back((float)src.at(var)/sum);
    }
    return hist;
}

vector<vector<Mat> > Utils::splitter(vector<Mat> src, int w, int h){
    vector<vector<Mat> > croppedImages;
    //split all the dataset images
    for (int  var = 0; var < src.size(); var++) {
        Mat image = src.at(var);
        Utils u;
        vector<Mat> subImages = u.splitImage(image, w, h);
        croppedImages.push_back(subImages);
    }
    return croppedImages;
}

vector<Mat> Utils::splitImage(Mat src, int w, int h){
    int imgHeight = floor(src.size().height/h);
    int imgWidth = floor(src.size().width/w);

    Size smallSize(imgWidth,imgHeight);
    vector<Mat> smallImages;

    for (int y = 0; y < src.rows; y += smallSize.height){
        if(y+smallSize.height > src.rows) return smallImages;
        for (int x = 0; x < src.cols; x += smallSize.width){
            if((x+smallSize.width > src.cols)) break;
            Rect rect =  Rect(x,y, smallSize.width, smallSize.height);
            smallImages.push_back(Mat(src, rect));
        }
    }
    return smallImages;
}

void Utils::classifyBorderInteriorPixels (vector<Mat> src){
    borderHistograms.clear();
    interiorHistograms.clear();

    for (int  var = 0; var < src.size(); var++) {
        Mat img = src.at(var);
        Size size = img.size();

        int **points;
        points = (int **)malloc(size.height * sizeof(int *));//linhas
        for (int  k = 0; k < size.height; k++) {
            points[k] = (int *)malloc(size.width * sizeof(int));//colunas
        }

        for (int i = 1; i < size.height - 1; i++) {
            for (int j = 1; j < size.width - 1; j++) {

                uchar up = img.at<uchar>(i-1,j);
                uchar down = img.at<uchar>(i+1,j);
                uchar left = img.at<uchar>(i,j-1);
                uchar right = img.at<uchar>(i,j+1);
                uchar center = img.at<uchar>(i,j);

                if(up == center && down == center && left == center && right == center){
                    //interior
                    points[i][j] = 1;
                } else {
                    //border
                    points[i][j] = 0;
                }
            }
        }
        bicColorHistogram(img, points);
    }
}

void Utils::bicColorHistogram(Mat src, int **points){

    Size size = src.size();
    vector<Mat> channels;
    split(src, channels);
    Utils u;

    vector<float> r_hist_border(TAM, 0), g_hist_border(TAM, 0),b_hist_border(TAM, 0);
    vector<float> r_hist_interior(TAM, 0), g_hist_interior(TAM, 0),b_hist_interior(TAM, 0);

    vector<float> r_hist_border_no_zeros, g_hist_border_no_zeros,b_hist_border_no_zeros;
    vector<float> r_hist_interior_no_zeros, g_hist_interior_no_zeros,b_hist_interior_no_zeros;

    for(int i = 1; i < size.height - 1; i++){
        for(int j = 1; j < size.width - 1; j++){
            Mat aux = channels.at(0);
            int value_b = aux.at<uchar>(i,j);
            aux = channels.at(1);
            int value_g = aux.at<uchar>(i,j);
            aux = channels.at(2);
            int value_r = aux.at<uchar>(i,j);

            if(points[i][j] == 1){
                r_hist_interior.at(value_r) = r_hist_interior.at(value_r) + 1;
                g_hist_interior.at(value_g) = g_hist_interior.at(value_g) + 1;
                b_hist_interior.at(value_b) = b_hist_interior.at(value_b) + 1;
            } else {
                r_hist_border.at(value_r) = r_hist_border.at(value_r) + 1;
                g_hist_border.at(value_g) = g_hist_border.at(value_g) + 1;
                b_hist_border.at(value_b) = b_hist_border.at(value_b) + 1;
            }
        }
    }


    //retira os valores 'zeros' dos vetores
    //caso a imagem seja reduzida em um valor diferente de
    //64 cores, os valores deverao ser diferentes para
    //o primeiro e terceiro parametro da funcao 'for' abaixo
    for (int  var = 2; var < r_hist_interior.size(); var = var + 4) {
        r_hist_interior_no_zeros.push_back(r_hist_interior.at(var));
        g_hist_interior_no_zeros.push_back(g_hist_interior.at(var));
        b_hist_interior_no_zeros.push_back(b_hist_interior.at(var));

        r_hist_border_no_zeros.push_back(r_hist_border.at(var));
        g_hist_border_no_zeros.push_back(g_hist_border.at(var));
        b_hist_border_no_zeros.push_back(b_hist_border.at(var));
    }

    vector<float> r_hist_interior_no_zeros_f, g_hist_interior_no_zeros_f,b_hist_interior_no_zeros_f;
    vector<float> r_hist_border_no_zeros_f, g_hist_border_no_zeros_f,b_hist_border_no_zeros_f;

    //normaliza os histogramas
    r_hist_interior_no_zeros_f = u.normalizeHist(r_hist_interior_no_zeros);
    g_hist_interior_no_zeros_f = u.normalizeHist(g_hist_interior_no_zeros);
    b_hist_interior_no_zeros_f = u.normalizeHist(b_hist_interior_no_zeros);

    r_hist_border_no_zeros_f = u.normalizeHist(r_hist_border_no_zeros);
    g_hist_border_no_zeros_f = u.normalizeHist(g_hist_border_no_zeros);
    b_hist_border_no_zeros_f = u.normalizeHist(b_hist_border_no_zeros);

    //adiciona nos vectores globais
    borderHistograms.push_back(r_hist_border_no_zeros_f);
    borderHistograms.push_back(g_hist_border_no_zeros_f);
    borderHistograms.push_back(b_hist_border_no_zeros_f);

    interiorHistograms.push_back(r_hist_interior_no_zeros_f);
    interiorHistograms.push_back(g_hist_interior_no_zeros_f);
    interiorHistograms.push_back(b_hist_interior_no_zeros_f);

}








vector<string> Utils::haralickDescriptors(int x, int y, vector<Mat> src){
    vector<string> lines;
    for (int var = 0; var < src.size(); var++) {
        Mat gray;
        cvtColor(src.at(var), gray, CV_BGR2GRAY);

        //criar matriz de coocorrencia normalizada
        Utils u;
        u.coocurrenceMatrix(x,y,gray);

        //extrair descritores
        string line = u.descriptors();

        //cout << "line "<<var<< ": "<<line<<endl;


        lines.push_back(line);
    }
    return lines;
}

void Utils::coocurrenceMatrix(int x, int y, Mat img){
    Size s = img.size();
    int sum = 0;
    int matrix[TAM][TAM];

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            matrix[i][j] = 0;
            normalized_matrix[i][j] = 0.0;
        }
    }
    for (int i = abs(x); i < s.height - abs(x); i++) {
        for (int j = abs(y); j < s.width - abs(y); j++) {
            int value_img = img.at<uchar>(i, j);
            int displacement_value = img.at<uchar>(i + x, j + y);

            matrix[value_img][displacement_value] = matrix[value_img][displacement_value] + 1;
        }
    }
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            sum = sum + matrix[i][j];
        }
    }
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            normalized_matrix[i][j] = (float)((float)matrix[i][j] / (float)sum);
        }
    }
}

string Utils::descriptors(){
    Utils u;

    float sam_f = sam();
    float entropy_f = entropy();
    float contrast_f = contrast();
    float homogeneity_f = homogeneity();
    float correlation_f = correlation();


    string sam_result = u.toString(sam_f);
    string entropy_result = u.toString(entropy_f);
    string contrast_result = u.toString(contrast_f);
    string homogeneity_result = u.toString(homogeneity_f);
    string correlation_result = u.toString(correlation_f);

    string ret = "";
    ret.append(sam_result).append("\t").append(entropy_result).append("\t")
            .append(contrast_result).append("\t").append(homogeneity_result)
            .append("\t").append(correlation_result).append("\n");

    return ret;
}


string Utils::toString(float num){
    string ret = "";
    char number[200];

    sprintf(number, "%f", num);
    ret.append(number);

    return ret;
}

float Utils::sam(){
    float f_sam = 0.0;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            f_sam = f_sam + pow(normalized_matrix[i][j],2);
        }
    }
    return f_sam;
}

float Utils::entropy(){
    float f_entropy = 0.0;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if(normalized_matrix[i][j] != 0){
                f_entropy = f_entropy - (normalized_matrix[i][j] * log2(normalized_matrix[i][j]));
            }
        }
    }
    return f_entropy;
}

float Utils::contrast(){
    float f_contrast = 0.0;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            f_contrast = f_contrast + pow((i-j),2) * normalized_matrix[i][j];
        }
    }
    return f_contrast;
}

float Utils::homogeneity(){
    float f_homogeneity = 0.0;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            f_homogeneity = f_homogeneity + ((1/(1+pow(i-j,2))) * normalized_matrix[i][j]);
        }
    }
    return f_homogeneity;
}

float Utils::correlation(){
    float f_correlation = 0.0;
    float mr = 0.0;
    float mc = 0.0;
    float sigma_r = 0.0;
    float sigma_c = 0.0;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) { mr = mr + (float)i * normalized_matrix[i][j];}
    }
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) { mc = mc + (float)j * normalized_matrix[i][j];}
    }
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {sigma_r = sigma_r + pow((i-mr),2) * normalized_matrix[i][j];}
    }
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {sigma_c = sigma_c + pow((j-mc),2) * normalized_matrix[i][j];}
    }
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {f_correlation = f_correlation + (((i-mr)*(j-mc)*normalized_matrix[i][j])/(sqrt(sigma_r*sigma_c)));}
    }
    return f_correlation;
}






vector<vector<float> > Utils::lbpHistograms(vector<Mat> src, int index){
    vector<vector<float> > ret;
    setRandP(index);

    for (int var = 0; var < src.size(); var++) {
        int sum;
        Mat aux = src.at(var);
        Size size = aux.size();
        vector<Mat> channels;
        split(aux, channels);

        if(p == 8){
            vector<float> histogram_r(256, 0), histogram_g(256, 0), histogram_b(256, 0);
            for(int i = r; i < size.height - r; i = i + 3){
                for(int j = r; j < size.width - r; j = j + 3){
                    sum = 0;
                    sum = sum8(channels.at(0), i, j);
                    histogram_b.at(sum) = histogram_b.at(sum) + 1;

                    sum = 0;
                    sum = sum8(channels.at(1), i, j);
                    histogram_g.at(sum) = histogram_g.at(sum) + 1;

                    sum = 0;
                    sum = sum8(channels.at(2), i, j);
                    histogram_r.at(sum) = histogram_r.at(sum) + 1;
                }
            }
            ret.push_back(normalizeHist(histogram_r));
            ret.push_back(normalizeHist(histogram_g));
            ret.push_back(normalizeHist(histogram_b));
        }

        if(p == 12){
            vector<float> histogram_r(4096, 0), histogram_g(4096, 0), histogram_b(4096, 0);
            for(int i = r; i < size.height - r; i = i + 5){
                for(int j = r; j < size.width - r; j = j + 5){
                    sum = 0;
                    sum = sum12(channels.at(0), i, j);
                    histogram_b.at(sum) = histogram_b.at(sum) + 1;

                    sum = 0;
                    sum = sum12(channels.at(1), i, j);
                    histogram_g.at(sum) = histogram_g.at(sum) + 1;

                    sum = 0;
                    sum = sum12(channels.at(2), i, j);
                    histogram_r.at(sum) = histogram_r.at(sum) + 1;
                }
            }
            ret.push_back(normalizeHist(histogram_r));
            ret.push_back(normalizeHist(histogram_g));
            ret.push_back(normalizeHist(histogram_b));
        }

        if(p == 16){
            vector<float> histogram_r(65536, 0), histogram_g(65536, 0), histogram_b(65536, 0);
            for(int i = r; i < size.height - r; i = i + 9){
                for(int j = r; j < size.width - r; j = j + 9){
                    sum = 0;
                    sum = sum16(channels.at(0), i, j);
                    histogram_b.at(sum) = histogram_b.at(sum) + 1;

                    sum = 0;
                    sum = sum16(channels.at(1), i, j);
                    histogram_g.at(sum) = histogram_g.at(sum) + 1;

                    sum = 0;
                    sum = sum16(channels.at(2), i, j);
                    histogram_r.at(sum) = histogram_r.at(sum) + 1;
                }
            }
            ret.push_back(normalizeHist(histogram_r));
            ret.push_back(normalizeHist(histogram_g));
            ret.push_back(normalizeHist(histogram_b));
        }
    }

    return ret;
}

void Utils::setRandP(int index){
    switch (index) {
    case 1: //Radius 1 with 8 Points
        r = 1;
        p = 8;
        break;
    case 2://Radius 2 with 12 Points
        r = 2;
        p = 12;
        break;
    case 3://Radius 4 with 16 Points
        r = 4;
        p = 16;
        break;
    }
}

int Utils::verifyS(int gc, int gp){
    if(gp >= gc){
        return 1;
    } else {
        return 0;
    }
}

int Utils::sum8(Mat aux, int i, int j){
    int s, sum = 0, gc = aux.at<uchar>(i,j), gp;

    gp = aux.at<uchar>(i,j+1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,0);

    gp = aux.at<uchar>(i+1,j+1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,1);

    gp = aux.at<uchar>(i+1,j);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,2);

    gp = aux.at<uchar>(i+1,j-1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,3);

    gp = aux.at<uchar>(i,j-1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,4);

    gp = aux.at<uchar>(i-1,j-1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,5);

    gp = aux.at<uchar>(i-1,j);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,6);

    gp = aux.at<uchar>(i-1,j+1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,7);

    return sum;
}

int Utils::sum12(Mat aux, int i, int j){
    int s, sum = 0, gc = aux.at<uchar>(i,j), gp;

    gp = aux.at<uchar>(i,j+2);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,0);

    gp = aux.at<uchar>(i+1,j+2);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,1);

    gp = aux.at<uchar>(i+2,j+1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,2);

    gp = aux.at<uchar>(i+2,j);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,3);

    gp = aux.at<uchar>(i+2,j-1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,4);

    gp = aux.at<uchar>(i+1,j-2);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,5);

    gp = aux.at<uchar>(i,j-2);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,6);

    gp = aux.at<uchar>(i-1,j-2);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,7);

    gp = aux.at<uchar>(i-2,j-1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,8);

    gp = aux.at<uchar>(i-2,j);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,9);

    gp = aux.at<uchar>(i-2,j+1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,10);

    gp = aux.at<uchar>(i-1,j+2);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,11);

    return sum;
}

int Utils::sum16(Mat aux, int i, int j){
    int s, sum = 0, gc = aux.at<uchar>(i,j), gp;

    gp = aux.at<uchar>(i,j+4);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,0);

    gp = aux.at<uchar>(i+1,j+4);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,1);

    gp = aux.at<uchar>(i+3,j+3);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,2);

    gp = aux.at<uchar>(i+4,j+1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,3);

    gp = aux.at<uchar>(i+4,j);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,4);

    gp = aux.at<uchar>(i+4,j-1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,5);

    gp = aux.at<uchar>(i+3,j-3);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,6);

    gp = aux.at<uchar>(i+1,j-4);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,7);

    gp = aux.at<uchar>(i,j-4);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,8);

    gp = aux.at<uchar>(i-1,j-4);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,9);

    gp = aux.at<uchar>(i-3,j-3);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,10);

    gp = aux.at<uchar>(i-4,j-1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,11);

    gp = aux.at<uchar>(i-4,j);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,12);

    gp = aux.at<uchar>(i-4,j+1);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,13);

    gp = aux.at<uchar>(i-3,j+3);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,14);

    gp = aux.at<uchar>(i-1,j+4);
    s = verifyS(gc, gp);
    sum = sum + s*pow(2,15);

    return sum;
}

vector<string> Utils::txt2OPF(QStringList opf_files_path, QString opf_path){
    vector<string> ret;
    for (int  var = 0; var < opf_files_path.size(); var++) {
        string command = "cd ";

        command.append(opf_path.toUtf8().data()).append(PATH_SEPARATOR)
                .append("tools").append(PATH_SEPARATOR)
                .append(" && ./txt2opf ")
                .append(opf_files_path.at(var).toUtf8().data()).append(" ")
                .append(opf_files_path.at(var).toUtf8().data()).append(".dat");
        system(command.c_str());


        string aux = opf_files_path.at(var).toUtf8().data();
        aux.append(".dat");

        ret.push_back(aux);
    }
    return ret;
}


vector<vector<vector<vector<int> > > > Utils::myShuffle(QStringList opf_files_path, int training_percentage){
    float training_percent = (float)((float)training_percentage/100);
    vector<vector<vector<vector<int> > > > ret;

    todas_classes_separadas.clear();

    for (int t = 0; t < opf_files_path.size(); t++) {
        IO io;
        vector<string> file = io.openFile(opf_files_path.at(t).toUtf8().data());

        vector<string> nomes_classes;
        vector<string> aux;
        classes_separadas.clear();
        for (int var = 1; var < file.size(); var++) {
            stringstream ss(file.at(var));
            string item;
            std::getline(ss, item, '\t');
            std::getline(ss, item, '\t');

            if(var==1){
                nomes_classes.push_back(item);
                aux.push_back(file.at(var));
            }
            if(var == file.size()-1){
                aux.push_back(file.at(var));
                classes_separadas.push_back(aux);
            }
            if(strcmp(item.c_str(), nomes_classes.at(nomes_classes.size()-1).c_str()) != 0){
                nomes_classes.push_back(item);
                classes_separadas.push_back(aux);
                aux.clear();
                aux.push_back(file.at(var));
            } else{
                if(var!=1){
                    aux.push_back(file.at(var));
                }
            }
        }

        todas_classes_separadas.push_back(classes_separadas);

        vector<vector<int> > all_train, all_test;
        for (int var = 0; var < classes_separadas.size(); var++) {
            vector<int> training_file, test_file;
            vector<string> aux = classes_separadas.at(var);
            vector<int> indices;
            for (int i = 0; i < aux.size(); i++) {
                indices.push_back(i);
            }
            random_shuffle(indices.begin(), indices.end());
            for (int i = 0; i < round(aux.size()*training_percent); i++) {
                training_file.push_back(indices.at(i));
            }
            for (int i = round(aux.size()*training_percent); i < aux.size(); i++) {
                test_file.push_back(indices.at(i));
            }
            all_train.push_back(training_file);
            all_test.push_back(test_file);
        }

        vector<vector<vector<int> > > pret;
        pret.push_back(all_train);
        pret.push_back(all_test);

        ret.push_back(pret);
    }

    return ret;

}

void Utils::myOPFSplit(QStringList opf_files_path, QString opf_path, vector<vector<vector<vector<int> > > > indices){

    for (int var = 0; var < opf_files_path.size(); var++) {

        vector<vector<vector<int> > > indices_uma_classe = indices.at(var);

        vector<vector<string> > classe_separada = todas_classes_separadas.at(var);

        //passar para txt
        string command_toTxt = "cd ";
        command_toTxt.append(opf_path.toUtf8().data()).append(PATH_SEPARATOR)
                .append("tools ").append(" && ./opf2txt ")
                .append(opf_files_path.at(var).toUtf8().data()).append(" ")
                .append(opf_files_path.at(var).toUtf8().data()).append(".txt.temp");

        system(command_toTxt.c_str());

        string path_file_temp = opf_files_path.at(var).toUtf8().data();
        path_file_temp.append(".txt.temp");

        //abrir arquivo txt
        IO io;
        vector<string> fileTemp = io.openFile(path_file_temp);

        //pega informacao do cabecalho
        stringstream ss2(fileTemp.at(0));
        string item;
        vector<string> elems;
        while (std::getline(ss2, item, ' ')) {
            elems.push_back(item);
        }

        string v = elems.at(1);
        int n_classes = atoi(v.c_str());
        v = elems.at(2);
        int n_features = atoi(v.c_str());

        //split
        vector<string> training_file, test_file;
        vector<vector<int> > indices_treinamento = indices_uma_classe.at(0);
        vector<vector<int> > indices_teste = indices_uma_classe.at(1);

        for (int i = 0; i < indices_treinamento.size(); i++) {
            vector<string> uma_classe = classe_separada.at(i);
            vector<int> indices_treinamento_uma_classe = indices_treinamento.at(i);
            for (int j = 0; j < indices_treinamento_uma_classe.size(); j++) {
                training_file.push_back(uma_classe.at(indices_treinamento_uma_classe.at(j)));
            }
        }

        for (int i = 0; i < indices_teste.size(); i++) {
            vector<string> uma_classe = classe_separada.at(i);
            vector<int> indices_teste_uma_classe = indices_teste.at(i);
            for (int j = 0; j < indices_teste_uma_classe.size(); j++) {
                test_file.push_back(uma_classe.at(indices_teste_uma_classe.at(j)));
            }
        }

        training_file = correctsNumbering(training_file);
        test_file = correctsNumbering(test_file);

        //cria os arquivos
        string header = createOPFHeader(training_file.size(), n_classes, n_features);
        bool saved = IO::saveTXT_temp(opf_files_path.at(var).toUtf8().data(), header,training_file, "training.txt");

        header = createOPFHeader(test_file.size(), n_classes, n_features);
        saved = IO::saveTXT_temp(opf_files_path.at(var).toUtf8().data(), header, test_file, "testing.txt");

        //converter training e testing para dat
        QStringList files;
        QString path_training = opf_files_path.at(var);
        path_training.append("_").append("training.txt");
        files.push_back(path_training);

        QString path_testing = opf_files_path.at(var);
        path_testing.append("_").append("testing.txt");
        files.push_back(path_testing);

        vector<string> new_files_paths = txt2OPF(files, opf_path);

        //apagar arquivos txt
        string rm;
        rm = "rm ";
        rm.append(path_training.toUtf8().data());
        system(rm.c_str());

        rm = "rm ";
        rm.append(path_testing.toUtf8().data());
        system(rm.c_str());

        rm = "rm ";
        rm.append(path_file_temp);
        system(rm.c_str());

        //rename
        string to = "";
        to.append(opf_files_path.at(var).toUtf8().data()).append("_files").append(PATH_SEPARATOR);

        QDir().mkdir(to.c_str());

        to.append("training.dat");

        string rn;
        rn = "mv ";
        rn.append(new_files_paths.at(0)).append(" ").append(to);

        system(rn.c_str());

        to = "";
        to.append(opf_files_path.at(var).toUtf8().data()).append("_files").append(PATH_SEPARATOR);
        to.append("testing.dat");

        rn = "mv ";
        rn.append(new_files_paths.at(1)).append(" ").append(to);

        system(rn.c_str());

        string command_txt = "cd ";
        command_txt.append(opf_path.toUtf8().data()).append(PATH_SEPARATOR)
                .append("tools ").append(" && ./opf2txt ")
                .append(opf_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("testing.dat ")
                .append(opf_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("testing.txt");
        system(command_txt.c_str());
    }
}

void Utils::myOPFTrain(QStringList opf_files_path, QString opf_path){
    for (int var = 0; var < opf_files_path.size(); var++) {
        string command_train = "cd ";
        command_train.append(opf_path.toUtf8().data()).append(PATH_SEPARATOR)
                .append("bin").append(PATH_SEPARATOR).append(" && ./opf_train ")
                .append(opf_files_path.at(var).toUtf8().data()).append("_files").append(PATH_SEPARATOR)
                .append("training.dat");
        system(command_train.c_str());

        string aux_path = opf_path.toUtf8().data();
        aux_path.append(PATH_SEPARATOR).append("bin");
        mvDAT(aux_path, opf_files_path.at(var).toUtf8().data(),"_files", 't');
    }
}

void Utils::myOPFClassify(QStringList opf_files_path, QString opf_path){
    for (int var = 0; var < opf_files_path.size(); var++) {
        string cp_s = (opf_files_path.at(var).toUtf8().data());
        cp_s.append("_files").append(PATH_SEPARATOR).append("classifier.opf");

        string aux_path = opf_path.toUtf8().data();
        aux_path.append(PATH_SEPARATOR).append("bin");

        cp(cp_s, aux_path);

        string command_classify = "cd ";
        command_classify.append(opf_path.toUtf8().data()).append(PATH_SEPARATOR)
                .append("bin").append(PATH_SEPARATOR).append(" && ./opf_classify ")
                .append(opf_files_path.at(var).toUtf8().data()).append("_files").append(PATH_SEPARATOR)
                .append("testing.dat");

        system(command_classify.c_str());
        mvDAT(aux_path, opf_files_path.at(var).toUtf8().data(),"_files", 's');
        string del_s = (opf_path.toUtf8().data());
        del_s.append(PATH_SEPARATOR).append("bin");
        del(del_s, "classifier.opf");
    }
}

void Utils::myOPFAccuracy(QStringList opf_files_path, QString opf_path){
    for (int var = 0; var < opf_files_path.size(); var++) {
        string command_accuracy = "cd ";
        command_accuracy.append(opf_path.toUtf8().data()).append(PATH_SEPARATOR)
                .append("bin").append(PATH_SEPARATOR).append(" && ./opf_accuracy ")
                .append(opf_files_path.at(var).toUtf8().data())
                .append("_files").append(PATH_SEPARATOR)
                .append("testing.dat");

        system(command_accuracy.c_str());
    }
}

void Utils::myOPFSaveIterations(QStringList opf_files_path, int i){
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
        cpFolders(from, to);
    }
}


vector<string> Utils::iterationsPaths(QStringList files_path, int n_of_iterations){
    vector<string> ret;
    for (int i = 0; i < n_of_iterations; i++) {
        for (int j = 0; j < files_path.size(); j++) {
            //copia todo o conteudo para uma pasta apos cada iteracao
            string from = files_path.at(j).toUtf8().data();
            from.append("_files").append(PATH_SEPARATOR);
            char number[5];
            sprintf(number, "%d", i);
            string it = number;
            string to = files_path.at(j).toUtf8().data();
            to.append("_iterations").append(PATH_SEPARATOR);
            to.append(it.c_str());
            ret.push_back(to);
        }
    }
    return ret;
}







void Utils::cp(string from, string to){
    string command = "cp "+from+" "+to;
    system(command.c_str());
}


void Utils::del(string fileName, string file){
    string command = "rm ";
    fileName.append(PATH_SEPARATOR).append(file);
    command.append(fileName);
    system(command.c_str());
}

void Utils::mvDAT(string from, string to, string folder, char type){
    to.append(folder);

    from.append(PATH_SEPARATOR);//esse eh o caminho total ate a pasta
    to.append(PATH_SEPARATOR);

    QString dir_to(to.c_str());
    QDir().mkdir(dir_to);

    string s = "cd ";
    s.append(from).append(" && ");

    //opf
    if(type == 's'){//split
        string f = "mv *.dat ";
        f.append(to);
        s.append(f);
        system(s.c_str());
    }
    if(type == 't'){//train
        string f = "mv *.opf ";
        f.append(to);
        s.append(f);
        system(s.c_str());
    }
    if(type == 'l'){//learn (no train proccess necessary
        string f = "mv *.time *.opf ";
        f.append(to);
        s.append(f);
        system(s.c_str());
    }
    if(type == 'd'){//distances
        string f = "mv distances.dat ";
        f.append(to);
        s.append(f);
        system(s.c_str());
    }
    if(type == 'c'){//classify
        string f = "mv *.time *.out ";
        f.append(to);
        s.append(f);
        system(s.c_str());
    }

    //utilizados s, t, l, d c
    //svm p,r
    if(type == 'p'){//split svm
        string f = "mv *.svm ";
        f.append(to);
        s.append(f);
        system(s.c_str());
    }
    if(type == 'r'){//grid
        string f = "mv *.png ";
        f.append(to);
        s.append(f);
        system(s.c_str());
    }
    if(type == 'a'){//train
        string f = "mv *.model ";
        f.append(to);
        s.append(f);
        system(s.c_str());
    }
}

float Utils::calcAVG(vector<string> src){
    float sum = 0.0;
    for (int  var = 0; var < n_iterations; var++) {
        string value = src.at(var);
        sum = sum + atof(value.c_str());
    }
    return (sum/n_iterations);
}


float Utils::calcDP(vector<string> src, float avg){
    float dp = 0.0;
    for (int  var = 0; var < n_iterations; var++) {
        string value = src.at(var);
        float value_f = atof(value.c_str());
        dp = dp + pow((value_f - avg), 2);
    }
    return sqrt(dp/n_iterations);
}


vector<vector<float> > Utils::calcResults(QStringList opf_files_path, string classifier){
    vector<vector<float> > result_vector;
    for (int var = 0; var < opf_files_path.size(); var++) {

        string full_path = opf_files_path.at(var).toUtf8().data();
        full_path.append("_files").append(PATH_SEPARATOR);

        string aux = full_path;
        if(strcmp(classifier.c_str(),"opf")==0) aux.append("testing.dat.time");
        if(strcmp(classifier.c_str(),"svm")==0) aux.append("testing.time");
        vector<string> testing_data_time = IO::openFile(aux);

        aux = full_path;
        if(strcmp(classifier.c_str(),"opf")==0) aux.append("training.dat.time");
        if(strcmp(classifier.c_str(),"svm")==0) aux.append("training.time");
        vector<string> training_data_time = IO::openFile(aux);

        aux = full_path;
        if(strcmp(classifier.c_str(),"opf")==0) aux.append("testing.dat.acc");
        if(strcmp(classifier.c_str(),"svm")==0) aux.append("accuracy.txt");
        vector<string> accuracy = IO::openFile(aux);

        vector<float> items;

        //acurácia média
        float avg_accuracy = calcAVG(accuracy);
        items.push_back(avg_accuracy);

        //tempo medio de treinamento
        float avg_training_data_time = calcAVG(training_data_time);
        items.push_back(avg_training_data_time);

        //tempo medio de classificacao
        float avg_testing_data_time = calcAVG(testing_data_time);
        items.push_back(avg_testing_data_time);

        //desvios-padrao
        float dp_accuracy = calcDP(accuracy, avg_accuracy);
        items.push_back(dp_accuracy);

        float dp_training_data_time = calcDP(training_data_time, avg_training_data_time);
        items.push_back(dp_training_data_time);

        float dp_testing_data_time = calcDP(testing_data_time, avg_testing_data_time);
        items.push_back(dp_testing_data_time);

        result_vector.push_back(items);
    }

    return result_vector;
}

void Utils::txt2SVM(const char *filenamein, const char *filenameout) {
    int auxnImages, auxnClasses, auxnFeatures, auxclasse;
    int i, j, descart;
    double auxfeat;
    FILE *fin, *fout;
    size_t result;

    fin = fopen(filenamein, "r");
    if (fin == NULL) {
        fprintf(stderr, "Error - could not read from file %s.\n", filenamein);
        exit(-1);
    }
    // 134	2	192
    result = fscanf(fin, "%d", &auxnImages);
    result = fscanf(fin, "%d", &auxnClasses);
    result = fscanf(fin, "%d", &auxnFeatures);

    fout = fopen(filenameout, "w");
    if (fout == NULL) {
        fprintf(stderr, "Error - could not read from file %s.\n", filenameout);
        exit(-1);
    }

    for (i = 0; i < auxnImages; i++) {
        result = fscanf(fin, "%d", &descart);//numero img
        result = fscanf(fin, "%d", &auxclasse);
        fprintf(fout, "%d ", auxclasse);
        for (j = 0; j < auxnFeatures; j++) {
            result = fscanf(fin, "%lf", &auxfeat);  // cast - double
            fprintf(fout, "%d", (j+1));
            fprintf(fout, ":");
            fprintf(fout, "%f ", auxfeat);
        }
        fprintf(fout, "\n");
    }
    fclose(fin);
    fclose(fout);
    fprintf(stdout, "\nConversão opf2svm realizada com sucesso!\tArquivo convertido com sucesso!\n");
    fflush(stdout);
}

void Utils::dat2SVM(QStringList opf_files_path, QStringList svm_files_path, QString opf_path){

    for (int var = 0; var < opf_files_path.size(); var++) {
        string dir = svm_files_path.at(var).toUtf8().data();
        dir.append("_files").append(PATH_SEPARATOR);
        QDir().mkdir(dir.c_str());

        //copia para a pasta svm
        string command_txt = "cd ";
        command_txt.append(opf_path.toUtf8().data()).append(PATH_SEPARATOR)
                .append("tools ").append(" && ./opf2txt ")
                .append(opf_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("testing.dat ")
                .append(svm_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("testing.txt && ./opf2txt ")

                .append(opf_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("training.dat ")
                .append(svm_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("training.txt");

        system(command_txt.c_str());

        //copia para a pasta da opf
        command_txt = "cd ";
        command_txt.append(opf_path.toUtf8().data()).append(PATH_SEPARATOR)
                .append("tools ").append(" && ./opf2txt ")
                .append(opf_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("testing.dat ")
                .append(opf_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("testing.txt && ./opf2txt ")

                .append(opf_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("training.dat ")
                .append(opf_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("training.txt");

        system(command_txt.c_str());

        string in_testing = "";
        in_testing.append(svm_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("testing.txt");

        string out_testing = "";
        out_testing.append(svm_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("testing.svm");

        string in_training = "";
        in_training.append(svm_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("training.txt");

        string out_training = "";
        out_training.append(svm_files_path.at(var).toUtf8().data()).append("_files")
                .append(PATH_SEPARATOR).append("training.svm");


        txt2SVM(in_testing.c_str(), out_testing.c_str());
        txt2SVM(in_training.c_str(), out_training.c_str());
    }
}

void Utils::mySVMTrain(QString svm_path, QString svm_file_path, vector<string> c_and_g, bool grid){
    string command_train = "cd ";

    clock_t tInicio, tFim;
    double tDecorrido;

    if(grid){
        command_train.append(svm_path.toUtf8().data()).append(PATH_SEPARATOR).append(" && ./svm-train -g ")
                .append(c_and_g.at(1)).append(" -c ").append(c_and_g.at(0)).append(" ")
                .append(svm_file_path.toUtf8().data()).append("_files").append(PATH_SEPARATOR)
                .append("training.svm");
    } else {
        command_train.append(svm_path.toUtf8().data()).append(PATH_SEPARATOR).append(" && ./svm-train ")
                .append(svm_file_path.toUtf8().data()).append("_files").append(PATH_SEPARATOR)
                .append("training.svm");
    }

    tInicio = clock();
    system(command_train.c_str());
    tFim = clock();

    string aux_path = svm_path.toUtf8().data();
    aux_path.append(PATH_SEPARATOR);
    mvDAT(aux_path, svm_file_path.toUtf8().data(),"_files", 'a');

    string file_name = svm_file_path.toUtf8().data();
    file_name.append("_files").append(PATH_SEPARATOR).append("training").append(".time");

    tDecorrido = ((double)(tFim - tInicio) / CLOCKS_PER_SEC ); //resultados
    IO::saveTime(tDecorrido, file_name);
}

void Utils::mySVMClassify(QString svm_path, QString svm_file_path){

    clock_t tInicio, tFim;
    double tDecorrido;

    string command_classify = "cd ";
    command_classify.append(svm_path.toUtf8().data()).append(PATH_SEPARATOR)
            .append(" && ./svm-predict ")
            .append(svm_file_path.toUtf8().data()).append("_files").append(PATH_SEPARATOR)
            .append("testing.svm ")
            .append(svm_file_path.toUtf8().data()).append("_files").append(PATH_SEPARATOR)
            .append("training.svm.model ")
            .append(svm_file_path.toUtf8().data()).append("_files").append(PATH_SEPARATOR)
            .append("output_file.txt");

    command_classify.append(" > ").append(svm_file_path.toUtf8().data()).append("_files")
            .append(PATH_SEPARATOR).append("acc_apagar.txt");

    tInicio = clock();
    system(command_classify.c_str());
    tFim = clock();

    string file_name = svm_file_path.toUtf8().data();
    file_name.append("_files").append(PATH_SEPARATOR).append("testing").append(".time");

    tDecorrido = ((double)(tFim - tInicio) / CLOCKS_PER_SEC ); //resultados
    IO::saveTime(tDecorrido, file_name);
}

void Utils::mySVMAccuracy(QString svm_file_path){
    //pegar acuracia do file
    string path_del = svm_file_path.toUtf8().data();
    path_del.append("_files").append(PATH_SEPARATOR).append("acc_apagar.txt");

    vector<string> acc = IO::openFile(path_del);

    vector<string> values;
    for (int var = 0; var < acc.size(); var++) {
        stringstream ss(acc.at(var));
        string item;
        vector<string> elems;
        while (std::getline(ss, item, '=')) {
            elems.push_back(item);
        }
        for (size_t j = 0; j < elems.size(); j++) {
            stringstream ss2(elems.at(j));
            while (std::getline(ss2, item, '%')) {
                values.push_back(item);
            }
        }
    }

    string path = svm_file_path.toUtf8().data();
    path.append("_files").append(PATH_SEPARATOR).append("accuracy.txt");

    saveAccuracySVM(path, values.at(1));

    del(path_del);
}

bool Utils::saveAccuracySVM(string svm_file_path, string line){
    string file_name = svm_file_path;

    bool saved = true;
    FILE *txt = NULL;
    txt = fopen(file_name.c_str(), "a");
    int result;

    if (txt == NULL){
        printf("Problemas na CRIACAO do arquivo TXT\n");
        return false;
    }

    line.append("\n");
    result = fprintf(txt, line.c_str());
    if (result == EOF){
        printf("Erro na Gravacao\n");
        saved = false;
    }

    fclose(txt);
    return saved;
}

void Utils::del(string filePath){
    string command = "rm ";
    command.append(filePath);
    system(command.c_str());
}

void Utils::cpFolders(string from, string to){
    string command = "cd ";
    command.append(from).append(" && cp -f *.* ").append(to);
    system(command.c_str());
}

void Utils::mySVMSplit(QString svm_path, QString svm_file_path, int training_percentage){
    string command_split = "cd ";
    command_split.append(svm_path.toUtf8().data()).append(PATH_SEPARATOR)
            .append("tools").append(PATH_SEPARATOR).append(" && python subset.py -s0 ")
            .append(svm_file_path.toUtf8().data()).append(" ");

    int size = IO::openFile(svm_file_path.toUtf8().data()).size();
    std::ostringstream s;
    s << round((size * ((float)training_percentage/100)));
    string training_percentage_s = s.str();

    command_split.append(training_percentage_s).append(" training.svm testing.svm");
    system(command_split.c_str());

    string aux_path = svm_path.toUtf8().data();
    aux_path.append(PATH_SEPARATOR).append("tools");
    mvDAT(aux_path, svm_file_path.toUtf8().data(),"_files", 'p');
}
