#ifndef ROUTERT_HH
#define ROUTERT_HH
#include "vector.hh"
#include "hashmap.hh"
#include "elementt.hh"
#include "error.hh"

class RouterT : public ElementClassT {

  HashMap<String, int> _element_type_map;
  Vector<String> _element_type_names;
  Vector<ElementClassT *> _element_classes;
  
  HashMap<String, int> _element_name_map;
  Vector<ElementT> _elements;	// contains types
  
  Vector<Hookup> _hookup_from;
  Vector<Hookup> _hookup_to;
  Vector<String> _hookup_landmark;

  void remove_bad_connections();
  void finish_remove_elements(Vector<int> &, ErrorHandler *);
  void expand_pseudoport(Vector<Hookup> *, bool is_input, int which,
			 Vector<Hookup> &results) const;
  
 public:

  static const int PSEUDOPORT_TYPE = 0;
  
  RouterT();
  RouterT(const RouterT &);
  virtual ~RouterT();

  int ntypes() const			{ return _element_classes.size(); }
  const String &type_name(int i) const	{ return _element_type_names[i]; }
  ElementClassT *element_class(int i) const { return _element_classes[i]; }
  int type_index(const String &s) const { return _element_type_map[s]; }
  int get_type_index(const String &, ElementClassT * = 0);
  int get_anon_type_index(const String &, ElementClassT * = 0);

  int nelements() const			{ return _elements.size(); }
  int findex(const String &s) const	{ return _element_name_map[s]; }
  const ElementT &element(int i) const	{ return _elements[i]; }
  ElementT &element(int i)		{ return _elements[i]; }
  String fname(int findex) const;
  int ftype(int findex) const;
  const String &fconfiguration(int i) const;
  String &fconfiguration(int i)		{ return _elements[i].configuration; }
  int fflags(int i) const		{ return _elements[i].flags; }
  int get_findex(const String &name, int ftype_index = -1, const String &configuration = String(), const String &landmark = String());
  
  int nhookup() const			{ return _hookup_from.size(); }
  const Vector<Hookup> &hookup_from() const { return _hookup_from; }
  Vector<Hookup> &hookup_from()		{ return _hookup_from; }
  const Vector<Hookup> &hookup_to() const { return _hookup_to; }
  Vector<Hookup> &hookup_to()		{ return _hookup_to; }
  const String &hookup_landmark(int i) const { return _hookup_landmark[i]; }
 
  void add_pseudoport_pair(String, String, const String &, ErrorHandler *);
  
  bool add_connection(const Hookup &, const Hookup &, const String &landmark = String());
  bool add_connection(int fidx, int fport, int tport, int tidx);
  void remove_connection(int);

  bool has_connection(const Hookup &, const Hookup &) const;
  void find_connections_from(const Hookup &, Vector<Hookup> &) const;
  void find_connections_to(const Hookup &, Vector<Hookup> &) const;
  
  bool next_element_match(RouterT *, Vector<int> &) const;
  bool next_connection_match(RouterT *, Vector<int> &) const;
  bool next_exclusive_connection_match(RouterT *, Vector<int> &) const;

  void add_components_to(RouterT *, const String &prefix = String()) const;

  void expand_compound(ElementT &, RouterT *, ErrorHandler *);
  
  void remove_unused_element_types();
  void remove_blank_elements(ErrorHandler * = 0);
  void remove_unconnected_elements();
  void remove_duplicate_connections();
  void remove_pseudoports(ErrorHandler *);
  void remove_compound_elements(ErrorHandler *);

  void compound_declaration_string(StringAccum &, const String &);
  void configuration_string(StringAccum &, const String & = String()) const;
  String configuration_string() const;

  RouterT *cast_router()		{ return this; }

};


inline String
RouterT::fname(int fidx) const
{
  if (fidx >= 0 && fidx < _elements.size())
    return _elements[fidx].name;
  else
    return String("/*BAD_") + String(fidx) + String("*/");
}

inline int
RouterT::ftype(int fidx) const
{
  if (fidx >= 0 && fidx < _elements.size())
    return _elements[fidx].type;
  else
    return -1;
}

inline const String &
RouterT::fconfiguration(int i) const
{
  return _elements[i].configuration;
}

inline bool
RouterT::add_connection(int fidx, int fport, int tport, int tidx)
{
  return add_connection(Hookup(fidx, fport), Hookup(tidx, tport));
}

#endif
