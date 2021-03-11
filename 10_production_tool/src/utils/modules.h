#ifndef MODULES_H
#define MODULES_H

typedef int (*init_fn_t)(void);

#define USED       __attribute__((used))
#define SECTION(x) __attribute__((section(x)))

#define INIT_EXPORT(fn, level) USED const init_fn_t __pt_init_##fn SECTION("exi_fn" level) = fn

#define START_FUNC_LEVEL(X) __start_exi_fn##X
#define STOP_FUN_LEVEL(X)   __stop_exi_fn##X

#define MODULE_EXPORT(fn) INIT_EXPORT(fn, "0")

#endif /* MODULES_H */