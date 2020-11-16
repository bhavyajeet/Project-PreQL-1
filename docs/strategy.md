# Strategy

## Phase 1
For B+ Tree, we used an overflow block to have all our rows stored.

For Hash, we used vector of vectors.

## Phase 2

For bulk insert, we sorted table again.
For alter table, we added column at last.
For group by, we just sorted it.
