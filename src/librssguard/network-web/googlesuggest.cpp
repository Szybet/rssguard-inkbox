// For license of this file, see <project-root-folder>/LICENSE.md.

// You may use this file under the terms of the BSD license as follows:
//
// "Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//   * Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in
//     the documentation and/or other materials provided with the
//     distribution.
//   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
//     of its contributors may be used to endorse or promote products derived
//     from this software without specific prior written permission.
//
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."

#include "network-web/googlesuggest.h"

#include "definitions/definitions.h"
#include "gui/reusable/locationlineedit.h"
#include "network-web/downloader.h"

#include <QDomDocument>
#include <QKeyEvent>
#include <QListWidget>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTextCodec>
#include <QTimer>
#include <QXmlStreamReader>

// Works likes trash on ereader
GoogleSuggest::GoogleSuggest(LocationLineEdit* editor, QObject* parent)
  : QObject(parent), editor(editor), m_downloader(new Downloader(this)), popup(new QListWidget()),
    m_enteredText(QString()) {

  editor->installEventFilter(this);
}

bool GoogleSuggest::eventFilter(QObject* object, QEvent* event) {
  if (event->type() == QEvent::KeyPress) {
    bool consumed = false;
    const int key = static_cast<QKeyEvent*>(event)->key();

    // this doesn't work
    qDebug() << "GoogleSuggest event filter activated" << key;

    switch (key) {
      case Qt::Key_Enter: {
        consumed = true;
        qDebug() << "GoogleSuggest event filter enter clicked" << editor->text();
        if(editor->text().contains("http") == true) {
          qDebug() << "Submiting bare link";
          editor->submit(editor->text());
        } else {
          qDebug() << "Submiting google link";
          editor->submit(QSL(GOOGLE_SEARCH_URL).arg(editor->text()));
        }
      }
      case Qt::Key_Return: {
        consumed = true;
        qDebug() << "GoogleSuggest event filter enter clicked" << editor->text();
        if(editor->text().contains("http") == true) {
          qDebug() << "Submiting bare link";
          editor->submit(editor->text());
        } else {
          qDebug() << "Submiting google link";
          editor->submit(QSL(GOOGLE_SEARCH_URL).arg(editor->text()));
        }
      }

      default:
        break;
    }

    return consumed;
  }

  return false;
}

void GoogleSuggest::showCompletion(const QStringList& choices) {

}

void GoogleSuggest::doneCompletion() {

}

void GoogleSuggest::preventSuggest() {

}

void GoogleSuggest::autoSuggest() {

}

void GoogleSuggest::handleNetworkData(const QUrl& url,
                                      QNetworkReply::NetworkError status,
                                      int http_code,
                                      const QByteArray& contents) {

}
