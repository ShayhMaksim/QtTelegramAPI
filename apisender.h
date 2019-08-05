#ifndef APISENDER_H
#define APISENDER_H
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QEventLoop>
#include <QString>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QNetworkProxy>

struct Info
{
    QString first_name;
    QString last_name;
    QString username;
};


class APISender: public QObject
{
    Q_OBJECT
public:
     APISender(QObject *pobj=nullptr);
     QSqlDatabase db;

public slots:
/*
 *функция получения результата API запроса на команды api.telegram.org
*/
QByteArray GET(QUrl r);
/*
 * функциия отправки сообщения(text) на id пользователя
*/
void SendMessage(const QString & chat_id,const QString & text);
/*
 * функция получения id пользователя по Имени и Фамилии
 * работает только в том случае, если к вам обратился собеседник
 * быстрый доступ
*/
QString GetID(const QString & first_name,const QString & last_name);
/*
 * получаем информацию по id пользователя telegram и записываем
 * данные в БД SQLite
 */
bool SetAll(const QString & chat_id);
/*
 *запись номера номера телефона в БД
 */
bool SetNumber(const QString & chat_id);
/*
 * получение номера телефона по Имени и Фамилии из БД
 */
QString GetNumber(const QString & first_name,const QString & last_name);//DB
/*
 * получение информации о пользователе по id
 */
Info GetInfo(const QString & chat_id);
/*
 * получение последнего присланного сообщения из Телеграм
 */
QString GetMessage(const QString & chat_id);
/*
 * установка соединения из БД
 */
bool createConnection();//DB
};




#endif // APISENDER_H
