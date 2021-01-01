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
        "//sun/util:common",
    ],
)

cc_test(
    name = "skiplist_test",
    srcs = ["skiplist_test.cc"],
    deps = [
        ":skiplist",
    ],
)