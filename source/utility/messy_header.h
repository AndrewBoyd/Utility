#ifndef ACCEPT_MESSY_HEADER
# error Please define ACCEPT_MESSY_HEADER to acknowledge that this header may pollute the global namespace and should not be used in low-level headers
#else
# undef ACCEPT_MESSY_HEADER
#endif
