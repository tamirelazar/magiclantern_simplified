/* === Libraries === */
#include <dryos.h> /* reversed engineered api of canon's native OS */
#include <module.h> /* important for communicating module metadata to ML main */
#include <menu.h> /* creates menu items, which run tasks - example below */
#include <config.h> /* for config vars, maybe more */
#include <console.h> /* ??? */

/* === Code Snippets === */

/* Print to console */

    /* Open the console */
    /* Also wait for background tasks to settle while closing ML menu */
    msleep(2000);
    console_clear();
    console_show();

    /* Plain printf goes to console */
    /* Very limited stdio support */
    printf("Nice Shot!.\n");
    
    /* Done */
    console_hide();

/* pull the last shot image from memory */

/* draw something transparent on liveview */

/* recognize when a photo was shot */
MODULE_CBRS_START()
    MODULE_CBR(CBR_SHOOT_TASK, isoless_refresh, CTX_SHOOT_TASK)
    MODULE_CBR(CBR_SHOOT_TASK, isoless_playback_fix, CTX_SHOOT_TASK)
MODULE_CBRS_END()

/* checks that camera is not writing to card/busy, def in property.h */
while (lens_info.job_state) msleep(100);