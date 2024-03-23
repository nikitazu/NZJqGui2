#include "jq_process.h"

#define PATH_BUFFER_SIZE 1024
#define JQ_EXE "jq-windows-amd64.exe"

bool_t jq_process_run(String* json, String* query, Stream* output)
{
    /* Проверка входных данных */

    cassert_fatal(json != NULL);
    cassert_fatal(query != NULL);
    cassert_fatal(output != NULL);

    bool_t result = FALSE;
    String* json_trimmed = NULL;
    String* query_trimmed = NULL;
    String* exec_file_name = NULL;
    String* exec_dir_path = NULL;
    String* exec_jq_path = NULL;

    json_trimmed = str_trim(tc(json));
    if (str_nchars(json_trimmed) == 0) {
        stm_printf(output, "[ОШБ] JSON пуст!\n");
        goto JQ_PROCESS_RUN__FINISH;
    }

    stm_printf(output, "[ИНФ] JSON: ЕСТЬ\n");

    query_trimmed = str_trim(tc(query));
    if (str_nchars(query_trimmed) == 0) {
        stm_printf(output, "[ОШБ] Запрос пуст!\n");
        goto JQ_PROCESS_RUN__FINISH;
    }

    stm_printf(output, "[ИНФ] Запрос: ЕСТЬ\n");

    /* Проверка ресурсов */

    char_t exec_path[PATH_BUFFER_SIZE];
    uint32_t exec_path_len = bfile_dir_exec(exec_path, PATH_BUFFER_SIZE);
    str_split_pathname(exec_path, &exec_dir_path, &exec_file_name);
    exec_jq_path = str_cpath("%s/%s", tc(exec_dir_path), JQ_EXE);

    stm_printf(output, "[ИНФ] Путь исполнения: <%s>\n", exec_path);
    stm_printf(output, "[ИНФ] Директория программы: <%s>\n", tc(exec_dir_path));
    stm_printf(output, "[ИНФ] Исполнимый файл: <%s>\n", tc(exec_file_name));
    stm_printf(output, "[ИНФ] Исполнимый файл JQ: <%s>\n", tc(exec_jq_path));

    if (!hfile_exists(tc(exec_jq_path), NULL)) {
        stm_printf(output, "[ОШБ] Исполняемый файл <jq> не найден в каталоге программы!\n");
        stm_printf(output, "      <%s>\n", tc(exec_jq_path));
        goto JQ_PROCESS_RUN__FINISH;
    }

    stm_printf(output, "[ИНФ] JQ: ЕСТЬ\n");

    // TODO create pipes
    // TODO start process
    // TODO write json to process stdin
    // TODO read process stdout / stderr
    // TODO wait for process exit
    // TODO check process exit code
    // TODO write output
    // TODO return success or failure

JQ_PROCESS_RUN__FINISH:

    str_destopt(&json_trimmed);
    str_destopt(&query_trimmed);
    str_destopt(&exec_file_name);
    str_destopt(&exec_dir_path);
    str_destopt(&exec_jq_path);

    return result;
}
