#include "QCefContextPrivate.h"

#pragma region qt_headers
#include <QThread>
#pragma endregion qt_headers

#include <QCefContext.h>

#include "CCefAppDelegate.h"
#include "QCefConfigPrivate.h"

const int64_t kCefWorkerIntervalMs = (1000 / 60); // 60 fps

QCefContextPrivate::QCefContextPrivate()
{
  QObject::connect(&cefWorkerTimer_, &QTimer::timeout, this, &QCefContextPrivate::performCefLoopWork);
  cefWorkerTimer_.setTimerType(Qt::PreciseTimer);
  cefWorkerTimer_.start(kCefWorkerIntervalMs);
}

QCefContextPrivate::~QCefContextPrivate() {}

bool
QCefContextPrivate::initialize(const QCefConfigPrivate* config)
{
  // create app delegate before initialization
  pAppDelegate_.reset(new CCefAppDelegate(this));

  // initialize CEF
  return initializeCef(config);
}

void
QCefContextPrivate::uninitialize()
{
  // reset delegate before cleanup
  pAppDelegate_.reset();

  // cleanup CEF
  uninitializeCef();
}

void
QCefContextPrivate::scheduleCefLoopWork(int64_t delayMs)
{
  // calculate the effective delay number
  auto delay = qMax((int64_t)0, qMin(delayMs, kCefWorkerIntervalMs));
  QTimer::singleShot(delay, this, SLOT(performCefLoopWork()));
}

void
QCefContextPrivate::performCefLoopWork()
{
  // process cef work
  CefDoMessageLoopWork();
}
