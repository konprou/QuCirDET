#ifndef CONNECTION_H
#define CONNECTION_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    QSqlQuery query;
    query.exec("create table qec_codes (ID int primary key,CodeName varchar(20),CodeType varchar(20),n float,k float,d float)");
    query.exec("insert into qec_codes values(2,         'Ham [[7,1,3]]',    'CSS',    7.0,      1,   3.0)");
    query.exec("insert into qec_codes values(4,      'Golay [[21,3,5]]',    'CSS',     21,      3,     5)");

    return true;
}
#endif // CONNECTION_H
