#ifndef ZSTACK_LOG_H
#define ZSTACK_LOG_H

//=============================================================================

enum log_level_e {
    LOG_ERROR,      // system can't run
    LOG_WARNING,    //
    LOG_USER,       // application info, user need to know
    LOG_DEBUG,       // developer, [function:line] ...
};

enum log_module_e {
    LOG_MODULE_DEFAULT,
    LOG_MODULE_APP,
    LOG_MODULE_UI,
    LOG_MODULE_MISC,
    LOG_MODULE_NET,
    LOG_MODULE_AVMIPS,
    LOG_MODULE_ALGO,
    LOG_MODULE_MAX
};

//=============================================================================

#ifndef LOG_MODULE
#define LOG_MODULE LOG_MODULE_DEFAULT
#endif

#define log(lvl, argv, ...) _log(LOG_MODULE, lvl, __FILE__, __FUNCTION__, __LINE__, argv, ##__VA_ARGS__)

#define ENTER log(LOG_DEBUG, "\n")
#define EXIT  log(LOG_DEBUG, "\n")

//=============================================================================

extern void log_init(char *config);
extern int _log(int module, int lvl, char *filename, char *function, int linenum, const char *fmt, ...);

//=============================================================================

#endif
