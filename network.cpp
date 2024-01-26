#include "network.h"

#include <QFile>
#include <QVariant>
#include <QDebug>

Network::Network()
{

}

void Network::updateProgress(QProgressBar* progressBar, qint64 bytesReceived, qint64 bytesTotal)
{
    progressBar->setRange(0, bytesTotal);
    progressBar->setValue(bytesReceived);
}

void Network::showProgress(QProgressBar *progressBar)
{
    progressBar->setRange(0,1);
    progressBar->setValue(0);
    progressBar->show();
}

void Network::onDownload(QNetworkReply* rep) {
    QString fileName = rep->property("file_name").toString();
    QByteArray data = rep->readAll();

    QFile file(fileName);
    file.open(QIODevice::Append);
    file.write(data);
    file.close();
}

void Network::closeConnection(QNetworkReply *rep)
{
/*   QVariant status_code = rep->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        QString status = status_code.toString(); // or status_code.toInt();
        qInfo() << status;
*/
    rep->deleteLater();
}
