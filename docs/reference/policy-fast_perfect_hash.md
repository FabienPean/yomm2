<span style="font-size:xx-large;">yorel::yomm2::policy::<strong>fast_perfect_hash</strong></span><br/>
<sub>defined in <yorel/yomm2/policy.hpp>, also provided by <yorel/yomm2/core.hpp>, <yorel/yomm2/keywords.hpp></sub><br/>

```c++
template<class Policy> struct fast_perfect_hash;
template<class Policy> struct checked_perfect_hash;
```

`fast_perfect_hash` is an implementation of [`type_hash`](/yomm2/reference/policy-type_hash.html). It uses the
following hash function:

```
H(x) = (x * M) >> S
```

The `M` and `S` coefficients are determined during initialization so that, for
the given set of type ids, the hash function is _perfect_, i.e. collision-free.
It is not _minimal_: there may be gaps in the output interval.

`hash_type_id` must not be called with ids not present in the input set passed
to `hash_initialize`. This happens when the class of a virtual argument was not
registered via [`use_classes`](/yomm2/reference/use_classes.html) or [`register_classes`](/yomm2/reference/use_classes.html). `ref_perfect_hash` will
silently return an index in the output range, to be used to fetch the v-table
for the class from a vector. From there, two things can happen. In the lucky
scenario, the index will correspond to a gap, a wasted entry in the v-table
vector, which will contain a null pointer. The program will segfault. Such
mistake is fairly easy to troubleshoot. In the unlucky scenario, there will be a
collision - the hash function is perfect only for the input ids - and the wrong
method will be called. The program will carry on, crashing far from the root
cause of the error, and, in the worst case, not at all.

To help detect such registration errors, it is highly recommended to run debug
builds of the program with [`checked_perfect_hash`](/yomm2/reference/policy-checked_perfect_hash.html), which detects
missing class registrations.

[`default_policy`](/yomm2/reference/policy-basic_policy.html) uses `fast_perfect_hash` in release builds, and
`checked_perfect_hash` in debug builds.

## Template parameters

**Policy** - the policy containing the facet.

## Static member functions

|                                     |                              |
| ----------------------------------- | ---------------------------- |
| [hash_initialize](#hash_initialize) | finds a hash function        |
| [hash_type_id](#hash_type_id)       | returns the hashed `type_id` |

## Static member variables

|                             |                               |
| --------------------------- | ----------------------------- |
| [hash_length](#hash_length) | length of the result interval |

### hash_initialize

```c++
template <typename ForwardIterator> static size_t
hash_initialize(ForwardIterator first, ForwardIterator last);
```

Finds the `M` and `S` parameters of the hash function. `ForwardIterator` is an
iterator that satisfies the requirements of forward iterators; dereferencing it
yields a `type_id`. Sets `type_hash_length` to the maximum hash value, plus one.

#### Parameters

**first**, **last** - a range of `type_id`s

#### Return value

None.

#### Errors

* Any exception thrown by Allocator::allocate() (typically std::bad_alloc).

### hash_type_id

```c++
static type_id hash_type_id(type_id type);
```

Returns the hashed value of `type`, which must be one of the values passed to
`hash_initialize`.

#### Parameters

**type** - a `type_id`

#### Return value

None.

#### Errors

None.

### hash_length

```c++
static std::size_t hash_length;
```

The length of the zero-based interval that contains the hashed values for all
the input values. In other words, the maximum value returned for legal inputs,
plus one.


## Interactions with other facets

* `error_handler` - to report error conditions.
* `error_output` - for diagnostics.
* `trace_output` - for trace.
