/* A very simple module
 * (example for aspiring module authors)
 */
#include <dryos.h>
#include <module.h>
#include <menu.h>
#include <config.h>
#include <console.h>

/* Config vars. They are used for persistent variables (settings, usually).
 *
 * In modules, these variables are declared as MODULE_CONFIG.
 */
static CONFIG_INT("hello.counter", hello_counter, 0);

/* This function runs as a new DryOS task, in parallel with everything else.
 *
 * Tasks started in this way have priority 0x1A (relevant to run_in_seperate_task in menu.c).
 * They can be interrupted by other tasks with higher priorities (which are lower values)
 * at any time, or by any other task when waiting.
 *
 * This means other tasks of the same priority won't interrupt outside of waiting.
 *
 * Also, tasks started this way will close ML menu and Canon powersave will be disabled while task is running.
 */
static void hello_task()
{
    /* Open the console */
    /* Also wait for background tasks to settle while closing ML menu */
    msleep(2000);
    console_clear();
    console_show();

    /* Plain printf goes to console */
    /* Very limited stdio support */
    printf("Tamir, you are amazing.\n");
    printf("You have been amazing %d times. \n", ++hello_counter);
    printf("Half press shutter to compliment mode. \n");

    /* half shutter is one of few keys that can be checked from regular task */
    /* other keys usually require keypress hook */
    while (!get_halfshutter_pressed())
    {
        /* this is a waiting loop, so we will sleep to let other tasks to run */
        msleep(100);
    }

    /* Done */
    console_hide();
}

static struct menu_entry hello_menu[] = 
{
    {
        .name    = "Hello, World!",
        .select  = run_in_separate_task,
        .priv    = hello_task,
        .help    = "Prints 'Hello, World!' on the console.",
    },
};

/* This is a fucntion that's called when the module loads. */
/* All module init functions are called seuentially,
 * in alphabetical order. */
static unsigned int hello_init()
{
    menu_add("Debug", hello_menu, COUNT(hello_menu));
    return 0;
}

/* this is for module unloading,
 * which is yet to be supported */
static unsigned int hello_deinit()
{
    return 0;
}

/* All modules have some metadata, specifying init/deinit functions,
 * config variables, event hooks, property handlers and more.
 */
MODULE_INFO_START()
    MODULE_INIT(hello_init)
    MODULE_DEINIT(hello_deinit)
MODULE_INFO_END()

MODULE_CONFIGS_START()
    MODULE_CONFIG(hello_counter)
MODULE_CONFIGS_END()