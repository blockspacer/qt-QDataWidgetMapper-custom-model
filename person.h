#pragma once

#include <QString>
#include <QtCore>
#include <QMetaType>

/*
class CustomType {
public:
  virtual ~CustomType() {}
  virtual QVariant toVariant() const { return QVariant(); }
};
*/
struct Person {
  // any unique id, without collisions
  int uid;

  QString name;

  QString surname;

  /*static QString PersonToString( Person* t ) {
     return t->name;
  }

  static QString PersonToString( Person t ) {
     return t.name + "_" + t.surname;
  }*/

/*
  QVariant toVariant() {
    return QVariant(name, );
  }*/
};

// allow type usage with QVariant
Q_DECLARE_METATYPE(Person);
//Q_DECLARE_METATYPE(Person*);

struct PersonsPageItem {
  Person person;
  int indexOnPage; // starts from 0
};

// allow type usage with QVariant
Q_DECLARE_METATYPE(PersonsPageItem);

struct PersonsPage {
  int pageStartCursor; // starts from 0, step = pagesize
  int pageId; // starts from 0, step = 1
  QList<PersonsPageItem> items;
};

// allow type usage with QVariant
Q_DECLARE_METATYPE(PersonsPage);
Q_DECLARE_METATYPE(QList<PersonsPageItem>);
