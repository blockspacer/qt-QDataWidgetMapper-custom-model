#ifndef PERSON_PAGE_WIDGET_H
#define PERSON_PAGE_WIDGET_H

#include "person.h"

#include <QWidget>
#include <QVariant>
#include <QStandardItem>
#include <QDataWidgetMapper>

namespace Ui {
class PersonPageWidget;
}

//typedef QString TPersonName;

class PersonPageWidget : public QWidget
{
Q_OBJECT

/// \note Mapped Property, see QDataWidgetMapper
Q_PROPERTY(QVariant m_PersonsPage READ getPersonsPage WRITE setPersonsPage NOTIFY personsPageChanged USER true)

public:
  explicit PersonPageWidget(QWidget *parent = nullptr);
  ~PersonPageWidget();

  QVariant getPersonsPage() const;

  void clearPage();

  void setMapper(QDataWidgetMapper* mapper);

  void refreshPageWidgets();
public slots:
    void setPersonsPage(const QVariant& val);

signals:
    void personsPageChanged(const QVariant& val);
    void PersonsPageModified(const QVariant& val);


private:
  void refreshPageWidgets(QList<PersonsPageItem>& pageItems);

private:
  Ui::PersonPageWidget *ui;
  QVariant m_PersonsPage;
  QDataWidgetMapper* m_mapper;
};

#endif // PERSON_PAGE_WIDGET_H
