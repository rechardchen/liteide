#ifndef OPENFILEDIALOG_H
#define OPENFILEDIALOG_H

#include <QDialog>

namespace Ui {
class openfiledialog;
}

class FileNameFinder;
class openfiledialog : public QDialog
{
    Q_OBJECT

public:
    explicit openfiledialog(QWidget *parent = 0);
    ~openfiledialog();

public slots:
    void reApplyFilter();
    void appendResult(const QFileInfo&);

private:
    FileNameFinder* finder;
    Ui::openfiledialog *ui;
};

#endif // OPENFILEDIALOG_H
