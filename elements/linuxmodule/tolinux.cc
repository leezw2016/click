#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "tolinux.hh"

ToLinux::ToLinux()
{
  add_input();
}

ToLinux::~ToLinux()
{
}

ToLinux *
ToLinux::clone() const
{
  return new ToLinux();
}

void
ToLinux::push(int port, Packet *p)
{
  struct sk_buff *skb1 = p->steal_skb();
  if (!skb1) return;
  
  skb1->mac.raw = skb1->data;
  skb1->protocol = skb1->mac.ethernet->h_proto;
  /* skb->pkt_type = ???; */
  
  /* skip past ether header */
  skb_pull(skb1, 14);
#ifdef HAVE_CLICK_KERNEL
  skb1->nh.raw = skb1->data;
  ptype_dispatch(skb1, skb1->protocol);
#endif
}

EXPORT_ELEMENT(ToLinux)
