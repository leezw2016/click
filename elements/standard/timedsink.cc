#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "timedsink.hh"
#include "confparse.hh"
#include "error.hh"
#include "glue.hh"

TimedSink::TimedSink()
  : _timer(this)
{
  add_input();
}

TimedSink *
TimedSink::clone() const
{
  return new TimedSink;
}

int
TimedSink::configure(const String &conf, Router *router, ErrorHandler *errh)
{
  return cp_va_parse(conf, this, router, errh,
		     cpInterval, "packet pull interval", &_interval,
		     0);
}

int
TimedSink::initialize(Router *, ErrorHandler *)
{
  _timer.schedule_after_ms(_interval);
  return 0;
}

void
TimedSink::uninitialize(Router *)
{
  _timer.unschedule();
}

bool
TimedSink::wants_packet_upstream() const
{
  return true;
}

void
TimedSink::run_scheduled()
{
  // if timer is scheduled, we don't care if a packet is upstream.
  if (_timer.scheduled())
    return;

  Packet *p = input(0).pull();
  if (p) {
    p->kill();
    _timer.schedule_after_ms(_interval);
  }
  // otherwise, rely on PACKET_UPSTREAM to wake us up
}

EXPORT_ELEMENT(TimedSink)
