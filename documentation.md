# Documentation

There is very little to know in terms of functions, types, etc. The API of
yomm2 consists of two headers, five pseudo keywords, and one function.

It is however very important to understand the semantics of method dispatch.

### header yorel/yomm2.hpp

#### Synopsis:
```
#include <yorel/yomm2.hpp>
```

This is the library's main header. It defines the YOMM2_* macros, and namespace
yorel::yomm2 and its content.

### header yorel/yomm2/cute.hpp

#### Synopsis:
```
#include <yorel/yomm2/cute.hpp>
```

### namespace yorel::yomm2

### yorel::yomm2::update_methods()

#### Synopsis:
```
int main(int argc, const char** argv) {
    yorel::yomm2::update_methods();
    // ...
}
```

Create the tables used during method dispatch.

This function must be called before any method is called (typically in
`main`). It must also be called after a shared library is dynamically loaded or
unloaded, if the library adds method declarations, method definitions, or
classes derived from classes that are used as virtual arguments.

### macros YOMM2_DECLARE, declare_method

#### Synopsis:
```
YOMM2_DECLARE(return_type, name, (parameter_types));
```

Declare a method. Create an inline function that has the specified name and
return type. The parameters of the function consist of the types passed in
`parameter_types` with the `virtual_` marker removed. The dynamic type of the
arguments corresponding to a virtual parameter are taken into account to select
the most specialized definition to call.

`declare_method` is an alias for `YOMM2_DECLARE` created by header
`yorel/yomm2/cute.hpp`.

NOTE:

* The parameter list _must_ be surrounded by parentheses.

* Parameters consist of _just_ a type, e.g. `int` is correct but `int i` is not.

### Examples:
```
declare_method(std::string, kick, (virtual_<Animal&>));
````
Declare function `std::string kick(Animal&)`, which calls the most specific definition of `kick` depending on the dynamic type of its first argument.

````
declare_method(std::string, meet, (virtual_<Animal&>, virtual_<Animal&>)); // 2
```
Declare function `std::string meet(Animal&, Animal&)`, which calls the most specific definition of `kick` depending on the dynamic types of its first and second argument.

### template virtual_

#### Synopsis:
```
virtual_<polymorphic_type>
```



### Examples:
```
struct Animal {
    virtual ~Animal();
};

declare_method(void, kick, (virtual_<Animal*>));
declare_method(void, kick, (virtual_<Animal&>));
declare_method(void, kick, (virtual_<shared_ptr<Animal>>));
declare_method(void, kick, (virtual_<const Animal*>));
declare_method(void, kick, (virtual_<const Animal&>));
declare_method(void, kick, (virtual_<shared_ptr<const Animal>>));

```
Given a polymorphic class `Animal`, these are all the valid ways of specifying a virtual Animal argument in a method declaration.


### macros begin_method, YOMM2_METHOD

#### Synopsis:
```
```

### next

#### Synopsis:
```
```

### macros end_method, YOMM2_END

#### Synopsis:
```
```

### macros register_class, YOMM2_CLASS

#### Synopsis:
```
```

### Semantics

### Dependencies

* a C++17 capable compiler

* The following Boost libraries: Preprocessor, DynamicBitset, TypeTraits

* For tests: Boost.Test version 1.65

* cmake version 3.5