#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "person.h"

#include <QMainWindow>
#include <QWidget>
#include <QDate>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QPair>
#include <QAbstractItemModel>
#include <QVector>
#include <QStandardItem>

#include <memory>

namespace Ui {
class MainWindow;
}

QT_BEGIN_NAMESPACE
class QDataWidgetMapper;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QStandardItemModel;
class QTextEdit;
class QSortFilterProxyModel;
QT_END_NAMESPACE

/*struct ModelField {
  QString fieldName;

  // must start from 0, without collisions
  int fieldId;
};*/


struct fetchedPageData {
  QList<Person> persons;
  int totalPages;
  int requestedPageSize;
  int totalItems;
  int requestedPageNum;
  int recievedPagePersonsNum;
};

/*enum class UserRoles
{
    CustomDataStart = Qt::UserRole,
    PersonRole
};*/

class PersonSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    PersonSortFilterProxyModel(QObject *parent = 0);

    QDate filterMinimumDate() const { return minDate; }
    void setFilterMinimumDate(const QDate &date);

    QDate filterMaximumDate() const { return maxDate; }
    void setFilterMaximumDate(const QDate &date);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    bool dateInRange(const QDate &date) const;

    QDate minDate;
    QDate maxDate;
};

enum class Columns  {
  Person = 0
  , PersonName
  , PersonSurname
  , TOTAL
};

QT_FORWARD_DECLARE_CLASS(QStandardItemModelPrivate)

class PersonsModel : public QStandardItemModel
{
Q_OBJECT
public:
    /*enum OptionID {
        PersonName,
    };*/

    enum Roles {
        NameRole = Qt::UserRole + 1
        , SurnameRole
        , TOTAL
        //, ModelRole
    };

    explicit PersonsModel(int rows, int columns, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

    //QVariant data1(const QModelIndex &index, int role) const;

    /*~PersonsModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool hasChildren(const QModelIndex &parent) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QStandardItem *item(int row, int column = 0) const;*/

    void addPerson(const QString &name);

    //QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

private:

    /*struct Node
    {
        Node(Node *parent = 0) : parent(parent), children(0) {}
        ~Node() { delete children; }
        Node *parent;
        QVector<Node> *children;
    };

    QStandardItem *node(int row, QStandardItem *parent) const;
    QStandardItem *parent(QStandardItem *child) const;
    int row(QStandardItem *node) const;

    QIcon services;
    int rc;
    int cc;
    QVector<QStandardItem> *tree;*/

    //QVector<Person> *tree;
    //QMap<QString, Person> personMap;
};

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  std::shared_ptr<fetchedPageData> fetchRemotePersonsToModel(int page, int personsPerPage, const QString& filter, const PersonsModel::Roles& filterRole);
  int fetchRemotePageCount(int pageSize);
  void refreshPageWidgets(std::shared_ptr<fetchedPageData> fetchedPageItems);

public slots:
  void onMapperIndexChanged(int row);

private:
  Ui::MainWindow *ui;
  //QStandardItemModel *model;
  PersonsModel *model;
  QDataWidgetMapper *mapper;
  QSortFilterProxyModel* filterModel;
  QTimer* timer;
  std::shared_ptr<fetchedPageData> lastFetchedData;
};

#endif // MAINWINDOW_H
