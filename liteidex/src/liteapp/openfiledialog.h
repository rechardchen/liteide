#ifndef OPENFILEDIALOG_H
#define OPENFILEDIALOG_H

#include <QDialog>

namespace Ui {
class openfiledialog;
}

class QSortFilterProxyModel;
class FolderListModel;
class openfiledialog : public QDialog
{
    Q_OBJECT

public:
    explicit openfiledialog(QWidget *parent = 0);
    ~openfiledialog();

    void setFolderViewModel(FolderListModel*);
public slots:
    void reApplyFilter();

private:
    QSortFilterProxyModel* m_proxy;
    Ui::openfiledialog *ui;
};

#endif // OPENFILEDIALOG_H
