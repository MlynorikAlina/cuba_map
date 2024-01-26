#include "asterdownloader.h"

#include "zip.h"
#include <QDir>
#include <QFile>
#include <QMutex>
#include <QNetworkReply>
#include <fstream>
#include <iomanip>
#include <network.h>
#include <qprocess.h>

AsterDownloader::AsterDownloader(const QString &asterDir,
                         const QString &asterURL)
    : nw_manager(this), asterDir(asterDir), asterURL(asterURL) {
    QDir dir(asterDir);
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());
    connect(this, &AsterDownloader::fileDownloaded, this, &AsterDownloader::checkFinished);
}

void AsterDownloader::unzip(QNetworkReply *rep) {
    QString s = rep->property("file_name").toString();
    QString ss = rep->property("_file_name").toString();
    QString asterDir = rep->property("asterDir").toString();
    QString temp = asterDir + QString::number(tf_count++) +".temp.zip";
    QString pref("ASTGTMV003_");
    QString zfn1(pref + ss + ".zip");
    QString zfn2(pref + ss + "_dem.tif");
    QString outf(asterDir + ss + ".tif");

    unzipFile(s, zfn1, temp);
    unzipFile(temp, zfn2, outf);

    qInfo()<<temp<<" "<<QFile::remove(temp)<<" "<<QFile::permissions(temp);
    qInfo()<<s<<" "<<QFile(s).remove();
    emit fileDownloaded();
    delete rep;
}

void AsterDownloader::checkFinished()
{
    filesCount++;
    if(filesCount==filesTotal)
        emit finished();
}

void AsterDownloader::setList(const QVector<int> &newList) { list = newList; }

void AsterDownloader::run() {
    filesTotal = (list[1] - list[0])*(list[3] - list[2]);
    filesCount = 0;
    for (int lon = list[0]; lon < list[1]; lon++)
        for (int lat = list[2]; lat < list[3]; lat++) {
            downloadAster(lat, lon);
        }

}

void AsterDownloader::downloadAster(int lat, int lon) {
    QString s, s2;
    QTextStream ss(&s2);
    ss.setFieldAlignment(QTextStream::AlignRight);
    ss.setPadChar('0');
    if (lat >= 0)
        ss << "N";
    else
        ss << "S";
    ss << qSetFieldWidth(2) << abs(lat);
    ss.setFieldWidth(1);
    if (lon >= 0)
        ss << "E";
    else
        ss << "W";
    ss << qSetFieldWidth(3) << abs(lon);
    s = asterDir + ss.string();
    if (!QFile::exists(s + ".tif")) {
        s += ".zip";
        QNetworkRequest request(asterURL + "Download_" + s2 + ".zip");
        QNetworkReply *rep = nw_manager.get(request);
        rep->setProperty("_file_name", s2);
        rep->setProperty("file_name", s);
        rep->setProperty("asterDir", asterDir);
        connect(rep, &QNetworkReply::readyRead,
                [rep] { Network::onDownload(rep); });
        connect(rep, &QNetworkReply::finished, [rep, this] {
            Network::closeConnection(rep);
            unzip(rep);
        });
    } else{
        emit fileDownloaded();
    }
}

zip_uint64_t BUF_SIZE = 1024;
void AsterDownloader::unzipFile(QString &zipArchive,
                            QString &fileName,
                            QString &outputFile) {
    if (QFile::exists(zipArchive)) {
        zip_t *za;
        int err;
        if ((za = zip_open(zipArchive.toStdString().c_str(), ZIP_RDONLY, &err)) != NULL) {
            zip_error_t error;
            zip_error_init_with_code(&error, err);
            zip_file_t *f = zip_fopen(za, fileName.toStdString().c_str(), 0);
            if (f != NULL) {
                struct zip_stat ist;
                zip_stat_init(&ist);
                zip_stat(za, fileName.toStdString().c_str(), 0, &ist);
                std::ofstream of(outputFile.toStdString().c_str(), std::ios::binary);
                while(ist.size>0){
                    zip_uint64_t size = std::min(BUF_SIZE, ist.size);
                    char *contents = new char[size];
                    zip_fread(f, contents, size);
                    of.write(contents, size);
                    ist.size-=size;
                    delete [] contents;
                }
                of.close();
            }
            zip_fclose(f);
        }
        zip_close(za);
    }
}
