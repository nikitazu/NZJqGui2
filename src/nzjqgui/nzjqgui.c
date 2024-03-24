/* Графический интерфейс для утилиты `jq`
 * ==
 */

#include <nappgui.h>
#include "gui_helpers.h"
#include "jq_process_win.h"
#include "main_menu.h"

/* Стандартные отступы
 * --
 */

const real32_t nzGUI_MARGIN_S = 4;
const real32_t nzGUI_MARGIN_M = 8;

/* Прочие константы
 * --
 */

const uint32_t nzMEMORY_STREAM_INIT_SIZE = 2048;

/* Модель приложения
 * --
 */

typedef struct _app_t App;

struct _app_t
{
    Window* window;
    Menu* main_menu;
    Edit* json_edit;
    Edit* query_edit;
    TextView* result_textview;
    uint32_t clicks;
};

static void i_OnButton(App* app, Event* e)
{
    const char_t* json = edit_get_text(app->json_edit);
    const char_t* query = edit_get_text(app->query_edit);

    Stream* output = stm_memory(nzMEMORY_STREAM_INIT_SIZE);

    bool_t success = jq_process_run_win(json, query, output);

    String* message = stm_str(output);
    stm_close(&output);

    textview_writef(app->result_textview, tc(message));
    str_destroy(&message);

    app->clicks += 1;
    unref(e);
}

static Panel* i_panel(App* app)
{
    /* Контент
     */

    Panel* panel = panel_create();
    Layout* layout = layout_create(1, 6);

    /* Надпись JSON */
    Label* json_label = label_create();
    label_text(json_label, "JSON");

    /* Поле ввода JSON */
    Edit* json_edit = edit_multiline();

    /* Надпись Запрос */
    Label* query_label = label_create();
    label_text(query_label, "Запрос");

    /* Панель Запрос */
    Layout* query_layout = layout_create(2, 1);

        /* Поле ввода Запрос */
        Edit* query_edit = edit_create();

        /* Кнопка Отправить */
        Button* send_button = button_push();
        button_text(send_button, "Отправить");

    /* Надпись Результат */
    Label* result_label = label_create();
    label_text(result_label, "Результат");

    /* Поле вывода Результат */
    TextView* result_textview = textview_create();

    /* Регистрация обработчиков событий
     */

    button_OnClick(send_button, listener(app, i_OnButton, App));

    app->json_edit = json_edit;
    app->query_edit = query_edit;
    app->result_textview = result_textview;

    /* Расположение
     */

    layout_label(layout, json_label, 0, 0);
    layout_edit(layout, json_edit, 0, 1);
    layout_label(layout, query_label, 0, 2);
    layout_layout(layout, query_layout, 0, 3);
        layout_edit(query_layout, query_edit, 0, 0);
        layout_button(query_layout, send_button, 1, 0);
    layout_label(layout, result_label, 0, 4);
    layout_textview(layout, result_textview, 0, 5);

    layout_vexpand2(layout, 1, 5, 0.5);
        layout_hexpand2(query_layout, 0, 1, 1);

    layout_margin(layout, nzGUI_MARGIN_M);
    layout_vmargin(layout, 0, nzGUI_MARGIN_S);
    layout_vmargin(layout, 1, nzGUI_MARGIN_S);
    layout_vmargin(layout, 2, nzGUI_MARGIN_S);
    layout_vmargin(layout, 3, nzGUI_MARGIN_S);
        layout_hmargin(query_layout, 0, nzGUI_MARGIN_S);
    layout_vmargin(layout, 4, nzGUI_MARGIN_S);
    panel_layout(panel, layout);

    return panel;
}

static void i_OnClose(App* app, Event* e)
{
    osapp_finish();
    unref(app);
    unref(e);
}

static App* i_create(void)
{
    App* app = heap_new0(App);
    Panel* panel = i_panel(app);

    app->main_menu = main_menu_create();
    app->window = window_create(ekWINDOW_STDRES);

    window_panel(app->window, panel);
    window_title(app->window, "NZ JQ Gui 2 v0.1.0");
    window_size(app->window, s2df(800, 600));
    window_set_center_screen_origin(app->window);
    osapp_menubar(app->main_menu, app->window);

    window_OnClose(app->window, listener(app, i_OnClose, App));
    window_show(app->window);

    return app;
}

static void i_destroy(App** app)
{
    Window* window = (*app)->window;
    Menu* main_menu = (*app)->main_menu;

    window_destroy(&window);
    menu_destroy(&main_menu);

    heap_delete(app, App);
}

#include "osmain.h"
osmain(i_create, i_destroy, "", App);
