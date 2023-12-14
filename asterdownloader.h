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
private:
    void downloadAster(int lat, int lon);
    static void unzipFile(const std::string &zipArchive, const std::string &fileName, const std::string &outputFile);
    void unzip(QNetworkReply *rep);


    QNetworkAccessManager nw_manager;
    QString asterDir;
    QString asterURL;

    unsigned int tf_count;
    QString asterParseDir;
    QVector<int> list;
public:
    void run();
};

#endif // ASTERDOWNLOADER_H
