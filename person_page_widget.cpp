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

QVariant PersonPageWidget::getPersonsPage() const
{
  return m_PersonsPage;
}

void PersonPageWidget::setPersonsPage(const QVariant& val)
{
  qDebug() << "setPersonsPage... ";

  clearPage(); // resets m_PersonsPage

  m_PersonsPage = val;

  PersonsPage pp = qvariant_cast<PersonsPage>(val);

  QList<PersonsPageItem> pageItems = pp.items;
  for(auto& page : pageItems) {
    Person person = page.person;
    //PersonPage p;
    qDebug() << "setPersonsPage " << person.name;
    qDebug() << "setPersonsPage " << person.surname;
    //ui->scrollAreaWidgetContentsLayout->addWidget();
  }

  //QList<QVariant> pageItems = val.toList();

  /*qDebug() << "setPersonsPage " << pageItems.size();
  for(auto& page : pageItems) {
    Person person = qvariant_cast<Person>(page);
    //PersonPage p;
    //qDebug() << "setPersonsPage " << person.name;
    //qDebug() << "setPersonsPage " << person.surname;
    //ui->scrollAreaWidgetContentsLayout->addWidget();
  }*/

  refreshPageWidgets(pageItems);

  /*QVariant qv(val);
  Person person = qvariant_cast<Person>(qv);
  qDebug() << "setPersonName " << person.name;
  qDebug() << "setPersonName " << person.surname;
  m_personName = val;
  ui->lineEdit->setText(val);*/

  emit personsPageChanged(val);
}

void PersonPageWidget::clearPage() {

  m_PersonsPage = QVariant();

  qDebug() << "PersonPageWidget clearPage";
  // remove all page widgets
  {
    QLayoutItem* item;
    while ( ( item = ui->scrollAreaWidgetContentsLayout->takeAt( 0 ) ) != nullptr )
    {
        //delete item->widget();
        item->widget()->deleteLater();
        //item->deleteLater();
        delete item;
    }
  }
}

void PersonPageWidget::refreshPageWidgets() {
  //QList<QVariant> pageItems = m_PersonsPage.toList();

  PersonsPage pp = qvariant_cast<PersonsPage>(m_PersonsPage);
  QList<PersonsPageItem> pageItems = pp.items;

  refreshPageWidgets(pageItems);
}

/**
 * \brief We use QScrollArea with QVBoxLayout because we
 * need interaction with custom widgets. Widgets also can be animated.
 * \note We managed to use QDataWidgetMapper to use custom widgets
 * with custom model. We pass custom data in model columns,
 * see data(const QModelIndex &index, int role) method of custom model.
 * \note We DON'T use any of QTreeView and other such classes.
 * Qt MVC is designed to process big amount of cognate data.
 * It is not designed to provide widget-based interaction.
 * So, if you want to "assign" one widget to each item and to interact with them,
 * than you will have a lot of problems with implementing delegates
 * (tracking mouse events, providing editor's factory).
 * You may create your own delegates with custom drawing
 * and custom processing of mouse events,
 * but it's much easy to use simple widgets.
 */
void PersonPageWidget::refreshPageWidgets(QList<PersonsPageItem>& pageItems) {
  qDebug() << "PersonPageWidget refreshPageWidgets " << pageItems.size();

  for (int i = 0; i < pageItems.size(); i++) {
    //const QModelIndex index = filterModel->index(itemRow, personColumnIndex);

    Person person = pageItems.at(i).person; //qvariant_cast<Person>(pageItems.at(i));
    //qDebug() << "setPersonsPage " << person.name;
    //qDebug() << "setPersonsPage " << person.surname;

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

      /*connect(item, SIGNAL(nameChanged(QString)),
            this, SLOT(onPersonsNameChanged(QString)));*/

      item->setName(person.name);

      item->setPageIndex(i);

      item->setSurname(person.surname);

      connect(item, &PersonItemWidget::nameChanged, item, [this, item](const QString& text) {
        int itemPageIndex = item->getPageIndex();

        qDebug() << "connect PersonItemWidget::nameChanged " << text;

        //QList<QVariant> pageItems = m_PersonsPage.toList();

        PersonsPage pp = qvariant_cast<PersonsPage>(m_PersonsPage);
        QList<PersonsPageItem> pageItems = pp.items;

        //QList<PersonsPageItem> pageItems = m_PersonsPage.toList();

        PersonsPageItem pPItem = pageItems.at(itemPageIndex);
        //Person person = pageItems.at(itemPageIndex).person;
        //person.name = text;
        pPItem.person.name = text;
        pageItems.replace(itemPageIndex, pPItem);

        //Person person1 = qvariant_cast<Person>(pageItems.at(itemPageIndex));
        //qDebug() << "connect PersonItemWidget::nameChanged " << person1.name;

        pp.items = pageItems;
        m_PersonsPage = QVariant::fromValue(pp);
        emit PersonsPageModified(m_PersonsPage);
      });

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
