----------------------------------------------------------------------
-- MSI Protocol with Atomic Bus Transactions
-- This version models bus transactions as atomic operations
-- to minimize state space exploration
----------------------------------------------------------------------

const
  NUM_CACHE : 3;

type
  StateType : enum {M, S, I};
  CacheId   : scalarset(NUM_CACHE);

  Cache : record
    state : StateType;
  end;

var
  caches : array[CacheId] of Cache;

----------------------------------------------------------------------
-- Processor Read Request (PrRd)
-- Atomic execution of BusRd transaction
----------------------------------------------------------------------
ruleset i : CacheId do

  rule "PrRd in I (atomic BusRd)"
    (caches[i].state = I)
  ==>
  begin
    -- Snoop all other caches atomically
    for j : CacheId do
      if j != i then
        if caches[j].state = M then
          -- Cache with M state provides data and downgrades to S
          caches[j].state := S;
        endif;
      endif;
    endfor;
    
    -- Update requesting cache to S
    caches[i].state := S;
  end;

end;

----------------------------------------------------------------------
-- Processor Write Request (PrWr)
-- Atomic execution of BusRdX or BusUpgr transaction
----------------------------------------------------------------------
ruleset i : CacheId do

  rule "PrWr in S (atomic BusRdX)"
    (caches[i].state = S)
  ==>
  begin
    -- Invalidate all other caches atomically
    for j : CacheId do
      if j != i then
        if caches[j].state = M | caches[j].state = S then
          caches[j].state := I;
        endif;
      endif;
    endfor;
    
    -- Update requesting cache to M
    caches[i].state := M;
  end;

  rule "PrWr in I (atomic BusRdX)"
    (caches[i].state = I)
  ==>
  begin
    -- Invalidate all other caches atomically
    for j : CacheId do
      if j != i then
        if caches[j].state = M | caches[j].state = S then
          caches[j].state := I;
        endif;
      endif;
    endfor;
    
    -- Update requesting cache to M
    caches[i].state := M;
  end;

end;

----------------------------------------------------------------------
-- Initial State
----------------------------------------------------------------------
startstate
begin
  for i : CacheId do
    caches[i].state := I;
  endfor;
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
