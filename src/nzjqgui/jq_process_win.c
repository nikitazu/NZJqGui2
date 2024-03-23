#include "jq_process_win.h"
#include "Windows.h"

#define PATH_BUFFER_SIZE 512
#define PROC_PIPE_BUFFER_SIZE 512
#define JQ_EXE "jq-windows-amd64.exe"


typedef struct _pipe_t Pipe;
typedef struct _utf16string_t Utf16string;
typedef struct _os_win_process_t OsWinProcess;


struct _pipe_t
{
    bool_t ok;
    HANDLE read_handle;
    HANDLE write_handle;
};


static Pipe* pipe_create(LPSECURITY_ATTRIBUTES security_attr_p)
{
    Pipe* pipe = heap_new0(Pipe);

    pipe->read_handle = NULL;
    pipe->write_handle = NULL;
    pipe->ok = CreatePipe(&(pipe->read_handle), &(pipe->write_handle), security_attr_p, 0);

    return pipe;
}

static void pipe_close(Pipe** pipe)
{
    Pipe* p = *pipe;

    if (p->write_handle != NULL) {
        CloseHandle(p->write_handle);
        p->write_handle = NULL;
    }

    if (p->read_handle != NULL) {
        CloseHandle(p->read_handle);
        p->read_handle = NULL;
    }

    heap_delete(pipe, Pipe);
    *pipe = NULL;
}

static void pipe_connect_stderr(Pipe* pipe, STARTUPINFOW* startup_info)
{
    startup_info->hStdError = pipe->write_handle;
}

static void pipe_connect_stdout(Pipe* pipe, STARTUPINFOW* startup_info)
{
    startup_info->hStdOutput = pipe->write_handle;
}

static void pipe_connect_stdin(Pipe* pipe, STARTUPINFOW* startup_info)
{
    startup_info->hStdInput = pipe->read_handle;
}

static bool_t pipe_set_read_no_inherit(Pipe* pipe)
{
    return SetHandleInformation(pipe->read_handle, HANDLE_FLAG_INHERIT, 0);
}

static bool_t pipe_set_write_no_inherit(Pipe* pipe)
{
    return SetHandleInformation(pipe->write_handle, HANDLE_FLAG_INHERIT, 0);
}

static bool_t pipe_write_string(Pipe* pipe, String* str)
{
    DWORD bytes_written = 0;
    char_t* str_p = tc(str);
    DWORD str_len = blib_strlen(str_p);

    bool_t ok = WriteFile(pipe->write_handle, str_p, str_len, &bytes_written, NULL);

    return ok && bytes_written > 0;
}

static bool_t pipe_read_string(Pipe* pipe, Stream* output)
{
    DWORD bytes_read = 0;
    char_t ch_buffer[PROC_PIPE_BUFFER_SIZE];
    bool_t ok = FALSE;

    do {
        ok = ReadFile(pipe->read_handle, ch_buffer, PROC_PIPE_BUFFER_SIZE, &bytes_read, NULL);

        if (ok) {
            stm_write(output, ch_buffer, bytes_read);
        }
    } while (ok && bytes_read == PROC_PIPE_BUFFER_SIZE);

    return ok;
}



struct _utf16string_t
{
    wchar_t* data_p;
    uint32_t size;
};

static Utf16string utf16string_from_string(String* str)
{
    Utf16string result = { NULL, 0 };

    char_t* str_p = tc(str);
    uint32_t str_len = blib_strlen(str_p);

    int buffer_size = MultiByteToWideChar(
        CP_UTF8,
        0,                  // flags are deprecated
        str_p,
        -1,                 // null terminated string
        NULL,               // we are calculating size, buffer not needed
        0                   // 0 means we need to calculate the required buffer size
    );

    if (buffer_size <= 0) {
        return result;
    }

    wchar_t* buffer = heap_new_n(buffer_size, wchar_t);

    int buffer_create_resut = MultiByteToWideChar(
        CP_UTF8,
        0,                  // flags are deprecated
        str_p,
        -1,                 // null terminated string
        buffer,
        buffer_size
    );

    result.data_p = buffer;
    result.size = buffer_size;

    return result;
}

static void utf16string_destroy(Utf16string str)
{
    if (str.data_p != NULL) {
        heap_delete_n(&str.data_p, str.size, wchar_t);
        str.data_p = NULL;
        str.size = 0;
    }
}



struct _os_win_process_t
{
    bool_t success;
    STARTUPINFOW startup_info;
    PROCESS_INFORMATION process_info;
    Pipe* pipe_in_p;
    Pipe* pipe_out_p;
};


static OsWinProcess* os_win_process_start(String* path, String* args, Pipe* pipe_in, Pipe* pipe_out)
{
    OsWinProcess* process = heap_new0(OsWinProcess);
    String* cli = NULL;

    process->startup_info.cb = sizeof(process->startup_info);
    process->startup_info.dwFlags |= STARTF_USESTDHANDLES;

    pipe_connect_stderr(pipe_out, &process->startup_info);
    pipe_connect_stdout(pipe_out, &process->startup_info);
    pipe_connect_stdin(pipe_in, &process->startup_info);

    /* Запуск подпроцесса <jq> */

    cli = str_printf("\"%s\" \"%s\"", tc(path), tc(args));

    // Utf16string exec_jq_path_utf16 = utf16string_from_string(path);
    // Utf16string jp_command_line_utf16 = utf16string_from_string(cli);

    process->success = CreateProcess( // W
        tc(path),
        tcc(args),
        // exec_jq_path_utf16.data_p,
        // jp_command_line_utf16.data_p,
        NULL,
        NULL,
        TRUE,
        CREATE_NO_WINDOW, // https://learn.microsoft.com/en-us/windows/win32/procthread/process-creation-flags
        NULL,
        NULL,
        &process->startup_info,
        &process->process_info
    );

    // utf16string_destroy(exec_jq_path_utf16);
    // utf16string_destroy(jp_command_line_utf16);
    str_destroy(&cli);

    return process;
}

static void os_win_process_destroy(OsWinProcess** process)
{
    OsWinProcess* p = *process;

    CloseHandle(p->process_info.hProcess);
    CloseHandle(p->process_info.hThread);

    heap_delete(process, OsWinProcess);
    *process = NULL;
}


bool_t jq_process_run_win(const char_t* json, const char_t* query, Stream* output)
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

    Utf16string json_trimmed_utf16 = { NULL, 0 };

    Pipe* child_in = NULL;
    Pipe* child_out = NULL;
    OsWinProcess* process = NULL;

    json_trimmed = str_trim(json);
    if (str_nchars(json_trimmed) == 0) {
        stm_printf(output, "[ОШБ] JSON пуст!\n");
        goto JQ_PROCESS_RUN__FINISH;
    }

    stm_printf(output, "[ИНФ] JSON: ЕСТЬ\n");

    query_trimmed = str_trim(query);
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

    /* Подготовка каналов ввода/вывода */

    SECURITY_ATTRIBUTES security_attr;
    ZeroMemory(&security_attr, sizeof(security_attr));
    security_attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    security_attr.bInheritHandle = TRUE;
    security_attr.lpSecurityDescriptor = NULL;

    child_in = pipe_create(&security_attr);

    if (!child_in->ok) {
        stm_printf(output, "[ОШБ] Не удалось создать канал ввода!\n");
        goto JQ_PROCESS_RUN__FINISH;
    }

    if (!pipe_set_write_no_inherit(child_in)) {
        stm_printf(output, "[ОШБ] Не удалось установить канал ввода NO INHERIT!\n");
        goto JQ_PROCESS_RUN__FINISH;
    }

    child_out = pipe_create(&security_attr);

    if (!child_out->ok) {
        stm_printf(output, "[ОШБ] Не удалось создать канал вывода!\n");
        goto JQ_PROCESS_RUN__FINISH;
    }

    if (!pipe_set_read_no_inherit(child_out)) {
        stm_printf(output, "[ОШБ] Не удалось установить канал вывода NO INHERIT!\n");
        goto JQ_PROCESS_RUN__FINISH;
    }

    /* Запуск подпроцесса <jq> */

    jq_command_line = str_printf("\"%s\" \"%s\"", tc(exec_jq_path), tc(query_trimmed));
    stm_printf(output, "[ИНФ] <jq> CLI: %s\n", tc(jq_command_line));

    process = os_win_process_start(exec_jq_path, jq_command_line, child_in, child_out);

    if (!process->success) {
        DWORD error_code = GetLastError();
        LPVOID lpMsgBuf = NULL;

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER
            | FORMAT_MESSAGE_FROM_SYSTEM
            | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error_code,
            MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
            (LPTSTR)&lpMsgBuf,
            0,
            NULL
        );

        stm_printf(output, "[ОШБ] Не удалось запустить <jq> (КОД ОШИБКИ %d)!\n", error_code);
        stm_printf(output, "      %s\n", lpMsgBuf);
        LocalFree(lpMsgBuf);
        goto JQ_PROCESS_RUN__FINISH;
    }

    if (!pipe_write_string(child_in, json_trimmed)) {
        stm_printf(output, "[ОШБ] Не удалось отправить <JSON> в <jq>!\n");
        goto JQ_PROCESS_RUN__FINISH;
    }

    pipe_close(&child_in);

    if (!pipe_read_string(child_out, output)) {
        stm_printf(output, "[ОШБ] Не удалось прочитать вывод <jq>!\n");
        goto JQ_PROCESS_RUN__FINISH;
    }

    result = TRUE;

JQ_PROCESS_RUN__FINISH:

    str_destopt(&json_trimmed);
    str_destopt(&query_trimmed);
    str_destopt(&exec_file_name);
    str_destopt(&exec_dir_path);
    str_destopt(&exec_jq_path);
    str_destopt(&jq_command_line);

    utf16string_destroy(json_trimmed_utf16);

    if (child_in != NULL) {
        pipe_close(&child_in);
    }

    if (child_out != NULL) {
        pipe_close(&child_out);
    }

    if (process != NULL) {
        os_win_process_destroy(&process);
    }

    return result;
}
