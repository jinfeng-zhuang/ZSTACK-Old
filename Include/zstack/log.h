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
    LOG_MODULE_FILE,
    LOG_MODULE_MAX
};

//=============================================================================

#ifndef LOG_MODULE
#define LOG_MODULE LOG_MODULE_DEFAULT
#endif

#ifndef log_info
#ifdef _WIN32
#define log_info(argv, ...) _log(LOG_MODULE, LOG_USER, __FILE__, __FUNCTION__, __LINE__, argv, ##__VA_ARGS__)
#else
#define log_info(argv, ...) do {} while (0)
#endif
#endif

#ifndef info
#define info(argv, ...) _log(LOG_MODULE, LOG_USER, __FILE__, __FUNCTION__, __LINE__, argv, ##__VA_ARGS__)
#endif

#ifndef warn
#define warn(argv, ...) _log(LOG_MODULE, LOG_WARNING, __FILE__, __FUNCTION__, __LINE__, argv, ##__VA_ARGS__)
#endif

#ifndef log_warn
#ifdef _WIN32
#define log_warn(argv, ...) _log(LOG_MODULE, LOG_WARNING, __FILE__, __FUNCTION__, __LINE__, argv, ##__VA_ARGS__)
#else
#define log_warn(argv, ...) do {} while (0)
#endif
#endif

#ifndef log_err
#ifdef _WIN32
#define log_err(argv, ...) _log(LOG_MODULE, LOG_ERROR, __FILE__, __FUNCTION__, __LINE__, argv, ##__VA_ARGS__)
#else
#define log_err(argv, ...) do {} while (0)
#endif
#endif

#ifndef debug
#ifdef _WIN32
#define debug(argv, ...) _log(LOG_MODULE, LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, argv, ##__VA_ARGS__)
#else
#define debug(argv, ...) do {} while (0)
#endif
#endif

#define ENTER debug("\n")
#define EXIT  debug("\n")

#define TRACE log_info("%s %d\n", __FUNCTION__, __LINE__)

#define LOG_CONFIG_LENGTH   (256)

//=============================================================================

extern void log_init(char *config);

#ifdef _WIN32
extern int _log(int module, int lvl, char *filename, char *function, int linenum, const char *fmt, ...);
#else
extern int _log(int module, int lvl, char *filename, char *function, int linenum, char *fmt, ...);
#endif

//=============================================================================

#endif
