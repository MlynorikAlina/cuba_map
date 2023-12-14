#ifndef NETWORKDOWNLOAD_H
#define NETWORKDOWNLOAD_H

#include <QNetworkReply>
#include <QProgressBar>

class NetworkDownload
{
public:
    static void onDownload(QNetworkReply *rep);
    static void closeConnection(QNetworkReply *rep);
    static void updateProgress(QProgressBar *progressBar, qint64 bytesReceived, qint64 bytesTotal);
    static void showProgress(QProgressBar *progressBar);
private:
    NetworkDownload();
};

#endif // NETWORKDOWNLOAD_H
