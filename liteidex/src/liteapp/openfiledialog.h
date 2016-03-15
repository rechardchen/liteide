#ifndef OPENFILEDIALOG_H
#define OPENFILEDIALOG_H

#include <QDialog>
#include <QThread>

class QFileInfo;
class QTableWidgetItem;

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
    QString toFind;
    volatile bool finding;
};

namespace Ui {
class openfiledialog;
}


class openfiledialog : public QDialog
{
    Q_OBJECT

public:
    explicit openfiledialog(QWidget *parent = 0);
    ~openfiledialog();

    void init(const QStringList& rootPaths);
    QString getResult() const;

public slots:
    void reApplyFilter();
    void appendResult(const QFileInfo&);
    void selectFile(int,int);

protected:
    virtual void showEvent(QShowEvent*);

signals:
    void fileSelected(const QString&);

private:
    FileNameFinder* finder;
    Ui::openfiledialog *ui;
    QStringList rootPaths;
    QString result;
};

#endif // OPENFILEDIALOG_H
