package(default_visibility = ["//visibility:public"])

cc_library(
    name = "comparator",
    hdrs = ["comparator.h"],
)

cc_test(
    name = "comparator_test",
    srcs = ["comparator_test.cc"],
    deps = [
        ":comparator",
    ],
)

cc_library(
    name = "heap",
    hdrs = ["heap.h"],
    deps = [
        ":comparator",
    ],
)

cc_test(
    name = "heap_test",
    srcs = ["heap_test.cc"],
    deps = [
        ":heap",
    ],
)

cc_library(
    name = "skiplist",
    hdrs = ["skiplist.h"],
    deps = [
        ":comparator",
        "//sun/util:random",
    ],
)

cc_test(
    name = "skiplist_test",
    srcs = ["skiplist_test.cc"],
    deps = [
        ":skiplist",
    ],
)

cc_library(
    name = "hash_table",
    hdrs = ["hash_table.h"],
    deps = [
        "//sun/util:common",
        "//sun/util:hash",
    ],
)

cc_test(
    name = "hash_table_test",
    srcs = ["hash_table_test.cc"],
    deps = [
        ":hash_table",
    ],
)

cc_library(
    name = "array",
    hdrs = ["array.h"],
    deps = [
        "//sun/util:common",
    ],
)

cc_test(
    name = "array_test",
    srcs = ["array_test.cc"],
    deps = [
        ":array",
    ],
)