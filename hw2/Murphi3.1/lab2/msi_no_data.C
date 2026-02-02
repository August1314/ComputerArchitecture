/******************************
  Program "msi_no_data.m" compiled by "Murphi Release 3.1"

  Murphi Last Modefied Date: "Jan 29 1999"
  Murphi Last Compiled date: "Nov 17 2025"
 ******************************/

/********************
  Parameter
 ********************/
#define MURPHI_VERSION "Murphi Release 3.1"
#define MURPHI_DATE "Jan 29 1999"
#define PROTOCOL_NAME "msi_no_data"
#define BITS_IN_WORLD 56
#define ALIGN

/********************
  Include
 ********************/
#include "mu_prolog.inc"

/********************
  Decl declaration
 ********************/

class mu_1_StateType: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1_StateType& val) { return value(val.value()); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1_StateType& val)
  {
    if (val.defined())
      return ( s << mu_1_StateType::values[ int(val) - 1] );
    else return ( s << "Undefined" );
  };

  mu_1_StateType (char *name, int os): mu__byte(1, 3, 2, name, os) {};
  mu_1_StateType (void): mu__byte(1, 3, 2) {};
  mu_1_StateType (int val): mu__byte(1, 3, 2, "Parameter or function result.", 0)
  {
     operator=(val);
  };
  char * Name() { return values[ value() -1]; };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
  virtual void print()
  {
    if (defined())
      cout << name << ":" << values[ value() -1] << '\n';
    else
      cout << name << ":Undefined\n";
  };
};

char *mu_1_StateType::values[] = {"M","S","I",NULL };

/*** end of enum declaration ***/
mu_1_StateType mu_1_StateType_undefined_var;

class mu_1_CacheId: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1_CacheId& val){ return value(val.value());};
  inline operator int() const { return value(); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1_CacheId& val)
    {
      if (val.defined())
        return ( s << mu_1_CacheId::values[ int(val) - 4 ] );
      else
        return ( s << "Undefined" );
    };

  mu_1_CacheId (char *name, int os): mu__byte(4, 6, 2, name, os) {};
  mu_1_CacheId (void): mu__byte(4, 6, 2) {};
  mu_1_CacheId (int val): mu__byte(4, 6, 2, "Parameter or function result.", 0)
    { operator=(val); };
  char * Name() { return values[ value() -4]; };
  virtual void print()
    {
      if (defined()) cout << name << ':' << values[ value() - 4] << '\n';
      else cout << name << ":Undefined\n";
    };
  void print_statistic() {};
friend int CompareWeight(mu_1_CacheId& a, mu_1_CacheId& b)
{
  if (!a.defined() && b.defined())
    return -1;
  else if (a.defined() && !b.defined())
    return 1;
  else
    return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
};
char *mu_1_CacheId::values[] =
  { "CacheId_1","CacheId_2","CacheId_3",NULL };

/*** end scalarset declaration ***/
mu_1_CacheId mu_1_CacheId_undefined_var;

class mu_1_CountType: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu_1_CountType& val) { return mu__byte::operator=((int) val); };
  mu_1_CountType (char *name, int os): mu__byte(0, 3, 3, name, os) {};
  mu_1_CountType (void): mu__byte(0, 3, 3) {};
  mu_1_CountType (int val): mu__byte(0, 3, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu_1_CountType mu_1_CountType_undefined_var;

class mu_1_CacheLine
{
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  void set_self(char *n, int os);
  mu_1_StateType mu_state;
  mu_1_CacheLine ( char *n, int os ) { set_self(n,os); };
  mu_1_CacheLine ( void ) {};

  virtual ~mu_1_CacheLine(); 
friend int CompareWeight(mu_1_CacheLine& a, mu_1_CacheLine& b)
  {
    int w;
    w = CompareWeight(a.mu_state, b.mu_state);
    if (w!=0) return w;
  return 0;
}
friend int Compare(mu_1_CacheLine& a, mu_1_CacheLine& b)
  {
    int w;
    w = Compare(a.mu_state, b.mu_state);
    if (w!=0) return w;
  return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    mu_state.MultisetSort();
  }
  void print_statistic()
  {
    mu_state.print_statistic();
  }
  void clear() {
    mu_state.clear();
 };
  void undefine() {
    mu_state.undefine();
 };
  void reset() {
    mu_state.reset();
 };
  void print() {
    mu_state.print();
  };
  void print_diff(state *prevstate) {
    mu_state.print_diff(prevstate);
  };
  void to_state(state *thestate) {
    mu_state.to_state(thestate);
  };
virtual bool isundefined() { Error.Error("Checking undefinedness of a non-base type"); return TRUE;}
virtual bool ismember() { Error.Error("Checking membership for a non-base type"); return TRUE;}
  mu_1_CacheLine& operator= (const mu_1_CacheLine& from) {
    mu_state.value(from.mu_state.value());
    return *this;
  };
};

  void mu_1_CacheLine::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1_CacheLine::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1_CacheLine::set_self(char *n, int os)
{
  name = n;
  mu_state.set_self_2(name, ".state", os + 0 );
}

mu_1_CacheLine::~mu_1_CacheLine()
{
}

/*** end record declaration ***/
mu_1_CacheLine mu_1_CacheLine_undefined_var;

class mu_1_Cache
{
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  void set_self(char *n, int os);
  mu_1_CacheLine mu_line;
  mu_1_Cache ( char *n, int os ) { set_self(n,os); };
  mu_1_Cache ( void ) {};

  virtual ~mu_1_Cache(); 
friend int CompareWeight(mu_1_Cache& a, mu_1_Cache& b)
  {
    int w;
    w = CompareWeight(a.mu_line, b.mu_line);
    if (w!=0) return w;
  return 0;
}
friend int Compare(mu_1_Cache& a, mu_1_Cache& b)
  {
    int w;
    w = Compare(a.mu_line, b.mu_line);
    if (w!=0) return w;
  return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    mu_line.MultisetSort();
  }
  void print_statistic()
  {
    mu_line.print_statistic();
  }
  void clear() {
    mu_line.clear();
 };
  void undefine() {
    mu_line.undefine();
 };
  void reset() {
    mu_line.reset();
 };
  void print() {
    mu_line.print();
  };
  void print_diff(state *prevstate) {
    mu_line.print_diff(prevstate);
  };
  void to_state(state *thestate) {
    mu_line.to_state(thestate);
  };
virtual bool isundefined() { Error.Error("Checking undefinedness of a non-base type"); return TRUE;}
virtual bool ismember() { Error.Error("Checking membership for a non-base type"); return TRUE;}
  mu_1_Cache& operator= (const mu_1_Cache& from) {
    mu_line = from.mu_line;
    return *this;
  };
};

  void mu_1_Cache::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1_Cache::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1_Cache::set_self(char *n, int os)
{
  name = n;
  mu_line.set_self_2(name, ".line", os + 0 );
}

mu_1_Cache::~mu_1_Cache()
{
}

/*** end record declaration ***/
mu_1_Cache mu_1_Cache_undefined_var;

class mu_1_MemBlock
{
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  void set_self(char *n, int os);
  mu_0_boolean mu_valid;
  mu_1_MemBlock ( char *n, int os ) { set_self(n,os); };
  mu_1_MemBlock ( void ) {};

  virtual ~mu_1_MemBlock(); 
friend int CompareWeight(mu_1_MemBlock& a, mu_1_MemBlock& b)
  {
    int w;
    w = CompareWeight(a.mu_valid, b.mu_valid);
    if (w!=0) return w;
  return 0;
}
friend int Compare(mu_1_MemBlock& a, mu_1_MemBlock& b)
  {
    int w;
    w = Compare(a.mu_valid, b.mu_valid);
    if (w!=0) return w;
  return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    mu_valid.MultisetSort();
  }
  void print_statistic()
  {
    mu_valid.print_statistic();
  }
  void clear() {
    mu_valid.clear();
 };
  void undefine() {
    mu_valid.undefine();
 };
  void reset() {
    mu_valid.reset();
 };
  void print() {
    mu_valid.print();
  };
  void print_diff(state *prevstate) {
    mu_valid.print_diff(prevstate);
  };
  void to_state(state *thestate) {
    mu_valid.to_state(thestate);
  };
virtual bool isundefined() { Error.Error("Checking undefinedness of a non-base type"); return TRUE;}
virtual bool ismember() { Error.Error("Checking membership for a non-base type"); return TRUE;}
  mu_1_MemBlock& operator= (const mu_1_MemBlock& from) {
    mu_valid.value(from.mu_valid.value());
    return *this;
  };
};

  void mu_1_MemBlock::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1_MemBlock::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1_MemBlock::set_self(char *n, int os)
{
  name = n;
  mu_valid.set_self_2(name, ".valid", os + 0 );
}

mu_1_MemBlock::~mu_1_MemBlock()
{
}

/*** end record declaration ***/
mu_1_MemBlock mu_1_MemBlock_undefined_var;

class mu_1_BusReqType: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1_BusReqType& val) { return value(val.value()); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1_BusReqType& val)
  {
    if (val.defined())
      return ( s << mu_1_BusReqType::values[ int(val) - 7] );
    else return ( s << "Undefined" );
  };

  mu_1_BusReqType (char *name, int os): mu__byte(7, 9, 2, name, os) {};
  mu_1_BusReqType (void): mu__byte(7, 9, 2) {};
  mu_1_BusReqType (int val): mu__byte(7, 9, 2, "Parameter or function result.", 0)
  {
     operator=(val);
  };
  char * Name() { return values[ value() -7]; };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
  virtual void print()
  {
    if (defined())
      cout << name << ":" << values[ value() -7] << '\n';
    else
      cout << name << ":Undefined\n";
  };
};

char *mu_1_BusReqType::values[] = {"BusNone","BusRd","BusRdX",NULL };

/*** end of enum declaration ***/
mu_1_BusReqType mu_1_BusReqType_undefined_var;

class mu_1_BusCtrlType: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1_BusCtrlType& val) { return value(val.value()); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1_BusCtrlType& val)
  {
    if (val.defined())
      return ( s << mu_1_BusCtrlType::values[ int(val) - 10] );
    else return ( s << "Undefined" );
  };

  mu_1_BusCtrlType (char *name, int os): mu__byte(10, 11, 2, name, os) {};
  mu_1_BusCtrlType (void): mu__byte(10, 11, 2) {};
  mu_1_BusCtrlType (int val): mu__byte(10, 11, 2, "Parameter or function result.", 0)
  {
     operator=(val);
  };
  char * Name() { return values[ value() -10]; };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
  virtual void print()
  {
    if (defined())
      cout << name << ":" << values[ value() -10] << '\n';
    else
      cout << name << ":Undefined\n";
  };
};

char *mu_1_BusCtrlType::values[] = {"CtrlNone","CtrlFlush",NULL };

/*** end of enum declaration ***/
mu_1_BusCtrlType mu_1_BusCtrlType_undefined_var;

class mu_1_Bus
{
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  void set_self(char *n, int os);
  mu_1_BusReqType mu_req;
  mu_1_CacheId mu_source;
  mu_1_BusCtrlType mu_ctrl;
  mu_1_Bus ( char *n, int os ) { set_self(n,os); };
  mu_1_Bus ( void ) {};

  virtual ~mu_1_Bus(); 
friend int CompareWeight(mu_1_Bus& a, mu_1_Bus& b)
  {
    int w;
    w = CompareWeight(a.mu_req, b.mu_req);
    if (w!=0) return w;
    w = CompareWeight(a.mu_source, b.mu_source);
    if (w!=0) return w;
    w = CompareWeight(a.mu_ctrl, b.mu_ctrl);
    if (w!=0) return w;
  return 0;
}
friend int Compare(mu_1_Bus& a, mu_1_Bus& b)
  {
    int w;
    w = Compare(a.mu_req, b.mu_req);
    if (w!=0) return w;
    w = Compare(a.mu_source, b.mu_source);
    if (w!=0) return w;
    w = Compare(a.mu_ctrl, b.mu_ctrl);
    if (w!=0) return w;
  return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    mu_req.MultisetSort();
    mu_source.MultisetSort();
    mu_ctrl.MultisetSort();
  }
  void print_statistic()
  {
    mu_req.print_statistic();
    mu_source.print_statistic();
    mu_ctrl.print_statistic();
  }
  void clear() {
    mu_req.clear();
    mu_source.clear();
    mu_ctrl.clear();
 };
  void undefine() {
    mu_req.undefine();
    mu_source.undefine();
    mu_ctrl.undefine();
 };
  void reset() {
    mu_req.reset();
    mu_source.reset();
    mu_ctrl.reset();
 };
  void print() {
    mu_req.print();
    mu_source.print();
    mu_ctrl.print();
  };
  void print_diff(state *prevstate) {
    mu_req.print_diff(prevstate);
    mu_source.print_diff(prevstate);
    mu_ctrl.print_diff(prevstate);
  };
  void to_state(state *thestate) {
    mu_req.to_state(thestate);
    mu_source.to_state(thestate);
    mu_ctrl.to_state(thestate);
  };
virtual bool isundefined() { Error.Error("Checking undefinedness of a non-base type"); return TRUE;}
virtual bool ismember() { Error.Error("Checking membership for a non-base type"); return TRUE;}
  mu_1_Bus& operator= (const mu_1_Bus& from) {
    mu_req.value(from.mu_req.value());
    mu_source.value(from.mu_source.value());
    mu_ctrl.value(from.mu_ctrl.value());
    return *this;
  };
};

  void mu_1_Bus::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1_Bus::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1_Bus::set_self(char *n, int os)
{
  name = n;
  mu_req.set_self_2(name, ".req", os + 0 );
  mu_source.set_self_2(name, ".source", os + 8 );
  mu_ctrl.set_self_2(name, ".ctrl", os + 16 );
}

mu_1_Bus::~mu_1_Bus()
{
}

/*** end record declaration ***/
mu_1_Bus mu_1_Bus_undefined_var;

class mu_1__type_0
{
 public:
  mu_1_Cache array[ 3 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_0 (char *n, int os) { set_self(n, os); };
  mu_1__type_0 ( void ) {};
  virtual ~mu_1__type_0 ();
  mu_1_Cache& operator[] (int index) /* const */
  {
#ifndef NO_RUN_TIME_CHECKING
    if ( ( index >= 4 ) && ( index <= 6 ) )
      return array[ index - 4 ];
    else
      {
        if (index==UNDEFVAL) 
          Error.Error("Indexing to %s using an undefined value.", name);
        else
          Error.Error("Funny index value %d for %s: CacheId is internally represented from 6 to 4.\nInternal Error in Type checking.",index, name);
        return array[0];
      }
#else
    return array[ index - 4 ];
#endif
  };
  mu_1__type_0& operator= (const mu_1__type_0& from)
  {
    for (int i = 0; i < 3; i++)
      array[i] = from.array[i];
    return *this;
  }

friend int CompareWeight(mu_1__type_0& a, mu_1__type_0& b)
  {
    return 0;
  }
friend int Compare(mu_1__type_0& a, mu_1__type_0& b)
  {
    int w;
    for (int i=0; i<3; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<3; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<3; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 3; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 3; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 3; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 3; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 3; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 3; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_0::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_0::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_0::set_self( char *n, int os)
  {
    int i=0;
    name = n;
array[i].set_self_ar(n,"CacheId_1", i * 8 + os);i++;
array[i].set_self_ar(n,"CacheId_2", i * 8 + os);i++;
array[i].set_self_ar(n,"CacheId_3", i * 8 + os);i++;
}
mu_1__type_0::~mu_1__type_0()
{
}
/*** end array declaration ***/
mu_1__type_0 mu_1__type_0_undefined_var;

const int mu_NUM_CACHE = 3;
const int mu_NUM_MEM = 1;
const int mu_M = 1;
const int mu_S = 2;
const int mu_I = 3;
const int mu_CacheId_1 = 4;
const int mu_CacheId_2 = 5;
const int mu_CacheId_3 = 6;
const int mu_BusNone = 7;
const int mu_BusRd = 8;
const int mu_BusRdX = 9;
const int mu_CtrlNone = 10;
const int mu_CtrlFlush = 11;
/*** Variable declaration ***/
mu_1__type_0 mu_caches("caches",0);

/*** Variable declaration ***/
mu_1_MemBlock mu_mem("mem",24);

/*** Variable declaration ***/
mu_1_Bus mu_bus("bus",32);

mu_1_CountType mu_CountModifiedCache()
{
/*** Variable declaration ***/
mu_1_CountType mu_cnt("cnt",0);

mu_cnt = 0;
{
for(int mu_i = 4; mu_i <= 6; mu_i++) {
if ( (mu_caches[mu_i].mu_line.mu_state) == (mu_M) )
{
mu_cnt = (mu_cnt) + (1);
}
};
};
return mu_cnt;
  Error.Error("The end of function CountModifiedCache reached without returning values.");
};
/*** end function declaration ***/

void mu_Snoop(const mu_1_CacheId& mu_i)
{
if ( (mu_bus.mu_req) == (mu_BusRd) )
{
switch ((int) mu_caches[mu_i].mu_line.mu_state) {
case mu_M:
mu_bus.mu_ctrl = mu_CtrlFlush;
mu_caches[mu_i].mu_line.mu_state = mu_S;
break;
case mu_S:
break;
case mu_I:
break;
}
}
else
{
if ( (mu_bus.mu_req) == (mu_BusRdX) )
{
switch ((int) mu_caches[mu_i].mu_line.mu_state) {
case mu_M:
mu_bus.mu_ctrl = mu_CtrlFlush;
mu_caches[mu_i].mu_line.mu_state = mu_I;
break;
case mu_S:
mu_caches[mu_i].mu_line.mu_state = mu_I;
break;
case mu_I:
break;
}
}
}
};
/*** end procedure declaration ***/





/********************
  The world
 ********************/
void world_class::clear()
{
  mu_caches.clear();
  mu_mem.clear();
  mu_bus.clear();
}
void world_class::undefine()
{
  mu_caches.undefine();
  mu_mem.undefine();
  mu_bus.undefine();
}
void world_class::reset()
{
  mu_caches.reset();
  mu_mem.reset();
  mu_bus.reset();
}
void world_class::print()
{
  static int num_calls = 0; /* to ward off recursive calls. */
  if ( num_calls == 0 ) {
    num_calls++;
  mu_caches.print();
  mu_mem.print();
  mu_bus.print();
    num_calls--;
}
}
void world_class::print_statistic()
{
  static int num_calls = 0; /* to ward off recursive calls. */
  if ( num_calls == 0 ) {
    num_calls++;
  mu_caches.print_statistic();
  mu_mem.print_statistic();
  mu_bus.print_statistic();
    num_calls--;
}
}
void world_class::print_diff( state *prevstate )
{
  if ( prevstate != NULL )
  {
    mu_caches.print_diff(prevstate);
    mu_mem.print_diff(prevstate);
    mu_bus.print_diff(prevstate);
  }
  else
print();
}
void world_class::to_state(state *newstate)
{
  mu_caches.to_state( newstate );
  mu_mem.to_state( newstate );
  mu_bus.to_state( newstate );
}
void world_class::setstate(state *thestate)
{
}


/********************
  Rule declarations
 ********************/
/******************** RuleBase0 ********************/
class RuleBase0
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    return tsprintf("Process bus transaction");
  }
  bool Condition(unsigned r)
  {
  return (mu_bus.mu_req) != (mu_BusNone);
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 0;
    while (what_rule < 1 )
      {
        if ( ( TRUE  ) ) {
              if ((mu_bus.mu_req) != (mu_BusNone)) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 0;
    }
  }

  void Code(unsigned r)
  {
{
for(int mu_i = 4; mu_i <= 6; mu_i++) {
if ( (mu_i) != (mu_bus.mu_source) )
{
mu_Snoop ( mu_i );
}
};
};
if ( (mu_bus.mu_req) == (mu_BusRd) )
{
mu_caches[mu_bus.mu_source].mu_line.mu_state = mu_S;
}
else
{
if ( (mu_bus.mu_req) == (mu_BusRdX) )
{
mu_caches[mu_bus.mu_source].mu_line.mu_state = mu_M;
}
}
if ( (mu_bus.mu_ctrl) == (mu_CtrlFlush) )
{
mu_mem.mu_valid = mu_true;
}
mu_bus.mu_req = mu_BusNone;
mu_bus.mu_source.undefine();
mu_bus.mu_ctrl = mu_CtrlNone;
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase1 ********************/
class RuleBase1
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_CacheId mu_i;
    mu_i.value((r % 3) + 4);
    r = r / 3;
    return tsprintf("PrWr, Cache State I, i:%s", mu_i.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_CacheId mu_i;
    mu_i.value((r % 3) + 4);
    r = r / 3;
bool mu__boolexpr1;
  if (!((mu_caches[mu_i].mu_line.mu_state) == (mu_I))) mu__boolexpr1 = FALSE ;
  else {
  mu__boolexpr1 = ((mu_bus.mu_req) == (mu_BusNone)) ; 
}
  return mu__boolexpr1;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 1;
    static mu_1_CacheId mu_i;
    mu_i.value((r % 3) + 4);
    r = r / 3;
    while (what_rule < 4 )
      {
        if ( ( TRUE  ) ) {
bool mu__boolexpr2;
  if (!((mu_caches[mu_i].mu_line.mu_state) == (mu_I))) mu__boolexpr2 = FALSE ;
  else {
  mu__boolexpr2 = ((mu_bus.mu_req) == (mu_BusNone)) ; 
}
              if (mu__boolexpr2) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 1;
    mu_i.value((r % 3) + 4);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_CacheId mu_i;
    mu_i.value((r % 3) + 4);
    r = r / 3;
mu_bus.mu_req = mu_BusRdX;
mu_bus.mu_source = mu_i;
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase2 ********************/
class RuleBase2
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_CacheId mu_i;
    mu_i.value((r % 3) + 4);
    r = r / 3;
    return tsprintf("PrWr, Cache State S, i:%s", mu_i.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_CacheId mu_i;
    mu_i.value((r % 3) + 4);
    r = r / 3;
bool mu__boolexpr3;
  if (!((mu_caches[mu_i].mu_line.mu_state) == (mu_S))) mu__boolexpr3 = FALSE ;
  else {
  mu__boolexpr3 = ((mu_bus.mu_req) == (mu_BusNone)) ; 
}
  return mu__boolexpr3;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 4;
    static mu_1_CacheId mu_i;
    mu_i.value((r % 3) + 4);
    r = r / 3;
    while (what_rule < 7 )
      {
        if ( ( TRUE  ) ) {
bool mu__boolexpr4;
  if (!((mu_caches[mu_i].mu_line.mu_state) == (mu_S))) mu__boolexpr4 = FALSE ;
  else {
  mu__boolexpr4 = ((mu_bus.mu_req) == (mu_BusNone)) ; 
}
              if (mu__boolexpr4) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 4;
    mu_i.value((r % 3) + 4);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_CacheId mu_i;
    mu_i.value((r % 3) + 4);
    r = r / 3;
mu_bus.mu_req = mu_BusRdX;
mu_bus.mu_source = mu_i;
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase3 ********************/
class RuleBase3
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_CacheId mu_i;
    mu_i.value((r % 3) + 4);
    r = r / 3;
    return tsprintf("PrRd, Cache State I, i:%s", mu_i.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_CacheId mu_i;
    mu_i.value((r % 3) + 4);
    r = r / 3;
bool mu__boolexpr5;
  if (!((mu_caches[mu_i].mu_line.mu_state) == (mu_I))) mu__boolexpr5 = FALSE ;
  else {
  mu__boolexpr5 = ((mu_bus.mu_req) == (mu_BusNone)) ; 
}
  return mu__boolexpr5;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 7;
    static mu_1_CacheId mu_i;
    mu_i.value((r % 3) + 4);
    r = r / 3;
    while (what_rule < 10 )
      {
        if ( ( TRUE  ) ) {
bool mu__boolexpr6;
  if (!((mu_caches[mu_i].mu_line.mu_state) == (mu_I))) mu__boolexpr6 = FALSE ;
  else {
  mu__boolexpr6 = ((mu_bus.mu_req) == (mu_BusNone)) ; 
}
              if (mu__boolexpr6) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 7;
    mu_i.value((r % 3) + 4);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_CacheId mu_i;
    mu_i.value((r % 3) + 4);
    r = r / 3;
mu_bus.mu_req = mu_BusRd;
mu_bus.mu_source = mu_i;
  };

  bool UnFair()
  { return FALSE; }
};
class NextStateGenerator
{
  RuleBase0 R0;
  RuleBase1 R1;
  RuleBase2 R2;
  RuleBase3 R3;
public:
void SetNextEnabledRule(unsigned & what_rule)
{
  category = CONDITION;
  if (what_rule<1)
    { R0.NextRule(what_rule);
      if (what_rule<1) return; }
  if (what_rule>=1 && what_rule<4)
    { R1.NextRule(what_rule);
      if (what_rule<4) return; }
  if (what_rule>=4 && what_rule<7)
    { R2.NextRule(what_rule);
      if (what_rule<7) return; }
  if (what_rule>=7 && what_rule<10)
    { R3.NextRule(what_rule);
      if (what_rule<10) return; }
}
bool Condition(unsigned r)
{
  category = CONDITION;
  if (r<=0) return R0.Condition(r-0);
  if (r>=1 && r<=3) return R1.Condition(r-1);
  if (r>=4 && r<=6) return R2.Condition(r-4);
  if (r>=7 && r<=9) return R3.Condition(r-7);
Error.Notrace("Internal: NextStateGenerator -- checking condition for nonexisting rule.");
}
void Code(unsigned r)
{
  if (r<=0) { R0.Code(r-0); return; } 
  if (r>=1 && r<=3) { R1.Code(r-1); return; } 
  if (r>=4 && r<=6) { R2.Code(r-4); return; } 
  if (r>=7 && r<=9) { R3.Code(r-7); return; } 
}
int Priority(unsigned short r)
{
  if (r<=0) { return R0.Priority(); } 
  if (r>=1 && r<=3) { return R1.Priority(); } 
  if (r>=4 && r<=6) { return R2.Priority(); } 
  if (r>=7 && r<=9) { return R3.Priority(); } 
}
char * Name(unsigned r)
{
  if (r<=0) return R0.Name(r-0);
  if (r>=1 && r<=3) return R1.Name(r-1);
  if (r>=4 && r<=6) return R2.Name(r-4);
  if (r>=7 && r<=9) return R3.Name(r-7);
  return NULL;
}
};
const unsigned numrules = 10;

/********************
  parameter
 ********************/
#define RULES_IN_WORLD 10


/********************
  Startstate records
 ********************/
/******************** StartStateBase0 ********************/
class StartStateBase0
{
public:
  char * Name(unsigned short r)
  {
    return tsprintf("Init");
  }
  void Code(unsigned short r)
  {
{
for(int mu_i = 4; mu_i <= 6; mu_i++) {
mu_caches[mu_i].mu_line.mu_state = mu_I;
};
};
mu_mem.mu_valid = mu_true;
mu_bus.mu_req = mu_BusNone;
mu_bus.mu_source.undefine();
mu_bus.mu_ctrl = mu_CtrlNone;
  };

  bool UnFair()
  { return FALSE; }
};
class StartStateGenerator
{
  StartStateBase0 S0;
public:
void Code(unsigned short r)
{
  if (r<=0) { S0.Code(r-0); return; }
}
char * Name(unsigned short r)
{
  if (r<=0) return S0.Name(r-0);
  return NULL;
}
};
unsigned short StartStateManager::numstartstates = 1;

/********************
  Invariant records
 ********************/
int mu__invariant_7() // Invariant "Modified Cache implies no Shared Cache"
{
bool mu__boolexpr8;
  if (!((mu_CountModifiedCache(  )) == (1))) mu__boolexpr8 = TRUE ;
  else {
bool mu__quant9; 
mu__quant9 = TRUE;
{
for(int mu_i = 4; mu_i <= 6; mu_i++) {
if ( !((mu_caches[mu_i].mu_line.mu_state) != (mu_S)) )
  { mu__quant9 = FALSE; break; }
};
};
  mu__boolexpr8 = (mu__quant9) ; 
}
return mu__boolexpr8;
};

bool mu__condition_10() // Condition for Rule "Modified Cache implies no Shared Cache"
{
  return mu__invariant_7( );
}

/**** end rule declaration ****/

int mu__invariant_11() // Invariant "Only one Cache may be Modified"
{
return (mu_CountModifiedCache(  )) <= (1);
};

bool mu__condition_12() // Condition for Rule "Only one Cache may be Modified"
{
  return mu__invariant_11( );
}

/**** end rule declaration ****/

const rulerec invariants[] = {
{"Only one Cache may be Modified", &mu__condition_12, NULL, FALSE},
{"Modified Cache implies no Shared Cache", &mu__condition_10, NULL, FALSE},
};
const unsigned short numinvariants = 2;

/******************/
bool mu__true_live() { return TRUE; }
/******************/

/********************
  Liveness records
 ********************/
const liverec livenesses[] = {
{ NULL, NULL, NULL, NULL, E }};
const unsigned short numlivenesses = 0;

/********************
  Fairstates records
 ********************/
const rulerec fairnesses[] = {
{ NULL, NULL, NULL, FALSE }};
const unsigned short numfairnesses = 0;

/********************
  Normal/Canonicalization for scalarset
 ********************/
/*
mem:NoScalarset
bus:ScalarsetVariable
caches:ScalarsetArrayOfFree
*/

/********************
Code for symmetry
 ********************/

/********************
 Permutation Set Class
 ********************/
class PermSet
{
public:
  // book keeping
  enum PresentationType {Simple, Explicit};
  PresentationType Presentation;

  void ResetToSimple();
  void ResetToExplicit();
  void SimpleToExplicit();
  void SimpleToOne();
  bool NextPermutation();

  void Print_in_size()
  { int ret=0; for (int i=0; i<count; i++) if (in[i]) ret++; cout << "in_size:" << ret << "\n"; }


  /********************
   Simple and efficient representation
   ********************/
  int class_mu_1_CacheId[3];
  int undefined_class_mu_1_CacheId;// has the highest class number

  void Print_class_mu_1_CacheId();
  bool OnlyOneRemain_mu_1_CacheId;
  bool MTO_class_mu_1_CacheId()
  {
    int i,j;
    if (OnlyOneRemain_mu_1_CacheId)
      return FALSE;
    for (i=0; i<3; i++)
      for (j=0; j<3; j++)
        if (i!=j && class_mu_1_CacheId[i]== class_mu_1_CacheId[j])
	    return TRUE;
    OnlyOneRemain_mu_1_CacheId = TRUE;
    return FALSE;
  }
  bool AlreadyOnlyOneRemain;
  bool MoreThanOneRemain();


  /********************
   Explicit representation
  ********************/
  unsigned long size;
  unsigned long count;
  // in will be of product of factorial sizes for fast canonicalize
  // in will be of size 1 for reduced local memory canonicalize
  bool * in;

  // auxiliary for explicit representation

  // in/perm/revperm will be of factorial size for fast canonicalize
  // they will be of size 1 for reduced local memory canonicalize
  // second range will be size of the scalarset
  int * in_mu_1_CacheId;
  typedef int arr_mu_1_CacheId[3];
  arr_mu_1_CacheId * perm_mu_1_CacheId;
  arr_mu_1_CacheId * revperm_mu_1_CacheId;

  int size_mu_1_CacheId[3];
  bool reversed_sorted_mu_1_CacheId(int start, int end);
  void reverse_reversed_mu_1_CacheId(int start, int end);

  // procedure for explicit representation
  bool ok0(mu_1_CacheId* perm, int size, mu_1_CacheId k);
  void GenPerm0(mu_1_CacheId* perm, int size, unsigned long& index);

  // General procedure
  PermSet();
  bool In(int i) const { return in[i]; };
  void Add(int i) { for (int j=0; j<i; j++) in[j] = FALSE;};
  void Remove(int i) { in[i] = FALSE; };
};
void PermSet::Print_class_mu_1_CacheId()
{
  cout << "class_mu_1_CacheId:\t";
  for (int i=0; i<3; i++)
    cout << class_mu_1_CacheId[i];
  cout << " " << undefined_class_mu_1_CacheId << "\n";
}
bool PermSet::MoreThanOneRemain()
{
  int i,j;
  if (AlreadyOnlyOneRemain)
    return FALSE;
  else {
    for (i=0; i<3; i++)
      for (j=0; j<3; j++)
        if (i!=j && class_mu_1_CacheId[i]== class_mu_1_CacheId[j])
	    return TRUE;
  }
  AlreadyOnlyOneRemain = TRUE;
  return FALSE;
}
PermSet::PermSet()
: Presentation(Simple)
{
  int i,j,k;
  if (  args->sym_alg.mode == argsym_alg::Exhaustive_Fast_Canonicalize) {
    mu_1_CacheId Perm0[3];

  /********************
   declaration of class variables
  ********************/
  in = new bool[6];
 in_mu_1_CacheId = new int[6];
 perm_mu_1_CacheId = new arr_mu_1_CacheId[6];
 revperm_mu_1_CacheId = new arr_mu_1_CacheId[6];

    // Set perm and revperm
    count = 0;
    for (i=4; i<=6; i++)
      {
        Perm0[0].value(i);
        GenPerm0(Perm0, 1, count);
      }
    if (count!=6)
      Error.Error( "unable to initialize PermSet");
    for (i=0; i<6; i++)
      for (j=4; j<=6; j++)
        for (k=4; k<=6; k++)
          if (revperm_mu_1_CacheId[i][k-4]==j)   // k - base 
            perm_mu_1_CacheId[i][j-4]=k; // j - base 

    // setting up combination of permutations
    // for different scalarset
    int carry;
    int i_mu_1_CacheId = 0;
    size = 6;
    count = 6;
    for (i=0; i<6; i++)
      {
        carry = 1;
        in[i]= TRUE;
      in_mu_1_CacheId[i] = i_mu_1_CacheId;
      i_mu_1_CacheId += carry;
      if (i_mu_1_CacheId >= 6) { i_mu_1_CacheId = 0; carry = 1; } 
      else { carry = 0; } 
    }
  }
  else
  {

  /********************
   declaration of class variables
  ********************/
  in = new bool[1];
 in_mu_1_CacheId = new int[1];
 perm_mu_1_CacheId = new arr_mu_1_CacheId[1];
 revperm_mu_1_CacheId = new arr_mu_1_CacheId[1];
  in[0] = TRUE;
    in_mu_1_CacheId[0] = 0;
  }
}
void PermSet::ResetToSimple()
{
  int i;
  for (i=0; i<3; i++)
    class_mu_1_CacheId[i]=0;
  undefined_class_mu_1_CacheId=0;
  OnlyOneRemain_mu_1_CacheId = FALSE;

  AlreadyOnlyOneRemain = FALSE;
  Presentation = Simple;
}
void PermSet::ResetToExplicit()
{
  for (int i=0; i<6; i++) in[i] = TRUE;
  Presentation = Explicit;
}
void PermSet::SimpleToExplicit()
{
  int i,j,k;
  int start, class_size;
  int start_mu_1_CacheId[3];
  int size_mu_1_CacheId[3];
  bool should_be_in_mu_1_CacheId[6];

  // Setup range for mapping
  start = 0;
  for (j=0; j<=undefined_class_mu_1_CacheId; j++) // class number
    {
      class_size = 0;
      for (k=0; k<3; k++) // step through class_mu_1_pid[k]
	if (class_mu_1_CacheId[k]==j)
	  class_size++;
      for (k=0; k<3; k++) // step through class_mu_1_pid[k]
	if (class_mu_1_CacheId[k]==j)
	  {
	    size_mu_1_CacheId[k] = class_size;
	    start_mu_1_CacheId[k] = start;
	  }
      start+=class_size;
    }

  // To be In or not to be
  for (i=0; i<6; i++) // set up
    should_be_in_mu_1_CacheId[i] = TRUE;
  for (i=0; i<6; i++) // to be in or not to be
    for (k=0; k<3; k++) // step through class_mu_1_pid[k]
      if (! (perm_mu_1_CacheId[i][k]-4 >=start_mu_1_CacheId[k] 
	     && perm_mu_1_CacheId[i][k]-4 < start_mu_1_CacheId[k] + size_mu_1_CacheId[k]) )
  	    {
	      should_be_in_mu_1_CacheId[i] = FALSE;
	      break;
	    }

  // setup explicit representation 
  // Set perm and revperm
  for (i=0; i<6; i++)
    {
      in[i] = TRUE;
      if (in[i] && !should_be_in_mu_1_CacheId[in_mu_1_CacheId[i]]) in[i] = FALSE;
    }
  Presentation = Explicit;
  if (args->test_parameter1.value==0) Print_in_size();
}
void PermSet::SimpleToOne()
{
  int i,j,k;
  int class_size;
  int start;


  // Setup range for mapping
  start = 0;
  for (j=0; j<=undefined_class_mu_1_CacheId; j++) // class number
    {
      class_size = 0;
      for (k=0; k<3; k++) // step through class_mu_1_pid[k]
	if (class_mu_1_CacheId[k]==j)
	  class_size++;
      for (k=0; k<3; k++) // step through class_mu_1_pid[k]
	if (class_mu_1_CacheId[k]==j)
	  {
	    size_mu_1_CacheId[k] = class_size;
	  }
      start+=class_size;
    }
  start = 0;
  for (j=0; j<=undefined_class_mu_1_CacheId; j++) // class number
    {
      for (k=0; k<3; k++) // step through class_mu_1_pid[k]
	    if (class_mu_1_CacheId[k]==j)
	      revperm_mu_1_CacheId[0][start++] = k+4;
    }
  for (j=0; j<3; j++)
    for (k=0; k<3; k++)
      if (revperm_mu_1_CacheId[0][k]==j+4)
        perm_mu_1_CacheId[0][j]=k+4;
  Presentation = Explicit;
}
bool PermSet::ok0(mu_1_CacheId* Perm, int size, mu_1_CacheId k)
{
  for (int i=0; i<size; i++)
    if(Perm[i].value()==k)
      return FALSE;
  return TRUE;
}
void PermSet::GenPerm0(mu_1_CacheId* Perm,int size, unsigned long& count)
{
  int i;
  if (size!=3)
    {
      for (i=4; i<=6; i++)
        if(ok0(Perm,size,i))
          {
            Perm[size].value(i);
            GenPerm0(Perm, size+1, count);
          }
    }
  else
    {
      for (i=4; i<=6; i++)
        revperm_mu_1_CacheId[count][i-4]=Perm[i-4].value();// i - base
      count++;
    }
}
bool PermSet::reversed_sorted_mu_1_CacheId(int start, int end)
{
  int i,j;

  for (i=start; i<end; i++)
    if (revperm_mu_1_CacheId[0][i]<revperm_mu_1_CacheId[0][i+1])
      return FALSE;
  return TRUE;
}
void PermSet::reverse_reversed_mu_1_CacheId(int start, int end)
{
  int i,j;
  int temp;

  for (i=start, j=end; i<j; i++,j--) 
    {
      temp = revperm_mu_1_CacheId[0][j];
      revperm_mu_1_CacheId[0][j] = revperm_mu_1_CacheId[0][i];
      revperm_mu_1_CacheId[0][i] = temp;
    }
}
bool PermSet::NextPermutation()
{
  bool nexted = FALSE;
  int start, end; 
  int class_size;
  int temp;
  int j,k;

  // algorithm
  // for each class
  //   if forall in the same class reverse_sorted, 
  //     { sort again; goto next class }
  //   else
  //     {
  //       nexted = TRUE;
  //       for (j from l to r)
  // 	       if (for all j+ are reversed sorted)
  // 	         {
  // 	           swap j, j+1
  // 	           sort all j+ again
  // 	           break;
  // 	         }
  //     }
  for (start = 0; start < 3; )
    {
      end = start-1+size_mu_1_CacheId[revperm_mu_1_CacheId[0][start]-4];
      if (reversed_sorted_mu_1_CacheId(start,end))
	       {
	  reverse_reversed_mu_1_CacheId(start,end);
	  start = end+1;
	}
      else
	{
	  nexted = TRUE;
	  for (j = start; j<end; j++)
	    {
	      if (reversed_sorted_mu_1_CacheId(j+1,end))
		{
		  for (k = end; k>j; k--)
		    {
		      if (revperm_mu_1_CacheId[0][j]<revperm_mu_1_CacheId[0][k])
			{
			  // swap j, k
			  temp = revperm_mu_1_CacheId[0][j];
			  revperm_mu_1_CacheId[0][j] = revperm_mu_1_CacheId[0][k];
			  revperm_mu_1_CacheId[0][k] = temp;
			  break;
			}
		    }
		  reverse_reversed_mu_1_CacheId(j+1,end);
		  break;
		}
	    }
	  break;
	}
    }
if (!nexted) return FALSE;
  for (j=0; j<3; j++)
    for (k=0; k<3; k++)
      if (revperm_mu_1_CacheId[0][k]==j+4)   // k - base 
	perm_mu_1_CacheId[0][j]=k+4; // j - base 
  return TRUE;
}

/********************
 Symmetry Class
 ********************/
class SymmetryClass
{
  PermSet Perm;
  bool BestInitialized;
  state BestPermutedState;

  // utilities
  void SetBestResult(int i, state* temp);
  void ResetBestResult() {BestInitialized = FALSE;};

public:
  // initializer
  SymmetryClass() : Perm(), BestInitialized(FALSE) {};
  ~SymmetryClass() {};

  void Normalize(state* s);

  void Exhaustive_Fast_Canonicalize(state *s);
  void Heuristic_Fast_Canonicalize(state *s);
  void Heuristic_Small_Mem_Canonicalize(state *s);
  void Heuristic_Fast_Normalize(state *s);

  void MultisetSort(state* s);
};


/********************
 Symmetry Class Members
 ********************/
void SymmetryClass::MultisetSort(state* s)
{
        mu_mem.MultisetSort();
        mu_bus.MultisetSort();
        mu_caches.MultisetSort();
}
void SymmetryClass::Normalize(state* s)
{
  switch (args->sym_alg.mode) {
  case argsym_alg::Exhaustive_Fast_Canonicalize:
    Exhaustive_Fast_Canonicalize(s);
    break;
  case argsym_alg::Heuristic_Fast_Canonicalize:
    Heuristic_Fast_Canonicalize(s);
    break;
  case argsym_alg::Heuristic_Small_Mem_Canonicalize:
    Heuristic_Small_Mem_Canonicalize(s);
    break;
  case argsym_alg::Heuristic_Fast_Normalize:
    Heuristic_Fast_Normalize(s);
    break;
  default:
    Heuristic_Fast_Canonicalize(s);
  }
}

/********************
 Permute and Canonicalize function for different types
 ********************/
void mu_1_StateType::Permute(PermSet& Perm, int i) {};
void mu_1_StateType::SimpleCanonicalize(PermSet& Perm) {};
void mu_1_StateType::Canonicalize(PermSet& Perm) {};
void mu_1_StateType::SimpleLimit(PermSet& Perm) {};
void mu_1_StateType::ArrayLimit(PermSet& Perm) {};
void mu_1_StateType::Limit(PermSet& Perm) {};
void mu_1_StateType::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for enum type.\n"); };
void mu_1_CacheId::Permute(PermSet& Perm, int i)
{
  if (Perm.Presentation != PermSet::Explicit)
    Error.Error("Internal Error: Wrong Sequence of Normalization");
  if (defined())
    value(Perm.perm_mu_1_CacheId[Perm.in_mu_1_CacheId[i]][value()-4]); // value - base
};
void mu_1_CacheId::SimpleCanonicalize(PermSet& Perm)
{
  int i, class_number;
  if (Perm.Presentation != PermSet::Simple)
    Error.Error("Internal Error: Wrong Sequence of Normalization");

  if (defined())
    if (Perm.class_mu_1_CacheId[value()-4]==Perm.undefined_class_mu_1_CacheId) // value - base
      {
        // it has not been mapped to any particular value
        for (i=0; i<3; i++)
          if (Perm.class_mu_1_CacheId[i] == Perm.undefined_class_mu_1_CacheId && i!=value()-4)
            Perm.class_mu_1_CacheId[i]++;
        value(4 + Perm.undefined_class_mu_1_CacheId++);
      }
    else 
      {
        value(Perm.class_mu_1_CacheId[value()-4]+4);
      }
}
void mu_1_CacheId::Canonicalize(PermSet& Perm)
{
  Error.Error("Calling canonicalize() for Scalarset.");
}
void mu_1_CacheId::SimpleLimit(PermSet& Perm)
{
  int i, class_number;
  if (Perm.Presentation != PermSet::Simple)
    Error.Error("Internal Error: Wrong Sequence of Normalization");

  if (defined())
    if (Perm.class_mu_1_CacheId[value()-4]==Perm.undefined_class_mu_1_CacheId) // value - base
      {
        // it has not been mapped to any particular value
        for (i=0; i<3; i++)
          if (Perm.class_mu_1_CacheId[i] == Perm.undefined_class_mu_1_CacheId && i!=value()-4)
            Perm.class_mu_1_CacheId[i]++;
        Perm.undefined_class_mu_1_CacheId++;
      }
}
void mu_1_CacheId::ArrayLimit(PermSet& Perm) {}
void mu_1_CacheId::Limit(PermSet& Perm) {}
void mu_1_CacheId::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset type.\n"); };
void mu_1_CountType::Permute(PermSet& Perm, int i) {};
void mu_1_CountType::SimpleCanonicalize(PermSet& Perm) {};
void mu_1_CountType::Canonicalize(PermSet& Perm) {};
void mu_1_CountType::SimpleLimit(PermSet& Perm) {};
void mu_1_CountType::ArrayLimit(PermSet& Perm) {};
void mu_1_CountType::Limit(PermSet& Perm) {};
void mu_1_CountType::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu_1_CacheLine::Permute(PermSet& Perm, int i)
{
};
void mu_1_CacheLine::SimpleCanonicalize(PermSet& Perm)
{ Error.Error("Internal: Simple Canonicalization of Record with no scalarset variable\n"); };
void mu_1_CacheLine::Canonicalize(PermSet& Perm)
{
};
void mu_1_CacheLine::SimpleLimit(PermSet& Perm){}
void mu_1_CacheLine::ArrayLimit(PermSet& Perm){}
void mu_1_CacheLine::Limit(PermSet& Perm)
{
};
void mu_1_CacheLine::MultisetLimit(PermSet& Perm)
{
};
void mu_1_Cache::Permute(PermSet& Perm, int i)
{
};
void mu_1_Cache::SimpleCanonicalize(PermSet& Perm)
{ Error.Error("Internal: Simple Canonicalization of Record with no scalarset variable\n"); };
void mu_1_Cache::Canonicalize(PermSet& Perm)
{
};
void mu_1_Cache::SimpleLimit(PermSet& Perm){}
void mu_1_Cache::ArrayLimit(PermSet& Perm){}
void mu_1_Cache::Limit(PermSet& Perm)
{
};
void mu_1_Cache::MultisetLimit(PermSet& Perm)
{
};
void mu_1_MemBlock::Permute(PermSet& Perm, int i)
{
};
void mu_1_MemBlock::SimpleCanonicalize(PermSet& Perm)
{ Error.Error("Internal: Simple Canonicalization of Record with no scalarset variable\n"); };
void mu_1_MemBlock::Canonicalize(PermSet& Perm)
{
};
void mu_1_MemBlock::SimpleLimit(PermSet& Perm){}
void mu_1_MemBlock::ArrayLimit(PermSet& Perm){}
void mu_1_MemBlock::Limit(PermSet& Perm)
{
};
void mu_1_MemBlock::MultisetLimit(PermSet& Perm)
{
};
void mu_1_BusReqType::Permute(PermSet& Perm, int i) {};
void mu_1_BusReqType::SimpleCanonicalize(PermSet& Perm) {};
void mu_1_BusReqType::Canonicalize(PermSet& Perm) {};
void mu_1_BusReqType::SimpleLimit(PermSet& Perm) {};
void mu_1_BusReqType::ArrayLimit(PermSet& Perm) {};
void mu_1_BusReqType::Limit(PermSet& Perm) {};
void mu_1_BusReqType::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for enum type.\n"); };
void mu_1_BusCtrlType::Permute(PermSet& Perm, int i) {};
void mu_1_BusCtrlType::SimpleCanonicalize(PermSet& Perm) {};
void mu_1_BusCtrlType::Canonicalize(PermSet& Perm) {};
void mu_1_BusCtrlType::SimpleLimit(PermSet& Perm) {};
void mu_1_BusCtrlType::ArrayLimit(PermSet& Perm) {};
void mu_1_BusCtrlType::Limit(PermSet& Perm) {};
void mu_1_BusCtrlType::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for enum type.\n"); };
void mu_1_Bus::Permute(PermSet& Perm, int i)
{
  mu_source.Permute(Perm,i);
};
void mu_1_Bus::SimpleCanonicalize(PermSet& Perm)
{
  mu_source.SimpleCanonicalize(Perm);
};
void mu_1_Bus::Canonicalize(PermSet& Perm)
{
};
void mu_1_Bus::SimpleLimit(PermSet& Perm)
{
  mu_source.SimpleLimit(Perm);
};
void mu_1_Bus::ArrayLimit(PermSet& Perm){}
void mu_1_Bus::Limit(PermSet& Perm)
{
};
void mu_1_Bus::MultisetLimit(PermSet& Perm)
{
};
void mu_1__type_0::Permute(PermSet& Perm, int i)
{
  static mu_1__type_0 temp("Permute_mu_1__type_0",-1);
  int j;
  for (j=0; j<3; j++)
    array[j].Permute(Perm, i);
  temp = *this;
  for (j=4; j<=6; j++)
    (*this)[j] = temp[Perm.revperm_mu_1_CacheId[Perm.in_mu_1_CacheId[i]][j-4]];};
void mu_1__type_0::SimpleCanonicalize(PermSet& Perm)
{ Error.Error("Internal: Simple Canonicalization of Scalarset Array\n"); };
void mu_1__type_0::Canonicalize(PermSet& Perm)
{
  // indexes
  int i,j,k,z;
  // sorting
  int count_mu_1_CacheId;
  int compare;
  static mu_1_Cache value[3];
  // limit
  bool exists;
  bool split;
  bool goodset_mu_1_CacheId[3];
  bool pos_mu_1_CacheId[3][3];
  // range mapping
  int start;
  int class_size;
  int size_mu_1_CacheId[3];
  int start_mu_1_CacheId[3];
  // canonicalization
  static mu_1__type_0 temp;
  // sorting mu_1_CacheId
  // only if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_CacheId())
    {
      for (i=0; i<3; i++)
        for (j=0; j<3; j++)
          pos_mu_1_CacheId[i][j]=FALSE;
      count_mu_1_CacheId = 0;
      for (i=0; i<3; i++)
        {
          for (j=0; j<count_mu_1_CacheId; j++)
            {
              compare = CompareWeight(value[j],(*this)[i+4]);
              if (compare==0)
                {
                  pos_mu_1_CacheId[j][i]= TRUE;
                  break;
                }
              else if (compare>0)
                {
                  for (k=count_mu_1_CacheId; k>j; k--)
                    {
                      value[k] = value[k-1];
                      for (z=0; z<3; z++)
                        pos_mu_1_CacheId[k][z] = pos_mu_1_CacheId[k-1][z];
                    }
                  value[j] = (*this)[i+4];
                  for (z=0; z<3; z++)
                    pos_mu_1_CacheId[j][z] = FALSE;
                  pos_mu_1_CacheId[j][i] = TRUE;
                  count_mu_1_CacheId++;
                  break;
                }
            }
          if (j==count_mu_1_CacheId)
            {
              value[j] = (*this)[i+4];
              for (z=0; z<3; z++)
                pos_mu_1_CacheId[j][z] = FALSE;
              pos_mu_1_CacheId[j][i] = TRUE;
              count_mu_1_CacheId++;
            }
        }
    }
  // if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_CacheId() && count_mu_1_CacheId>1)
    {
      // limit
      for (j=0; j<3; j++) // class priority
        {
          for (i=0; i<count_mu_1_CacheId; i++) // for value priority
            {
              exists = FALSE;
              for (k=0; k<3; k++) // step through class
                goodset_mu_1_CacheId[k] = FALSE;
              for (k=0; k<3; k++) // step through class
                if (pos_mu_1_CacheId[i][k] && Perm.class_mu_1_CacheId[k] == j)
                  {
                    exists = TRUE;
                    goodset_mu_1_CacheId[k] = TRUE;
                    pos_mu_1_CacheId[i][k] = FALSE;
                  }
              if (exists)
                {
                  split=FALSE;
                  for (k=0; k<3; k++)
                    if ( Perm.class_mu_1_CacheId[k] == j && !goodset_mu_1_CacheId[k] ) 
                      split= TRUE;
                  if (split)
                    {
                      for (k=0; k<3; k++)
                        if (Perm.class_mu_1_CacheId[k]>j
                            || ( Perm.class_mu_1_CacheId[k] == j && !goodset_mu_1_CacheId[k] ) )
                          Perm.class_mu_1_CacheId[k]++;
                      Perm.undefined_class_mu_1_CacheId++;
                    }
                }
            }
        }
    }
  if (Perm.MTO_class_mu_1_CacheId())
    {

      // setup range for maping
      start = 0;
      for (j=0; j<=Perm.undefined_class_mu_1_CacheId; j++) // class number
        {
          class_size = 0;
          for (k=0; k<3; k++) // step through class[k]
            if (Perm.class_mu_1_CacheId[k]==j)
              class_size++;
          for (k=0; k<3; k++) // step through class[k]
            if (Perm.class_mu_1_CacheId[k]==j)
              {
                size_mu_1_CacheId[k] = class_size;
                start_mu_1_CacheId[k] = start;
              }
          start+=class_size;
        }

      // canonicalize
      temp = *this;
      for (i=0; i<3; i++)
        for (j=0; j<3; j++)
         if (i >=start_mu_1_CacheId[j] 
             && i < start_mu_1_CacheId[j] + size_mu_1_CacheId[j])
           {
             array[i+0] = temp[j+4];
             break;
           }
    }
  else
    {

      // fast canonicalize
      temp = *this;
      for (j=0; j<3; j++)
        array[Perm.class_mu_1_CacheId[j]+0] = temp[j+4];
    }
}
void mu_1__type_0::SimpleLimit(PermSet& Perm){}
void mu_1__type_0::ArrayLimit(PermSet& Perm)
{
  // indexes
  int i,j,k,z;
  // sorting
  int count_mu_1_CacheId;
  int compare;
  static mu_1_Cache value[3];
  // limit
  bool exists;
  bool split;
  bool goodset_mu_1_CacheId[3];
  bool pos_mu_1_CacheId[3][3];
  // sorting mu_1_CacheId
  // only if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_CacheId())
    {
      for (i=0; i<3; i++)
        for (j=0; j<3; j++)
          pos_mu_1_CacheId[i][j]=FALSE;
      count_mu_1_CacheId = 0;
      for (i=0; i<3; i++)
        {
          for (j=0; j<count_mu_1_CacheId; j++)
            {
              compare = CompareWeight(value[j],(*this)[i+4]);
              if (compare==0)
                {
                  pos_mu_1_CacheId[j][i]= TRUE;
                  break;
                }
              else if (compare>0)
                {
                  for (k=count_mu_1_CacheId; k>j; k--)
                    {
                      value[k] = value[k-1];
                      for (z=0; z<3; z++)
                        pos_mu_1_CacheId[k][z] = pos_mu_1_CacheId[k-1][z];
                    }
                  value[j] = (*this)[i+4];
                  for (z=0; z<3; z++)
                    pos_mu_1_CacheId[j][z] = FALSE;
                  pos_mu_1_CacheId[j][i] = TRUE;
                  count_mu_1_CacheId++;
                  break;
                }
            }
          if (j==count_mu_1_CacheId)
            {
              value[j] = (*this)[i+4];
              for (z=0; z<3; z++)
                pos_mu_1_CacheId[j][z] = FALSE;
              pos_mu_1_CacheId[j][i] = TRUE;
              count_mu_1_CacheId++;
            }
        }
    }
  // if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_CacheId() && count_mu_1_CacheId>1)
    {
      // limit
      for (j=0; j<3; j++) // class priority
        {
          for (i=0; i<count_mu_1_CacheId; i++) // for value priority
            {
              exists = FALSE;
              for (k=0; k<3; k++) // step through class
                goodset_mu_1_CacheId[k] = FALSE;
              for (k=0; k<3; k++) // step through class
                if (pos_mu_1_CacheId[i][k] && Perm.class_mu_1_CacheId[k] == j)
                  {
                    exists = TRUE;
                    goodset_mu_1_CacheId[k] = TRUE;
                    pos_mu_1_CacheId[i][k] = FALSE;
                  }
              if (exists)
                {
                  split=FALSE;
                  for (k=0; k<3; k++)
                    if ( Perm.class_mu_1_CacheId[k] == j && !goodset_mu_1_CacheId[k] ) 
                      split= TRUE;
                  if (split)
                    {
                      for (k=0; k<3; k++)
                        if (Perm.class_mu_1_CacheId[k]>j
                            || ( Perm.class_mu_1_CacheId[k] == j && !goodset_mu_1_CacheId[k] ) )
                          Perm.class_mu_1_CacheId[k]++;
                      Perm.undefined_class_mu_1_CacheId++;
                    }
                }
            }
        }
    }
}
void mu_1__type_0::Limit(PermSet& Perm){}
void mu_1__type_0::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };

/********************
 Auxiliary function for error trace printing
 ********************/
bool match(state* ns, StatePtr p)
{
  int i;
  static PermSet Perm;
  static state temp;
  StateCopy(&temp, ns);
  if (args->symmetry_reduction.value)
    {
      if (  args->sym_alg.mode == argsym_alg::Exhaustive_Fast_Canonicalize) {
        Perm.ResetToExplicit();
        for (i=0; i<Perm.count; i++)
          if (Perm.In(i))
            {
              if (ns != workingstate)
                  StateCopy(workingstate, ns);
              
              mu_mem.Permute(Perm,i);
              if (args->multiset_reduction.value)
                mu_mem.MultisetSort();
              mu_bus.Permute(Perm,i);
              if (args->multiset_reduction.value)
                mu_bus.MultisetSort();
              mu_caches.Permute(Perm,i);
              if (args->multiset_reduction.value)
                mu_caches.MultisetSort();
            if (p.compare(workingstate)) {
              StateCopy(workingstate,&temp); return TRUE; }
          }
        StateCopy(workingstate,&temp);
        return FALSE;
      }
      else {
        Perm.ResetToSimple();
        Perm.SimpleToOne();
        if (ns != workingstate)
          StateCopy(workingstate, ns);

          mu_mem.Permute(Perm,0);
          if (args->multiset_reduction.value)
            mu_mem.MultisetSort();
          mu_bus.Permute(Perm,0);
          if (args->multiset_reduction.value)
            mu_bus.MultisetSort();
          mu_caches.Permute(Perm,0);
          if (args->multiset_reduction.value)
            mu_caches.MultisetSort();
        if (p.compare(workingstate)) {
          StateCopy(workingstate,&temp); return TRUE; }

        while (Perm.NextPermutation())
          {
            if (ns != workingstate)
              StateCopy(workingstate, ns);
              
              mu_mem.Permute(Perm,0);
              if (args->multiset_reduction.value)
                mu_mem.MultisetSort();
              mu_bus.Permute(Perm,0);
              if (args->multiset_reduction.value)
                mu_bus.MultisetSort();
              mu_caches.Permute(Perm,0);
              if (args->multiset_reduction.value)
                mu_caches.MultisetSort();
            if (p.compare(workingstate)) {
              StateCopy(workingstate,&temp); return TRUE; }
          }
        StateCopy(workingstate,&temp);
        return FALSE;
      }
    }
  if (!args->symmetry_reduction.value
      && args->multiset_reduction.value)
    {
      if (ns != workingstate)
          StateCopy(workingstate, ns);
      mu_mem.MultisetSort();
      mu_bus.MultisetSort();
      mu_caches.MultisetSort();
      if (p.compare(workingstate)) {
        StateCopy(workingstate,&temp); return TRUE; }
      StateCopy(workingstate,&temp);
      return FALSE;
    }
  return (p.compare(ns));
}

/********************
 Canonicalization by fast exhaustive generation of
 all permutations
 ********************/
void SymmetryClass::Exhaustive_Fast_Canonicalize(state* s)
{
  int i;
  static state temp;
  Perm.ResetToExplicit();

  StateCopy(&temp, workingstate);
  ResetBestResult();
  for (i=0; i<Perm.count; i++)
    if (Perm.In(i))
      {
        StateCopy(workingstate, &temp);
        mu_mem.Permute(Perm,i);
        if (args->multiset_reduction.value)
          mu_mem.MultisetSort();
        SetBestResult(i, workingstate);
      }
  StateCopy(workingstate, &BestPermutedState);

  StateCopy(&temp, workingstate);
  ResetBestResult();
  for (i=0; i<Perm.count; i++)
    if (Perm.In(i))
      {
        StateCopy(workingstate, &temp);
        mu_bus.Permute(Perm,i);
        if (args->multiset_reduction.value)
          mu_bus.MultisetSort();
        SetBestResult(i, workingstate);
      }
  StateCopy(workingstate, &BestPermutedState);

  StateCopy(&temp, workingstate);
  ResetBestResult();
  for (i=0; i<Perm.count; i++)
    if (Perm.In(i))
      {
        StateCopy(workingstate, &temp);
        mu_caches.Permute(Perm,i);
        if (args->multiset_reduction.value)
          mu_caches.MultisetSort();
        SetBestResult(i, workingstate);
      }
  StateCopy(workingstate, &BestPermutedState);

};

/********************
 Canonicalization by fast simple variable canonicalization,
 fast simple scalarset array canonicalization,
 fast restriction on permutation set with simple scalarset array of scalarset,
 and fast exhaustive generation of
 all permutations for other variables
 ********************/
void SymmetryClass::Heuristic_Fast_Canonicalize(state* s)
{
  int i;
  static state temp;

  Perm.ResetToSimple();

  mu_bus.SimpleCanonicalize(Perm);

  mu_caches.Canonicalize(Perm);

};

/********************
 Canonicalization by fast simple variable canonicalization,
 fast simple scalarset array canonicalization,
 fast restriction on permutation set with simple scalarset array of scalarset,
 and fast exhaustive generation of
 all permutations for other variables
 and use less local memory
 ********************/
void SymmetryClass::Heuristic_Small_Mem_Canonicalize(state* s)
{
  unsigned long cycle;
  static state temp;

  Perm.ResetToSimple();

  mu_bus.SimpleCanonicalize(Perm);

  mu_caches.Canonicalize(Perm);

};

/********************
 Normalization by fast simple variable canonicalization,
 fast simple scalarset array canonicalization,
 fast restriction on permutation set with simple scalarset array of scalarset,
 and for all other variables, pick any remaining permutation
 ********************/
void SymmetryClass::Heuristic_Fast_Normalize(state* s)
{
  int i;
  static state temp;

  Perm.ResetToSimple();

  mu_bus.SimpleCanonicalize(Perm);

  mu_caches.Canonicalize(Perm);

};

/********************
  Include
 ********************/
#include "mu_epilog.inc"
