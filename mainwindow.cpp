#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "person_item_widget.h"
#include "person_page_widget.h"

#include <cmath>

#include <QStandardItem>
#include <QSortFilterProxyModel>
#include <QDate>
#include <QVariant>
#include <QDataWidgetMapper>
#include <QDebug>

static int personColumnIndex = static_cast<int>(Columns::Person);
//static QMap<QString, ModelField> modelRowFields;
//static QMap<int, Person> dummyRemotePersons;
static QVector<Person> dummyRemotePersons;
//static QMap<int, Item*> personsToItemWidget;
static const int kPersonsPerPage = 2;

static PersonsModel::Roles filterRole = PersonsModel::Roles::NameRole;

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
/*
PersonsModel::PersonsModel(int rows, int columns, QObject *parent)
    : QAbstractItemModel(parent),
      services(QPixmap(":/images/services.png")),
      rc(rows), cc(columns),
      tree(new QVector<QStandardItem>(rows, QStandardItem(0)))
{

}

PersonsModel::~PersonsModel()
{
    delete tree;
}

QModelIndex PersonsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < rc && row >= 0 && column < cc && column >= 0) {
        QStandardItem *parentNode = static_cast<QStandardItem*>(parent.internalPointer());
        QStandardItem *childNode = node(row, parentNode);
        if (childNode)
            return createIndex(row, column, childNode);
    }
    return QModelIndex();
}

QModelIndex PersonsModel::parent(const QModelIndex &child) const
{
    if (child.isValid()) {
        QStandardItem *childNode = static_cast<QStandardItem*>(child.internalPointer());
        QStandardItem *parentNode = parent(childNode);
        if (parentNode)
            return createIndex(row(parentNode), 0, parentNode);
    }
    return QModelIndex();
}

int PersonsModel::rowCount(const QModelIndex &parent) const
{
    return (parent.isValid() && parent.column() != 0) ? 0 : rc;
}

int PersonsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return cc;
}

QVariant PersonsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role == Qt::DisplayRole)
        return QVariant("Item " + QString::number(index.row()) + ':' + QString::number(index.column()));
    //if (role == Qt::DecorationRole) {
    //    if (index.column() == 0)
    //        return iconProvider.icon(QFileIconProvider::Folder);
    //    return iconProvider.icon(QFileIconProvider::File);
    //}
    return QVariant();
}

QVariant PersonsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        return QString::number(section);
    if (role == Qt::DecorationRole)
        return QVariant::fromValue(services);
    return QAbstractItemModel::headerData(section, orientation, role);
}

bool PersonsModel::hasChildren(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return false;
    return rc > 0 && cc > 0;
}

Qt::ItemFlags PersonsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsDragEnabled|QAbstractItemModel::flags(index);
}

QStandardItem *PersonsModel::node(int row, QStandardItem *parent) const
{
    if (parent && !parent->children)
        parent->children = new QVector<QStandardItem>(rc, QStandardItem(parent));
    QVector<QStandardItem> *v = parent ? parent->children : tree;
    return const_cast<QStandardItem*>(&(v->at(row)));
}

PersonsModel::QStandardItem *PersonsModel::parent(QStandardItem *child) const
{
    return child ? child->parent : 0;
}

int PersonsModel::row(QStandardItem *node) const
{
    const QStandardItem *first = node->parent ? &(node->parent->children->at(0)) : &(tree->at(0));
    return node - first;
}*/

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

    QModelIndex index = sourceModel()->index(sourceRow, personColumnIndex, sourceParent);
    //QVariant data = sourceModel()->data(index, static_cast<int>(filterRole())).value<QVariant>();
    //QVariant data = sourceModel()->data(index, static_cast<int>(filterRole())).value<QVariant>();
    //Person item;
    //item.name = qvariant_cast<QString>(sourceModel()->data(index, static_cast<int>(PersonsModel::NameRole)).value<QVariant>());
    QString item = qvariant_cast<QString>(sourceModel()->data(index, static_cast<int>(filterRole())).value<QVariant>());

    //Person item = qvariant_cast<Person>(data);

    //qDebug() << "filterRegExp item " << item;
    /*qDebug() << "filterRegExp " << filterRegExp();
    qDebug() << "filterRegExp contains " << item.name.contains(filterRegExp());*/

    return item.contains(filterRegExp());
    // TODO: support regex
    //return filterRegExp().exactMatch(item.name);

    /*QModelIndex index0 = sourceModel()->index(sourceRow, personColumnIndex, sourceParent);
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
    /*QVariant leftData = sourceModel()->data(left, static_cast<int>(sortRole())).value<QVariant>();
    QVariant rightData = sourceModel()->data(right, static_cast<int>(sortRole())).value<QVariant>();

    Person lItem = qvariant_cast<Person>(leftData);
    Person rItem = qvariant_cast<Person>(rightData);*/

    //Person lItem;
    //lItem.name = qvariant_cast<QString>(sourceModel()->data(left, static_cast<int>(sortRole())).value<QVariant>());
    QString lItem = qvariant_cast<QString>(sourceModel()->data(left, static_cast<int>(sortRole())).value<QVariant>());

    //Person rItem;
    //rItem.name = qvariant_cast<QString>(sourceModel()->data(right, static_cast<int>(sortRole())).value<QVariant>());
    QString rItem = qvariant_cast<QString>(sourceModel()->data(right, static_cast<int>(sortRole())).value<QVariant>());

    /*qDebug() << "lItem " << lItem;
    qDebug() << "rItem " << rItem;*/

    //const auto attributes = mColumnSortStringAttributesList.attributesForColumn(column);

    if(isSortLocaleAware())
      return QString::localeAwareCompare(lItem, rItem) < 0;

    return QString::compare(lItem, rItem) < 0;

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

  {
    Person Frank;
    Frank.name = "Frank";
    Frank.surname = "Grechkin";
    Frank.uid = totalPersons++;
    dummyRemotePersons.push_back(Frank);
    //dummyRemotePersons[Frank.uid] = Frank;
  }

  {
    Person Frank;
    Frank.name = "Frank";
    Frank.surname = "Ovsyannikov";
    Frank.uid = totalPersons++;
    dummyRemotePersons.push_back(Frank);
    //dummyRemotePersons[Frank.uid] = Frank;
  }
}

QList<Person> retrieveRemotePersonsFiltered(const QString& filter, const PersonsModel::Roles& filterRole) {
  QList<Person> dummyRemotePersonsFiltered;
  for (int i = 0; i < dummyRemotePersons.size(); i++) {
    // TODO: regex support
    const auto& person = dummyRemotePersons.at(i);
    QString filterItem = "";

    if (filterRole == PersonsModel::Roles::NameRole) {
      filterItem = person.name;
      //qDebug() << "PersonsModel::Roles::NameRole";
    } else if (filterRole == PersonsModel::Roles::SurnameRole) {
      filterItem = person.surname;
      //qDebug() << "PersonsModel::Roles::SurnameRole";
    } else {
      // TODO
    }

    if(!filter.isEmpty() && !filterItem.contains(filter)) {
      //qDebug() << "skipped " << filterItem;
      continue;
    } else {
      //qDebug() << "NOT skipped " << filterItem;
    }
    dummyRemotePersonsFiltered.push_back(person);
  }
  return dummyRemotePersonsFiltered;
}

std::shared_ptr<fetchedPageData> retrieveRemotePersons(int page, int personsPerPage, const QString& filter, const PersonsModel::Roles& filterRole) {
  std::shared_ptr<fetchedPageData> result = std::make_shared<fetchedPageData>();

  QList<Person> dummyRemotePersonsPage;

  auto filtered = retrieveRemotePersonsFiltered(filter, filterRole);

  int cursorI = page * personsPerPage;
  if (cursorI >= filtered.size()) {
    qDebug() << "not enough persons. Requested page " << page;
  }

  int pagePersonsNum = std::min(cursorI + personsPerPage, filtered.size());

  for (; cursorI < pagePersonsNum; cursorI++) {
    //filtered[cursorI].name = "Bao";//;GetRandomString();

    dummyRemotePersonsPage.push_back(filtered.at(cursorI));
  }

  std::div_t res = std::div(filtered.size(), personsPerPage);
  // Fast ceiling of an integer division
  int pageCount = res.rem ? (res.quot + 1) : res.quot;

  result->persons = dummyRemotePersonsPage;
  result->requestedPageNum = page;
  result->totalPages = pageCount;
  result->totalItems = filtered.size(); // TODO
  result->recievedPagePersonsNum = pagePersonsNum;
  result->requestedPageSize = personsPerPage;

  return result;
}

std::shared_ptr<fetchedPageData> MainWindow::fetchRemotePersonsToModel(int page, int personsPerPage, const QString& filter, const PersonsModel::Roles& filterRole)
{
  //int pageCount = fetchRemotePageCount(personsPerPage);

  int pageIndex = page;

  std::shared_ptr<fetchedPageData> result = retrieveRemotePersons(pageIndex, personsPerPage, filter, filterRole);
  int pageCount = result->totalPages;

  //Q_ASSERT(pageIndex < pageCount);
  if (pageIndex >= pageCount) {
    qDebug() << "nothing to show!";
    return result;
  }

  int modelRow = page * personsPerPage; // person index in page
  for ( Person& person : result->persons) {
    //qDebug() << "person.name person.name" << person.name;
    //QStandardItem *item = new QStandardItem(person.name);
    //QVariant qvar = QVariant::fromValue(person);
    //qDebug() << "qvar.toString()" << qvar.toString();

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
    //QVariant variant = QVariant::fromValue(person);

    //item->setData(variant);
    //model->setItem(pageIndex, personCulumnIndex, item);
    //if(model->rowCount() < modelRow) {
    if (m_model->hasIndex(modelRow, personColumnIndex)) {

      QStandardItem* existingItem = m_model->item(modelRow);

      if(existingItem) {
        //person.name = "gg";
        /*existingItem->setData(QVariant::fromValue(person), static_cast<int>(PersonsModel::PersonVariantRole));
        existingItem->setData(person.name, static_cast<int>(PersonsModel::NameRole));
        existingItem->setData(person.surname, static_cast<int>(PersonsModel::SurnameRole));
        */
        QModelIndex existingIndexPerson = m_model->index(modelRow, personColumnIndex);
        QModelIndex existingIndexPersonPage = m_model->index(modelRow, static_cast<int>(Columns::PersonsPage));
        /*QVector<int> roles;
        roles.push_back(static_cast<int>(PersonsModel::Roles::PersonVariantRole));
        roles.push_back(static_cast<int>(PersonsModel::Roles::NameRole));
        roles.push_back(static_cast<int>(PersonsModel::Roles::SurnameRole));*/
        //m_model->dataChanged(existingIndexPerson, existingIndexPerson, roles);
        //m_model->dataChanged(existingIndexPersonPage, existingIndexPersonPage, roles);
        m_model->setData(existingIndexPerson, QVariant::fromValue(person.name), Qt::EditRole);
        m_model->setData(existingIndexPerson, QVariant::fromValue(person), static_cast<int>(PersonsModel::PersonVariantRole));
        m_model->setData(existingIndexPerson, QVariant::fromValue(person.name), static_cast<int>(PersonsModel::NameRole));
        m_model->setData(existingIndexPerson, QVariant::fromValue(person.surname), static_cast<int>(PersonsModel::SurnameRole));
        //m_model->setData()
      } else {
        qDebug() << "invalid model item at index" << modelRow;
      }

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
      QStandardItem *item = new QStandardItem(person.name);
      item->setEditable(true);
      item->setData(QVariant::fromValue(person), static_cast<int>(PersonsModel::PersonVariantRole));
      item->setData(person.name, static_cast<int>(PersonsModel::NameRole));
      item->setData(person.surname, static_cast<int>(PersonsModel::SurnameRole));
      //qDebug() << "added" << person.name << " to " << modelRow;
      m_model->appendRow(item);

      //QModelIndex nameIndex = model->index(modelRow, 1);
      //model->setData(nameIndex, person.name, PersonsModel::NameRole);

      /*QList<QStandardItem*> rowItems;
      rowItems.push_back(item);
      model->insertRow(modelRow, rowItems);*/
      //model->setItem(modelRow, 0, item);
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

  /*result->persons = personsPage.first;
  result->requestedPageNum = page;
  result->totalPages = pageCount;
  result->totalItems = pageCount * kPersonsPerPage; // TODO
  result->recievedPersonsNum = personsPage.first.size();
  qDebug() << "result->recievedPersonsNum " << result->recievedPersonsNum;
  result->requestedPageSize = kPersonsPerPage;*/

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
m_ui(new Ui::MainWindow)
{
  setupDummyRemotePersons(); // TODO: use remote server

  m_ui->setupUi(this);

  /// \note we reserved columns for custom data
  int columsCount = static_cast<int>(Columns::TOTAL);
  /// \note we will append rows dynamically, so we use 0 starting rows
  int predefinedRowNum = 0;
  m_model = new PersonsModel(predefinedRowNum, columsCount, this);
  //model->headerData(0 , Qt::Horizontal, static_cast<int>(PersonsModel::Roles::NameRole));

  m_mapper = new QDataWidgetMapper(this);
  m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
  //mapper->setModel(model);
  m_mapper->setOrientation(Qt::Horizontal);

  m_personsWidget = new PersonPageWidget();

  connect(m_personsWidget, SIGNAL(PersonsPageModified(QVariant)),
        this, SLOT(onPersonsPageChanged(QVariant)));

  m_ui->pwLayout->addWidget(m_personsWidget);
  //filterModel->sourceModel();

  m_filterModel = new PersonSortFilterProxyModel();
  m_mapper->setModel(m_filterModel);

  //m_mapper->addMapping(m_ui->lineEdit, 0, "name");
  m_mapper->addMapping(m_ui->lineEdit, 0);

  /// \note Any change in the model will be
  /// automatically propagated to the widget and back
  m_mapper->addMapping(m_personsWidget, static_cast<int>(Columns::PersonsPage), "PersonsPage");

  ///model->sort(personColumnIndex, Qt::AscendingOrder);
  //model->setSortRole(static_cast<int>(UserRoles::PersonRole));
  //model->sort(personColumnIndex, Qt::AscendingOrder);
  m_filterModel->setSourceModel(m_model);
  //filterModel->setFilterRole(MissionListModel::MissionNameRole);
  //filterModel->setSortRole(MissionListModel::MissionNameRole);
  m_filterModel->setFilterRole(filterRole);
  m_filterModel->setSortRole(filterRole);
  m_filterModel->setDynamicSortFilter(true);
  m_filterModel->setSortCaseSensitivity (Qt::CaseInsensitive);
  m_filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
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

  /*fetchRemotePersonsToModel(0, kPersonsPerPage, "");
  fetchRemotePersonsToModel(1, kPersonsPerPage, "");*/

  connect(m_ui->searchButton, &QPushButton::clicked, [this]()
  {
    if (!isDisconnected) {
      // TODO: fetch from remote
      // TODO: clear old model cache
      m_model->clear();
      /// \note we reserved columns for custom data
      int columsCount = static_cast<int>(Columns::TOTAL);
      m_model->setColumnCount(columsCount);
      //model->removeRows(0, model->rowCount());
      //model->removeColumns(0, model->columnCount());
      //qDebug() << "model->rowCount()" << model->rowCount();
      //filterModel->clear();
      m_filterModel->invalidate();

      //mapper->addMapping(pw, static_cast<int>(Columns::PersonsPage), "PersonsPage");
      // NOTE: reset page to 0
      //fetchRemotePersons(0, kPersonsPerPage, ui->searchEdit->text());
      //fetchRemotePersonsToModel(0, kPersonsPerPage);
      //fetchRemotePersonsToModel(1, kPersonsPerPage);

      //model->sort(0, Qt::AscendingOrder);

      // TODO: filter param
      m_lastFetchedData = fetchRemotePersonsToModel(0, kPersonsPerPage, m_ui->searchEdit->text(), filterRole);
      //lastFetchedData = nullptr;

      //qDebug()<<"lastFetchedData->recievedPersonsNum " << lastFetchedData->recievedPagePersonsNum;
      //refreshPageWidgets(lastFetchedData);

      //qDebug()<<"clicked" << ui->searchEdit->text();
      m_filterModel->setFilterFixedString(m_ui->searchEdit->text());
      //filterModel->filterRole();
      //refreshPageWidgets(lastFetchedData);

      //mapper->toFirst(); // refresh
    } else {
      //qDebug()<<"clicked" << ui->searchEdit->text();
      m_filterModel->setFilterFixedString(m_ui->searchEdit->text());
      //filterModel->filterRole();

      // TODO: filter param
      //m_lastFetchedData = fetchRemotePersonsToModel(0, kPersonsPerPage, "", filterRole);
      m_lastFetchedData = nullptr;

      //refreshPageWidgets(lastFetchedData);

      //Q_ASSERT(model->rowCount()); // setCurrentIndex will not work with empty model
      //mapper->setCurrentIndex(mapper->currentIndex()); // refresh
      //refreshPageWidgets(lastFetchedData);
    }

    refreshPageWidgets(m_lastFetchedData);
    // NOTE: empty mapper won't call currentIndexChanged
    m_personsWidget->clearPage();
    //model->dataChanged(QModelIndex(),QModelIndex());
    if (!m_lastFetchedData || !m_lastFetchedData->recievedPagePersonsNum) {
      qDebug() << "nothing to show";
      m_ui->prevButton->setEnabled(false);
      m_ui->nextButton->setEnabled(false);
      return;
    }

    if (!isDisconnected) {
      m_mapper->toFirst();
    } else {
      m_mapper->setCurrentIndex(m_mapper->currentIndex());
    }

  });

  connect(m_ui->resetButton, &QPushButton::clicked, [this]()
  {
    if (isDisconnected) {
      m_lastFetchedData = nullptr;
      return; // TODO
    }

     //qDebug()<<"clicked" << ui->searchEdit->text();
     m_ui->searchEdit->setText("");
     m_filterModel->setFilterFixedString("");
     //filterModel->filterRole();

     // TODO: filter param
     m_lastFetchedData = fetchRemotePersonsToModel(0, kPersonsPerPage, "", filterRole);
     //refreshPageWidgets(lastFetchedData);

     Q_ASSERT(m_model->rowCount()); // setCurrentIndex will not work with empty model
     //mapper->setCurrentIndex(mapper->currentIndex()); // refresh

    refreshPageWidgets(m_lastFetchedData);
    // NOTE: empty mapper won't call currentIndexChanged
    m_personsWidget->clearPage();
    //model->dataChanged(QModelIndex(),QModelIndex());
    if (!m_lastFetchedData || !m_lastFetchedData->recievedPagePersonsNum) {
      qDebug() << "nothing to show";
      m_ui->prevButton->setEnabled(false);
      m_ui->nextButton->setEnabled(false);
      return;
    }

     m_mapper->toFirst();
  });

  //connect(ui->prevButton, &QAbstractButton::clicked, mapper, &QDataWidgetMapper::toPrevious);
  //connect(ui->nextButton, &QAbstractButton::clicked, mapper, &QDataWidgetMapper::toNext);

  connect(m_ui->prevButton, &QAbstractButton::clicked, [this]() {
    if (!isDisconnected) {
      m_lastFetchedData = fetchRemotePersonsToModel(m_mapper->currentIndex() - 1, kPersonsPerPage, m_ui->searchEdit->text(), filterRole);
    } else {
      m_lastFetchedData = nullptr;
    }

    refreshPageWidgets(m_lastFetchedData);
    // NOTE: empty mapper won't call currentIndexChanged
    m_personsWidget->clearPage();
    //model->dataChanged(QModelIndex(),QModelIndex());
    if (!m_lastFetchedData || !m_lastFetchedData->recievedPagePersonsNum) {
      qDebug() << "nothing to show";
      m_ui->prevButton->setEnabled(false);
      m_ui->nextButton->setEnabled(false);
      return;
    }

    m_mapper->toPrevious();
  });

  connect(m_ui->checkBox, &QCheckBox::stateChanged, [this](int state) {
    isDisconnected = state > 0 ? true : false;
    qDebug() << "isDisconnected " << isDisconnected;
  });

  connect(m_ui->nextButton, &QAbstractButton::clicked, [this]() {
    if (!isDisconnected) {
      m_lastFetchedData = fetchRemotePersonsToModel(m_mapper->currentIndex() + 1, kPersonsPerPage, m_ui->searchEdit->text(), filterRole);
    } else {
      m_lastFetchedData = nullptr;
    }

    refreshPageWidgets(m_lastFetchedData);
    m_personsWidget->clearPage();
    // NOTE: empty mapper won't call currentIndexChanged
    //model->dataChanged(QModelIndex(),QModelIndex());
    if (!m_lastFetchedData || !m_lastFetchedData->recievedPagePersonsNum) {
      qDebug() << "nothing to show";
      m_ui->prevButton->setEnabled(false);
      m_ui->nextButton->setEnabled(false);
      return;
    }

    m_mapper->toNext();
  });

  connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this,SLOT(onModelDataChanged(QModelIndex,QModelIndex,QVector<int>)));
  connect(m_filterModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this,SLOT(onModelDataChanged(QModelIndex,QModelIndex,QVector<int>)));
  //connect(m_mapper, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this,SLOT(onModelDataChanged(QModelIndex,QModelIndex,QVector<int>)));

  connect(m_mapper, &QDataWidgetMapper::currentIndexChanged, this, &MainWindow::onMapperIndexChanged);

  if(isDisconnected) {
    /*// TODO: filter param
    lastFetchedData = fetchRemotePersonsToModel(0, kPersonsPerPage);
    refreshPageWidgets(lastFetchedData);
    mapper->toFirst();*/

    // TODO: reconnect signal
    return;
  } else {
    m_timer = new QTimer;
    m_timer->setSingleShot(true);
    // imitate remote load delay
    m_timer->start(1000);
    QObject::connect(m_timer, &QTimer::timeout, [this](){

        // TODO: filter param
        m_lastFetchedData = fetchRemotePersonsToModel(0, kPersonsPerPage, "", filterRole);
        refreshPageWidgets(m_lastFetchedData);

        m_mapper->toFirst();
    });
  }
}

void MainWindow::onModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    //qDebug() << "onModelDataChanged" << roles.count();
    QModelIndex parent = topLeft.parent();
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row)
    {
        for (int column = topLeft.column(); column <= bottomRight.column(); ++column)
        {
            QModelIndex index = m_model->index(row, column, parent);
            if (QStandardItem *item = m_model->itemFromIndex(index))
            {
              for (int rolesI = 0; rolesI < roles.size(); ++rolesI)
              {
                qDebug() << "onModelDataChanged" << item->data(roles.at(rolesI));
                //existingItem->setData(person.name, static_cast<int>(PersonsModel::NameRole));
                //qDebug() << "onModelDataChanged" << item->data(roles.at(rolesI)).value<QVariant>();
                //qDebug() << "onModelDataChanged" << qvariant_cast<QString>(item->data(roles.at(rolesI)).value<QVariant>());

                //Person person = qvariant_cast<Person>(item->data(roles.at(rolesI)).value<QVariant>());
                //qDebug() << "onModelDataChanged" << person.name;
                //qDebug() << "onModelDataChanged" << person.surname;
              }
            }
        }
    }
}

void MainWindow::refreshPageWidgets(std::shared_ptr<fetchedPageData> fetchedPageItems) {
  // remove all page widgets
  {
    QLayoutItem* item;
    while ( ( item = m_ui->wrapperLayout->takeAt( 0 ) ) != nullptr )
    {
        delete item->widget();
        delete item;
    }
  }

    int pageStart = 0;
    int totalRetrievedItems = 0;
    int totalAvailibleItems = 0;
  if(isDisconnected) {
    pageStart = 0;
    totalRetrievedItems = m_filterModel->rowCount();//personsPage.size(); // TODO
    totalAvailibleItems = m_filterModel->rowCount();
  } else {
    if (!fetchedPageItems || !fetchedPageItems->recievedPagePersonsNum) {
      qDebug() << "nothing to show...";
      //ui->prevButton->setEnabled(false);
      //ui->nextButton->setEnabled(false);
      return;
    }

    pageStart = fetchedPageItems->requestedPageNum * fetchedPageItems->requestedPageSize;
    totalRetrievedItems = fetchedPageItems->recievedPagePersonsNum;//personsPage.size(); // TODO
    totalAvailibleItems = std::min(totalRetrievedItems, fetchedPageItems->requestedPageSize);
  }

  //for (int i = 0; i < fetchedPageItems->requestedPageSize; i++) {
  for (int i = 0; i < totalAvailibleItems; i++) {
    /*const QModelIndex index = model->index(row, i);
    QStandardItem* item = model->itemFromIndex(index);*/
    int itemRow = pageStart + i;

    //qDebug() << " model->rowCount"<< filterModel->rowCount();
    //qDebug() << "pageStart + i"<< itemRow;

    /*if (!filterModel->hasIndex(itemRow, personColumnIndex)) {
      continue;
    }*/

    /*if (!model->item(itemRow, personColumnIndex)->isEnabled()) {
      continue;
    }*/

    const QModelIndex index = m_filterModel->index(itemRow, personColumnIndex);

    //Person person = qvariant_cast<Person>(filterModel->data(index, static_cast<int>(PersonsModel::NameRole)).value<QVariant>());
    Person person;
    person.name = qvariant_cast<QString>(m_filterModel->data(index, static_cast<int>(PersonsModel::NameRole)).value<QVariant>());
    person.surname = qvariant_cast<QString>(m_filterModel->data(index, static_cast<int>(PersonsModel::SurnameRole)).value<QVariant>());
    //qDebug() << "person.name " << person.name;
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

      m_ui->wrapperLayout->addWidget(item);

      //QLineEdit* boundWidget = static_cast<QLineEdit*>(item->getPersonNameWidget());
      //boundWidget->setText(person.name);
      // , static_cast<int>(PersonsModel::NameRole)
      //mapper->addMapping(boundWidget, 0, "name");
      //mapper->addMapping(boundWidget, PersonsModel::NameRole);
      //mapper->addMapping(boundWidget, 0, "name");
      //mapper->addMapping(boundWidget, 1, "name");
      //mapper->submit();
    }
    //mapper->toLast();

  }
}

/*void MainWindow::onCheckboxChanged(const int state)
{
  qDebug() << "onCheckboxChanged " << state;
}*/

void MainWindow::onPersonsPageChanged(const QVariant& val) {
  qDebug() << "MainWindow onPersonsPageChanged " << val;

  if(!m_lastFetchedData) {
    return;
  }

  int pageNum = m_lastFetchedData->requestedPageNum;
  int pageStart = pageNum * m_lastFetchedData->requestedPageSize;
  int itemTotalIndex = pageStart;
  int itemPageIndex = pageNum;

  QList<Person> newPersonsPage;

  for(auto& page : val.toList()) {
    Person person = qvariant_cast<Person>(page);
    qDebug() << "onPersonsPageChanged " << person.name;
    qDebug() << "onPersonsPageChanged " << person.surname;

    QModelIndex pageIndexPerson = m_model->index(itemTotalIndex, personColumnIndex);
    m_model->setData(pageIndexPerson, val, static_cast<int>(PersonsModel::PersonVariantRole));

    m_model->setData(pageIndexPerson, QVariant::fromValue(person.name), Qt::EditRole);
    m_model->setData(pageIndexPerson, QVariant::fromValue(person), static_cast<int>(PersonsModel::PersonVariantRole));
    m_model->setData(pageIndexPerson, QVariant::fromValue(person.name), static_cast<int>(PersonsModel::NameRole));
    m_model->setData(pageIndexPerson, QVariant::fromValue(person.surname), static_cast<int>(PersonsModel::SurnameRole));

    //QModelIndex pageIndexPersonsPage1 = m_model->index(itemTotalIndex, static_cast<int>(Columns::PersonsPage));
    //m_model->setData(pageIndexPersonsPage1, QVariant(""), Qt::EditRole);

    newPersonsPage.push_back(person);

    //ui->scrollAreaWidgetContentsLayout->addWidget();
    itemTotalIndex++;
    itemPageIndex++;
  }

    refreshPageWidgets(m_lastFetchedData);

    QModelIndex pageIndexPersonsPage = m_model->index(pageNum, static_cast<int>(Columns::PersonsPage));
    m_model->setData(pageIndexPersonsPage, QVariant::fromValue(newPersonsPage), Qt::EditRole);
    //m_model->setData(pageIndexPersonsPage, QVariant(""), Qt::EditRole);

        //QVector<int> roles;
        //roles.push_back(static_cast<int>(PersonsModel::Roles::PersonVariantRole));
        //roles.push_back(static_cast<int>(PersonsModel::Roles::NameRole));
        //roles.push_back(static_cast<int>(PersonsModel::Roles::SurnameRole));
        //roles.push_back(Qt::EditRole);
        //m_model->dataChanged(pageIndexPersonsPage, pageIndexPersonsPage, roles);

  //m_personsWidget->clearPage();
  //m_personsWidget->refreshPageWidgets();

      //m_mapper->setCurrentIndex(m_mapper->currentIndex());

        /*QModelIndex existingIndexPerson = m_model->index(modelRow, personColumnIndex);
        QModelIndex existingIndexPersonPage = m_model->index(modelRow, static_cast<int>(Columns::PersonsPage));
        QVector<int> roles;
        roles.push_back(static_cast<int>(PersonsModel::Roles::PersonVariantRole));
        roles.push_back(static_cast<int>(PersonsModel::Roles::NameRole));
        roles.push_back(static_cast<int>(PersonsModel::Roles::SurnameRole));
        //m_model->dataChanged(existingIndexPerson, existingIndexPerson, roles);
        //m_model->dataChanged(existingIndexPersonPage, existingIndexPersonPage, roles);
        m_model->setData(existingIndexPerson, QVariant::fromValue(person.name), Qt::EditRole);
        m_model->setData(existingIndexPerson, QVariant::fromValue(person), static_cast<int>(PersonsModel::PersonVariantRole));
        m_model->setData(existingIndexPerson, QVariant::fromValue(person.name), static_cast<int>(PersonsModel::NameRole));
        m_model->setData(existingIndexPerson, QVariant::fromValue(person.surname), static_cast<int>(PersonsModel::SurnameRole));
        //m_model->setData()*/
}

void MainWindow::onMapperIndexChanged(int pageNum) {
  //qDebug() << "onMapperIndexChanged for page " << pageNum;

  //lastFetchedData = fetchRemotePersonsToModel(pageNum, kPersonsPerPage, "");

  /*if (lastFetchedData)
    qDebug() << "onMapperIndexChanged recievedPersonsNum " << lastFetchedData->recievedPagePersonsNum;
*/
  //refreshPageWidgets(lastFetchedData);

  if (!m_lastFetchedData || !m_lastFetchedData->recievedPagePersonsNum) {
    qDebug() << "nothing to show";
    //ui->prevButton->setEnabled(false);
    //ui->nextButton->setEnabled(false);
    return;
  }

  //int pageStart = lastFetchedData->requestedPageNum * lastFetchedData->requestedPageSize;
  //int totalPersons = filterModel->rowCount() - 1;
  //int totalPersons = lastFetchedData->totalPages * lastFetchedData->requestedPageSize;
  int totalPersons = m_lastFetchedData->totalItems;
  int recievedPersons = m_lastFetchedData->recievedPagePersonsNum;
  int pageStart = m_lastFetchedData->requestedPageNum * m_lastFetchedData->requestedPageSize;
  //ui->prevButton->setEnabled(pageStart > 0 && pageStart < totalPersons);
  //ui->nextButton->setEnabled((pageStart + lastFetchedData->requestedPageSize) < totalPersons);

  //qDebug() << "onMapperIndexChanged requestedPageNum " << lastFetchedData->requestedPageNum;
  //qDebug() << "onMapperIndexChanged totalPages " << lastFetchedData->totalPages;

  m_ui->prevButton->setEnabled(m_lastFetchedData->requestedPageNum > 0);
  m_ui->nextButton->setEnabled(m_lastFetchedData->requestedPageNum < (m_lastFetchedData->totalPages - 1));
}

MainWindow::~MainWindow()
{
  delete m_ui;
}

PersonsModel::PersonsModel(int rows, int columns, QObject *parent) : QStandardItemModel(rows, columns, parent)
{

}

#ifdef customdata
QVariant PersonsModel::data(const QModelIndex &index, int role) const
{
  /*QStandardItemModelPrivate *d = reinterpret_cast<QStandardItemModelPrivate *>(QStandardItemModel::d_ptr.data());

  Q_D(const QStandardItemModel);
  QStandardItem *item = d->itemFromIndex(index);*/

  //see https://github.com/alexandru/address-book/blob/master/tablemodel.cpp#L30
  if (!index.isValid())
    return QVariant();

  /// \note QDataWidgetMapper uses EditRole to populate mappings
  if (role == Qt::EditRole && index.column() == static_cast<int>(Columns::PersonsPage)) {
    QList<QVariant> page;
    int pageStartCursor = index.row() * kPersonsPerPage;
    qDebug() << "pageStartCursor" << pageStartCursor;
    int availiblePageItems = std::min(rowCount(), pageStartCursor + kPersonsPerPage);
    for (int i = pageStartCursor; i < availiblePageItems; i++) {
      QModelIndex index_person = this->index(i, static_cast<int>(Columns::Person));
      QStandardItem* itm = this->itemFromIndex(index_person);
      //QString item = qvariant_cast<QString>(itm->data(PersonsModel::Roles::PersonVariantRole).value<QVariant>());
      QVariant item = itm->data(PersonsModel::Roles::PersonVariantRole).value<QVariant>();

      //qDebug() << "index.column()";
      page.push_back(item);
    }
    return QVariant(page);
  } else {
    QStandardItem* itm = this->itemFromIndex(index);

    //QModelIndex index_ = this->index(index.row(), index.column());
    QString item = qvariant_cast<QString>(itm->data(role).value<QVariant>());
    //qDebug() << "Qt::NameRole" << item;

    return QVariant(item);
  }

  /*if(role == PersonsModel::Roles::NameRole)
  {
    QStandardItem* itm = this->itemFromIndex(index);

    //QModelIndex index_ = this->index(index.row(), index.column());
    QString item = qvariant_cast<QString>(itm->data(role).value<QVariant>());
    qDebug() << "Qt::NameRole" << item;

    return QVariant(item);
  }
  if(role == PersonsModel::Roles::SurnameRole)
  {
    QStandardItem* itm = this->itemFromIndex(index);

    //QModelIndex index_ = this->index(index.row(), index.column());
    QString item = qvariant_cast<QString>(itm->data(role).value<QVariant>());
    qDebug() << "Qt::SurnameRole" << item;

    return QVariant(item);
  }*/

  return QVariant();
}
#endif

void PersonsModel::addPerson(const QString &name)
{
    QStandardItem *item = new QStandardItem;
    item->setData(name, NameRole);
    //item->setData(model, ModelRole);
    appendRow(item);
}

/*QHash<int, QByteArray> PersonsModel::roleNames() const
{
    QHash<int, QByteArray> mapping = QStandardItemModel::roleNames();
    mapping[NameRole] = "name";
    mapping[SurnameRole] = "surname";
    return mapping;
}*/
