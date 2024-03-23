#ifndef JQ_PROCESS_H
#define JQ_PROCESS_H

/* АПИ для работы с утилитой `jq` (Windows) */

#include <nappgui.h>

__EXTERN_C

bool_t jq_process_run_win(String* json, String* query, Stream* output);

__END_C

#endif
