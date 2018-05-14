#ifndef LOG_H
#define LOG_H


#define TRACE_DBG_BASE(argModule, argFormat,...)    \
    fprintf(    stdout,                             \
                "DBG: %s \t :: %s +%d (%s) \t=> "argFormat"\n", \
                argModule, __FILE__, __LINE__, __FUNCTION__,    \
                ##__VA_ARGS__ );

#define TRACE_ERR_BASE(argModule, argFormat,...)    \
    fprintf(    stderr,                             \
                "ERR: %s \t :: %s +%d (%s) \t=> "argFormat"\n", \
                argModule, __FILE__, __LINE__, __FUNCTION__,    \
                ##__VA_ARGS__ );

#endif  /*< LOG_H */

