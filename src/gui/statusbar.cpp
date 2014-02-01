#include "gui/statusbar.h"

#include "gui/iconthemefactory.h"

#include <QToolButton>
#include <QLabel>
#include <QProgressBar>


StatusBar::StatusBar(QWidget *parent) : QStatusBar(parent) {
  setSizeGripEnabled(false);
  setContentsMargins(0, 0, 0, 0);

  // Initializations of widgets for status bar.
  m_fullscreenSwitcher = new QToolButton(this);
  m_fullscreenSwitcher->setAutoRaise(true);
  m_fullscreenSwitcher->setIcon(IconThemeFactory::instance()->fromTheme("view-fullscreen"));
  m_fullscreenSwitcher->setText(tr("Fullscreen mode"));
  m_fullscreenSwitcher->setToolTip(tr("Switch application between fulscreen/normal states right from this status bar icon."));

  m_progressBar = new QProgressBar(this);
  m_progressBar->setTextVisible(false);
  m_progressBar->setFixedWidth(100);
  m_progressBar->setVisible(false);

  m_progressLabel = new QLabel(this);
  m_progressLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  m_progressLabel->setVisible(false);

  // Add widgets.
  addWidget(m_progressBar);
  addWidget(m_progressLabel);
  addPermanentWidget(m_fullscreenSwitcher);

  setStyleSheet("StatusBar::item { border: none; }");
}

StatusBar::~StatusBar() {
  qDebug("Destroying StatusBar instance.");
}



void StatusBar::showProgress(int progress, const QString &label) {
  m_progressLabel->setVisible(true);
  m_progressBar->setVisible(true);

  m_progressLabel->setText(label);
  m_progressBar->setValue(progress);
}

void StatusBar::clearProgress() {
  m_progressLabel->setVisible(false);
  m_progressBar->setVisible(false);
}
