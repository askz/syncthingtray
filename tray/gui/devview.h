#ifndef DEVVIEW_H
#define DEVVIEW_H

#include <QTreeView>

namespace Data {
struct SyncthingDev;
}

namespace QtGui {

class DevView : public QTreeView
{
    Q_OBJECT
public:
    DevView(QWidget *parent = nullptr);

Q_SIGNALS:
    void pauseResumeDev(const Data::SyncthingDev &dev);

protected:
    void mouseReleaseEvent(QMouseEvent *event);

private Q_SLOTS:
    void showContextMenu();
    void copySelectedItem();
    void copySelectedItemId();

};

}

#endif // DEVVIEW_H
