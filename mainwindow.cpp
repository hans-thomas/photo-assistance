#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QtDebug>
#include <QThread>

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

void MainWindow::ack()
{
    QTextEdit *myTextEdit = ui->logTE;
    myTextEdit->moveCursor (QTextCursor::End);
    myTextEdit->insertPlainText ("Done.");
}

QString MainWindow::resize(QString file, int percent)
{
    QImage img(file);

    int w = img.width();
    int h = img.height();
    double ratio = static_cast<double>(percent) / 100.00;

    int newWidth = w * ratio;
    int newHeight = h * ratio;
    QImage resizedImg = img.scaled(newWidth, newHeight, Qt::KeepAspectRatio);

    resizedImg.save(file,nullptr,100);

    return file;
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

    log("Processing started.");

    foreach(QString fileName, fileNames)
    {
        log("The "+fileName+" is processing...");

        QString path = directory.absolutePath()+"/edited/"+fileName;
        QFile::copy(directory.absoluteFilePath(fileName),path);
        qint64 fileSize;

        while(true){
            QFile file(path);

            fileSize = file.size();
            if(fileSize >= 818176){ // ~799kb
                path = resize(path, 90);
            }else if(fileSize <= 308224){ // 301kb
                path = resize(path, 110);
            }else{
                break;
            }

            QThread::msleep(100);  // Delay of 250 milliseconds
            file.close();
        }

        ack();
    }

    log("Processing finished.");
}

