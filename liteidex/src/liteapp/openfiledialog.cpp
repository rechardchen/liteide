#include <QDateTime>
#include <QThread>
#include <QDir>
#include "openfiledialog.h"
#include "ui_openfiledialog.h"



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
    finding = true;

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
        if (fileInfo.fileName().indexOf(toFind, 0, Qt::CaseInsensitive) != -1)
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
    connect(ui->filelist, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(selectFile(int,int)));
}

openfiledialog::~openfiledialog()
{
    delete ui;
}

QString openfiledialog::getResult() const
{
    return result;
}

void openfiledialog::init(const QStringList& paths)
{
    rootPaths = paths;
    result = "";
}

void openfiledialog::reApplyFilter()
{
    QString text = ui->filename->text().trimmed();
    if (text.length() > 0)
    {
        finder->stop();
        ui->filelist->setRowCount(0);

        finder->setFind(text);
        finder->setFindPaths(rootPaths);
        finder->start();
    }
}

void openfiledialog::appendResult(const QFileInfo & fileInfo)
{
    int row = ui->filelist->rowCount();
    ui->filelist->insertRow(row);

    ui->filelist->setItem(row, 0, new QTableWidgetItem(fileInfo.fileName()));
    ui->filelist->setItem(row, 1, new QTableWidgetItem(fileInfo.path()));
    ui->filelist->setItem(row, 2, new QTableWidgetItem(fileInfo.lastModified().toString()));
}

void openfiledialog::selectFile(int r, int c)
{
    QString name = ui->filelist->item(r, 0)->text();
    QString path = ui->filelist->item(r, 1)->text();
    result = QDir::cleanPath(path+QDir::separator()+name);
    accept();
}

void openfiledialog::showEvent(QShowEvent *evt)
{
    QDialog::showEvent(evt);
    ui->filename->setFocus();
}


