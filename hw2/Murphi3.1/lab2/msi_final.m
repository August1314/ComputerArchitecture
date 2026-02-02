----------------------------------------------------------------------
-- Simple Snooping MSI Protocol (3 caches, 1 memory block)
-- Simplified version without data tracking for minimal state space
----------------------------------------------------------------------

const
  NUM_CACHE : 2;

type
  StateType : enum {M, S, I};
  CacheId   : scalarset(NUM_CACHE);

  Cache : record
    state : StateType;
  end;

  BusReqType : enum {BusNone, BusRd, BusRdX};
  
var
  caches : array[CacheId] of Cache;
  bus_req : BusReqType;
  bus_source : CacheId;

----------------------------------------------------------------------
-- Processor Read Request (PrRd)
----------------------------------------------------------------------
ruleset i : CacheId do

  rule "PrRd, Cache State I"
    (caches[i].state = I) & (bus_req = BusNone)
  ==>
  begin
    -- Cache miss, issue BusRd to request shared copy
    bus_req := BusRd;
    bus_source := i;
  end;

end;

----------------------------------------------------------------------
-- Processor Write Request (PrWr)
----------------------------------------------------------------------
ruleset i : CacheId do

  rule "PrWr, Cache State S"
    (caches[i].state = S) & (bus_req = BusNone)
  ==>
  begin
    -- Need exclusive permission, issue BusRdX
    bus_req := BusRdX;
    bus_source := i;
  end;

  rule "PrWr, Cache State I"
    (caches[i].state = I) & (bus_req = BusNone)
  ==>
  begin
    -- Cache miss, issue BusRdX to request exclusive copy
    bus_req := BusRdX;
    bus_source := i;
  end;

end;

----------------------------------------------------------------------
-- Bus Transaction Processing
----------------------------------------------------------------------

rule "Process BusRd"
  (bus_req = BusRd)
==>
begin
  -- Snoop all other caches
  for i : CacheId do
    if i != bus_source then
      if caches[i].state = M then
        -- Provide data and downgrade to S
        caches[i].state := S;
      endif;
    endif;
  endfor;
  
  -- Update requesting cache to S
  caches[bus_source].state := S;
  
  -- Clear bus
  bus_req := BusNone;
  undefine bus_source;
end;

rule "Process BusRdX"
  (bus_req = BusRdX)
==>
begin
  -- Snoop all other caches
  for i : CacheId do
    if i != bus_source then
      if caches[i].state = M | caches[i].state = S then
        -- Invalidate
        caches[i].state := I;
      endif;
    endif;
  endfor;
  
  -- Update requesting cache to M
  caches[bus_source].state := M;
  
  -- Clear bus
  bus_req := BusNone;
  undefine bus_source;
end;

----------------------------------------------------------------------
-- Initial State
----------------------------------------------------------------------
startstate
begin
  for i : CacheId do
    caches[i].state := I;
  endfor;
  
  bus_req := BusNone;
  undefine bus_source;
end;

----------------------------------------------------------------------
-- Invariants
----------------------------------------------------------------------

-- At most one cache in M state
invariant "Unique Modified"
  forall i : CacheId do
    forall j : CacheId do
      (i != j) -> !(caches[i].state = M & caches[j].state = M)
    end
  end;

-- If one cache is in M state, no other cache can be in S state
invariant "Exclusive Modified"
  forall i : CacheId do
    forall j : CacheId do
      (i != j) -> !(caches[i].state = M & caches[j].state = S)
    end
  end;
