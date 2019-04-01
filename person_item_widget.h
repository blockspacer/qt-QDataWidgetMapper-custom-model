#ifndef ITEM_H
#define ITEM_H

#include <QWidget>

namespace Ui {
class Item;
}

class PersonItemWidget : public QWidget
{
Q_OBJECT

public:
  explicit PersonItemWidget(QWidget *parent = nullptr);
  ~PersonItemWidget();
  void setName(const QString& text);
  void setSurname(const QString &text);

  QWidget *getPersonNameWidget();
private:
  Ui::Item *ui;
  QString m_name;
  QString m_surname;

  void refreshTexts();
};

#endif // ITEM_H
