#include "main_menu.h"


/* Определения "наперёд"
 * --
 */

static Menu* i_file_menu_create(void);
static void i_OnFileOpen(void* ctrl, Event* e);
static void i_OnFileSave(void* ctrl, Event* e);
static void i_OnFileClose(void* ctrl, Event* e);
static Menu* i_help_menu_create(void);
static void i_OnHelpAbout(void* ctrl, Event* e);
static void i_OnHelpManual(void* ctrl, Event* e);


/* Публичные функции
 * --
 */

Menu* main_menu_create(void)
{
    Menu* menu = menu_create();

    MenuItem* file_item = menuitem_create();
    MenuItem* help_item = menuitem_create();

    Menu* file_submenu = i_file_menu_create();
    Menu* help_submenu = i_help_menu_create();

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

static Menu* i_file_menu_create(void)
{
    Menu* menu = menu_create();

    MenuItem* open_item = menuitem_create();
    MenuItem* save_item = menuitem_create();
    MenuItem* quit_item = menuitem_create();

    menuitem_text(open_item, "Открыть");
    menuitem_text(save_item, "Сохранить");
    menuitem_text(quit_item, "Выход");

    menuitem_OnClick(open_item, listener(NULL, i_OnFileOpen, void*));
    menuitem_OnClick(save_item, listener(NULL, i_OnFileSave, void*));
    menuitem_OnClick(quit_item, listener(NULL, i_OnFileClose, void*));

    menu_item(menu, open_item);
    menu_item(menu, save_item);
    menu_item(menu, quit_item);

    return menu;
}

static void i_OnFileOpen(void* ctrl, Event* e)
{
    bstd_printf("[ИНФ] Меню: Файл -> Открыть\n");
    unref(e);
}

static void i_OnFileSave(void* ctrl, Event* e)
{
    bstd_printf("[ИНФ] Меню: Файл -> Сохранить\n");
    unref(e);
}

static void i_OnFileClose(void* ctrl, Event* e)
{
    bstd_printf("[ИНФ] Меню: Файл -> Выход\n");
    unref(e);
}

/* Меню Справка */

static Menu* i_help_menu_create(void)
{
    Menu* menu = menu_create();

    MenuItem* about_item = menuitem_create();
    MenuItem* manual_item = menuitem_create();

    menuitem_text(about_item, "О программе");
    menuitem_text(manual_item, "Руководство пользователя");

    menuitem_OnClick(about_item, listener(NULL, i_OnHelpAbout, void*));
    menuitem_OnClick(manual_item, listener(NULL, i_OnHelpManual, void*));

    menu_item(menu, about_item);
    menu_item(menu, manual_item);

    return menu;
}

static void i_OnHelpAbout(void* ctrl, Event* e)
{
    bstd_printf("[ИНФ] Меню: Справка -> О программе\n");
    unref(e);
}

static void i_OnHelpManual(void* ctrl, Event* e)
{
    bstd_printf("[ИНФ] Меню: Справка -> Руководство пользователя\n");
    unref(e);
}
