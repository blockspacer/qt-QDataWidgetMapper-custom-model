#include "person_item_widget.h"
#include "ui_person_item_widget.h"

#include <QDebug>

PersonItemWidget::PersonItemWidget(QWidget *parent) :
QWidget(parent),
ui(new Ui::Item)
{
  ui->setupUi(this);

  connect(ui->PersonNameEdit, SIGNAL(textChanged(QString)),
        this, SLOT(onPersonNameEdited(QString)));
}

PersonItemWidget::~PersonItemWidget()
{
  delete ui;
}

void PersonItemWidget::refreshTexts() {
   ui->PersonNameEdit->setText(m_name);
   ui->PersonSurnameEdit->setText(m_surname);
}

void PersonItemWidget::setName(const QString& text) {
  qDebug() << "PersonItemWidget setName " << text;
  if(m_name != text) {
    m_name = text;
    refreshTexts();

    emit nameChanged(text);
  }
}

void PersonItemWidget::setSurname(const QString& text) {
  m_surname = text;
  refreshTexts();
}


QWidget* PersonItemWidget::getPersonNameWidget() {
  return ui->PersonNameEdit;
}

void PersonItemWidget::onPersonNameEdited(const QString &text)
{
  qDebug() << "PersonItemWidget onPersonNameEdited " << text;
  if(m_name != text) {
    m_name = text;
    emit nameChanged(text);
  }
}
