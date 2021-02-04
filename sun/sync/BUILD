package(default_visibility = ["//visibility:public"])

cc_library(
    name = "count_down_latch",
    hdrs = ["count_down_latch.h"],
    deps = [
        "//sun/util:common",
    ],
)

cc_library(
    name = "spinlock",
    hdrs = ["spinlock.h"],
    srcs = ["spinlock.cc"],
    deps = [
        "//sun/util:common",
    ],
)

cc_test(
    name = "spinlock_test",
    srcs = ["spinlock_test.cc"],
    deps = [
        ":spinlock",
    ],
)

cc_library(
    name = "queue",
    hdrs = ["queue.h"],
    deps = [
        "//sun/data_structure:array",
    ],
)

cc_test(
    name = "queue_test",
    srcs = ["queue_test.cc"],
    deps = [
        ":queue",
    ],
)

cc_library(
    name = "heap",
    hdrs = ["heap.h"],
    deps = [
        "//sun/data_structure:heap",
    ],
)

cc_library(
    name = "list",
    hdrs = ["list.h"],
)

cc_test(
    name = "list_test",
    srcs = ["list_test.cc"],
    deps = [
        ":list",
    ],
)

cc_library(
    name = "task_queue",
    hdrs = ["task_queue.h"],
    deps = [
        ":list",
        ":heap",
        ":queue",
    ],
)