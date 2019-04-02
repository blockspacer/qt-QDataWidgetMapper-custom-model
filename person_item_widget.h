#ifndef ITEM_H
#define ITEM_H

#include <QWidget>

namespace Ui {
class Item;
}

class PersonItemWidget : public QWidget
{
Q_OBJECT

Q_PROPERTY(QString m_name READ getName WRITE setName NOTIFY nameChanged)

public:
  explicit PersonItemWidget(QWidget *parent = nullptr);
  ~PersonItemWidget();
  void setName(const QString& text);
  void setSurname(const QString &text);

  QString getName() const {
    return m_name;
  };

  QWidget *getPersonNameWidget();

public slots:
  void onPersonNameEdited(const QString& val);

signals:
    void nameChanged(const QString& text);

private:
  Ui::Item *ui;
  QString m_name;
  QString m_surname;

  void refreshTexts();
};

#endif // ITEM_H
