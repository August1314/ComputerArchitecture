----------------------------------------------------------------------
-- Simple Snooping MSI Protocol (3 caches, 1 memory block)
----------------------------------------------------------------------

const
  NUM_CACHE : 3;
  NUM_MEM   : 1;

type
  StateType : enum {M, S, I};
  CacheId   : scalarset(NUM_CACHE);
  CountType : 0..NUM_CACHE;

  CacheLine : record
    state : StateType;
    data  : 0..15;
  end;

  Cache : record
    line : CacheLine;
  end;

  MemBlock : record
    data : 0..15;
  end;

  BusReqType : enum {BusNone, BusRd, BusRdX};
  BusCtrlType : enum {CtrlNone, CtrlFlush};
  Bus : record
    req     : BusReqType;
    source  : CacheId;
    data    : -1..15;
    ctrl    : BusCtrlType;
  end;

var
  caches : array[CacheId] of Cache;
  mem    : MemBlock;
  bus    : Bus;

function CountModifiedCache(): CountType;
var cnt: CountType;
begin
  cnt := 0;
  for i: CacheId do
    if (caches[i].line.state = M) then
      cnt := cnt + 1;
    end;
  end;
  return cnt;
end;

----------------------------------------------------------------------
-- Bus Snooping
----------------------------------------------------------------------
procedure Snoop(i: CacheId);
begin
  if bus.req = BusRd then
    switch caches[i].line.state
      case M:
        /* TODO */

      case S:
        /* TODO */

      case I:
        /* TODO */

    endswitch;
  elsif bus.req = BusRdX then
    switch caches[i].line.state
      case M:
        /* TODO */

      case S:
        /* TODO */

      case I:
        /* TODO */

    endswitch;
  endif;
end;

----------------------------------------------------------------------
-- Processor Read Request (PrRd)
----------------------------------------------------------------------
ruleset i : CacheId do

  rule "PrRd, Cache State M"
    (caches[i].line.state = M) &
    (bus.req = BusNone)
  ==>
  begin
    /* TODO */

  end;

  rule "PrRd, Cache State S"
    (caches[i].line.state = S) &
    (bus.req = BusNone)
  ==>
  begin
    /* TODO */
    
  end;

  rule "PrRd, Cache State I"
    (caches[i].line.state = I) &
    (bus.req = BusNone)
  ==>
  begin
    /* TODO */

  end;

end;

----------------------------------------------------------------------
-- Processor Write Request (PrWr)
----------------------------------------------------------------------
ruleset i : CacheId do
  rule "PrWr, Cache State M"
    (caches[i].line.state = M) &
    (bus.req = BusNone)
  ==>
  begin
    -- simulate a write operation
    caches[i].line.data := (caches[i].line.data + 1)%16;
  end;

  rule "PrWr, Cache State S"
    (caches[i].line.state = S) &
    (bus.req = BusNone)
  ==>
  begin
    /* TODO */

  end;

  rule "PrWr, Cache State I"
    (caches[i].line.state = I) &
    (bus.req = BusNone)
  ==>
  begin
    /* TODO */

  end;

end;

----------------------------------------------------------------------
-- Process Bus Transaction
----------------------------------------------------------------------
rule "Process bus transaction"
  bus.req != BusNone
  ==>
  begin
    -- Cache Responds to Bus Transaction
    for i : CacheId do
      if i != bus.source then
        Snoop(i);
      endif;
    endfor;

    /* TODO: Memory responds to the bus transaction, then writes the data back to the cache and updates the cache state */



    bus.req := BusNone;
    undefine bus.source;
    bus.data := -1;
    bus.ctrl := CtrlNone;

  end;

----------------------------------------------------------------------
-- Initialization
----------------------------------------------------------------------
startstate "Init"
  for i : CacheId do
    caches[i].line.state := I;
    undefine caches[i].line.data;
  end;

  mem.data := 0;

  bus.req := BusNone;
  undefine bus.source;
  bus.data := -1;
  bus.ctrl := CtrlNone;

end;

----------------------------------------------------------------------
-- Invariants
----------------------------------------------------------------------
invariant "Only one Cache may be Modified"
  CountModifiedCache() <= 1;

invariant "All Shared Caches have same data"
  forall i : CacheId do
    forall j : CacheId do
        ((caches[i].line.state = S) &
         (caches[j].line.state = S))
        ->
        (caches[i].line.data = caches[j].line.data)
    end
  end;

invariant "Modified Cache implies no Shared Cache"
  ( CountModifiedCache() = 1 )
  ->
  forall i : CacheId do
    caches[i].line.state != S
  end;

invariant "Shared Caches equal memory (when no Modified cache exists)"
  ( CountModifiedCache() = 0 )
  -> 
  forall i : CacheId do
    (caches[i].line.state = S) -> (caches[i].line.data = mem.data)
  end;