#include "person_page_widget.h"
#include "ui_person_page_widget.h"

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

QString PersonPageWidget::PersonName() const
{
  return m_personName;
}

void PersonPageWidget::setPersonName(const QString& val)
{
  qDebug() << "setPersonName " << val;
  QVariant qv(val);
  Person person = qvariant_cast<Person>(qv);
  qDebug() << "setPersonName " << person.name;
  qDebug() << "setPersonName " << person.surname;
  m_personName = val;
  ui->lineEdit->setText(val);
  emit PersonNameChanged(val);
}
