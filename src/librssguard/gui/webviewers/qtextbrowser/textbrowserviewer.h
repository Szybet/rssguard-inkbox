// For license of this file, see <project-root-folder>/LICENSE.md.

#ifndef TEXTBROWSERVIEWER_H
#define TEXTBROWSERVIEWER_H

#include "gui/webviewers/webviewer.h"

#include <QTextBrowser>

#include "network-web/adblock/adblockmanager.h"

#include <QNetworkReply>
#include <QPixmap>
#include <QPointer>
#include <QTimer>

class QContextMenuEvent;
class QResizeEvent;
class WebBrowser;
class Downloader;

class TextBrowserViewer;

class TextBrowserDocument : public QTextDocument {
    Q_OBJECT

  public:
    explicit TextBrowserDocument(TextBrowserViewer* parent = nullptr);

  protected:
    virtual QVariant loadResource(int type, const QUrl& name);

  private:
    QPointer<TextBrowserViewer> m_viewer;
};

class TextBrowserViewer : public QTextBrowser, public WebViewer {
    Q_OBJECT
    Q_INTERFACES(WebViewer)

  public:
    explicit TextBrowserViewer(QWidget* parent = nullptr);

    virtual QSize sizeHint() const;

  public:
    QVariant loadOneResource(int type, const QUrl& name);

  public:
    virtual void bindToBrowser(WebBrowser* browser);
    virtual void findText(const QString& text, bool backwards);
    virtual void setUrl(const QUrl& url);
    virtual void setHtml(const QString& html, const QUrl& base_url = {});
    virtual QString html() const;
    virtual QUrl url() const;
    virtual void clear();
    virtual void loadMessages(const QList<Message>& messages, RootItem* root);
    virtual double verticalScrollBarPosition() const;
    virtual void setVerticalScrollBarPosition(double pos);
    virtual void applyFont(const QFont& fon);
    virtual qreal zoomFactor() const;
    virtual void setZoomFactor(qreal zoom_factor);

  protected:
    virtual void contextMenuEvent(QContextMenuEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void wheelEvent(QWheelEvent* event);

  public slots:
    void latestUrlGet(QString url);

  private slots:
    void enableResources(bool enable);
    void openLinkInExternalBrowser();
    void downloadLink();
    void onAnchorClicked(const QUrl& url);

    bool resourcesEnabled() const;
    void setResourcesEnabled(bool enabled);

  signals:
    void reloadDocument();

  private slots:
    void reloadHtmlDelayed();
    void downloadNextNeededResource();
    void resourceDownloaded(const QUrl& url,
                            QNetworkReply::NetworkError status,
                            int http_code,
                            QByteArray contents = QByteArray());

  private:
    bool m_resourcesEnabled;
    QList<QUrl> m_neededResources; // All URLs here must be resolved.
    QScopedPointer<Downloader> m_resourceDownloader;
    QMap<QUrl, QByteArray> m_loadedResources; // All URLs here must be resolved.
    QPixmap m_placeholderImage;
    QPixmap m_placeholderImageError;

  signals:
    void pageTitleChanged(const QString& new_title);
    void pageUrlChanged(const QUrl& url);
    void pageIconChanged(const QIcon&);
    void linkMouseHighlighted(const QUrl& url);
    void loadingStarted();
    void loadingProgress(int progress);
    void loadingFinished(bool success);
    void newWindowRequested(WebViewer* viewer);
    void closeWindowRequested();

  private:
    void setHtmlPrivate(const QString& html, const QUrl& base_url);

    BlockingResult blockedWithAdblock(const QUrl& url);
    QScopedPointer<Downloader> m_downloader;
    PreparedHtml prepareHtmlForMessage(const QList<Message>& messages, RootItem* selected_item) const;

  private:
    QUrl m_currentUrl;
    QString m_currentHtml;

    QPointer<RootItem> m_root;
    QFont m_baseFont;
    qreal m_zoomFactor = 1.0;
    QScopedPointer<QAction> m_actionEnableResources;
    QScopedPointer<QAction> m_actionOpenExternalBrowser;
    QScopedPointer<QAction> m_actionDownloadLink;
    QScopedPointer<TextBrowserDocument> m_document;
    QPoint m_lastContextMenuPos;
    QString latestUrlRed;
    QUrl latestUsedUrlInFun;
};

#endif // TEXTBROWSERVIEWER_H
