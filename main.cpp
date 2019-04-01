#include "mainwindow.h"
#include "person.h"
#include <QApplication>

int main(int argc, char *argv[])
{

  //QMetaType::registerConverter<Person,QString>( Person::PersonToString );

  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
