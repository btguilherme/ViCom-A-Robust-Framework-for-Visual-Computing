#include "header.h"

IO::IO(){}
IO::~IO(){}

//getters
vector<string> IO::getFilesNames(){ return filesNames;}
int IO::getNumberOfSamples(){ return number_of_samples;}
vector<vector<Mat> > IO::getHistory(){ return history;}
string IO::getClassesDir(){ return classesDir;}
vector<string> IO::getDirs(){ return dirs;}
vector<string> IO::getFullPaths(){ return full_paths;}
vector<vector<string> > IO::getFiles(){ return files;}

//setter
void IO::setDirectoryClasses(QString directory){
    directory_classes = directory;
}

void IO::load(){
    dirs = IO::listNames(directory_classes.toUtf8().constData(), 'd');

    //p.ex : /home/guilherme/ALOI/
    string dir = directory_classes.toUtf8().constData();
    classesDir = dir;

    for (int var = 0; var < dirs.size(); var++) {
        string aux = dir;
        full_paths.push_back(aux.append(PATH_SEPARATOR).append(dirs.at(var)).append(PATH_SEPARATOR));
        history.push_back(IO::openDir(full_paths.at(var)));
        files.push_back(IO::getFilesNames());
    }

    number_of_samples = 0;
    for (int var = 0; var < full_paths.size(); var++) {
        vector<string> aux = IO::listNames(full_paths.at(var), 'f');
        number_of_samples = number_of_samples + aux.size();
    }
}

vector<string> IO::listNames(string path, char type){
    DIR *dir = 0;
    struct dirent *entrada = 0;
    unsigned char isDir = 0x4;
    unsigned char isFile = 0x8;

    vector<string> names;

    dir = opendir(path.c_str());

    if (dir == 0) {
        //std::cerr << "Nao foi possivel abrir diretorio." << std::endl;
        exit(1);
    }

    if(type == 'd'){
        while (entrada = readdir(dir)){
            if (entrada->d_type == isDir){
                if( strcmp(".", entrada->d_name) == 0 ||
                        strcmp("..", entrada->d_name) == 0 ||
                        strcmp("txt_dat_files", entrada->d_name) == 0 ){}else{
                    names.push_back(entrada->d_name);
                }
            }
        }
    } else if(type == 'f'){
        while (entrada = readdir(dir)){
            if (entrada->d_type == isFile){
                if( strcmp(".", entrada->d_name) == 0 || strcmp("..", entrada->d_name) == 0 ){}else{
                    names.push_back(entrada->d_name);
                }
            }
        }
    }
    closedir (dir);
    return names;
}

vector<Mat> IO::openDir(string directory){
    struct dirent **namelist;
    char *pch;
    int n;
    vector<Mat> ret;
    Mat src;
    Size size_img;

    n = scandir(directory.c_str(), &namelist, NULL, alphasort);
    if (n < 0){
        //QMessageBox::information(this,tr("Warning"),"Empty directory");
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.exec();
    } else {
        filesNames.clear();
        while(n--) {
            pch = strstr(namelist[n]->d_name,".png");
            if(pch == NULL){
                pch = strstr(namelist[n]->d_name,".jpg");
            }
            if(pch != NULL){
                char file[100];
                strcpy(file, directory.c_str());
                strcat(file,PATH_SEPARATOR);
                strcat(file, namelist[n]->d_name);

                filesNames.push_back(namelist[n]->d_name);

                src = imread(file, CV_LOAD_IMAGE_COLOR);
                size_img = src.size();
                ret.push_back(src);
                free(namelist[n]);
            }else{
                free(namelist[n]);
            }
        }
        free(namelist);
    }

    return ret;
}

bool IO::saveTXT(QString directory, string head, vector<string> body, string file_name){
    bool saved = true;
    //cria arquivo txt e dat no disco
    char file[400], file_txt[500];

    strcpy(file, directory.toUtf8());
    strcat(file,PATH_SEPARATOR);
    strcat(file, "txt_dat_files");
    QDir().mkdir(file);
    strcat(file,PATH_SEPARATOR);
    strcat(file, file_name.c_str());

    strcpy(file_txt, file);
    strcat(file_txt, ".txt");

    FILE *txt = NULL;

    txt = fopen(file_txt, "wt");
    if (txt == NULL){
        printf("Problemas na CRIACAO do arquivo TXT\n");
        return false;
    }

    int result;
    //grava o "head"
    result = fprintf(txt, head.c_str());
    if (result == EOF){
        printf("Erro na Gravacao\n");
        saved = false;
    }
    //grava o "body"
    for (int i = 0; i < body.size(); i++){
        string aux = body.at(i);
        result = fprintf(txt, aux.c_str());
        if (result == EOF){
            printf("Erro na Gravacao\n");
            saved = false;
        }
    }

    fclose(txt);
    return saved;
}

bool IO::saveTime(double tDecorrido, string path){
    bool saved = true;
    FILE *txt = NULL;
    //txt = fopen(file_name.c_str(), "wt");
    txt = fopen(path.c_str(), "a");
    int result;

    if (txt == NULL){
        printf("Problemas na CRIACAO do arquivo TXT\n");
        return false;
    }

    ostringstream sstream;
    sstream << tDecorrido;
    string aux = sstream.str();
    aux.append("\n");
    result = fprintf(txt, aux.c_str());
    if (result == EOF){
        printf("Erro na Gravacao\n");
        saved = false;
    }

    fclose(txt);
    return saved;
}

vector<string> IO::openFile(string file){
    FILE *arq;
    char info[99999];
    vector<string> ret;

    arq = fopen(file.c_str(), "r");
    if(arq == NULL){
        printf("Erro, nao foi possivel abrir o arquivo\n");
    }else{
        while( (fgets(info, sizeof(info), arq))!=NULL ){
            ret.push_back(info);
        }
    }
    fclose(arq);
    return ret;
}

bool IO::saveTXT_temp(string path, string head, vector<string> body, string file_name){
    bool saved = true;
    const char *file_txt;

    path.append("_").append(file_name);
    file_txt = path.c_str();

    FILE *txt = NULL;

    txt = fopen(file_txt, "wt");
    if (txt == NULL){
        printf("Problemas na CRIACAO do arquivo TXT\n");
        return false;
    }

    int result;
    //grava o "head"
    result = fprintf(txt, head.c_str());
    if (result == EOF){
        printf("Erro na Gravacao\n");
        saved = false;
    }
    //grava o "body"
    for (unsigned int i = 0; i < body.size(); i++){
        string aux = body.at(i);
        result = fprintf(txt, aux.c_str());
        if (result == EOF){
            printf("Erro na Gravacao\n");
            saved = false;
        }
    }

    fclose(txt);
    return saved;
}
