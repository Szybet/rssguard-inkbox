#include "core/feedsmodelstandardfeed.h"

#include "core/defs.h"
#include "gui/iconfactory.h"
#include "gui/iconthemefactory.h"

#include <QVariant>
#include <QThread>
#include <unistd.h>


FeedsModelStandardFeed::FeedsModelStandardFeed(FeedsModelRootItem *parent_item)
  : FeedsModelFeed(parent_item) {
}

FeedsModelStandardFeed::~FeedsModelStandardFeed() {
  qDebug("Destroying FeedsModelStandardFeed instance.");
}

void FeedsModelStandardFeed::setDescription(const QString &description) {
  m_description = description;
}

FeedsModelStandardFeed *FeedsModelStandardFeed::loadFromRecord(const QSqlRecord &record) {
  FeedsModelStandardFeed *feed = new FeedsModelStandardFeed(NULL);

  feed->setTitle(record.value(FDS_DB_TITLE_INDEX).toString());
  feed->setId(record.value(FDS_DB_ID_INDEX).toInt());
  feed->setDescription(record.value(FDS_DB_DESCRIPTION_INDEX).toString());
  feed->setCreationDate(QDateTime::fromString(record.value(FDS_DB_DCREATED_INDEX).toString(),
                                              Qt::ISODate));
  feed->setIcon(IconFactory::fromByteArray(record.value(FDS_DB_ICON_INDEX).toByteArray()));
  feed->setEncoding(record.value(FDS_DB_ENCODING_INDEX).toString());
  feed->setUrl(record.value(FDS_DB_URL_INDEX).toString());
  feed->setLanguage(record.value(FDS_DB_LANGUAGE_INDEX).toString());
  feed->updateCounts();

  return feed;
}

QDateTime FeedsModelStandardFeed::creationDate() const {
  return m_creationDate;
}

void FeedsModelStandardFeed::setCreationDate(const QDateTime &creation_date) {
  m_creationDate = creation_date;
}

QString FeedsModelStandardFeed::encoding() const {
  return m_encoding;
}

void FeedsModelStandardFeed::setEncoding(const QString &encoding) {
  m_encoding = encoding;
}

QString FeedsModelStandardFeed::url() const {
  return m_url;
}

void FeedsModelStandardFeed::setUrl(const QString &url) {
  m_url = url;
}

QString FeedsModelStandardFeed::language() const {
  return m_language;
}

void FeedsModelStandardFeed::setLanguage(const QString &language) {
  m_language = language;
}

QString FeedsModelStandardFeed::description() const {
  return m_description;
}

QVariant FeedsModelStandardFeed::data(int column, int role) const {
  switch (role) {
    case Qt::DisplayRole:
      if (column == FDS_MODEL_TITLE_INDEX) {
        return m_title;
      }
      else if (column == FDS_MODEL_COUNTS_INDEX) {
        return QString("(%1)").arg(QString::number(countOfUnreadMessages()));
      }
      else {
        return QVariant();
      }

    case Qt::EditRole:
      if (column == FDS_MODEL_TITLE_INDEX) {
        return m_title;
      }
      else if (column == FDS_MODEL_COUNTS_INDEX) {
        return countOfUnreadMessages();
      }
      else {
        return QVariant();
      }

    case Qt::DecorationRole:
      return column == FDS_MODEL_TITLE_INDEX ?
            m_icon :
            QVariant();

    case Qt::ToolTipRole:
      if (column == FDS_MODEL_TITLE_INDEX) {
        return QObject::tr("%1\n\n"
                           "Feed type: %2\n"
                           "URL: %3\n"
                           "Encoding: %4\n"
                           "Language: %5").arg(m_title,
                                               FeedsModelFeed::typeToString(m_type),
                                               m_url,
                                               m_encoding,
                                               m_language.isEmpty() ?
                                                 "-" :
                                                 m_language);      }
      else if (column == FDS_MODEL_COUNTS_INDEX) {
        return QObject::tr("%n unread message(s).", "", countOfUnreadMessages());
      }
      else {
        return QVariant();
      }

    case Qt::TextAlignmentRole:
      if (column == FDS_MODEL_COUNTS_INDEX) {
        return Qt::AlignCenter;
      }
      else {
        return QVariant();
      }

    default:
      return QVariant();
  }
}

void FeedsModelStandardFeed::update() {
  usleep(5500000);
}
