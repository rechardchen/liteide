#include <QDateTime>
#include <QSortFilterProxyModel>
#include "folderview/folderlistmodel.h"
#include "openfiledialog.h"
#include "ui_openfiledialog.h"

class OpenFileFilter: public QSortFilterProxyModel
{
public:
    OpenFileFilter(QObject* parent): QSortFilterProxyModel(parent)
    {

    }

    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {
        FolderListModel *sm = qobject_cast<FolderListModel*>(sourceModel());
        QModelIndex index = sm->index(source_row, 0, source_parent);
        if (index.isValid())
        {
            QFileInfo fileInfo = sm->fileInfo(index);
            if (fileInfo.isFile())
                return true;
        }
        return false;
    }

    virtual int columnCount(const QModelIndex&) const
    {
        return 3; //fname, dir, modified date
    }

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return tr("File Name");
            case 1:
                return tr("Directory");
            case 2:
                return tr("Modified Date");
            }
        }
        return QVariant();
    }

    virtual QVariant data(const QModelIndex &index, int role) const
    {
        QVariant dat;

        if (role == Qt::DisplayRole)
        {
            int col = index.column();
            QModelIndex sIndex = mapToSource(index);
            FolderListModel *sm = qobject_cast<FolderListModel*>(sourceModel());
            QFileInfo fileInfo = sm->fileInfo(sIndex);
            switch (col)
            {
            case 0:
                dat = fileInfo.fileName();
                break;
            case 1:
                dat = fileInfo.absolutePath();
                break;
            case 2:
                dat = fileInfo.lastModified().toString();
                break;
            }
        }

        return dat;
    }

    QModelIndex mapToSource(const QModelIndex &proxyIndex) const
    {
        if (proxyIndex.column() > 1)
            return QSortFilterProxyModel::mapToSource(proxyIndex.sibling(proxyIndex.row(), 0));
        else
            return QSortFilterProxyModel::mapToSource(proxyIndex);
    }

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const
    {
        if (left.column() > 1 || right.column() > 1)
            return left.row() < right.row();
        return QSortFilterProxyModel::lessThan(left, right);
    }

    QModelIndex index(int row, int column, const QModelIndex &parent) const
    {
        if (column > 1 && parent.isValid())
            return createIndex(row, column, parent.child(row, 0).internalPointer());
        return index(row, column, parent);
    }

    QModelIndex parent(const QModelIndex &child) const
    {
        if (child.column() > 1 && child.isValid())
            return QSortFilterProxyModel::parent(createIndex(child.row(), 0, child.internalPointer()));
        return QSortFilterProxyModel::parent(child);
    }
};

openfiledialog::openfiledialog(QWidget *parent) :
    QDialog(parent),
    m_proxy(0),
    ui(new Ui::openfiledialog)
{
    ui->setupUi(this);

    ui->typecombo->addItem(tr("Fixed string"), QRegExp::FixedString);
    ui->typecombo->addItem(tr("Wildcard"), QRegExp::Wildcard);
    ui->typecombo->addItem(tr("Regular expression"), QRegExp::RegExp);
}

openfiledialog::~openfiledialog()
{
    delete ui;
}

void openfiledialog::setFolderViewModel(FolderListModel *model)
{
    m_proxy = new OpenFileFilter(this);
    m_proxy->setSourceModel(model);
    m_proxy->setFilterKeyColumn(0);

    ui->filelist->setModel(m_proxy);

    connect(ui->filename, SLOT(textChanged()), this, SLOT(reApplyFilter()));
    connect(ui->typecombo, SLOT(currentIndexChanged(int)), this, SLOT(reApplyFilter()));
}

void openfiledialog::reApplyFilter()
{
    QRegExp::PatternSyntax syntax = (QRegExp::PatternSyntax)ui->typecombo->itemData(ui->typecombo->currentIndex()).toInt();
    QRegExp regExp(ui->filename->text(), Qt::CaseInsensitive, syntax);
    m_proxy->setFilterRegExp(regExp);
}
