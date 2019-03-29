#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDate>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QPair>

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

struct Person {
  // any unique id, without collisions
  int uid;

  QString name;

  QString surname;
};

// allow type usage with QVariant
Q_DECLARE_METATYPE(Person);


struct fetchedPageData {
  QList<Person> persons;
  int totalPages;
  int requestedPageSize;
  int totalItems;
  int requestedPageNum;
  int recievedPersonsNum;
};

enum class UserRoles
{
    CustomDataStart = Qt::UserRole,
    PersonRole
};

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

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  std::shared_ptr<fetchedPageData> fetchRemotePersonsToModel(int page, int personsPerPage, const QString& filter);
  int fetchRemotePageCount(int pageSize);
  void refreshPageWidgets(std::shared_ptr<fetchedPageData> fetchedPageItems);

public slots:
  void onMapperIndexChanged(int row);

private:
  Ui::MainWindow *ui;
  QStandardItemModel *model;
  QDataWidgetMapper *mapper;
  QSortFilterProxyModel* filterModel;
  QTimer* timer;
  std::shared_ptr<fetchedPageData> lastFetchedData;
};

#endif // MAINWINDOW_H
