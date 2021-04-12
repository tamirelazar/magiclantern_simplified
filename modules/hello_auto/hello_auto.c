/***
 * An expanded version of the original hello_world module from the forum.
 * expansions are mostly extra documentation for myself.
 ***/

/* === Libraries === */
#include <dryos.h> /* reversed engineered api of canon's native OS */
#include <module.h> /* important for communicating module metadata to ML main */
#include <menu.h> /* creates menu items, which run tasks - example below */
#include <config.h> /* for config vars, maybe more */
#include <console.h> /* ??? */
#include <property.h>
#include <lens.h>

/* Config vars. They are used for persistent variables (settings, usually).
 *
 * In modules, these variables are declared as MODULE_CONFIG.
 */
static CONFIG_INT("hello_auto.is_active", is_module_active, 0);
static CONFIG_INT("hello_auto.counter", hello_auto_counter, 0);



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
static void hello_auto_task()
{
    /* Open the console */
    /* Also wait for background tasks to settle while closing ML menu */
    msleep(2000);
    console_clear();
    console_show();

    /* Plain printf goes to console */
    /* Very limited stdio support */
    printf("Nice shot!.\n");
    printf("You took %d nice shots since I started count. \n", ++hello_auto_counter);
    printf("Half press shutter to exit compliment mode. \n");

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

static struct menu_entry hello_auto_menu[] = 
{
    {
        .name    = "Picture Counter",
        .select  = run_in_separate_task,
        .priv    = hello_auto_task,
        .help    = "Prints the number of pictures taken since activation.",
    },
};

/* This is a fucntion that's called when the module loads. */
/* All module init functions are called seuentially,
 * in alphabetical order. */
static unsigned int hello_auto_init()
{
    menu_add("Debug", hello_auto_menu, COUNT(hello_auto_menu));
    return 0;
}

/* this is for module unloading,
 * which is yet to be supported */
static unsigned int hello_auto_deinit()
{
    return 0;
}

/* All modules have some metadata, specifying init/deinit functions,
 * config variables, event hooks, property handlers and more.
 */
MODULE_INFO_START()
    MODULE_INIT(hello_auto_init)
    MODULE_DEINIT(hello_auto_deinit)
MODULE_INFO_END()

MODULE_CBRS_START()
    MODULE_CBR(CBR_SHOOT_TASK, hello_auto_task, 0)
MODULE_CBRS_END()

MODULE_CONFIGS_START()
    MODULE_CONFIG(is_module_active)
    MODULE_CONFIG(hello_auto_counter)
MODULE_CONFIGS_END()