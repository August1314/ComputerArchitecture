----------------------------------------------------------------------
-- Simple Snooping MSI Protocol (3 caches, 1 memory block)
----------------------------------------------------------------------

const
  NUM_CACHE : 2;
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
        -- Provide data and downgrade to S
        bus.data := caches[i].line.data;
        bus.ctrl := CtrlFlush;
        caches[i].line.state := S;

      case S:
        -- Remain in S state, no action needed

      case I:
        -- Remain in I state, no action needed

    endswitch;
  elsif bus.req = BusRdX then
    switch caches[i].line.state
      case M:
        -- Provide data and invalidate
        bus.data := caches[i].line.data;
        bus.ctrl := CtrlFlush;
        caches[i].line.state := I;
        undefine caches[i].line.data;

      case S:
        -- Invalidate cache line
        caches[i].line.state := I;
        undefine caches[i].line.data;

      case I:
        -- Remain in I state, no action needed

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
    -- Cache hit, read directly from cache
    -- No bus transaction needed
    -- State remains M
  end;

  rule "PrRd, Cache State S"
    (caches[i].line.state = S) &
    (bus.req = BusNone)
  ==>
  begin
    -- Cache hit, read directly from cache
    -- No bus transaction needed
    -- State remains S
  end;

  rule "PrRd, Cache State I"
    (caches[i].line.state = I) &
    (bus.req = BusNone)
  ==>
  begin
    -- Cache miss, issue BusRd to request shared copy
    bus.req := BusRd;
    bus.source := i;
    -- Will transition to S state after bus transaction completes
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
    -- Need exclusive permission, issue BusRdX
    bus.req := BusRdX;
    bus.source := i;
    -- Will transition to M state and perform write after bus transaction
  end;

  rule "PrWr, Cache State I"
    (caches[i].line.state = I) &
    (bus.req = BusNone)
  ==>
  begin
    -- Cache miss, issue BusRdX to request exclusive copy
    bus.req := BusRdX;
    bus.source := i;
    -- Will transition to M state and perform write after bus transaction
  end;

end;

----------------------------------------------------------------------
-- Process Bus Transaction
----------------------------------------------------------------------
rule "Process bus transaction"
  bus.req != BusNone
  ==>
  begin
    -- 1. Cache Responds to Bus Transaction (snooping)
    for i : CacheId do
      if i != bus.source then
        Snoop(i);
      endif;
    endfor;

    -- 2. Memory responds if no cache provided data
    if bus.data = -1 then
      bus.data := mem.data;
    endif;

    -- 3. Update requesting cache state and data
    if bus.req = BusRd then
      -- Transition to S state
      caches[bus.source].line.state := S;
      caches[bus.source].line.data := bus.data;
    elsif bus.req = BusRdX then
      -- Transition to M state and perform write
      caches[bus.source].line.state := M;
      caches[bus.source].line.data := bus.data;
      -- Perform the write operation
      caches[bus.source].line.data := (caches[bus.source].line.data + 1) % 16;
    endif;

    -- 4. Update memory if data was written back
    if bus.ctrl = CtrlFlush then
      mem.data := bus.data;
    endif;

    -- 5. Clear bus state
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