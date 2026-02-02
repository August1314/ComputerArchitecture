----------------------------------------------------------------------
-- Simple Snooping MESI Protocol (3 caches, 1 memory block)
----------------------------------------------------------------------

const
  NUM_CACHE : 2;
  NUM_MEM   : 1;

type
  StateType : enum {M, E, S, I};
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

  BusReqType : enum {BusNone, BusRd, BusRdX, BusUpgr};
  BusCtrlType : enum {CtrlNone, CtrlFlush, CtrlFlushOpt};
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

function CountExclusiveCache(): CountType;
var cnt: CountType;
begin
  cnt := 0;
  for i: CacheId do
    if (caches[i].line.state = E) then
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

      case E:
        -- Provide data and downgrade to S
        bus.data := caches[i].line.data;
        bus.ctrl := CtrlFlushOpt;
        caches[i].line.state := S;

      case S:
        -- Mark that there are shared copies
        bus.ctrl := CtrlFlushOpt;

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

      case E:
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

  elsif bus.req = BusUpgr then
    switch caches[i].line.state
      case S:
        -- Invalidate cache line (no data transfer needed)
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

  rule "PrRd, Cache State E"
    (caches[i].line.state = E) &
    (bus.req = BusNone)
  ==>
  begin
    -- Cache hit, read directly from cache
    -- No bus transaction needed
    -- State remains E
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
    -- Cache miss, issue BusRd to request data
    bus.req := BusRd;
    bus.source := i;
    -- Will transition to E or S based on whether other caches have the data
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
    -- State remains M
  end;

  rule "PrWr, Cache State E"
    (caches[i].line.state = E) &
    (bus.req = BusNone)
  ==>
  begin
    -- Cache hit, write directly and upgrade to M
    -- No bus transaction needed (key MESI optimization!)
    caches[i].line.data := (caches[i].line.data + 1)%16;
    caches[i].line.state := M;
  end;

  rule "PrWr, Cache State S"
    (caches[i].line.state = S) &
    (bus.req = BusNone)
  ==>
  begin
    -- Need to invalidate other copies, issue BusUpgr
    bus.req := BusUpgr;
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
rule "Process bus request"
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
      -- Decide E or S based on whether other caches have the data
      if bus.ctrl = CtrlNone then
        -- No other cache has the data, go to E (Exclusive)
        caches[bus.source].line.state := E;
      else
        -- Other caches have the data, go to S (Shared)
        caches[bus.source].line.state := S;
      endif;
      caches[bus.source].line.data := bus.data;
      
    elsif bus.req = BusRdX then
      -- Transition to M state and perform write
      caches[bus.source].line.state := M;
      caches[bus.source].line.data := bus.data;
      caches[bus.source].line.data := (caches[bus.source].line.data + 1) % 16;
      
    elsif bus.req = BusUpgr then
      -- Upgrade from S to M, already have data, just change state and write
      caches[bus.source].line.state := M;
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
invariant "Only one Cache may be Modified or Exclusive"
  CountModifiedCache() + CountExclusiveCache() <= 1;

invariant "All Shared Caches have same data"
  forall i : CacheId do
    forall j : CacheId do
        ((caches[i].line.state = S) &
         (caches[j].line.state = S))
        ->
        (caches[i].line.data = caches[j].line.data)
    end
  end;

invariant "Modified or Exclusive Cache implies no Shared Cache"
  ( CountModifiedCache() = 1 | CountExclusiveCache() = 1)
  ->
  forall i : CacheId do
    caches[i].line.state != S
  end;

invariant "Shared/Exclusive Caches equal memory (when no Modified cache exists)"
  ( CountModifiedCache() = 0 )
  -> 
  forall i : CacheId Do
    (caches[i].line.state = S | caches[i].line.state = E) -> (caches[i].line.data = mem.data)
  end;