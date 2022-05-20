#pragma once

// All macros starting with _ are implementation details of other macros, and should be ignored

#define MMAGIC_EMPTY() 
#define MMAGIC_EXECUTE( X ) do{ X; } while (false)

#define MMAGIC_EVAL(...) MMAGIC_EVAL1024(__VA_ARGS__)
#define MMAGIC_EVAL1024(...) MMAGIC_EVAL512(MMAGIC_EVAL512(__VA_ARGS__))
#define MMAGIC_EVAL512(...) MMAGIC_EVAL256(MMAGIC_EVAL256(__VA_ARGS__))
#define MMAGIC_EVAL256(...) MMAGIC_EVAL128(MMAGIC_EVAL128(__VA_ARGS__))
#define MMAGIC_EVAL128(...) MMAGIC_EVAL64(MMAGIC_EVAL64(__VA_ARGS__))
#define MMAGIC_EVAL64(...) MMAGIC_EVAL32(MMAGIC_EVAL32(__VA_ARGS__))
#define MMAGIC_EVAL32(...) MMAGIC_EVAL16(MMAGIC_EVAL16(__VA_ARGS__))
#define MMAGIC_EVAL16(...) MMAGIC_EVAL8(MMAGIC_EVAL8(__VA_ARGS__))
#define MMAGIC_EVAL8(...) MMAGIC_EVAL4(MMAGIC_EVAL4(__VA_ARGS__))
#define MMAGIC_EVAL4(...) MMAGIC_EVAL2(MMAGIC_EVAL2(__VA_ARGS__))
#define MMAGIC_EVAL2(...) MMAGIC_EVAL1(MMAGIC_EVAL1(__VA_ARGS__))
#define MMAGIC_EVAL1(...) __VA_ARGS__

#define MMAGIC_DEFER1(m) m MMAGIC_EMPTY()
#define MMAGIC_DEFER2(m) m MMAGIC_EMPTY MMAGIC_EMPTY()()
#define MMAGIC_DEFER3(m) m MMAGIC_EMPTY MMAGIC_EMPTY MMAGIC_EMPTY()()()
#define MMAGIC_DEFER4(m) m MMAGIC_EMPTY MMAGIC_EMPTY MMAGIC_EMPTY MMAGIC_EMPTY()()()()

// These contain workarounds for MSVC's incorrect handling of __VA_ARGS__
#define MMAGIC_FIRST(a, ...) MMAGIC_EVAL1(MMAGIC_DEFER1(_MMAGIC_FIRST)(a, __VA_ARGS__))
#define _MMAGIC_FIRST(a, ...) a
#define MMAGIC_SECOND(a, b, ...) MMAGIC_EVAL1(MMAGIC_DEFER1(_MMAGIC_SECOND)(a, b, ...))
#define _MMAGIC_SECOND(a, b, ...) b

#define MMAGIC_IS_PROBE(...) MMAGIC_SECOND(__VA_ARGS__, 0)
#define MMAGIC_PROBE() ~, 1
#define MMAGIC_CAT(a, b) a ## b
#define MMAGIC_NOT(x) MMAGIC_IS_PROBE(MMAGIC_CAT(_MMAGIC_NOT_, x))
#define _MMAGIC_NOT_0 MMAGIC_PROBE()
#define MMAGIC_BOOL(x) MMAGIC_NOT(MMAGIC_NOT(x))

#define MMAGIC_IF_ELSE(condition) _MMAGIC_IF_ELSE(MMAGIC_BOOL(condition))
#define _MMAGIC_IF_ELSE(condition) MMAGIC_CAT(_MMAGIC_IF_, condition)
#define _MMAGIC_IF_1(...) __VA_ARGS__ _MMAGIC_IF_1_ELSE
#define _MMAGIC_IF_0(...)             _MMAGIC_IF_0_ELSE
#define _MMAGIC_IF_1_ELSE(...)
#define _MMAGIC_IF_0_ELSE(...) __VA_ARGS__

#define MMAGIC_HAS_ARGS(...) MMAGIC_BOOL(MMAGIC_FIRST(_MMAGIC_END_OF_ARGUMENTS_ __VA_ARGS__)())
#define _MMAGIC_END_OF_ARGUMENTS_() 0


#define MMAGIC_MAP(map, first, ...)                     \
    map(first)                                          \
    MMAGIC_IF_ELSE(MMAGIC_HAS_ARGS(__VA_ARGS__))(       \
        MMAGIC_DEFER2(_MMAGIC_MAP)()(map, __VA_ARGS__)    \
    )(                                                  \
        /* terminate */                                 \
    )
#define _MMAGIC_MAP() MMAGIC_MAP

#define MMAGIC_COUNT_ARGS(...) MMAGIC_EVAL(MMAGIC_MAP(_MMAGIC_COUNT_ARG, __VA_ARGS__)0)
#define _MMAGIC_COUNT_ARG(x) 1+
