#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "idle.hh"
#include "bitvector.hh"

Idle::Idle()
{
  add_output();
}

Idle::~Idle()
{
}

void
Idle::notify_inputs(int n)
{
  n = (n > 1 ? 1 : n);
  add_inputs(n - ninputs());
}

void
Idle::notify_outputs(int n)
{
  n = (n > 1 ? 1 : n);
  add_outputs(n - noutputs());
}

Bitvector
Idle::forward_flow(int) const
{
  return Bitvector(noutputs(), false);
}

Bitvector
Idle::backward_flow(int) const
{
  return Bitvector(ninputs(), false);
}

void
Idle::push(int, Packet *p)
{
  p->kill();
}

Packet *
Idle::pull(int)
{
  return 0;
}

bool
Idle::wants_packet_upstream() const
{
  return input_is_pull(0);
}

void
Idle::run_scheduled()
{
  if (Packet *p = input(0).pull()) {
    p->kill();
    schedule_tail();
  }
}

EXPORT_ELEMENT(Idle)
