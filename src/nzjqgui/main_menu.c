#include "main_menu.h"
#include "controller.h"
#include "gui_helpers.h"
#include "gui_sizes.h"


#define FILE_TYPE_ARR_SIZE 1
#define DIALOG_WINDOW_FLAGS ekWINDOW_TITLE | ekWINDOW_CLOSE | ekWINDOW_RETURN | ekWINDOW_ESC
#define DIALOG_WINDOW_SIZE s2df(500, 300)


/* Определения "наперёд"
 * --
 */

static Menu* i_file_menu_create(Controller* ctrl);
static void i_OnFileOpen(Controller* ctrl, Event* e);
static void i_OnFileSave(Controller* ctrl, Event* e);
static void i_OnFileQuit(Controller* ctrl, Event* e);
static Menu* i_help_menu_create(Controller* ctrl);
static void i_OnHelpAbout(Controller* ctrl, Event* e);
static void i_OnHelpManual(Controller* ctrl, Event* e);


/* Публичные функции
 * --
 */

Menu* main_menu_create(Controller* ctrl)
{
    Menu* menu = menu_create();

    MenuItem* file_item = menuitem_create();
    MenuItem* help_item = menuitem_create();

    Menu* file_submenu = i_file_menu_create(ctrl);
    Menu* help_submenu = i_help_menu_create(ctrl);

    menuitem_text(file_item, "Файл");
    menuitem_text(help_item, "Справка");

    menuitem_submenu(file_item, &file_submenu);
    menuitem_submenu(help_item, &help_submenu);

    menu_item(menu, file_item);
    menu_item(menu, help_item);

    return menu;
}


/* Закрытые функции
 * --
 */

/* Меню Файл */

static Menu* i_file_menu_create(Controller* ctrl)
{
    Menu* menu = menu_create();

    MenuItem* open_item = menuitem_create();
    MenuItem* save_item = menuitem_create();
    MenuItem* sepr_item = menuitem_separator();
    MenuItem* quit_item = menuitem_create();

    menuitem_text(open_item, "Открыть");
    menuitem_text(save_item, "Сохранить (в разработке)");
    menuitem_text(quit_item, "Выход");

    menuitem_enabled(save_item, FALSE);

    menuitem_OnClick(open_item, listener(ctrl, i_OnFileOpen, Controller));
    menuitem_OnClick(save_item, listener(ctrl, i_OnFileSave, Controller));
    menuitem_OnClick(quit_item, listener(ctrl, i_OnFileQuit, Controller));

    menu_item(menu, open_item);
    menu_item(menu, save_item);
    menu_item(menu, sepr_item);
    menu_item(menu, quit_item);

    return menu;
}

static void i_OnFileOpen(Controller* ctrl, Event* e)
{
    char_t* path = NULL;
    String* file_content = NULL;
    file_type_t file_type;

    bstd_printf("[ИНФ] Меню: Файл -> Открыть\n");

    Window* main_window = controller_get_main_window(ctrl);
    char_t* file_types[FILE_TYPE_ARR_SIZE] = { "*.json" };

    path = comwin_open_file(main_window, file_types, FILE_TYPE_ARR_SIZE, NULL);

    if (path != NULL) {
        bstd_printf("[ИНФ] Выбран файл: %s\n", path);
        if (hfile_exists(path, &file_type) && file_type == ekARCHIVE) {
            bstd_printf("[ИНФ] Найден файл: %s\n", path);
            ferror_t err;
            file_content = hfile_string(path, &err);
            if (err == ekFOK && file_content != NULL) {
                Edit* json_edit = controller_get_json_edit(ctrl);
                edit_text(json_edit, tc(file_content));
            }
        }
    }

    str_destopt(&file_content);
    unref(e);
}

static void i_OnFileSave(Controller* ctrl, Event* e)
{
    bstd_printf("[ИНФ] Меню: Файл -> Сохранить\n");
    unref(e);
}

static void i_OnFileQuit(Controller* ctrl, Event* e)
{
    bstd_printf("[ИНФ] Меню: Файл -> Выход\n");
    osapp_finish();
    unref(ctrl);
    unref(e);
}

/* Меню Справка */

static Menu* i_help_menu_create(Controller* ctrl)
{
    Menu* menu = menu_create();

    MenuItem* about_item = menuitem_create();
    MenuItem* manual_item = menuitem_create();

    menuitem_text(about_item, "О программе (в разработке)");
    menuitem_text(manual_item, "Руководство пользователя (в разработке)");

    menuitem_enabled(manual_item, FALSE);

    menuitem_OnClick(about_item, listener(ctrl, i_OnHelpAbout, Controller));
    menuitem_OnClick(manual_item, listener(ctrl, i_OnHelpManual, Controller));

    menu_item(menu, about_item);
    menu_item(menu, manual_item);

    return menu;
}

static void i_OnHelpAbout(Controller* ctrl, Event* e)
{
    bstd_printf("[ИНФ] Меню: Справка -> О программе\n");

    Window* main_win = controller_get_main_window(ctrl);
    S2Df main_win_size = window_get_size(main_win);
    V2Df main_win_pos = window_get_origin(main_win);

    Window* about_win = window_create(DIALOG_WINDOW_FLAGS);
    Panel* panel = panel_create();
    Layout* main_layout = layout_create(1, 4);
    Layout* prop_layout = layout_create(2, 3);
    Layout* foot_layout = layout_create(1, 2);

    /* Название программы */
    Label* title_label = label_create();
    label_text(title_label, "NZ JQ Gui 2");

    /* Версия */
    Label* version_label = label_create();
    label_text(version_label, "Версия");

    /* Значение версия */
    Label* version_val_label = label_create();
    label_text(version_val_label, "0.1.0");

    /* Автор */
    Label* author_label = label_create();
    label_text(author_label, "Автор");

    /* Значение автора */
    Label* author_val_label = label_create();
    label_text(author_val_label, "Никита Б. Зуев <nikitazu@gmail.com>");

    /* Лицензия */
    Label* license_label = label_create();
    label_text(license_label, "Лицензия");

    /* Значение лицензии */
    Label* license_val_label = label_create();
    label_text(license_val_label, "GPLv3");

    /* Благодарности */
    Label* thanks_label = label_create();
    label_text(thanks_label, "Благодарности");

    /* Текст благодарности */
    Label* thanks_val_label = label_multiline();
    label_align(thanks_val_label, ekTOP);
    label_text(
        thanks_val_label,
        "Francisco García Collado,\n"
        "  автор NAppGUI SDK (MIT License)\n"
        "  https://nappgui.com/en/home/web/home.html\n"
        "  \n"
    );

    layout_label(main_layout, title_label, 0, 0);
    layout_layout(main_layout, prop_layout, 0, 1);
        layout_label(prop_layout, version_label, 0, 0);
        layout_label(prop_layout, version_val_label, 1, 0);
        layout_label(prop_layout, author_label, 0, 1);
        layout_label(prop_layout, author_val_label, 1, 1);
        layout_label(prop_layout, license_label, 0, 2);
        layout_label(prop_layout, license_val_label, 1, 2);
    layout_layout(main_layout, foot_layout, 0, 2);
        layout_label(foot_layout, thanks_label, 0, 0);
        layout_label(foot_layout, thanks_val_label, 0, 1);

    layout_vexpand2(main_layout, 2, 3, 0.3);
        layout_vexpand(foot_layout, 1);

    layout_margin(main_layout, nzGUI_MARGIN_M);
    layout_vmargin(main_layout, 0, nzGUI_MARGIN_XL);
        layout_vmargin(prop_layout, 0, nzGUI_MARGIN_S);
        layout_vmargin(prop_layout, 1, nzGUI_MARGIN_S);
    layout_vmargin(main_layout, 1, nzGUI_MARGIN_XL);
        layout_vmargin(foot_layout, 0, nzGUI_MARGIN_S);

    panel_layout(panel, main_layout);
    window_panel(about_win, panel);
    window_title(about_win, "О программе");
    window_size(about_win, DIALOG_WINDOW_SIZE);
    window_set_center_parent_origin(about_win, main_win_size, main_win_pos);

    uint32_t result = window_modal(about_win, main_win);
    bstd_printf("[ИНФ] Результат: %ud\n", result);

    window_destroy(&about_win);
    unref(e);
}

static void i_OnHelpManual(Controller* ctrl, Event* e)
{
    bstd_printf("[ИНФ] Меню: Справка -> Руководство пользователя\n");
    unref(e);
}
