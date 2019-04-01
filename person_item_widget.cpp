#include "person_item_widget.h"
#include "ui_item.h"

PersonItemWidget::PersonItemWidget(QWidget *parent) :
QWidget(parent),
ui(new Ui::Item)
{
  ui->setupUi(this);
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
  m_name = text;
  refreshTexts();
}

void PersonItemWidget::setSurname(const QString& text) {
  m_surname = text;
  refreshTexts();
}


QWidget* PersonItemWidget::getPersonNameWidget() {
  return ui->PersonNameEdit;
}
