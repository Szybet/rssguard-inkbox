// This file is part of RSS Guard.

//
// Copyright (C) 2011-2017 by Martin Rotter <rotter.martinos@gmail.com>
// Copyright (C) 2010-2014 by David Rosca <nowrep@gmail.com>
//
// RSS Guard is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RSS Guard is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RSS Guard. If not, see <http://www.gnu.org/licenses/>.

#include "services/inoreader/inoreaderserviceroot.h"

#include "miscellaneous/application.h"
#include "miscellaneous/databasequeries.h"
#include "miscellaneous/iconfactory.h"
#include "network-web/oauth2service.h"
#include "services/abstract/recyclebin.h"
#include "services/inoreader/gui/formeditinoreaderaccount.h"
#include "services/inoreader/inoreaderentrypoint.h"
#include "services/inoreader/network/inoreadernetworkfactory.h"
#include "services/inoreader/network/inoreadernetworkfactory.h"

InoreaderServiceRoot::InoreaderServiceRoot(InoreaderNetworkFactory* network, RootItem* parent) : ServiceRoot(parent),
  m_serviceMenu(QList<QAction*>()), m_network(network) {
  if (network == nullptr) {
    m_network = new InoreaderNetworkFactory(this);
  }
  else {
    m_network->setParent(this);
  }

  m_network->setService(this);
  setIcon(InoreaderEntryPoint().icon());
}

InoreaderServiceRoot::~InoreaderServiceRoot() {}

void InoreaderServiceRoot::updateTitle() {
  setTitle(m_network->userName() + QSL(" (Inoreader)"));
}

void InoreaderServiceRoot::loadFromDatabase() {
  QSqlDatabase database = qApp->database()->connection(metaObject()->className(), DatabaseFactory::FromSettings);
  Assignment categories = DatabaseQueries::getCategories(database, accountId());
  Assignment feeds = DatabaseQueries::getInoreaderFeeds(database, accountId());

  // All data are now obtained, lets create the hierarchy.
  assembleCategories(categories);
  assembleFeeds(feeds);

  // As the last item, add recycle bin, which is needed.
  appendChild(recycleBin());
  updateCounts(true);
}

void InoreaderServiceRoot::saveAccountDataToDatabase() {
  QSqlDatabase database = qApp->database()->connection(metaObject()->className(), DatabaseFactory::FromSettings);

  if (accountId() != NO_PARENT_CATEGORY) {
    if (DatabaseQueries::overwriteInoreaderAccount(database, m_network->userName(),
                                                   m_network->oauth()->refreshToken(),
                                                   m_network->batchSize(),
                                                   accountId())) {
      updateTitle();
      itemChanged(QList<RootItem*>() << this);
    }
  }
  else {
    bool saved;
    int id_to_assign = DatabaseQueries::createAccount(database, code(), &saved);

    if (saved) {
      if (DatabaseQueries::createInoreaderAccount(database, id_to_assign,
                                                  m_network->userName(),
                                                  m_network->oauth()->refreshToken(),
                                                  m_network->batchSize())) {
        setId(id_to_assign);
        setAccountId(id_to_assign);
        updateTitle();
      }
    }
  }
}

bool InoreaderServiceRoot::canBeEdited() const {
  return true;
}

bool InoreaderServiceRoot::editViaGui() {
  FormEditInoreaderAccount form_pointer(qApp->mainFormWidget());

  form_pointer.execForEdit(this);
  return true;
}

bool InoreaderServiceRoot::supportsFeedAdding() const {
  return true;
}

bool InoreaderServiceRoot::supportsCategoryAdding() const {
  return false;
}

void InoreaderServiceRoot::start(bool freshly_activated) {
  Q_UNUSED(freshly_activated)

  loadFromDatabase();
  m_network->oauth()->login();
}

void InoreaderServiceRoot::stop() {}

QList<QAction*> InoreaderServiceRoot::serviceMenu() {
  if (m_serviceMenu.isEmpty()) {
    QAction* act_sync_in = new QAction(qApp->icons()->fromTheme(QSL("view-refresh")), tr("Sync in"), this);

    connect(act_sync_in, &QAction::triggered, this, &InoreaderServiceRoot::syncIn);
    m_serviceMenu.append(act_sync_in);
  }

  return m_serviceMenu;
}

QString InoreaderServiceRoot::code() const {
  return InoreaderEntryPoint().code();
}

RootItem* InoreaderServiceRoot::obtainNewTreeForSyncIn() const {
  return m_network->feedsCategories(true);
}

void InoreaderServiceRoot::addNewFeed(const QString& url) {
  Q_UNUSED(url)
}

void InoreaderServiceRoot::addNewCategory() {}
