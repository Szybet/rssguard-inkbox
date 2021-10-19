// For license of this file, see <project-root-folder>/LICENSE.md.

#include "gui/reusable/progressbarwithtext.h"

#include "definitions/definitions.h"
#include "miscellaneous/application.h"

ProgressBarWithText::ProgressBarWithText(QWidget* parent) : QProgressBar(parent) {
  //setSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Preferred);
}

/*
   QSize ProgressBarWithText::minimumSizeHint() const {
   QString txt = text();

   return QSize(fontMetrics().boundingRect(txt).width() + 32,
               QProgressBar::minimumSizeHint().height());
   }

   QSize ProgressBarWithText::sizeHint() const {
   QString txt = text();

   return QSize(fontMetrics().boundingRect(txt).width() + 32,
               QProgressBar::sizeHint().height());
   }
 */

QString ProgressBarWithText::text() const {
  qint64 total_steps = qint64(maximum()) - minimum();
  QString result = format();
  QLocale locale;

  locale.setNumberOptions(locale.numberOptions() | QLocale::OmitGroupSeparator);
  result.replace(QLatin1String("%m"), locale.toString(total_steps));
  result.replace(QLatin1String("%v"), locale.toString(value()));

  // If max and min are equal and we get this far, it means that the
  // progress bar has one step and that we are on that step. Return
  // 100% here in order to avoid division by zero further down.
  if (total_steps == 0) {
    result.replace(QLatin1String("%p"), locale.toString(100));
    return result;
  }

  const auto progress = static_cast<int>((qint64(value()) - minimum()) * 100.0 / total_steps);

  result.replace(QLatin1String("%p"), locale.toString(progress));
  return result;
}
