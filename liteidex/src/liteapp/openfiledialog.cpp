#include <QDateTime>
#include <QThread>
#include <QDir>
#include "openfiledialog.h"
#include "ui_openfiledialog.h"

class FileNameFinder: public QThread
{
    Q_OBJECT
public:
    explicit FileNameFinder(QObject* parent);
    virtual void run();
    void stop();

    void setFind(const QString& find) { toFind = find; }
    void setFindPaths(const QStringList& l) { findPaths = l; }

private:
    void findDir(const QString&);

signals:
    void findResult(const QFileInfo&);

private:
    QStringList findPaths;
    QStringList nameFilters;
//    QRegExp reg;
    QString toFind;
    volatile bool finding;
};

FileNameFinder::FileNameFinder(QObject *parent)
    :QThread(parent)
    ,finding(false)
{
    nameFilters << "*.txt" << "*.go" << "*.cpp" << "*.c" << "*.h";
}

void FileNameFinder::run()
{
    if (finding)
        return;

    foreach (QString fp, findPaths) {
        findDir(fp);
        if (!finding)
            return;
    }
}

void FileNameFinder::stop()
{
    finding = false;
    if (this->isRunning()) {
        if (!this->wait(200))
            this->terminate();
    }
}

void FileNameFinder::findDir(const QString & path)
{
    QDir dir(path);
    if (!dir.exists())
        return;

    foreach (QFileInfo fileInfo, dir.entryInfoList(nameFilters, QDir::Files | QDir::NoSymLinks)) {
        if (fileInfo.fileName().indexOf(toFind) != -1)
            emit findResult(fileInfo);
        if (!finding)
            return;
    }

    foreach (QFileInfo dirInfo, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        findDir(dirInfo.filePath());
        if (!finding)
            return;
    }
}

openfiledialog::openfiledialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openfiledialog)
{
    ui->setupUi(this);

    finder = new FileNameFinder(this);

//    ui->typecombo->addItem(tr("Fixed string"), QRegExp::FixedString);
//    ui->typecombo->addItem(tr("Wildcard"), QRegExp::Wildcard);
//    ui->typecombo->addItem(tr("Regular expression"), QRegExp::RegExp);

    connect(ui->filename, SIGNAL(textChanged(QString)), this, SLOT(reApplyFilter()));
    connect(ui->typecombo, SIGNAL(currentIndexChanged(int)), this, SLOT(reApplyFilter()));

    connect(finder, SIGNAL(findResult(QFileInfo)), this, SLOT(appendResult(QFileInfo)));
}

openfiledialog::~openfiledialog()
{
    delete ui;
}

void openfiledialog::reApplyFilter()
{
    QString text = ui->filename->text().trimmed();
    if (text.length() > 0)
    {
//        QRegExp::PatternSyntax syntax = (QRegExp::PatternSyntax)ui->typecombo->itemData(ui->typecombo->currentIndex()).toInt();
//        QRegExp regExp(ui->filename->text(), Qt::CaseInsensitive, syntax);
        ui->filelist->clear();
        finder->stop();

        finder->setFind(text);
        finder->start();
    }
}

void openfiledialog::appendResult(const QFileInfo & fileInfo)
{
    //TODO
}


