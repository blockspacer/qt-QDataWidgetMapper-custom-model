#include "person_page_widget.h"
#include "ui_person_page_widget.h"
#include "person_item_widget.h"

#include <QDebug>

PersonPageWidget::PersonPageWidget(QWidget *parent) :
QWidget(parent),
ui(new Ui::PersonPageWidget)
{
ui->setupUi(this);
}

PersonPageWidget::~PersonPageWidget()
{
  delete ui;
}

QVariant PersonPageWidget::PersonsPage() const
{
  return m_PersonsPage;
}

void PersonPageWidget::setPersonsPage(const QVariant& val)
{
  qDebug() << "setPersonsPage ";
  QList<QVariant> pageItems = val.toList();
  for(auto& page : pageItems) {
    Person person = qvariant_cast<Person>(page);
    qDebug() << "setPersonsPage " << person.name;
    qDebug() << "setPersonsPage " << person.surname;
    //ui->scrollAreaWidgetContentsLayout->addWidget();
  }

  refreshPageWidgets(pageItems);

  /*QVariant qv(val);
  Person person = qvariant_cast<Person>(qv);
  qDebug() << "setPersonName " << person.name;
  qDebug() << "setPersonName " << person.surname;
  m_personName = val;
  ui->lineEdit->setText(val);*/

  emit PersonsPageChanged(val);
}

void PersonPageWidget::refreshPageWidgets(QList<QVariant> pageItems) {
  qDebug() << "refreshPageWidgets " << pageItems.size();

  // remove all page widgets
  {
    QLayoutItem* item;
    while ( ( item = ui->scrollAreaWidgetContentsLayout->takeAt( 0 ) ) != nullptr )
    {
        delete item->widget();
        delete item;
    }
  }

  for (int i = 0; i < pageItems.size(); i++) {
    //const QModelIndex index = filterModel->index(itemRow, personColumnIndex);

    Person person = qvariant_cast<Person>(pageItems.at(i));
    qDebug() << "setPersonsPage " << person.name;
    qDebug() << "setPersonsPage " << person.surname;

    //Person person = qvariant_cast<Person>(filterModel->data(index, static_cast<int>(PersonsModel::NameRole)).value<QVariant>());
    /*Person person;
    person.name = qvariant_cast<QString>(filterModel->data(index, static_cast<int>(PersonsModel::NameRole)).value<QVariant>());
    person.surname = qvariant_cast<QString>(filterModel->data(index, static_cast<int>(PersonsModel::SurnameRole)).value<QVariant>());
    */
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

      ui->scrollAreaWidgetContentsLayout->addWidget(item);

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
