#ifndef ZSTACK_LOG_H
#define ZSTACK_LOG_H

//=============================================================================

enum log_level_e {
    LOG_ERROR,      // system can't run
    LOG_WARNING,    //
    LOG_USER,       // application info, user need to know
    LOG_FILE,       // developer, [filename] ...
    LOG_FUNC,       // developer, [filename:function:line] ...
};

//=============================================================================

#define log(lvl, argv, ...) _log(lvl, __FILE__, __FUNCTION__, __LINE__, argv, ##__VA_ARGS__)

#define ENTER log(LOG_FUNC, "\n")
#define EXIT  log(LOG_FUNC, "\n")

//=============================================================================

extern void log_init(int level);
extern int _log(int lvl, char *filename, char *function, int linenum, const char *fmt, ...);

//=============================================================================

#endif
