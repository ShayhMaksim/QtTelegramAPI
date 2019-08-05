#include "apisender.h"

APISender::APISender(QObject *pobj):QObject(pobj)
{
}

QByteArray APISender::GET(QUrl r)
{
    QNetworkAccessManager* manager =new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->get(QNetworkRequest (r));
    QEventLoop wait;
    connect(manager, SIGNAL(finished(QNetworkReply*)),&wait, SLOT(quit()));
    QTimer:: singleShot(10000, &wait,SLOT(quit()));
    wait.exec();
    QByteArray answer = reply->readAll();
    reply->deleteLater();
    return answer;
}
void APISender::SendMessage(const QString & chat_id,const QString & text)
{
     QUrl current("https://api.telegram.org/<bot_token>/sendMessage");
     QUrlQuery Param;
     Param.addQueryItem("chat_id",chat_id);
     Param.addQueryItem("text",text);
     current.setQuery(Param);
     QByteArray answer = GET(current);
}
/*
*
  */
QString APISender::GetID(const QString & first_name,const QString & last_name)
{
    QByteArray answer = GET(QUrl("https://api.telegram.org/<bot_token>/getUpdates"));
    QJsonDocument  data = QJsonDocument::fromJson(answer);
    QJsonObject  var= data.object();
    QJsonArray Result= var.value("result").toArray();
    QJsonValue g;
    int res=0;
    int i=0;
    bool key=false;
    while(key==false)
    {
       g=Result.at(i);
      if ((g["message"]["from"]["last_name"].toString()==last_name) &&
        (g["message"]["from"]["first_name"].toString()==first_name))
      {
          res=g["message"]["from"]["id"].toInt();
          key=true;
      }
      ++i;
      if  (g.isUndefined())  key=true;
    }
    return QString::number(res);
}
Info APISender::GetInfo(const QString & chat_id)
{
   Info information;
   QUrl current("https://api.telegram.org/<bot_token>/getChatMember");
   QUrlQuery Param;
   Param.addQueryItem("chat_id",chat_id);
   Param.addQueryItem("user_id",chat_id);
   current.setQuery(Param);
   QByteArray answer=GET(current);
   QJsonDocument  data = QJsonDocument::fromJson(answer);
   qDebug()<<data;
   QJsonObject  var= data.object();
   QJsonValue g=var["result"];

   information.username=g["user"]["username"].toString();
   information.first_name=g["user"]["first_name"].toString();
   information.last_name=g["user"]["last_name"].toString();
   return information;
}
QString APISender::GetMessage(const QString & chat_id)
{
    QByteArray answer = GET(QUrl("https://api.telegram.org/<bot_token>/getUpdates"));
    QJsonDocument  data = QJsonDocument::fromJson(answer);
    QJsonObject  var= data.object();
    QJsonArray Result= var.value("result").toArray();
    QJsonValue g;
    QString res;
    int i=0;
    bool key=false;
    while(key==false)
    {
       g=Result.at(i);
      if (g["message"]["from"]["id"].toInt()==(chat_id).toInt())
      {
          res=g["message"]["text"].toString();
      }
      ++i;
      if  (g.isUndefined())  key=true;
    }
    return res;
}
bool APISender::createConnection()
{
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/maksim/Telegram/Telegram");
    if (!db.open())
    {
        qDebug()<< "Cannot open database:"<< db.lastError();
        return false;
    }
    return true;
}
bool APISender::SetAll(const QString & chat_id)
{
    QSqlQuery query(db);
    QStringList lst =db.tables();
    foreach(QString str,lst)
    {
        qDebug()<<"Tables:" <<str;
    }
    Info * result=new Info(GetInfo(chat_id));
    if (result==nullptr) return false;
    QString strTest="Select chat_id from Telegram where " "chat_id="+chat_id;
    query.exec(strTest);
    QString chatid;
    while(query.next())
    {
        chatid=query.value(0).toString();
    }
    if (chatid!=nullptr) return false;
    QString strEdit="Insert into Telegram (username,first_name,last_name,chat_id) " "values('%1','%2','%3',%4);";
    QString str=strEdit
            .arg(result->username)
            .arg(result->first_name)
            .arg(result->last_name)
            .arg(chat_id);
    query.exec(str);
    return true;
}
QString APISender::GetNumber(const QString & first_name,const QString & last_name)
{
   QSqlQuery query(db);
   QString strTest="Select phone from Telegram where " "first_name='"+first_name+"' and last_name='"+last_name+"'";
   query.exec(strTest);
   int phone;
   while(query.next())
   {
       phone=query.value(0).toInt();
   }
   return QString::number(phone);
}
bool APISender::SetNumber(const QString & chat_id)
{
    QString phone=GetMessage(chat_id);
    QSqlQuery query(db);
    QString strEdit="Update Telegram set phone="+phone+" where chat_id="+chat_id;
    qDebug() << query.exec(strEdit);
    return query.exec(strEdit);
}
