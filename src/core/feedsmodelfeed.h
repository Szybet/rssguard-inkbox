#ifndef FEEDSMODELFEED_H
#define FEEDSMODELFEED_H

#include "core/feedsmodelrootitem.h"


// Represents BASE class for feeds contained in FeedsModel.
// NOTE: This class should be derived to create PARTICULAR feed types.
class FeedsModelFeed : public FeedsModelRootItem {
  public:
    // Describes possible types of feeds.
    // NOTE: This is equivalent to attribute Feeds(type).
    enum Type {
      StandardRss   = 0,
      StandardRdf   = 1,
      StandardAtom  = 2
    };

    // Constructors and destructors.
    explicit FeedsModelFeed(FeedsModelRootItem *parent_item = NULL);
    virtual ~FeedsModelFeed();

    int childCount() const;

    int countOfUnreadMessages() const;
    int countOfAllMessages() const;

  protected:
    int m_totalCount;
    int m_unreadCount;
};

#endif // FEEDSMODELFEED_H
