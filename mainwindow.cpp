#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "person_item_widget.h"

#include <cmath>

#include <QStandardItem>
#include <QSortFilterProxyModel>
#include <QDate>
#include <QVariant>
#include <QDataWidgetMapper>
#include <QDebug>

static int personColumnIndex = 0;
//static QMap<QString, ModelField> modelRowFields;
//static QMap<int, Person> dummyRemotePersons;
static QVector<Person> dummyRemotePersons;
//static QMap<int, Item*> personsToItemWidget;
const int kPersonsPerPage = 2;

static bool isDisconnected = false;

QString GetRandomString()
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   const int randomStringLength = 12; // assuming you want random strings of 12 characters

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}

PersonSortFilterProxyModel::PersonSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

void PersonSortFilterProxyModel::setFilterMinimumDate(const QDate &date)
{
    minDate = date;
    invalidateFilter();
}

void PersonSortFilterProxyModel::setFilterMaximumDate(const QDate &date)
{
    maxDate = date;
    invalidateFilter();
}

bool PersonSortFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    if(filterRegExp().pattern().isEmpty()) {
      return true;
    }

    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QVariant data = sourceModel()->data(index, static_cast<int>(filterRole())).value<QVariant>();

    Person item = qvariant_cast<Person>(data);

    /*qDebug() << "filterRegExp item " << item.name;
    qDebug() << "filterRegExp " << filterRegExp();
    qDebug() << "filterRegExp contains " << item.name.contains(filterRegExp());*/

    return item.name.contains(filterRegExp());
    // TODO: support regex
    //return filterRegExp().exactMatch(item.name);

    /*QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    //Person lItem = qvariant_cast<Person>(sourceModel()->data(left, static_cast<int>(UserRoles::PersonRole)).value<QVariant>());

    QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);
    qDebug() << "filterAcceptsRow" << index0.row() << index0.column();
    qDebug() << "filterAcceptsRow" << sourceModel()->data(index0).toString();
    qDebug() << "filterAcceptsRow" << sourceModel()->data(index1).toString();
    qDebug() << "filterAcceptsRow" << sourceModel()->data(index2).toString();
    qDebug() << "filterAcceptsRow" << data(index0, static_cast<int>(UserRoles::PersonRole)).value<QVariant>().toString();
    qDebug() << "filterAcceptsRow" << data(index1, static_cast<int>(UserRoles::PersonRole)).value<QVariant>().toString();
    qDebug() << "filterAcceptsRow" << data(index2, static_cast<int>(UserRoles::PersonRole)).value<QVariant>().toString();

    return (sourceModel()->data(index0).toString().contains(filterRegExp())
            || sourceModel()->data(index1).toString().contains(filterRegExp()))
            && dateInRange(sourceModel()->data(index2).toDate());*/
}

bool PersonSortFilterProxyModel::lessThan(const QModelIndex &left,
                                      const QModelIndex &right) const
{
    Q_ASSERT( sourceModel() != nullptr );

    if( (!left.isValid()) || (!right.isValid()) ){
      return QSortFilterProxyModel::lessThan(left, right);
    }

    //QVariant leftData = sourceModel()->data(left);
    //QVariant rightData = sourceModel()->data(right);
    QVariant leftData = sourceModel()->data(left, static_cast<int>(sortRole())).value<QVariant>();
    QVariant rightData = sourceModel()->data(right, static_cast<int>(sortRole())).value<QVariant>();

    Person lItem = qvariant_cast<Person>(leftData);
    Person rItem = qvariant_cast<Person>(rightData);

    /*qDebug() << "lItem " << lItem.name;
    qDebug() << "rItem " << rItem.name;*/

    //const auto attributes = mColumnSortStringAttributesList.attributesForColumn(column);

    if(isSortLocaleAware())
      return QString::localeAwareCompare(lItem.name, rItem.name) < 0;

    return QString::compare(lItem.name, rItem.name) < 0;

    /*qDebug() << "lessThan" << left.row() << left.column();
    qDebug() << "lessThan" << right.row() << right.column();

    qDebug() << "lessThan" << leftData.toString() << rightData.toString();

    qDebug() << "lessThan" << sourceModel()->data(left).toString();
    qDebug() << "lessThan" << sourceModel()->data(right).toString();

    //const QModelIndex index = filterModel->index(row, i);
    qDebug() << "lessThan sourceModel" << sourceModel()->data(left, static_cast<int>(UserRoles::PersonRole));
    qDebug() << "lessThan sourceModel" << sourceModel()->data(right, static_cast<int>(UserRoles::PersonRole)).value<QVariant>();
{Person person = qvariant_cast<Person>(sourceModel()->data(left, static_cast<int>(UserRoles::PersonRole)).value<QVariant>());
    qDebug() << "person " << person.name;}
{Person person = qvariant_cast<Person>(sourceModel()->data(right, static_cast<int>(UserRoles::PersonRole)).value<QVariant>());
    qDebug() << "person " << person.name;}
    if (leftData.type() == QVariant::DateTime) {
        return leftData.toDateTime() < rightData.toDateTime();
    } else {
        static const QRegularExpression emailPattern("[\\w\\.]*@[\\w\\.]*");

        QString leftString = leftData.toString();
        if (left.column() == 1) {
            const QRegularExpressionMatch match = emailPattern.match(leftString);
            if (match.hasMatch())
                leftString = match.captured(0);
        }
        QString rightString = rightData.toString();
        if (right.column() == 1) {
            const QRegularExpressionMatch match = emailPattern.match(rightString);
            if (match.hasMatch())
                rightString = match.captured(0);
        }

        return QString::localeAwareCompare(leftString, rightString) < 0;
    }*/
}

bool PersonSortFilterProxyModel::dateInRange(const QDate &date) const
{
    return (!minDate.isValid() || date > minDate)
            && (!maxDate.isValid() || date < maxDate);
}

void setupDummyRemotePersons() {
  int totalPersons = 0;
  {
    Person Bob;
    Bob.name = "Bob";
    Bob.surname = "Ivanov";
    Bob.uid = totalPersons++;
    dummyRemotePersons.push_back(Bob);
    //dummyRemotePersons[Bob.uid] = Bob;

    {
      //Item* item = new  Item();
      //personsToItemWidget[Bob.uid] = item;
      //ui->wrapperLayout->addWidget(item);
      //item->setText("item1");
    }
  }

  {
    Person Alice;
    Alice.name = "Alice";
    Alice.surname = "Radionova";
    Alice.uid = totalPersons++;
    dummyRemotePersons.push_back(Alice);
    //dummyRemotePersons[Alice.uid] = Alice;
  }

  {
    Person Frank;
    Frank.name = "Frank";
    Frank.surname = "Petrovich";
    Frank.uid = totalPersons++;
    dummyRemotePersons.push_back(Frank);
    //dummyRemotePersons[Frank.uid] = Frank;
  }

  {
    Person Frank;
    Frank.name = "Frank";
    Frank.surname = "Obramovich";
    Frank.uid = totalPersons++;
    dummyRemotePersons.push_back(Frank);
    //dummyRemotePersons[Frank.uid] = Frank;
  }

  {
    Person Frank;
    Frank.name = "Frank";
    Frank.surname = "Potemkin";
    Frank.uid = totalPersons++;
    dummyRemotePersons.push_back(Frank);
    //dummyRemotePersons[Frank.uid] = Frank;
  }
}

QList<Person> retrieveRemotePersonsFiltered(const QString& filter) {
  QList<Person> dummyRemotePersonsFiltered;
  for (int i = 0; i < dummyRemotePersons.size(); i++) {
    // TODO: regex support
    const auto& person = dummyRemotePersons.at(i);
    if(!filter.isEmpty() && !filter.contains(person.name)) {
      //qDebug() << "skipped " << person.name;
      continue;
    } else {
      //qDebug() << "NOT skipped " << person.name;
    }
    dummyRemotePersonsFiltered.push_back(person);
  }
  return dummyRemotePersonsFiltered;
}

QPair<QList<Person>, int> retrieveRemotePersons(int page, int personsPerPage, const QString& filter) {
  QList<Person> dummyRemotePersonsPage;

  auto filtered = retrieveRemotePersonsFiltered(filter);

  int cursorI = page * personsPerPage;
  if (cursorI >= filtered.size()) {
    qDebug() << "not enough persons. Requested page " << page;
  }

  int availiblePersons = std::min(cursorI + personsPerPage, filtered.size());

  for (; cursorI < availiblePersons; cursorI++) {
    //filtered[cursorI].name = "Bao";//;GetRandomString();

    dummyRemotePersonsPage.push_back(filtered.at(cursorI));
  }
  return QPair<QList<Person>, int>(dummyRemotePersonsPage, filtered.size());
}

std::shared_ptr<fetchedPageData> MainWindow::fetchRemotePersonsToModel(int page, int personsPerPage, const QString& filter)
{
  std::shared_ptr<fetchedPageData> result = std::make_shared<fetchedPageData>();

  //int pageCount = fetchRemotePageCount(personsPerPage);

  int pageIndex = page;
  QPair<QList<Person>, int> personsPage = retrieveRemotePersons(pageIndex, personsPerPage, filter);
  int pageCount = personsPage.second;

  //Q_ASSERT(pageIndex < pageCount);
  if (pageIndex >= pageCount) {
    qDebug() << "nothing to show!";
    return result;
  }

  int modelRow = page * personsPerPage; // person index in page
  for ( Person& person : personsPage.first) {
    //qDebug() << "person.name person.name" << person.name;
    //QStandardItem *item = new QStandardItem(person.name);
    QStandardItem *item = new QStandardItem();
    item->setEditable(true);

    /*if (model->hasIndex(modelRow, personColumnIndex)) {
      person.name = "Frank";
      qDebug() << "true hasIndex";
    } else {
      qDebug() << "false hasIndex";
    }*/
    //person.name = GetRandomString();

    //item->setData("PersonName");
    /*Person person1;
    person1.name = GetRandomString();
    QVariant variant = QVariant::fromValue(person1);
    qDebug() << "variant " << variant.value<Person>().name;
    qDebug() << "variant2 " << variant2.value<Person>().name;*/
    QVariant variant = QVariant::fromValue(person);
    item->setData(QVariant::fromValue(person), static_cast<int>(UserRoles::PersonRole));
    //item->setData(variant);
    //model->setItem(pageIndex, personCulumnIndex, item);
    //model->setItem(pageIndex, personCulumnIndex, item);
    //if(model->rowCount() < modelRow) {
    if (model->hasIndex(modelRow, personColumnIndex)) {

      auto existingItem = model->item(modelRow);
      //person.name = "gg";
      existingItem->setData(QVariant::fromValue(person), static_cast<int>(UserRoles::PersonRole));

    //if(existingItem) {
      //model->setItem(modelRow, personColumnIndex, item);
      //model->removeRow(modelRow);
      //model->insertRow(modelRow);
      //model->setItem(modelRow, 0, item);

      // emit commitData();

      /*auto existingItem = model->takeItem(modelRow);
      if(existingItem) {
        existingItem->setData(QVariant::fromValue(person), static_cast<int>(UserRoles::PersonRole));
        model->setItem(modelRow, personColumnIndex, existingItem);
      } else {
        qDebug() << "no item at " << modelRow;
      }*/
      //model->dataChanged(QModelIndex(modelRow,),QModelIndex(),QVector<int>(static_cast<int>(UserRoles::PersonRole)));
      //qDebug() << "updated" << person.name << " to " << modelRow;
    } else {
      //qDebug() << "added" << person.name << " to " << modelRow;
      model->appendRow(item);
      /*QList<QStandardItem*> rowItems;
      rowItems.push_back(item);
      model->appendRow(rowItems);*/
      //model->setItem(modelRow, personColumnIndex, item);
    }
    //Item* personItemWidget = personsToItemWidget[person.uid];
    //QLineEdit* boundWidget = static_cast<QLineEdit*>(personItemWidget->getPersonNameWidget());
    //mapper->addMapping(boundWidget, modelRowFields["PersonName"].fieldId);
    //boundWidget->setText(person.name);
    //qDebug() << "PersonName name = " << person.name;
    //qDebug() << "PersonName fieldId = " << modelRowFields["PersonName"].fieldId;
    {
      //Q_ASSERT(modelRow < pageCount);
      modelRow++;
    }
  }
  //qDebug() << "QVariant::fromValue(personsPage.at(0)).toString() " << QVariant::fromValue(personsPage.at(0)).toString();

  //int columnid = personColumnIndex;
  //filterModel->sort(columnid, Qt::DescendingOrder);

  result->persons = personsPage.first;
  result->requestedPageNum = page;
  result->totalPages = pageCount;
  result->totalItems = pageCount * kPersonsPerPage;
  result->recievedPersonsNum = personsPage.first.size();
  qDebug() << "result->recievedPersonsNum " << result->recievedPersonsNum;
  result->requestedPageSize = kPersonsPerPage;

  return result;
}

/*int MainWindow::fetchRemotePageCount(int pageSize, filter) {
  //qDebug() << "dummyRemotePersons size = " << dummyRemotePersons.size();
  std::div_t res = std::div(dummyRemotePersons.size(), pageSize);
  // Fast ceiling of an integer division
  int pageCount = res.rem ? (res.quot + 1) : res.quot;
  //qDebug() << "pageCount = " << pageCount;
  return pageCount;
}*/

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
  setupDummyRemotePersons(); // TODO: use remote server

  ui->setupUi(this);

  int columsCount = 1; // person column
  int predefinedRowNum = 0; // we will append rows dynamically
  model = new QStandardItemModel(predefinedRowNum, columsCount, this);

  mapper = new QDataWidgetMapper(this);
  mapper->setModel(model);

  filterModel = new PersonSortFilterProxyModel();

  ///model->sort(0, Qt::AscendingOrder);
  //model->setSortRole(static_cast<int>(UserRoles::PersonRole));
  //model->sort(0, Qt::AscendingOrder);
  filterModel->setSourceModel(model);
  //filterModel->setFilterRole(MissionListModel::MissionNameRole);
  //filterModel->setSortRole(MissionListModel::MissionNameRole);
  filterModel->setFilterRole(static_cast<int>(UserRoles::PersonRole));
  filterModel->setSortRole(static_cast<int>(UserRoles::PersonRole));
  filterModel->setDynamicSortFilter(true);
  filterModel->setSortCaseSensitivity (Qt::CaseInsensitive);
  filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  //filterModel->
  //filterModel->setFilterFixedString("Bob");
  //filterModel->setFilterFixedString(QVariant::fromValue(personsPage.at(0)).toString());

  /*{
    QList<Person> personsPage = retrieveRemotePersons(0, kPersonsPerPage);
    for (auto& person : personsPage)
      qDebug() << "personsPage 0 = " << person.name;
  }
  {
    QList<Person> personsPage = retrieveRemotePersons(1, kPersonsPerPage);
    for (auto& person : personsPage)
      qDebug() << "personsPage 1 = " << person.name;
  }
  {
    QList<Person> personsPage = retrieveRemotePersons(2, kPersonsPerPage);
    for (auto& person : personsPage)
      qDebug() << "personsPage 2 = " << person.name;
  }*/

  /*model = new QStandardItemModel(kPersonsPerPage, modelRowFields.size(), this);
  mapper = new QDataWidgetMapper(this);*/

  /*int totalFields = 0;
  {
    ModelField field;
    field.fieldName = "PersonName";
    field.fieldId = totalFields++;
    modelRowFields[field.fieldName] = field;
  }

  qDebug() << "Persons size = " << dummyRemotePersons.size();
  model = new QStandardItemModel(dummyRemotePersons.size(), modelRowFields.size(), this);
  mapper = new QDataWidgetMapper(this);
  mapper->setModel(model);

  int modelRow = 0;
  for ( auto& person : dummyRemotePersons) {
    QStandardItem *item = new QStandardItem(person.name);
    //QStandardItem *item = new QStandardItem();
    //item->setData("PersonName");
    //item->setData(QVariant::fromValue(person), static_cast<int>(UserRoles::PersonRole));
    model->setItem(modelRow, modelRowFields["PersonName"].fieldId, item);
    //Item* personItemWidget = personsToItemWidget[person.uid];
    //QLineEdit* boundWidget = static_cast<QLineEdit*>(personItemWidget->getPersonNameWidget());
    //mapper->addMapping(boundWidget, modelRowFields["PersonName"].fieldId);
    //boundWidget->setText(person.name);
    qDebug() << "PersonName name = " << person.name;
    qDebug() << "PersonName fieldId = " << modelRowFields["PersonName"].fieldId;
    {
      Q_ASSERT(modelRow < dummyRemotePersons.size());
      modelRow++;
    }
  }*/

  //fetchRemotePersons(0, kPersonsPerPage);
  //fetchRemotePersons(1, kPersonsPerPage);

  connect(ui->searchButton, &QPushButton::clicked, [this]()
  {
    if (!isDisconnected) {
      // TODO: fetch from remote
      // TODO: clear old model cache
      model->clear();
      // NOTE: reset page to 0
      //fetchRemotePersons(0, kPersonsPerPage, ui->searchEdit->text());
      //fetchRemotePersonsToModel(0, kPersonsPerPage);
      //fetchRemotePersonsToModel(1, kPersonsPerPage);

      //model->sort(0, Qt::AscendingOrder);

      // TODO: filter param
      lastFetchedData = fetchRemotePersonsToModel(0, kPersonsPerPage, ui->searchEdit->text());
      qDebug()<<"lastFetchedData->recievedPersonsNum " << lastFetchedData->recievedPersonsNum;
      refreshPageWidgets(lastFetchedData);

      //qDebug()<<"clicked" << ui->searchEdit->text();
      filterModel->setFilterFixedString(ui->searchEdit->text());
      //filterModel->filterRole();

      mapper->toFirst(); // refresh
    } else {
      //qDebug()<<"clicked" << ui->searchEdit->text();
      filterModel->setFilterFixedString(ui->searchEdit->text());
      //filterModel->filterRole();

      // TODO: filter param
      lastFetchedData = fetchRemotePersonsToModel(0, kPersonsPerPage, "");
      refreshPageWidgets(lastFetchedData);

      Q_ASSERT(model->rowCount()); // setCurrentIndex will not work with empty model
      mapper->setCurrentIndex(mapper->currentIndex()); // refresh
    }
  });

  connect(ui->resetButton, &QPushButton::clicked, [this]()
  {
     //qDebug()<<"clicked" << ui->searchEdit->text();
     ui->searchEdit->setText("");
     filterModel->setFilterFixedString("");
     //filterModel->filterRole();

      // TODO: filter param
      lastFetchedData = fetchRemotePersonsToModel(0, kPersonsPerPage, "");
      refreshPageWidgets(lastFetchedData);

     Q_ASSERT(model->rowCount()); // setCurrentIndex will not work with empty model
     mapper->setCurrentIndex(mapper->currentIndex()); // refresh
  });

  //connect(ui->prevButton, &QAbstractButton::clicked, mapper, &QDataWidgetMapper::toPrevious);
  //connect(ui->nextButton, &QAbstractButton::clicked, mapper, &QDataWidgetMapper::toNext);

  connect(ui->prevButton, &QAbstractButton::clicked, [this]() {
    lastFetchedData = fetchRemotePersonsToModel(0, kPersonsPerPage, ui->searchEdit->text());
    mapper->toPrevious();
    //  refreshPageWidgets(lastFetchedData);
  });

  connect(ui->nextButton, &QAbstractButton::clicked, [this]() {
    lastFetchedData = fetchRemotePersonsToModel(0, kPersonsPerPage, ui->searchEdit->text());
    mapper->toNext();
    //  refreshPageWidgets(lastFetchedData);
  });

  connect(mapper, &QDataWidgetMapper::currentIndexChanged, this, &MainWindow::onMapperIndexChanged);

  if(isDisconnected) {
    /*// TODO: filter param
    lastFetchedData = fetchRemotePersonsToModel(0, kPersonsPerPage);
    refreshPageWidgets(lastFetchedData);
    mapper->toFirst();*/

    // TODO: reconnect signal
  } else {
    timer = new QTimer;
    timer->setSingleShot(true);
    // imitate remote load delay
    timer->start(1000);
    QObject::connect(timer, &QTimer::timeout, [this](){

        // TODO: filter param
        lastFetchedData = fetchRemotePersonsToModel(0, kPersonsPerPage, "");
        refreshPageWidgets(lastFetchedData);

        mapper->toFirst();
    });
  }
}

void MainWindow::refreshPageWidgets(std::shared_ptr<fetchedPageData> fetchedPageItems) {
  // remove all page widgets
  {
    QLayoutItem* item;
    while ( ( item = ui->wrapperLayout->takeAt( 0 ) ) != nullptr )
    {
        delete item->widget();
        delete item;
    }
  }

  if (!fetchedPageItems || !lastFetchedData->recievedPersonsNum) {
    qDebug() << "nothing to show";
    ui->prevButton->setEnabled(false);
    ui->nextButton->setEnabled(false);
    return;
  }

  int pageStart = fetchedPageItems->requestedPageNum * fetchedPageItems->requestedPageSize;

  int totalRetrievedItems = fetchedPageItems->recievedPersonsNum;//personsPage.size(); // TODO

  int totalAvailibleItems = std::min(totalRetrievedItems, fetchedPageItems->requestedPageSize);
  //for (int i = 0; i < fetchedPageItems->requestedPageSize; i++) {
  for (int i = 0; i < totalAvailibleItems; i++) {
    /*const QModelIndex index = model->index(row, i);
    QStandardItem* item = model->itemFromIndex(index);*/
    int itemRow = pageStart + i;

    //qDebug() << " model->rowCount"<< filterModel->rowCount();
    //qDebug() << "pageStart + i"<< itemRow;

    if (!filterModel->hasIndex(itemRow, personColumnIndex)) {
      continue;
    }
    const QModelIndex index = filterModel->index(itemRow, personColumnIndex);

    Person person = qvariant_cast<Person>(filterModel->data(index, static_cast<int>(UserRoles::PersonRole)).value<QVariant>());
    //qDebug() << person.name;
    /*QStandardItem* item = filterModel->data(index, static_cast<int>(UserRoles::PersonRole)).value<QStandardItem*>();
    if (item) {
      qDebug() << "got item";
      Person person = qvariant_cast<Person>(item->data(static_cast<int>(UserRoles::PersonRole)).value<QVariant>());
      qDebug() << person.name;
    }*/

    // recreate page widgets
    {
      PersonItemWidget* item = new PersonItemWidget();
      //item->setGeometry(0,0,500)
      //item->setFixedHeight(150);
      //item->setMinimumHeight(150);
      //item->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::MinimumExpanding);
      item->setName(person.name);
      item->setSurname(person.surname);
      ui->wrapperLayout->addWidget(item);
    }

  }
}

void MainWindow::onMapperIndexChanged(int pageNum) {
  qDebug() << "onMapperIndexChanged for page " << pageNum;

  if (lastFetchedData )
    qDebug() << "onMapperIndexChanged recievedPersonsNum " << lastFetchedData->recievedPersonsNum;

  refreshPageWidgets(lastFetchedData);

  if (!lastFetchedData || !lastFetchedData->recievedPersonsNum) {
    qDebug() << "nothing to show";
    //ui->prevButton->setEnabled(false);
    //ui->nextButton->setEnabled(false);
    return;
  }

  //int pageStart = lastFetchedData->requestedPageNum * lastFetchedData->requestedPageSize;
  //int totalPersons = filterModel->rowCount() - 1;
  //int totalPersons = lastFetchedData->totalPages * lastFetchedData->requestedPageSize;
  int totalPersons = lastFetchedData->totalItems;
  int recievedPersons = lastFetchedData->recievedPersonsNum;
  int pageStart = lastFetchedData->requestedPageNum * lastFetchedData->requestedPageSize;
  //ui->prevButton->setEnabled(pageStart > 0 && pageStart < totalPersons);
  //ui->nextButton->setEnabled((pageStart + lastFetchedData->requestedPageSize) < totalPersons);

  ui->prevButton->setEnabled(lastFetchedData->requestedPageNum > 0);
  ui->nextButton->setEnabled(lastFetchedData->requestedPageNum < lastFetchedData->totalPages);
}

MainWindow::~MainWindow()
{
  delete ui;
}
