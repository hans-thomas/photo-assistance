#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QString folder READ getTargetFolder WRITE setTargetFolder NOTIFY targetFolderChanged)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void log(QString context);

    QString getTargetFolder();
    void setTargetFolder(QString path);

    QImage resize(QString file, int percent);

private slots:
    void on_openBtn_clicked();
    void on_runBtn_clicked();

private:
    Ui::MainWindow *ui;
    QString target_folder;

signals:
    void targetFolderChanged();
};
#endif // MAINWINDOW_H
