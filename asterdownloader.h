#ifndef ASTERDOWNLOADER_H
#define ASTERDOWNLOADER_H

#include <QNetworkAccessManager>
#include <QThread>

class AsterDownloader: public QObject
{
    Q_OBJECT
public:
    AsterDownloader(const QString &asterDir, const QString &asterURL);

    void setList(const QVector<int> &newList);

signals:
    void fileDownloaded();
    void finished();
private:
    void downloadAster(int lat, int lon);
    static void unzipFile(QString &zipArchive, QString &fileName, QString &outputFile);
    void unzip(QNetworkReply *rep);


    QNetworkAccessManager nw_manager;
    QString asterDir;
    QString asterURL;

    unsigned int tf_count;
    QAtomicInt filesTotal;
    QAtomicInt filesCount;
    QString asterParseDir;
    QVector<int> list;

private slots:
    void checkFinished();
public:
    void run();
};

#endif // ASTERDOWNLOADER_H
