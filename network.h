#ifndef NETWORK_H
#define NETWORK_H

#include <QNetworkReply>
#include <QProgressBar>

class Network
{
public:
    static void onDownload(QNetworkReply *rep);
    static void closeConnection(QNetworkReply *rep);
    static void updateProgress(QProgressBar *progressBar, qint64 bytesReceived, qint64 bytesTotal);
    static void showProgress(QProgressBar *progressBar);
private:
    Network();
};

#endif // NETWORK_H
