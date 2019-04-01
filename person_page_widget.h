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

Q_PROPERTY(QString PersonName READ PersonName WRITE setPersonName NOTIFY PersonNameChanged USER true)

public:
explicit PersonPageWidget(QWidget *parent = nullptr);
~PersonPageWidget();

    QString PersonName() const;

public slots:
    void setPersonName(const QString& val);

signals:
    void PersonNameChanged(const QString& val);

private:
Ui::PersonPageWidget *ui;
QString m_personName;
};

#endif // PERSON_PAGE_WIDGET_H
