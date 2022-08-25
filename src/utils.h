#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include <QtQml>
#include <QMetaMethod>
#include <QMetaObject>
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QColor>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QStringList>
#include <QTextDocument>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QPixmap>
#ifndef Q_OS_IOS
#include <QProcess>
#endif
#include <QTimerEvent>
#include <QUuid>
#include <QMimeDatabase>
#include <QImageReader>
#include <QJsonDocument>
#include <QThread>
#include <QRunnable>
#include <QThreadPool>
#include <QImageWriter>
class Utils : public QObject
{
    Q_OBJECT
    Q_ENUMS(HashMethod)
    Q_DISABLE_COPY(Utils)
public:
    explicit Utils(QObject *parent = nullptr,QQmlEngine *engine=nullptr);
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {

        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return new Utils;
    }
    enum HashMethod {
        Md5,
        Sha224,
        Sha256,
        Sha384,
        Sha512,
        Sha3_224,
        Sha3_256,
        Sha3_384,
        Sha3_512
    };
    static void imageResize(const QString &path, const QSize &size, const QString &dest, QObject *base, std::function<void(bool)> callback);
    static void init();
    static QObject* engine(QQmlEngine *engine, QJSEngine *scriptEngine);
public Q_SLOTS:
    static void debug( const QVariant & var );

    static QDateTime currentDate();
    static QString dateToMSec(const QDateTime &dt);
    static QDateTime mSecToDate(const QString &ms);
    static qint64 dateToSec(const QDateTime &dt);
    static QDateTime dateFromSec(qint64 date);
    static QString dateToString(const QDateTime &dt, const QString &format = QString());
    static QDateTime datefromString(const QString &dt, const QString &format = QString());
    static QString msecToTimeString(qint32 msec, const QString &format = QString());
    static QDate dateAddDays(const QDate &date, int days);
    static QStringList toStringList(const QVariant &value);
    static QVariantMap toVariantMap(const QVariant &value);
    static QVariantList toVariantList(const QVariant &value);

    static QString byteArrayToString(const QVariant &value);

    static QByteArray compress(const QByteArray &data, int level = -1);
    static QByteArray uncompress(const QByteArray &data);

    QVariant bytesToVariant(const QByteArray &data);
    QByteArray variantToBytes(const QVariant &var);

    static QString fileName( const QString & path );
    static QString fileSuffix( const QString & path );
    static QString fileMime(const QString &path);
    static QString fileParent( const QString & path );
    static QString fileSize( const QString & path );
    static QString readText( const QString & path );
    static bool writeText(const QString & path , const QString &text);
    static bool fileExists(const QString & path);
    static QStringList filesOf(const QString &path);
    void callJs(QJSValue fn, QJSValue arg=QJSValue());

    static QSize imageSize(const QString &path);
    int extractZip(const QString &path,const QString &dir,QJSValue obj);
    int imageToPdf(const QStringList paths, const QString saveTo,  QJSValue obj);
    int runCMD(const QString &path);
    int execCMD(const QString &path);
    int runPy(const QString &path, const QJSValue argg);
    int runCMDCallack(const QString &prog,const QString &arg, QJSValue obj);
    static bool writeFile(const QString &path, const QVariant &data, bool compress = false);
    static bool appendText(const QString &path, const QString &text, bool compress = false);
    static QByteArray readFile(const QString &path, bool uncompress = false);

    static QString className(QObject *obj);

    static QUrl stringToUrl(const QString &path);
    static QString urlToLocalPath(const QUrl &url);

    static QString qtVersion();

    static void deleteItemDelay( QObject *o, int ms );

    static qreal colorHue( const QColor & clr );
    static qreal colorLightness( const QColor & clr );
    static qreal colorSaturation( const QColor & clr );

    static void mkDir(const QString &dir);

    static QVariantMap colorHsl( const QColor & clr );

    static bool createVideoThumbnail(const QString &video, const QString &output, QString ffmpegPath = QString());

    static QString translateNumbers( QString input );
    static QString trNums(QString input);
    static QString passToMd5( const QString & pass );
    static QString md5(const QString & str );
    static QString passToHash(const QString &pass, int method);
    static QString hash(const QString &str, int method);
    static QString createUuid();

    static QString htmlToPlaintText(const QString &html);

    static QString currentDirectory(){
        return QDir::currentPath();
    }
    static void copyDirectory( const QString & src, const QString & dst );
    static bool copy( const QString & src, const QString & dst );
    static bool rename( const QString & src, const QString & dst );
    static void deleteFile(const QString &file);
    static void clearDirectory(const QString &dir);
    static void setProperty( QObject *obj, const QString & property, const QVariant & v );
    static QVariant property( QObject *obj, const QString & property );

    static Qt::LayoutDirection directionOf( const QString & str );


private:
    QQmlEngine *m_engine;

};

#endif // UTILS_H
