#include "jq_process.h"

#define PATH_BUFFER_SIZE 512
#define PROC_PIPE_BUFFER_SIZE 512
#define JQ_EXE "jq-windows-amd64.exe"

static bool_t bproc_jq_write_string(Proc* proc, String* str)
{
    Stream* tmp = stm_memory(PROC_PIPE_BUFFER_SIZE);
    str_write(tmp, str);
    bool_t result = bproc_write(proc, stm_buffer(tmp), stm_buffer_size(tmp), NULL, NULL);
    stm_close(&tmp);
    return result;
}

static bool_t bproc_jq_read(Proc* proc, Stream* output)
{
    bool_t result = TRUE;
    byte_t buffer[PROC_PIPE_BUFFER_SIZE];
    uint32_t bytes_read = 0;

    bstd_printf("[DBG] b bytes_read=%u\n", bytes_read);

    while (bproc_read(proc, buffer, PROC_PIPE_BUFFER_SIZE, &bytes_read, NULL)) {
        bstd_printf("[DBG] b bytes_read=%u\n", bytes_read);
        stm_write(output, buffer, bytes_read);
    }

    while (bproc_eread(proc, buffer, PROC_PIPE_BUFFER_SIZE, &bytes_read, NULL)) {
        bstd_printf("[ERR] b bytes_read=%u from <STDERR>\n", bytes_read);
        bstd_printf("      %s\n", buffer);
        stm_write(output, buffer, bytes_read);
        result = FALSE;
    }

    return result;
}


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
    String* jq_command_line = NULL;

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

    stm_printf(output, "[ИНФ] <jq>: ЕСТЬ\n");

    /* Запуск подпроцесса <jq> */

    jq_command_line = str_printf("\"%s\" \"%s\"", tc(exec_jq_path), tc(query));

    stm_printf(output, "[ИНФ] <jq> CLI: %s\n", tc(jq_command_line));

    perror_t proc_error;
    Proc* proc = bproc_exec(jq_command_line, &proc_error);

    if (proc == NULL) {
        stm_printf(output, "[ОШБ] Не удалось запустить <jq>!\n");
        goto JQ_PROCESS_RUN__FINISH;
    }

    switch (proc_error)
    {
    case ekPPIPE:
        stm_printf(output, "[ОШБ] Ошибка ввода вывода!\n");
        goto JQ_PROCESS_RUN__FINISH;

    case ekPEXEC:
        stm_printf(output, "[ОШБ] Некорректные параметры запуска!\n");
        goto JQ_PROCESS_RUN__FINISH;

    case ekPOK:
    default:
        stm_printf(output, "[ИНФ] <jq> запущен успешно!\n");
        break;
    }

    if (!bproc_jq_write_string(proc, json)) {
        stm_printf(output, "[ОШБ] Не удалось отправить <JSON> в <jq>!\n");
        bproc_write_close(proc);
        goto JQ_PROCESS_RUN__FINISH;
    }

    bproc_write_close(proc);

    if (!bproc_jq_read(proc, output)) {
        stm_printf(output, "[ОШБ] Не удалось прочитать вывод <jq>!\n");
        goto JQ_PROCESS_RUN__FINISH;
    }

    result = TRUE;

    // TODO check process exit code

JQ_PROCESS_RUN__FINISH:

    str_destopt(&json_trimmed);
    str_destopt(&query_trimmed);
    str_destopt(&exec_file_name);
    str_destopt(&exec_dir_path);
    str_destopt(&exec_jq_path);
    str_destopt(&jq_command_line);

    bproc_read_close(proc);
    bproc_eread_close(proc);
    bproc_wait(proc);
    bproc_close(&proc);

    return result;
}
