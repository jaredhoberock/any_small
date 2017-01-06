# any_small
`any_small` is a type-erasing container for small objects.

The main differences between `any_small` and `std::any`:

1. The size of the object contained by `any_small` must be less than or equal to `any_small::max_size`.
2. `any_small` always holds a value.
3. `any_small` has a couple of unimplemented member functions such as `.emplace()` and `.swap()`.

