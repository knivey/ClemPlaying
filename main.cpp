#include <QCoreApplication>
#include <QtDBus/QtDBus>

#define SRV_NAME "org.mpris.clementine"
#define SRV_PATH "/Player"
#define SRV_IFACE ""

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (!QDBusConnection::sessionBus().isConnected()) {
        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                        "To start it, run:\n"
                        "\teval `dbus-launch --auto-syntax`\n");
        return 1;
    }

    QDBusInterface iface(SRV_NAME, SRV_PATH, SRV_IFACE, QDBusConnection::sessionBus());
    if (!iface.isValid()) {
        fprintf(stderr, "DBus Error: %s\n",
                qPrintable(QDBusConnection::sessionBus().lastError().message()));
        return 1;
    }

    QDBusReply<QMap<QString, QVariant> > reply = iface.call("GetMetadata");
    if (!reply.isValid()) {
        fprintf(stderr, "Call failed: %s\n", qPrintable(reply.error().message()));
        return 1;
    }

    QMap<QString, QString> rpl;
    for(auto k : {"title","artist","album","year","genre","audio-bitrate"}) {
        if(!reply.value().contains(k)) {
            rpl[k] = QString("");
        } else {
            rpl[k] = reply.value()[k].toString();
        }
    }

    QString o("");
    QTextStream out(&o);
    if(rpl["artist"] != "") {
        out << rpl["artist"] << " - ";
    }
    if(rpl["title"] != "") {
        out << rpl["title"];
    } else {
        out << "?";
    }
    out << " ";
    if(rpl["album"] != "" && rpl["year"] != "") {
        out << "[" << rpl["album"] << ", " << rpl["year"] << "] ";
    } else if(rpl["album"] != "" || rpl["year"] != "") {
        out << "[" << rpl["album"] << rpl["year"] << "] ";
    }
    if(rpl["genre"] != "") {
        out << "-- " << rpl["genre"] << " ";
    }
    if(rpl["audio-bitrate"] != "") {
        out << "[" << rpl["audio-bitrate"] << "Kbps]";
    }

    puts(qPrintable(o));
    return 0;
}

