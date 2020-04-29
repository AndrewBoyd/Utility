#pragma once
#include "macro_magic.h"

#define REFLECTAGA_TEMPLATE_HEADER template <typename reflect_action_t>
#define RELFECTAGA_FUNCTION_DECLERATION( TYPENAME ) void reflectAction( reflect_action_t & action, TYPENAME & type )
#define REFLECTAGA_ACTION_LINE( X ) action(type.X, #X); 
#define REFLECTAGA_ACTION_LINES( ... ) MMAGIC_MAP(REFLECTAGA_ACTION_LINE, __VA_ARGS__)
#define REFLECTAGA_COUNT_MEMBERS(TYPENAME, ...) \
    namespace reflectaga{ template<> inline constexpr int getNumberOfMembers<TYPENAME>(){ return MMAGIC_COUNT_ARGS(__VA_ARGS__); } }

#define REFLECTAGA(TYPENAME, ...) \
REFLECTAGA_TEMPLATE_HEADER \
RELFECTAGA_FUNCTION_DECLERATION(TYPENAME){ \
 MMAGIC_EVAL(REFLECTAGA_ACTION_LINES( __VA_ARGS__ )) \
} \
REFLECTAGA_COUNT_MEMBERS(TYPENAME, __VA_ARGS__) 

namespace reflectaga
{
    template<typename value_t> constexpr int getNumberOfMembers() { return 0; }

    class ReflectagaProbe
    {
        template <typename value_t>
        void operator()(value_t&, const char* name) {};
    };

    template <typename value_t>
    concept Reflectaga_t = requires(ReflectagaProbe & probe, value_t value)
    {
        reflectAction(probe, value);
    };

}

