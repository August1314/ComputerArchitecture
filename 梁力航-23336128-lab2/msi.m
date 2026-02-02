----------------------------------------------------------------------
-- Simple Snooping MSI Protocol (3 caches, 1 memory block)
-- Version without data tracking to minimize state space
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
    -- No data field
  end;

  Cache : record
    line : CacheLine;
  end;

  MemBlock : record
    valid : boolean;  -- Just track if memory has data
  end;

  BusReqType : enum {BusNone, BusRd, BusRdX};
  BusCtrlType : enum {CtrlNone, CtrlFlush};
  Bus : record
    req     : BusReqType;
    source  : CacheId;
    ctrl    : BusCtrlType;
    -- No data field
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
        bus.ctrl := CtrlFlush;
        caches[i].line.state := I;

      case S:
        -- Invalidate cache line
        caches[i].line.state := I;

      case I:
        -- Remain in I state, no action needed

    endswitch;
  endif;
end;

----------------------------------------------------------------------
-- Processor Read Request (PrRd)
----------------------------------------------------------------------
ruleset i : CacheId do

  rule "PrRd, Cache State I"
    (caches[i].line.state = I) &
    (bus.req = BusNone)
  ==>
  begin
    -- Cache miss, issue BusRd to request shared copy
    bus.req := BusRd;
    bus.source := i;
  end;

end;

----------------------------------------------------------------------
-- Processor Write Request (PrWr)
----------------------------------------------------------------------
ruleset i : CacheId do

  rule "PrWr, Cache State S"
    (caches[i].line.state = S) &
    (bus.req = BusNone)
  ==>
  begin
    -- Need exclusive permission, issue BusRdX
    bus.req := BusRdX;
    bus.source := i;
  end;

  rule "PrWr, Cache State I"
    (caches[i].line.state = I) &
    (bus.req = BusNone)
  ==>
  begin
    -- Cache miss, issue BusRdX to request exclusive copy
    bus.req := BusRdX;
    bus.source := i;
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

    -- 2. Update requesting cache state
    if bus.req = BusRd then
      -- Transition to S state
      caches[bus.source].line.state := S;
    elsif bus.req = BusRdX then
      -- Transition to M state
      caches[bus.source].line.state := M;
    endif;

    -- 3. Update memory if data was written back
    if bus.ctrl = CtrlFlush then
      mem.valid := true;
    endif;

    -- 4. Clear bus state
    bus.req := BusNone;
    undefine bus.source;
    bus.ctrl := CtrlNone;

  end;

----------------------------------------------------------------------
-- Initialization
----------------------------------------------------------------------
startstate "Init"
  for i : CacheId do
    caches[i].line.state := I;
  end;

  mem.valid := true;

  bus.req := BusNone;
  undefine bus.source;
  bus.ctrl := CtrlNone;

end;

----------------------------------------------------------------------
-- Invariants
----------------------------------------------------------------------
invariant "Only one Cache may be Modified"
  CountModifiedCache() <= 1;

invariant "Modified Cache implies no Shared Cache"
  ( CountModifiedCache() = 1 )
  ->
  forall i : CacheId do
    caches[i].line.state != S
  end;
