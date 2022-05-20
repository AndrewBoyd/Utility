#pragma once
#include "macro_magic.h"
#include <map>

#define REFLECTAGA_TEMPLATE_HEADER template <typename reflect_action_t>
#define RELFECTAGA_FUNCTION_DECLERATION( TYPENAME ) void reflectAction( reflect_action_t & action, TYPENAME & type )
#define REFLECTAGA_ACTION_LINE( X ) action(type.X, #X); 
#define REFLECTAGA_ACTION_LINES( ... ) MMAGIC_MAP(REFLECTAGA_ACTION_LINE, __VA_ARGS__)
#define REFLECTAGA_COUNT_MEMBERS(TYPENAME, ...) \
    namespace reflectaga{ template<> inline constexpr int getNumberOfMembers<TYPENAME>(){ return MMAGIC_COUNT_ARGS(__VA_ARGS__); } }

#define REFLECTAGA(TYPENAME, ...)                    \
REFLECTAGA_TEMPLATE_HEADER                           \
RELFECTAGA_FUNCTION_DECLERATION(TYPENAME){           \
 MMAGIC_EVAL(REFLECTAGA_ACTION_LINES( __VA_ARGS__ )) \
}                                                    \
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
//
//#define REFLECTAGA_ENUM_MAP( enum_name ) MMAGIC_EVAL(MMAGIC_CAT(REFLECTAGA_, enum_name))
//#define REFLECTAGA_ENUM_LINE_E( X ) X,
//#define REFLECTAGA_ENUM_LINES_E( ... ) MMAGIC_MAP(REFLECTAGA_ENUM_LINE_E, __VA_ARGS__)
//#define REFLECTAGA_ENUM_LINE_S( X ) #X,
//#define REFLECTAGA_ENUM_LINES_S( ... ) MMAGIC_MAP(REFLECTAGA_ENUM_LINE_S, __VA_ARGS__)
//#define REFLECTAGA_ENUM_TOSTRING( enum_name ) MMAGIC_EVAL(MMAGIC_CAT(REFLECTAGA_TOSTRING_, enum_name))
//#define REFLECTAGA_ENUM_FROMSTRING( enum_name ) MMAGIC_EVAL(MMAGIC_CAT(REFLECTAGA_FROMSTRING_, enum_name))
//
//
//#define REFLECTAGA_ENUM(ENUM_NAME, ...) \
//enum class ENUM_NAME                                        \
//    { MMAGIC_EVAL(REFLECTAGA_ENUM_LINES_E(__VA_ARGS__) ) }; \
//                                                            \
//static char const * REFLECTAGA_ENUM_MAP(ENUM_NAME) [] =     \
//    { MMAGIC_EVAL(REFLECTAGA_ENUM_LINES_S(__VA_ARGS__)) };                                                                                          //
//
//void REFLECTAGA_ENUM_TOSTRING(enum_name)( enum_name val )                                   \
//{ return MMAGIC_EVAL(REFLECTAGA_ENUM_MAP(enum_name)).at(val); }                             \
//void REFLECTAGA_ENUM_FROMSTRING(enum_name)(std::string name)                                \
//{ return MMAGIC_EVAL(REFLECTAGA_ENUM_MAP(enum_name)).find(name)->second; }                  //
