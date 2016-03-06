#ifndef QSPICEWIDGET_H
#define QSPICEWIDGET_H

/* http://www.spice-space.org/page/Whiteboard/AgentProtocol#Clipboard
 * To do so, the guest agent and the client play a symmetric role:
 * they can both claim ownership (GRAB), RELEASE ownership,
 * REQUEST clipboard data and send CLIPBOARD data. For example,
 * the GRAB message is sent after receiving a system notification of clipboard data
 * available after a Copy operation in some application.
 * When the clipboard is emptied, the grab must be RELEASEd. The other side can REQUEST
 * the data while the GRAB is active, and should expect a CLIPBOARD reply with the data.
 * Important:
 * If a GRAB message has been sent and is currently active,
 * then a successive GRAB message is received from the peer,
 * no RELEASE message should be sent to the peer for the previous active grab.
 * It has been implicitly released by the peer. Sending an extra RELEASE message
 * would only confuse the peer.
 * ------------------------------------------------------------------------------------
 * Then will implemented the copy/paste cross blocking.
 */

#include <QTimer>
#include <QLabel>
#include <QHBoxLayout>

#include "qspicesession.h"
#include "qspicemainchannel.h"
#include "qspicedisplaychannel.h"
#include "qspiceinputschannel.h"
#include "qspicecursorchannel.h"
#include "qspicesmartcardchannel.h"
#include "qspiceusbredirchannel.h"
#include "qspicewebdavchannel.h"
#include "qspiceusbdevicemanager.h"
#include "spiceusbdevicewidget.h"
#include "qspicesmartcardmanager.h"
#include "spicesmartcardwidget.h"
#include "qspiceplaybackchannel.h"
#include "qspicerecordchannel.h"
#include "qspiceaudio.h"

class QSpiceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QSpiceWidget(QWidget *parent = nullptr);
    virtual ~QSpiceWidget();

    /*
     *  Connect to guest by uri, start spice-session.
     */
    bool Connect(QString &uri);

    /*
     * Disconnect from guest, stop spice-session.
     */
    void Disconnect();

    /*
     * Send key sequience to guest.
     */
    void SendKeySequience(Qt::Key);

    /*
     * Send files to guest.
     */
    void mainFileCopyAsync(QStringList&);

    /*
     * Copy guest Clipboard current data to client Clipboard.
     */
    void copyClipboardFromGuest();

    /*
     * Send current client Clipboard data to guest Clipboard.
     */
    void sendClipboardDataToGuest(quint32, const uchar*, size_t);

signals:
    /*
     * Emitted, when display resize event occurred.
     */
    void DisplayResize(const QSize&);

    /*
     * Emitted, when new portion of files will downloaded to guest.
     */
    void downloaded(int, int);

    /*
     * Emitted the state, when channel will initiated.
     */
    void cursorChannelChanged(bool);

    /*
     * Emitted the state, when channel will initiated.
     */
    void inputsChannelChanged(bool);

    /*
     * Emitted the state, when channel will initiated.
     */
    void displayChannelChanged(bool);

    /*
     * Emitted the state, when channel will initiated.
     */
    void usbredirChannelChanged(bool);

    /*
     * Emitted the state, when channel will initiated.
     */
    void smartcardChannelChanged(bool);

    /*
     * Emitted the state, when channel will initiated.
     */
    void webdavChannelChanged(bool);

    /*
     * Emitted the state, when channel will initiated.
     */
    void playbackChannelChanged(bool);

    /*
     * Emitted the state, when channel will initiated.
     */
    void recordChannelChanged(bool);

    /*
     * Emitted, when some error occured.
     */
    void errMsg(QString&);

    /*
     * Emitted, when guest/client clipboards in processing.
     */
    void clipboardsReleased(bool);

private:
    QString                  guestName;

protected:
    QSpiceSession           *spiceSession;
    QSpiceMainChannel       *main;
    QSpiceDisplayChannel    *display;
    QSpiceInputsChannel     *inputs;
    QSpiceCursorChannel     *cursor;
    QSpiceSmartcardChannel  *smartcard;
    QSpiceUSBRedirChannel   *usbredir;
    QSpiceWebDAVChannel     *webdav;
    QSpicePlaybackChannel   *playback;
    QSpiceRecordChannel     *record;
    QSpiceUsbDeviceManager  *usbDevManager;
    QSpiceSmartcardManager  *smartcardManager;
    QSpiceAudio             *spiceAudio;

    Qt::TransformationMode   tr_mode;
    QImage                  *img;
    QLabel                  *m_Image;
    QTimer                   resizeTimer;
    QHBoxLayout             *commonLayout;
    int                      _height, _width,
                             init_h, init_w;
    qreal                    zoom;

private slots:
    void ChannelNew(QSpiceChannel *channel);
    void channelDestroyed();

    void mainAgentUpdate();
    void mainClipboardSelection(uint, void*, uint);
    void mainClipboardSelectionGrab();
    void guestClipboardSelectionRelease(uint);
    void clientClipboardSelectionRequest(uint, uint);
    void mainMouseUpdate();

    void usbDevAutoConnectFailed(QString&, QString&);
    void usbDevAdded(QString&);
    void usbDevError(QString&, QString&);
    void usbDevRemoved(QString&);

    void cardInserted(QString&);
    void cardRemoved(QString&);
    void readerAdded(QString&);
    void readerRemoved(QString&);

    void displayPrimaryCreate(
         int                 format,
         int                 width,
         int                 height,
         int                 stride,
         int                 shmid,
         void *              imgdata);

    void displayInvalidate(
        int                 x,
        int                 y,
        int                 width,
        int                 height);

    void displayPrimaryDestroy();

    void displayMark(int mark);

    void cursorSet(
            int                width,
            int                height,
            int                hot_x,
            int                hot_y,
            void *             rgba);

    bool eventFilter(QObject *object, QEvent *event);
    void resizeEvent(QResizeEvent *event);
    void reloadUsbDevList(void*);
    void reloadSmartcardList(void*);
    void resizeDone();

public slots:
    /*
     * Set basic name for shapshots.
     */
    void setGuestName(QString&);

    /*
     * Set new widget size (width, height).
     */
    void setNewSize(int, int);

    /*
     * Show up USB Device widget.
     */
    void showUsbDevWidget();

    /*
     * Show up Smartcard widget.
     */
    void showSmartCardWidget();

    /*
     * Get and save Screenshot (pixbuff) from guest.
     */
    void getScreenshot();

    /*
     * Set transformation mode for display.
     */
    void setTransformationMode(Qt::TransformationMode);
};

#endif // QSPICEWIDGET_H