#ifndef SYNCTHINGCONNECTION_H
#define SYNCTHINGCONNECTION_H

#include "./syncthingdir.h"
#include "./syncthingdev.h"

#include <QObject>
#include <QList>
#include <QSslError>
#include <QTimer>

#include <functional>
#include <vector>
#include <limits>

QT_FORWARD_DECLARE_CLASS(QNetworkAccessManager)
QT_FORWARD_DECLARE_CLASS(QNetworkReply)
QT_FORWARD_DECLARE_CLASS(QNetworkRequest)
QT_FORWARD_DECLARE_CLASS(QUrlQuery)
QT_FORWARD_DECLARE_CLASS(QJsonObject)
QT_FORWARD_DECLARE_CLASS(QJsonArray)

namespace Data {

struct SyncthingConnectionSettings;

QNetworkAccessManager LIB_SYNCTHING_CONNECTOR_EXPORT &networkAccessManager();

enum class SyncthingStatus
{
    Disconnected,
    Reconnecting,
    Idle,
    Scanning,
    Paused,
    Synchronizing,
    OutOfSync,
    BeingDestroyed
};

enum class SyncthingErrorCategory
{
    OverallConnection,
    SpecificRequest,
    Parsing
};

struct LIB_SYNCTHING_CONNECTOR_EXPORT SyncthingLogEntry
{
    SyncthingLogEntry(const QString &when, const QString &message) :
        when(when),
        message(message)
    {}
    QString when;
    QString message;
};

class LIB_SYNCTHING_CONNECTOR_EXPORT SyncthingConnection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString syncthingUrl READ syncthingUrl WRITE setSyncthingUrl)
    Q_PROPERTY(QByteArray apiKey READ apiKey WRITE setApiKey)
    Q_PROPERTY(SyncthingStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool hasUnreadNotifications READ hasUnreadNotifications)
    Q_PROPERTY(bool hasOutOfSyncDirs READ hasOutOfSyncDirs)
    Q_PROPERTY(int trafficPollInterval READ trafficPollInterval WRITE setTrafficPollInterval)
    Q_PROPERTY(int devStatsPollInterval READ devStatsPollInterval WRITE setDevStatsPollInterval)
    Q_PROPERTY(QString configDir READ configDir NOTIFY configDirChanged)
    Q_PROPERTY(QString myId READ myId NOTIFY myIdChanged)
    Q_PROPERTY(int totalIncomingTraffic READ totalIncomingTraffic NOTIFY trafficChanged)
    Q_PROPERTY(int totalOutgoingTraffic READ totalOutgoingTraffic NOTIFY trafficChanged)
    Q_PROPERTY(double totalIncomingRate READ totalIncomingRate NOTIFY trafficChanged)
    Q_PROPERTY(double totalOutgoingRate READ totalOutgoingRate NOTIFY trafficChanged)

public:
    explicit SyncthingConnection(const QString &syncthingUrl = QStringLiteral("http://localhost:8080"), const QByteArray &apiKey = QByteArray(), QObject *parent = nullptr);
    ~SyncthingConnection();

    const QString &syncthingUrl() const;
    void setSyncthingUrl(const QString &url);
    const QByteArray &apiKey() const;
    void setApiKey(const QByteArray &apiKey);
    const QString &user() const;
    const QString &password() const;
    void setCredentials(const QString &user, const QString &password);
    SyncthingStatus status() const;
    QString statusText() const;
    bool isConnected() const;
    bool hasUnreadNotifications() const;
    bool hasOutOfSyncDirs() const;
    int trafficPollInterval() const;
    void setTrafficPollInterval(int trafficPollInterval);
    int devStatsPollInterval() const;
    void setDevStatsPollInterval(int devStatsPollInterval);
    int errorsPollInterval() const;
    void setErrorsPollInterval(int errorsPollInterval);
    int autoReconnectInterval() const;
    unsigned int autoReconnectTries() const;
    void setAutoReconnectInterval(int interval);
    const QString &configDir() const;
    const QString &myId() const;
    uint64 totalIncomingTraffic() const;
    uint64 totalOutgoingTraffic() const;
    double totalIncomingRate() const;
    double totalOutgoingRate() const;
    static constexpr uint64 unknownTraffic = std::numeric_limits<uint64>::max();
    const std::vector<SyncthingDir> &dirInfo() const;
    const std::vector<SyncthingDev> &devInfo() const;
    QMetaObject::Connection requestQrCode(const QString &text, std::function<void (const QByteArray &)> callback);
    QMetaObject::Connection requestLog(std::function<void (const std::vector<SyncthingLogEntry> &)> callback);
    const QList<QSslError> &expectedSslErrors();
    SyncthingDir *findDirInfo(const QString &dirId, int &row);
    SyncthingDev *findDevInfo(const QString &devId, int &row);
    SyncthingDev *findDevInfoByName(const QString &devName, int &row);
    const std::vector<SyncthingDir *> &completedDirs() const;

public Q_SLOTS:
    bool loadSelfSignedCertificate();
    bool applySettings(SyncthingConnectionSettings &connectionSettings);
    void connect();
    void connect(SyncthingConnectionSettings &connectionSettings);
    void disconnect();
    void reconnect();
    void reconnect(SyncthingConnectionSettings &connectionSettings);
    void pause(const QString &devId);
    void pauseAllDevs();
    void resume(const QString &devId);
    void resumeAllDevs();
    void rescan(const QString &dirId);
    void rescanAllDirs();
    void restart();
    void shutdown();
    void considerAllNotificationsRead();

Q_SIGNALS:
    void newConfig(const QJsonObject &config);
    void newDirs(const std::vector<SyncthingDir> &dirs);
    void newDevices(const std::vector<SyncthingDev> &devs);
    void newEvents(const QJsonArray &events);
    void dirStatusChanged(const SyncthingDir &dir, int index);
    void devStatusChanged(const SyncthingDev &dev, int index);
    void downloadProgressChanged();
    void newNotification(ChronoUtilities::DateTime when, const QString &message);
    void error(const QString &errorMessage, SyncthingErrorCategory category, int networkError);
    void statusChanged(SyncthingStatus newStatus);
    void configDirChanged(const QString &newConfigDir);
    void myIdChanged(const QString &myNewId);
    void trafficChanged(uint64 totalIncomingTraffic, uint64 totalOutgoingTraffic);
    void rescanTriggered(const QString &dirId);
    void pauseTriggered(const QString &devId);
    void resumeTriggered(const QString &devId);
    void restartTriggered();
    void shutdownTriggered();

private Q_SLOTS:
    void requestConfig();
    void requestStatus();
    void requestConnections();
    void requestErrors();
    void requestClearingErrors();
    void requestDirStatistics();
    void requestDeviceStatistics();
    void requestEvents();
    void abortAllRequests();

    void readConfig();
    void readDirs(const QJsonArray &dirs);
    void readDevs(const QJsonArray &devs);
    void readStatus();
    void readConnections();
    void readDirStatistics();
    void readDeviceStatistics();
    void readErrors();
    void readClearingErrors();
    void readEvents();
    void readStartingEvent(const QJsonObject &eventData);
    void readStatusChangedEvent(ChronoUtilities::DateTime eventTime, const QJsonObject &eventData);
    void readDownloadProgressEvent(ChronoUtilities::DateTime eventTime, const QJsonObject &eventData);
    void readDirEvent(ChronoUtilities::DateTime eventTime, const QString &eventType, const QJsonObject &eventData);
    void readDeviceEvent(ChronoUtilities::DateTime eventTime, const QString &eventType, const QJsonObject &eventData);
    void readItemStarted(ChronoUtilities::DateTime eventTime, const QJsonObject &eventData);
    void readItemFinished(ChronoUtilities::DateTime eventTime, const QJsonObject &eventData);
    void readRescan();
    void readPauseResume();
    void readRestart();
    void readShutdown();

    void continueConnecting();
    void continueReconnecting();
    void autoReconnect();
    void setStatus(SyncthingStatus status);
    void emitNotification(ChronoUtilities::DateTime when, const QString &message);

private:
    QNetworkRequest prepareRequest(const QString &path, const QUrlQuery &query, bool rest = true);
    QNetworkReply *requestData(const QString &path, const QUrlQuery &query, bool rest = true);
    QNetworkReply *postData(const QString &path, const QUrlQuery &query, const QByteArray &data = QByteArray());
    SyncthingDir *addDirInfo(std::vector<SyncthingDir> &dirs, const QString &dirId);
    SyncthingDev *addDevInfo(std::vector<SyncthingDev> &devs, const QString &devId);

    QString m_syncthingUrl;
    QByteArray m_apiKey;
    QString m_user;
    QString m_password;
    SyncthingStatus m_status;
    bool m_keepPolling;
    bool m_reconnecting;
    int m_lastEventId;
    QTimer m_trafficPollTimer;
    QTimer m_devStatsPollTimer;
    QTimer m_errorsPollTimer;
    QTimer m_autoReconnectTimer;
    unsigned int m_autoReconnectTries;
    QString m_configDir;
    QString m_myId;
    uint64 m_totalIncomingTraffic;
    uint64 m_totalOutgoingTraffic;
    double m_totalIncomingRate;
    double m_totalOutgoingRate;
    QNetworkReply *m_configReply;
    QNetworkReply *m_statusReply;
    QNetworkReply *m_connectionsReply;
    QNetworkReply *m_errorsReply;
    QNetworkReply *m_eventsReply;
    bool m_unreadNotifications;
    bool m_hasConfig;
    bool m_hasStatus;
    std::vector<SyncthingDir> m_dirs;
    std::vector<SyncthingDir *> m_syncedDirs;
    std::vector<SyncthingDir *> m_completedDirs;
    std::vector<SyncthingDev> m_devs;
    ChronoUtilities::DateTime m_lastConnectionsUpdate;
    ChronoUtilities::DateTime m_lastFileTime;
    ChronoUtilities::DateTime m_lastErrorTime;
    QString m_lastFileName;
    bool m_lastFileDeleted;
    QList<QSslError> m_expectedSslErrors;
};

/*!
 * \brief Returns the URL used to connect to Syncthing.
 */
inline const QString &SyncthingConnection::syncthingUrl() const
{
    return m_syncthingUrl;
}

/*!
 * \brief Sets the URL used to connect to Syncthing.
 */
inline void SyncthingConnection::setSyncthingUrl(const QString &url)
{
    m_syncthingUrl = url;
}

/*!
 * \brief Returns the API key used to connect to Syncthing.
 */
inline const QByteArray &SyncthingConnection::apiKey() const
{
    return m_apiKey;
}

/*!
 * \brief Sets the API key used to connect to Syncthing.
 */
inline void SyncthingConnection::setApiKey(const QByteArray &apiKey)
{
    m_apiKey = apiKey;
}

/*!
 * \brief Returns the user name which has been set using setCredentials().
 */
inline const QString &SyncthingConnection::user() const
{
    return m_user;
}

/*!
 * \brief Returns the password which has been set using setCredentials().
 */
inline const QString &SyncthingConnection::password() const
{
    return m_password;
}

/*!
 * \brief Provides credentials used for HTTP authentication.
 */
inline void SyncthingConnection::setCredentials(const QString &user, const QString &password)
{
    m_user = user, m_password = password;
}

/*!
 * \brief Returns the connection status.
 */
inline SyncthingStatus SyncthingConnection::status() const
{
    return m_status;
}

/*!
 * \brief Returns whether the connection has been established.
 */
inline bool SyncthingConnection::isConnected() const
{
    return m_status != SyncthingStatus::Disconnected && m_status != SyncthingStatus::Reconnecting;
}

/*!
 * \brief Returns whether there are unread notifications available.
 * \remarks This flag is set to true when new notifications become available. It can be unset again by calling considerAllNotificationsRead().
 */
inline bool SyncthingConnection::hasUnreadNotifications() const
{
    return m_unreadNotifications;
}

/*!
 * \brief Considers all notifications as read; hence might trigger a status update.
 */
inline void SyncthingConnection::considerAllNotificationsRead()
{
    m_unreadNotifications = false;
    requestClearingErrors();
}

/*!
 * \brief Returns the interval for polling traffic status (which currently can not be received via event API) in milliseconds.
 * \remarks Default value is 2000 milliseconds.
 */
inline int SyncthingConnection::trafficPollInterval() const
{
    return m_trafficPollTimer.interval();
}

/*!
 * \brief Sets the interval for polling traffic status (which currently can not be received via event API) in milliseconds.
 * \remarks Default value is 2000 milliseconds.
 */
inline void SyncthingConnection::setTrafficPollInterval(int trafficPollInterval)
{
    if(!trafficPollInterval) {
        m_trafficPollTimer.stop();
    }
    m_trafficPollTimer.setInterval(trafficPollInterval);
}

/*!
 * \brief Returns the interval for polling device statistics (which currently can not be received via event API) in milliseconds.
 * \remarks Default value is 60000 milliseconds.
 */
inline int SyncthingConnection::devStatsPollInterval() const
{
    return m_devStatsPollTimer.interval();
}

/*!
 * \brief Sets the interval for polling device statistics (which currently can not be received via event API) in milliseconds.
 * \remarks Default value is 60000 milliseconds.
 */
inline void SyncthingConnection::setDevStatsPollInterval(int devStatsPollInterval)
{
    if(!devStatsPollInterval) {
        m_devStatsPollTimer.stop();
    }
    m_devStatsPollTimer.setInterval(devStatsPollInterval);
}

/*!
 * \brief Returns the interval for polling Syncthing errors (which currently can not be received via event API) in milliseconds.
 * \remarks Default value is 30000 milliseconds.
 */
inline int SyncthingConnection::errorsPollInterval() const
{
    return m_errorsPollTimer.interval();
}

/*!
 * \brief Sets the interval for polling Syncthing errors (which currently can not be received via event API) in milliseconds.
 * \remarks Default value is 30000 milliseconds.
 */
inline void SyncthingConnection::setErrorsPollInterval(int errorPollInterval)
{
    if(!errorPollInterval) {
        m_errorsPollTimer.stop();
    }
    m_errorsPollTimer.setInterval(errorPollInterval);
}

/*!
 * \brief Returns the reconnect interval in milliseconds.
 * \remarks Default value is 0 which indicates disabled auto-reconnect.
 */
inline int SyncthingConnection::autoReconnectInterval() const
{
    return m_autoReconnectTimer.interval();
}

/*!
 * \brief Returns the current number of auto-reconnect tries.
 */
inline unsigned int SyncthingConnection::autoReconnectTries() const
{
    return m_autoReconnectTries;
}

/*!
 * \brief Sets the reconnect interval in milliseconds.
 * \remarks Default value is 0 which indicates disabled auto-reconnect.
 */
inline void SyncthingConnection::setAutoReconnectInterval(int interval)
{
    if(!interval) {
        m_autoReconnectTimer.stop();
    }
    m_autoReconnectTimer.setInterval(interval);
}

/*!
 * \brief Returns the Syncthing home/configuration directory.
 */
inline const QString &SyncthingConnection::configDir() const
{
    return m_configDir;
}

/*!
 * \brief Returns the ID of the own Syncthing device.
 */
inline const QString &SyncthingConnection::myId() const
{
    return m_myId;
}

/*!
 * \brief Returns the total incoming traffic in byte.
 */
inline uint64 SyncthingConnection::totalIncomingTraffic() const
{
    return m_totalIncomingTraffic;
}

/*!
 * \brief Returns the total outgoing traffic in byte.
 */
inline uint64 SyncthingConnection::totalOutgoingTraffic() const
{
    return m_totalOutgoingTraffic;
}

/*!
 * \brief Returns the total incoming transfer rate in kbit/s.
 */
inline double SyncthingConnection::totalIncomingRate() const
{
    return m_totalIncomingRate;
}

/*!
 * \brief Returns the total outgoing transfer rate in kbit/s.
 */
inline double SyncthingConnection::totalOutgoingRate() const
{
    return m_totalOutgoingRate;
}

/*!
 * \brief Returns all available directory information.
 * \remarks The returned object container object is persistent. However, the contained
 *          info objects are invalidated when the newConfig() signal is emitted.
 */
inline const std::vector<SyncthingDir> &SyncthingConnection::dirInfo() const
{
    return m_dirs;
}

/*!
 * \brief Returns all available device information.
 * \remarks The returned object container object is persistent. However, the contained
 *          info objects are invalidated when the newConfig() signal is emitted.
 */
inline const std::vector<SyncthingDev> &SyncthingConnection::devInfo() const
{
    return m_devs;
}

/*!
 * \brief Returns a list of all expected certificate errors. This is meant to allow self-signed certificates.
 * \remarks This list is updated via loadSelfSignedCertificate().
 */
inline const QList<QSslError> &SyncthingConnection::expectedSslErrors()
{
    return m_expectedSslErrors;
}

/*!
 * \brief Returns the directories which have been synchronized during the last synchronizing status().
 */
inline const std::vector<SyncthingDir *> &SyncthingConnection::completedDirs() const
{
    return m_completedDirs;
}

}

#endif // SYNCTHINGCONNECTION_H
