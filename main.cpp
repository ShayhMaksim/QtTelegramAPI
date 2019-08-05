#include <QCoreApplication>
#include "apisender.h"
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
  setlocale(LC_CTYPE,"rus");
    APISender arg;
    arg.createConnection();

   arg.SendMessage(QString("765952550"),"Hello,Maksim");

  Info b=(arg.GetInfo(""));
  arg.SetNumber("765952550");
  qDebug()<<b.username <<b.last_name << b.first_name;
  qDebug()<< arg.GetMessage("");
  arg.SetAll("");

  qDebug()<< "--------------";
  qDebug()<<arg.GetNumber("","");

    return a.exec();
}
