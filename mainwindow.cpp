#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QtDebug>

const QString FOLDER;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    log("Welcome to Photo Assistance application.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::log(QString context)
{
    ui->logTE->append(context);
}

QImage MainWindow::resize(QString file, int percent)
{
    QImage img(file);

    int w = img.width();
    int h = img.height();
    double ratio = static_cast<double>(percent) / 100.00;

    int newWidth = w * ratio;
    int newHeight = h * ratio;
    QImage resizedImg = img.scaled(newWidth, newHeight, Qt::KeepAspectRatio);

    return resizedImg;
}

QString MainWindow::getTargetFolder()
{
    return target_folder;
}

void MainWindow::setTargetFolder(QString path)
{
    target_folder = path;
    emit targetFolderChanged();
}

void MainWindow::on_openBtn_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this,"Open Directory","/",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->targetFolderLE->setText(folder);
    setTargetFolder(folder);
    log("Target folder: \'"+folder+"\' selected.");
}

void MainWindow::on_runBtn_clicked()
{
    QDir directory(getTargetFolder());

    QStringList filters;
    filters << "*.jpg" << "*.JPG" << "*.jpeg" << "*.JPEG" << "*.png" << "*.PNG";
    directory.setNameFilters(filters);

    QStringList fileNames = directory.entryList(QDir::Files, QDir::Name);

    directory.mkdir("edited");

    foreach(QString fileName, fileNames)
    {
        log("The "+fileName+" is processing...");

        QImage file(directory.absoluteFilePath(fileName));
        qsizetype fileSize;
        while(true){
            fileSize = file.sizeInBytes();
            if(fileSize >= 818176){ // ~799kb
                file = resize(directory.absoluteFilePath(fileName), 90);
            }else if(fileSize <= 308224){ // 301kb
                file = resize(directory.absoluteFilePath(fileName), 110);
            }else{
                break;
            }
        }

        file.save(directory.absolutePath()+"/edited/"+fileName);

        log("The "+fileName+" processed.");
    }
}

