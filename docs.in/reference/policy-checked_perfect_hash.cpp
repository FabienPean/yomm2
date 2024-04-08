#define BOOST_TEST_MODULE checked_perfect_hash
#include <boost/test/included/unit_test.hpp>

/***

entry: policy::checked_perfect_hash
headers: yorel/yomm2/policy.hpp, yorel/yomm2/core.hpp, yorel/yomm2/keywords.hpp
```
template<class Policy>
struct checked_perfect_hash;
```

`checked_perfect_hash` is a subclass of ->`policy-fast_perfect_hash`. It checks that
the id passed to `hash_type_index` is valid, i.e. was in the set of ids
registered via `hash_initialize`.

## Template parameters

**Policy** - the policy containing the facet.

## Static member functions
|                                     |                              |
| ----------------------------------- | ---------------------------- |
| [hash_initialize](#hash_initialize) | finds a hash function        |
| [hash_type_id](#hash_type_id)       | returns the hashed `type_id` |

### hash_initialize

```c++
template<class Policy>
template <typename ForwardIterator>
static size_t hash_initialize(ForwardIterator first, ForwardIterator last)
```

Calls
[`fast_perfect_hash::hash_initialize`](fast_perfect_hash.md#hash_initialize).
Also build a reverse mapping, from hashed ids to registered ids.


#### Parameters

**first**, **last** - a range of `type_id`s

#### Return value

None.

#### Errors

* Any exception thrown by Allocator::allocate() (typically std::bad_alloc).

### hash_type_id

```c++
template<class Policy>
static type_id hash_type_id(type_id type)
```

[`fast_perfect_hash::hash_type_index`](fast_perfect_hash.md#hash_initialize).
Retrieve the registered id corresponding to the hashed id. Compare it with
`type`. If they are the same, return the hashed value. If not, report an error
via `Policy::error` if `Policy` has an `error_handler` facet; otherwise,
abort the program.

#### Parameters

**type** - a `type_id`

#### Return value

None.

#### Errors

* ->`unknown_class_error`, with `context` set to `unknown_class_error::update`.

## Interactions with other facets

* `error_handler` - to report error conditions.
* `error_output` - for diagnostics.
* `trace_output` - for trace.

### Example

***/

//***
#include <yorel/yomm2/policy.hpp>

// for brevity
using namespace yorel::yomm2;
using namespace yorel::yomm2::policy;

// Create a policy that contains 'checked_perfect_hash' and `throw_error`.
struct checked_policy
    : basic_policy<
          checked_policy, checked_perfect_hash<checked_policy>,
          throw_error> {};

BOOST_AUTO_TEST_CASE(ref_check_checked_perfect_hash) {
    std::vector<type_id> ids = {42, 1963, 602701};
    checked_policy::hash_initialize(ids.begin(), ids.end());
    bool caught = false;

    try {
        // 'fast_perfect_hash' would return a bogus result...
        checked_policy::hash_type_id(666);
    } catch (unknown_class_error& error) {
        // ...but 'checked_perfect_hash' catches it.
        caught = true;
    }

    BOOST_TEST(caught);
}
//***
