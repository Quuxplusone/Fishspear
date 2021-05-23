## Fishspear (Fischer & Paterson, 1992)

_Fishspear_ is a priority-queue algorithm developed by Michael J. Fischer and Michael S. Paterson
of the University of Warwick in 1992. Its interesting property is that it's based on
"sequential storage" (lists) rather than "random-access storage" (arrays). This makes
this C++ implementation super slow compared to the standard `priority_queue`; but it
does have three potential upsides:

- Fishspear might be well suited to _non-RAM_ storage, such as a priority queue
  stored on tape. (This repo's implementation is strictly in-memory; this is a
  possible advantage of the algorithm in general, if you reimplemented it for that
  kind of storage.)

- By linking elements instead of keeping them contiguous, we lose RAM-friendliness
  but we gain the ability to keep newly inserted items right near the top of the
  data structure instead of having to insert them all the way at the bottom.
  When the shape of the workload makes it likely that "a newly inserted element
  will very soon be deleted," a classic heap will still do O(lg n) comparisons
  on that element to get it into its proper place; Fishspear will do O(1).

- An advantage of this C++ version of Fishspear over `std::priority_queue` is that
  Fishspear never needs to move or copy elements.

This C++ version keeps its elements in a collection of `std::list`s internally,
and uses the standard member functions `list::merge` and `list::splice` to implement
key steps of the algorithm.


### Original citation

- Michael J. Fischer and Michael S. Paterson (1992). ["Fishspear: a priority queue algorithm."](http://wrap.warwick.ac.uk/60910/6/WRAP_cs-rr-221.pdf)
    University of Warwick. Department of Computer Science. (Department of Computer
    Science Research Report, Unpublished) CS-RR-221

C++ implementation by Arthur O'Dwyer, 2021.
All bugs-of-implementation are my own.
