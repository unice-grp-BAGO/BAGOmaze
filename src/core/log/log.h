#ifndef LOG_H
#define LOG_H

/* ########################################################################## */
/* ########################################################################## */


#define TRACE_DBG_BASE(argModule, argFormat,...)                \
    log_debug(  "%s \t :: %s +%d (%s) \t=> "argFormat"\n",      \
                argModule, __FILE__, __LINE__, __FUNCTION__,    \
                ##__VA_ARGS__ );

#define TRACE_ERR_BASE(argModule, argFormat,...)                \
    log_error(  "%s \t :: %s +%d (%s) \t=> "argFormat"\n",      \
                argModule, __FILE__, __LINE__, __FUNCTION__,    \
                ##__VA_ARGS__ );

/* ########################################################################## */
/* ########################################################################## */

int     log_begin(void);
void    log_end(void);

void    log_debug( const char* argFormat, ... );
void    log_error( const char* argFormat, ... );

/* ########################################################################## */
/* ########################################################################## */

#endif  /*< LOG_H */
