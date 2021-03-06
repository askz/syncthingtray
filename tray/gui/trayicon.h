#ifndef TRAY_ICON_H
#define TRAY_ICON_H

#include "./traymenu.h"

#include <c++utilities/chrono/datetime.h>

#ifdef QT_UTILITIES_SUPPORT_DBUS_NOTIFICATIONS
# include <qtutilities/misc/dbusnotification.h>
#endif

#include <QSystemTrayIcon>
#include <QIcon>

QT_FORWARD_DECLARE_CLASS(QPixmap)

namespace Data {
enum class SyncthingStatus;
enum class SyncthingErrorCategory;
}

namespace QtGui {

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    TrayIcon(QObject *parent = nullptr);
    TrayMenu &trayMenu();

public slots:
    void showInternalError(const QString &errorMsg, Data::SyncthingErrorCategory category, int networkError);
    void showSyncthingNotification(ChronoUtilities::DateTime when, const QString &message);
    void updateStatusIconAndText(Data::SyncthingStatus status);

private slots:
    void handleActivated(QSystemTrayIcon::ActivationReason reason);
    void handleSyncthingNotificationAction(const QString &action);

private:
    QPixmap renderSvgImage(const QString &path);

    bool m_initialized;
    const QSize m_size;
    const QIcon m_statusIconDisconnected;
    const QIcon m_statusIconIdling;
    const QIcon m_statusIconScanning;
    const QIcon m_statusIconNotify;
    const QIcon m_statusIconPause;
    const QIcon m_statusIconSync;
    const QIcon m_statusIconError;
    const QIcon m_statusIconErrorSync;
    TrayMenu m_trayMenu;
    QMenu m_contextMenu;
    Data::SyncthingStatus m_status;
#ifdef QT_UTILITIES_SUPPORT_DBUS_NOTIFICATIONS
    MiscUtils::DBusNotification m_disconnectedNotification;
    MiscUtils::DBusNotification m_internalErrorNotification;
    MiscUtils::DBusNotification m_syncthingNotification;
    MiscUtils::DBusNotification m_syncCompleteNotification;
#endif
};

inline TrayMenu &TrayIcon::trayMenu()
{
    return m_trayMenu;
}

}

#endif // TRAY_ICON_H
