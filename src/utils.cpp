#include "utils.h"

Utils::Utils(QObject *parent,QQmlEngine *engine) : QObject(parent), m_engine(engine)
{

}
void Utils::debug(const QVariant &var)
{
    qDebug() << var;
}

QDateTime Utils::currentDate()
{
    return QDateTime::currentDateTime();
}

QString Utils::dateToMSec(const QDateTime &dt)
{
    return QString::number(dt.toMSecsSinceEpoch());
}

QDateTime Utils::mSecToDate(const QString &ms)
{
    return QDateTime::fromMSecsSinceEpoch(ms.toLongLong());
}

qint64 Utils::dateToSec(const QDateTime &dt)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
    return dt.toSecsSinceEpoch();
#else
   return dt.toMSecsSinceEpoch()/1000;
#endif
}

QDateTime Utils::dateFromSec(qint64 date)
{
    return QDateTime::fromSecsSinceEpoch(date);
}

QString Utils::dateToString(const QDateTime &dt, const QString & format)
{
    if(format.isEmpty())
        return dt.toString();
    else
        return dt.toString(format);
}

QDateTime Utils::datefromString(const QString &dt, const QString &format)
{
    if(format.isEmpty())
        return QDateTime::fromString(dt);
    else
        return QDateTime::fromString(dt, format);
}

QString Utils::msecToTimeString(qint32 msec, const QString &format)
{
    return QTime::fromMSecsSinceStartOfDay(msec).toString(format);
}

QDate Utils::dateAddDays(const QDate &date, int days)
{
    return date.addDays(days);
}

QStringList Utils::toStringList(const QVariant &value)
{
    return value.toStringList();
}

QVariantMap Utils::toVariantMap(const QVariant &value)
{
    return value.toMap();
}

QVariantList Utils::toVariantList(const QVariant &value)
{
    return value.toList();
}

QString Utils::byteArrayToString(const QVariant &value)
{
    return QString::fromUtf8(value.toByteArray());
}

QByteArray Utils::compress(const QByteArray &data, int level)
{
    return qCompress(data, level);
}

QByteArray Utils::uncompress(const QByteArray &data)
{
    return qUncompress(data);
}

QVariant Utils::bytesToVariant(const QByteArray &_data)
{
    QVariant result;
    QByteArray data = _data;
    QDataStream stream(&data, QIODevice::ReadOnly);
    stream >> result;
    return result;
}

QByteArray Utils::variantToBytes(const QVariant &var)
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << var;
    return result;
}

QString Utils::fileName(const QString &_path)
{
    QString path = _path;
    return QFileInfo(path).baseName();
}

QString Utils::fileSuffix(const QString &_path)
{
    QString path = _path;


    QString pathRight = path.right(10);
    QString result = pathRight.mid(pathRight.lastIndexOf(QStringLiteral("."))+1);
    if(!result.isEmpty())
        return result;

    result = QFileInfo(path).suffix().toLower();
    if(!result.isEmpty())
        return result;

    QMimeDatabase db;
    const QStringList &suffixes = db.mimeTypeForFile(path).suffixes();
    if(!suffixes.isEmpty())
        result = suffixes.first().toLower();

    return result;
}

QString Utils::fileMime(const QString &path)
{
    QMimeDatabase db;
    return db.mimeTypeForFile(path).name();
}

QString Utils::fileParent(const QString &path)
{
    if(path.count() == 1 && path[0] == '/')
        return path;
#ifdef Q_OS_WIN
    if(path.count() == 3 && path[0].isLetter() && path[1] == ':')
        return path;
#endif
    QString res = path.mid(0, path.lastIndexOf(QStringLiteral("/")));
#ifndef Q_OS_WIN
    if(res.isEmpty())
        res += "/";
#endif
    return res;
}

QString Utils::fileSize(const QString &_path)
{
    QString path = _path;
    return QString::number(QFileInfo(path).size());
}

QString Utils::readText(const QString &path)
{
    QFile file(path);
    if( !file.open(QFile::ReadOnly) )
        return QString();

    QString res = QString::fromUtf8(file.readAll());
    return res;
}

bool Utils::writeText(const QString &path, const QString &text)
{
    QFile file(path);
    if( !file.open(QFile::WriteOnly) )
        return false;

    file.write(text.toUtf8());
    file.close();
    return true;
}

bool Utils::fileExists(const QString &_path)
{
    QString path = _path;


    return QFileInfo::exists(path);
}

QStringList Utils::filesOf(const QString &path)
{
    return QDir(path).entryList(QDir::Files);
}

void Utils::callJs(QJSValue fn, QJSValue arg)
{
    QJSValueList list;
    list<<arg;
    fn.call(list);
}

QSize Utils::imageSize(const QString &_path)
{
    QString path = _path;

    QImageReader image(path);
    return image.size();
}



int Utils::extractZip(const QString &path,const QString &dir, QJSValue obj)
{
    QString program = "./unzip.exe";
    QStringList arguments;
        arguments << "-o" << path << "-d" << "./smeck/"+dir;
    QProcess *myProcess = new QProcess();
    QJSValue nothing;
    connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](){
        this->callJs(obj,nothing);
    });
        myProcess->start(program, arguments);
        return 1;
}

int Utils::imageToPdf(const QStringList paths, const QString saveTo, QJSValue obj)
{
    QFile file(saveTo);
    if( !file.open(QIODevice::WriteOnly) )
        return false;

    QPdfWriter* writer = new QPdfWriter(&file);
    writer->setPageMargins(QMargins(5,0,0,0));
    writer->setResolution(96);
    QPainter* p = new QPainter(writer);
    writer->setPageSize(QPageSize(QPageSize::A4));
    p->setPen(QColor("red"));
    int count=0;
    for(const auto &path : paths) {
        if(count!=0)
            writer->newPage();
        QPixmap pixmap(path);
        p->drawPixmap(0,0,pixmap);
        count++;
    }
    delete p;
    delete writer;
    file.close();
    this->callJs(obj);
    return 0;
}
int Utils::runCMD(const QString &path)
{
    QString program = "./smeck/server00101/py/pix.exe";
    QStringList arguments;
        //arguments << "-c" << "open('test.txt','w').write('Hello world')";
    QProcess *myProcess = new QProcess();
    connect(myProcess, &QProcess::readyReadStandardOutput, [=](){
       qDebug()<<myProcess->readAllStandardOutput();
    });
    connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](){
        QString data(myProcess->readAllStandardOutput());
        qDebug()<<data;
    });
        myProcess->start(path,arguments);//program, arguments);
        return 1;
}

int Utils::execCMD(const QString &path)
{
    QStringList arguments;
    QProcess *myProcess = new QProcess();
    connect(myProcess, &QProcess::readyReadStandardOutput, [=](){
       qDebug()<<myProcess->readAllStandardOutput();
    });
    connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](){
        QString data(myProcess->readAllStandardOutput());
        qDebug()<<data;
    });
    QString program = path.split(" ")[0];
    arguments = path.split(" ");
    arguments.removeFirst();
    qDebug() << program << arguments;
    myProcess->start(program, arguments);//program, arguments);
    return 1;
}

int Utils::runPy(const QString &path, const QJSValue argg)
{
    QString program = path;
    QStringList arguments(QString(argg.toString()).split("---"));
    QProcess *myProcess = new QProcess();
    connect(myProcess, &QProcess::readyReadStandardOutput, [=](){
       qDebug()<<myProcess->readAllStandardOutput();
    });
    connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](){
        QString data(myProcess->readAllStandardOutput());
        qDebug()<<data;
    });
        myProcess->start(program,arguments);//program, arguments);
    return 1;
}

int Utils::runCMDCallack(const QString &prog, const QString &arg,QJSValue obj)
{
    QStringList arguments;
        arguments << arg ;
    QProcess *myProcess = new QProcess();
    myProcess->setReadChannel(QProcess::StandardOutput);
    connect(myProcess, &QProcess::readyReadStandardOutput, [=](){
       //qDebug()<<myProcess->readAllStandardOutput();
    });
    connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](){
        //qDebug()<<myProcess->readAll();
        QString data(myProcess->readAllStandardOutput());
        //qDebug()<<data;
        QJSValue val(data);
        this->callJs(obj,val);
    });
        myProcess->start(prog,arguments);//program, arguments);
    return 1;
}

void Utils::imageResize(const QString &_path, const QSize &size, const QString &dest, QObject *base, std::function<void (bool)> callback)
{
    QString path = _path;

    class ResizeRunnable: public QRunnable {
        void run() {
            QImageReader reader(_path);
            reader.setScaledSize(_size);

            QImage img = reader.read();
            QImageWriter writer(_dest);
            bool res = writer.write(img);

            if(!_base)
                return;

            QObject *obj = new QObject;

            auto callback = _callback;
            obj->connect(obj, &QObject::destroyed, _base, [callback, res](){
                callback(res);
            }, Qt::QueuedConnection);

            delete obj;
        }

    public:
        QString _path;
        QSize _size;
        QString _dest;
        QPointer<QObject> _base;
        std::function<void (bool)> _callback;
    };

    ResizeRunnable *run = new ResizeRunnable;
    run->setAutoDelete(true);
    run->_path = path;
    run->_size = size;
    run->_dest = dest;
    run->_base = base;
    run->_callback = callback;

    QThreadPool::globalInstance()->start(run);
}

QObject* Utils::engine(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return new Utils();
}

void Utils::init()
{
    qmlRegisterSingletonType<Utils>("SIUtils", 1, 0, "SIUtils", &Utils::engine);
}



bool Utils::writeFile(const QString &path, const QVariant &data, bool compress)
{
    QString dir = path.left(path.lastIndexOf(QStringLiteral("/"))+1);
    QDir().mkpath(dir);

    QFile file(path);
    if(!file.open(QFile::WriteOnly))
        return false;

    QByteArray bytes;
    if(data.type() == QVariant::ByteArray)
        bytes = data.toByteArray();
    else
    {
        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream << data;
    }

    if(compress)
        bytes = qCompress(bytes);

    file.write(bytes);
    file.close();
    return true;
}

bool Utils::appendText(const QString &path, const QString &text, bool compress)
{
    QFile file(path);
    if( !file.open(QFile::Append) )
        return false;

    file.write(text.toUtf8());
    file.close();
    return true;
}

QByteArray Utils::readFile(const QString &path, bool uncompress)
{
    QFile file(path);
    if(!file.open(QFile::ReadOnly))
        return QByteArray();

    QByteArray res = file.readAll();
    file.close();

    if(uncompress)
        res = qUncompress(res);

    return res;
}

QString Utils::className(QObject *obj)
{
    QString result;
    if(obj)
        result = QString::fromUtf8(obj->metaObject()->className());

    int idx = result.indexOf(QStringLiteral("_QMLTYPE"));
    if(idx >= 0)
        result = result.left(idx+8);
    return result;
}


QUrl Utils::stringToUrl(const QString &path)
{
    return QUrl(path);
}

QString Utils::urlToLocalPath(const QUrl &url)
{
    QString res = url.toLocalFile();
    if(res.isEmpty() || url.toString().contains(QStringLiteral("#")))
        res = url.toString();
    if(res.left(6) == QStringLiteral("file:/"))
        res = res.mid(6);

    int idx1 = res.indexOf(QStringLiteral(":/"));
    if(idx1 != -1)
    {
        int idx2 = res.lastIndexOf(QStringLiteral(":/"));
        if(idx2 != -1)
            res.remove(idx1, idx2-idx1);
    }
    if(res.left(5) == QStringLiteral("qrc:/"))
        res = res.mid(3);

#ifdef Q_OS_WIN
    while(res.count() && res[0] == '/')
#else
    while(res.count() > 1 && res[0] == '/' && res[1] == '/')
#endif
          res = res.mid(1);

    return res;
}

QString Utils::qtVersion()
{
    return QString::fromUtf8(qVersion());
}

void Utils::deleteItemDelay(QObject *o, int ms)
{
    QTimer::singleShot( ms, o, SLOT(deleteLater()) );
}

qreal Utils::colorHue(const QColor &clr)
{
    return clr.hue()/359.0;
}

qreal Utils::colorLightness(const QColor &clr)
{
    return clr.lightness()/255.0;
}

qreal Utils::colorSaturation(const QColor &clr)
{
    return clr.saturation()/255.0;
}

void Utils::mkDir(const QString &pt)
{
    QString path = pt;

    QDir().mkpath(path);
}

QVariantMap Utils::colorHsl(const QColor &clr)
{
    QVariantMap res;
    res[QStringLiteral("hue")] = colorHue(clr);
    res[QStringLiteral("lightness")] = colorLightness(clr);
    res[QStringLiteral("saturation")] = colorSaturation(clr);

    return res;
}

bool Utils::createVideoThumbnail(const QString &video, const QString &output, QString ffmpegPath)
{
#ifdef Q_OS_IOS
    Q_UNUSED(video)
    Q_UNUSED(output)
    Q_UNUSED(ffmpegPath)
    return false;
#else
    if(ffmpegPath.isEmpty())
#ifdef Q_OS_WIN
        ffmpegPath = QCoreApplication::applicationDirPath() + "/ffmpeg.exe";
#else
#ifdef Q_OS_MAC
        ffmpegPath = QCoreApplication::applicationDirPath() + "/ffmpeg";
#else
    {
        if(QFileInfo::exists(QStringLiteral("/usr/bin/avconv")))
            ffmpegPath = QStringLiteral("/usr/bin/avconv");
        else
            ffmpegPath = QStringLiteral("ffmpeg");
    }
#endif
#endif

    QStringList args;
    args << QStringLiteral("-itsoffset");
    args << QStringLiteral("-4");
    args << QStringLiteral("-i");
    args << video;
    args << QStringLiteral("-vcodec");
    args << QStringLiteral("mjpeg");
    args << QStringLiteral("-vframes");
    args << QStringLiteral("1");
    args << QStringLiteral("-an");
    args << QStringLiteral("-f");
    args << QStringLiteral("rawvideo");
    args << output;
    args << QStringLiteral("-y");

    QProcess prc;
    prc.start(ffmpegPath, args);
    prc.waitForStarted();
    prc.waitForFinished();

    return prc.exitCode() == 0;
#endif
}

QString Utils::translateNumbers(QString input)
{
    input.replace(QStringLiteral("0"), Utils::tr("0"));
    input.replace(QStringLiteral("1"), Utils::tr("1"));
    input.replace(QStringLiteral("2"), Utils::tr("2"));
    input.replace(QStringLiteral("3"), Utils::tr("3"));
    input.replace(QStringLiteral("4"), Utils::tr("4"));
    input.replace(QStringLiteral("5"), Utils::tr("5"));
    input.replace(QStringLiteral("6"), Utils::tr("6"));
    input.replace(QStringLiteral("7"), Utils::tr("7"));
    input.replace(QStringLiteral("8"), Utils::tr("8"));
    input.replace(QStringLiteral("9"), Utils::tr("9"));
    return input;
}

QString Utils::trNums(QString input)
{
    return Utils::translateNumbers(input);
}

QString Utils::passToMd5(const QString &pass)
{
    if( pass.isEmpty() )
        return QString();

    return md5(pass);
}

QString Utils::md5(const QString &str)
{
    return hash(str, Md5);
}

QString Utils::passToHash(const QString &pass, int method)
{
    if( pass.isEmpty() )
        return QString();

    return hash(pass, method);
}

QString Utils::hash(const QString &str, int method)
{
    QCryptographicHash::Algorithm alg = QCryptographicHash::Md5;
    switch (method)
    {
    case Md5:
        alg = QCryptographicHash::Md5;
        break;
    case Sha224:
        alg = QCryptographicHash::Sha224;
        break;
    case Sha256:
        alg = QCryptographicHash::Sha256;
        break;
    case Sha384:
        alg = QCryptographicHash::Sha384;
        break;
    case Sha512:
        alg = QCryptographicHash::Sha512;
        break;
    case Sha3_224:
        alg = QCryptographicHash::Sha3_224;
        break;
    case Sha3_256:
        alg = QCryptographicHash::Sha3_256;
        break;
    case Sha3_384:
        alg = QCryptographicHash::Sha3_384;
        break;
    case Sha3_512:
        alg = QCryptographicHash::Sha3_512;
        break;
    }

    return QString::fromUtf8(QCryptographicHash::hash(str.toUtf8(), alg).toHex());
}

QString Utils::createUuid()
{
    return QUuid::createUuid().toString();
}


QString Utils::htmlToPlaintText(const QString &html)
{
    QTextDocument doc;
    doc.setHtml(html);
    return doc.toPlainText();
}

void Utils::copyDirectory(const QString &_src, const QString &_dst)
{
    QString src = _src;
    QString dst = _dst;

    QDir().mkpath(dst);

    const QStringList & dirs = QDir(src).entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    for(const QString &d: dirs)
        copyDirectory(src+"/"+d, dst+"/"+d);

    const QStringList & files = QDir(src).entryList(QDir::Files);
    for(const QString &f: files)
        QFile::copy(src+"/"+f, dst+"/"+f);
}

bool Utils::copy(const QString &_src, const QString &_dst)
{
    QString src = _src;
    QString dst = _dst;

    return QFile::copy(src, dst);
}

bool Utils::rename(const QString &_src, const QString &_dst)
{
    QString src = _src;
    QString dst = _dst;

    return QFile::rename(src, dst);
}

void Utils::deleteFile(const QString &pt)
{
    QString path = pt;

    QFile::remove(path);
}

void Utils::clearDirectory(const QString &pt)
{
    QString path = pt;

    const QStringList & files = QDir(path).entryList(QDir::Files);
    for(const QString &f: files)
        deleteFile(path+"/"+f);
}

void Utils::setProperty(QObject *obj, const QString &property, const QVariant &v)
{
    if( !obj || property.isEmpty() )
        return;

    obj->setProperty( property.toUtf8(), v );
}

QVariant Utils::property(QObject *obj, const QString &property)
{
    if( !obj || property.isEmpty() )
        return QVariant();

    return obj->property(property.toUtf8());
}

Qt::LayoutDirection Utils::directionOf(const QString &str)
{
    Qt::LayoutDirection res = Qt::LeftToRight;
    if( str.isEmpty() )
        return res;

    int ltr = 0;
    int rtl = 0;

    for(const QChar &ch: str)
    {
        QChar::Direction dir = ch.direction();
        switch( static_cast<int>(dir) )
        {
        case QChar::DirL:
        case QChar::DirLRE:
        case QChar::DirLRO:
        case QChar::DirEN:
            ltr++;
            break;

        case QChar::DirR:
        case QChar::DirRLE:
        case QChar::DirRLO:
        case QChar::DirAL:
            rtl++;
            break;
        }
    }

    if( ltr >= rtl )
        res = Qt::LeftToRight;
    else
        res = Qt::RightToLeft;

    return res;
}

