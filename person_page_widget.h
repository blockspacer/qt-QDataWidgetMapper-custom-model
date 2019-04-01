#ifndef PERSON_PAGE_WIDGET_H
#define PERSON_PAGE_WIDGET_H

#include "person.h"

#include <QWidget>
#include <QVariant>
#include <QStandardItem>

namespace Ui {
class PersonPageWidget;
}

//typedef QString TPersonName;

class PersonPageWidget : public QWidget
{
Q_OBJECT

Q_PROPERTY(QVariant PersonsPage READ PersonsPage WRITE setPersonsPage NOTIFY PersonsPageChanged USER true)

public:
explicit PersonPageWidget(QWidget *parent = nullptr);
~PersonPageWidget();

    QVariant PersonsPage() const;

    void clearPage();

public slots:
    void setPersonsPage(const QVariant& val);

signals:
    void PersonsPageChanged(const QVariant& val);

private:
  void refreshPageWidgets(QList<QVariant> pageItems);

private:
  Ui::PersonPageWidget *ui;
  QVariant m_PersonsPage;
};

#endif // PERSON_PAGE_WIDGET_H
