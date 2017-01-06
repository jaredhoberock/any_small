# any_small
A type-erasing container for small objects.

The main differences between `any_small` and `std::any`:

1. The type of object contained by `any_small` must be less than or equal to `any_small::max_size`.
2. `any_small` always holds a value.
3. `any_small` has a couple of unimplemented member functions such as `.emplace()` and `.swap()`.

