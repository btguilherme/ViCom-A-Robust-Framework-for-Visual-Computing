#include "header.h"

Extractors::Extractors()
{

}

Extractors::~Extractors()
{

}

bool Extractors::gch(vector<vector<Mat> > imgs, vector<string> dirs, string directory){
    int n_samples = 0;
    vector<string> body;
    Utils utils;

    for (int var = 0; var < imgs.size(); var++) {
        vector<Mat> history = imgs.at(var);
        n_samples = n_samples + history.size();

        //4 representa 64 cores. Caso mude este paraâmetro, o 'for' da funcao 'colorHistogram'
        //tambem deverá mudar (o 'for' que retira os zeros dos vetores)
        vector<Mat> temp = utils.colorReduce(history, 4);

        //três (r,g,b) primeiros vector<int> pertencem a primeira imagem
        //os proximos três vector<int> pertencem a segunda imagem, e assim sucessivamente
        vector<vector<float> > histograms = utils.colorHistogram(temp, 'n');

        //cria o corpo do arquivo da OPF
        //segundo parametro '0' significa que nao sabemos a qual classe
        //pertence a imagem
        vector<string> aux = utils.createOPFBody(history.size(), dirs.at(var), histograms);

        if(var == 0){
            body = aux;
        }else{
            body.insert(body.end(), aux.begin(), aux.end());
        }
    }
    //correcao das numeraçoes
    body = utils.correctsNumbering(body);
    //cria o cabeçalho do arquivo da OPF
    string header = utils.createOPFHeader(n_samples, imgs.size(), 3*64);
    //salva num diretorio o arquivo da OPF
    QString dir(directory.c_str());
    bool saved = IO::saveTXT(dir, header,body,"gch");
    return saved;
}

bool Extractors::lch(vector<vector<Mat> > imgs, vector<string> dirs, string directory, int width, int height){
    int n_samples = 0;
    vector<string> body;
    int number_of_features;
    Utils utils;

    for (int  var = 0; var < imgs.size(); var++) {
        //vetor com imagens de uma pasta (classe)
        vector<Mat> oneClass = imgs.at(var);
        n_samples = n_samples + oneClass.size();

        //vector com fragmentos de todas as imgs (de uma pasta)
        vector<vector<Mat> > oneClassCroppedImages = utils.splitter(oneClass, width, height);
        vector<string> body_aux;
        for (int  i = 0; i < oneClassCroppedImages.size(); i++) {
            //para subimagens de cada imagem da classe...
            vector<Mat> oneImageCropped = oneClassCroppedImages.at(i);//4 subimagens
            vector<vector<float> > histograms = utils.colorHistogram(oneImageCropped, 'n');
            number_of_features = histograms.size();
            vector<string> aux = utils.createOPFBody(1, dirs.at(var), histograms);
            if(i == 0){
                body_aux = aux;
            }else{
                body_aux.insert(body_aux.end(), aux.begin(), aux.end());
            }
        }
        if(var == 0){
            body = body_aux;
        }else{
            body.insert(body.end(), body_aux.begin(), body_aux.end());
        }
    }
    //correcao das numeraçoes
    body = utils.correctsNumbering(body);
    //cria o cabeçalho do arquivo da OPF
    string header = utils.createOPFHeader(n_samples, imgs.size(), number_of_features*64);
    QString dir(directory.c_str());
    bool saved = IO::saveTXT(dir, header,body,"lch");
    return saved;
}

bool Extractors::cgch(vector<vector<Mat> > imgs, vector<string> dirs, string directory){

    int n_samples = 0;
    vector<string> body;
    Utils utils;

    for (int  var = 0; var < imgs.size(); var++) {
        vector<Mat> history = imgs.at(var);
        n_samples = n_samples + history.size();

        //4 representa 64 cores. Caso mude este paraâmetro, o 'for' da funcao 'colorHistogram'
        //tambem deverá mudar (o 'for' que retira os zeros dos vetores)
        vector<Mat>temp = utils.colorReduce(history, 4);

        //três (r,g,b) primeiros vector<int> pertencem a primeira imagem
        //os proximos três vector<int> pertencem a segunda imagem, e assim sucessivamente
        vector<vector<float> > histograms = utils.colorHistogram(temp, 'c');

        //cria o corpo do arquivo da OPF
        //segundo parametro '0' significa que nao sabemos a qual classe
        //pertence a imagem
        vector<string> aux = utils.createOPFBody(history.size(), dirs.at(var), histograms);

        if(var == 0){
            body = aux;
        }else{
            body.insert(body.end(), aux.begin(), aux.end());
        }
    }
    //correcao das numeraçoes
    body = utils.correctsNumbering(body);
    //cria o cabeçalho do arquivo da OPF
    string header = utils.createOPFHeader(n_samples, imgs.size(), 3*64);
    //salva num diretorio o arquivo da OPF
    QString dir(directory.c_str());
    bool saved = IO::saveTXT(dir, header,body,"cgch");
    return saved;
}

bool Extractors::bic(vector<vector<Mat> > imgs, vector<string> dirs, string directory){

    int n_samples = 0;
    vector<string> body;
    Utils utils;

    for (int var = 0; var < imgs.size(); var++) {
        vector<Mat> history = imgs.at(var);
        n_samples = n_samples + history.size();

        vector<Mat> temp = utils.colorReduce(history, 4);

        utils.classifyBorderInteriorPixels(temp);

        //histogramas separados em tres canais
        vector<vector<float> > border_histograms = utils.getBorderHistograms();
        vector<vector<float> > interior_histograms = utils.getInteriorHistograms();

        //insere os valores de interior_histograms no final do vetor border_histograms
        //resultando em um vetor com os valores dos histogramas das bordas + interior
        border_histograms.insert(border_histograms.end(), interior_histograms.begin(), interior_histograms.end());

        //cria o corpo do arquivo da OPF
        //segundo parametro '0' significa que nao sabemos a qual classe
        //pertence a imagem
        vector<string> aux = utils.createOPFBody(history.size(), dirs.at(var), border_histograms);

        if(var == 0){
            body = aux;
        }else{
            body.insert(body.end(), aux.begin(), aux.end());
        }
    }
    //correcao das numeraçoes
    body = utils.correctsNumbering(body);
    //cria o cabeçalho do arquivo da OPF
    string header = utils.createOPFHeader(n_samples, imgs.size(), 2*3*64);
    //salva num diretorio o arquivo da OPF
    QString dir(directory.c_str());
    bool saved = IO::saveTXT(dir, header,body,"bic");
    return saved;
}

bool Extractors::haralick(vector<vector<Mat> > imgs, vector<string> dirs, string directory, int x, int y){
    int n_samples = 0;
    vector<string> body;

    for (int var = 0; var < imgs.size(); var++) {
        vector<Mat> history = imgs.at(var);
        n_samples = n_samples + history.size();
        Utils utils;

        vector<string> content = utils.haralickDescriptors(x, y, history);

        //cria o corpo do arquivo da OPF
        vector<string> aux = utils.createOPFBody(history.size(), dirs.at(var), content);

        if(var == 0){
            body = aux;
        }else{
            body.insert(body.end(), aux.begin(), aux.end());
        }
    }
    Utils u;
    //correcao das numeraçoes
    body = u.correctsNumbering(body);
    //cria o cabeçalho do arquivo da OPF
    string header = u.createOPFHeader(n_samples, imgs.size(), 5);
    //salva num diretorio o arquivo da OPF
    QString dir(directory.c_str());
    bool saved = IO::saveTXT(dir, header,body,"haralick");
    return saved;
}

bool Extractors::lbp(vector<vector<Mat> > imgs, vector<string> dirs, string directory, int index){
    int n_samples = 0;
    vector<string> body;
    Utils utils;

    for (int var = 0; var < imgs.size(); var++) {
        vector<Mat> history = imgs.at(var);
        n_samples = n_samples + history.size();

        vector<vector<float> > histograms = utils.lbpHistograms(history, index);
        vector<string> aux = utils.createOPFBody(history.size(), dirs.at(var), histograms);
        if(var == 0){
            body = aux;
        }else{
            body.insert(body.end(), aux.begin(), aux.end());
        }
    }

    //correcao das numeraçoes
    body = utils.correctsNumbering(body);

    //cria o cabeçalho do arquivo da OPF
    string header;
    switch (index) {
    case 1: //Radius 1 with 8 Points
        //256
        header = utils.createOPFHeader(n_samples, imgs.size(), 3*256);
        break;
    case 2://Radius 2 with 12 Points
        //4096
        header = utils.createOPFHeader(n_samples, imgs.size(), 3*4096);
        break;
    case 3://Radius 4 with 16 Points
        //65536
        header = utils.createOPFHeader(n_samples, imgs.size(), 3*65536);
        break;
    }
    //salva num diretorio o arquivo da OPF
    QString dir(directory.c_str());
    bool saved = IO::saveTXT(dir, header,body,"lbp");
    return saved;
}
