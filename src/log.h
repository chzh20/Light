/*
 * @Author: chen
 * @Date: 2021-09-28 20:46:35
 * @LastEditors: chen
 * @LastEditTime: 2021-09-28 22:44:28
 * @FilePath: /Light/src/log.h
 */

#ifndef __LIGHT__LOG_H
#define __LIGHT__LOG_H
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>

namespace light
{
    class Logger;
    class LogLevel
    {

    public:
        enum Level
        {
            UNKNOW = 0,
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5,
        };

        /**
         * @name: 
         * @msg: 
         * @param {Level} level
         * @return {*}
         */
        static const char *Tostring(LogLevel::Level level);

        static LogLevel::Level FromString(const std::string &str);
    };

    class LogEvent
    {

    public:
        typedef std::shared_ptr<LogEvent> ptr;
        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char *file, int32_t line,
                 uint32_t elapse, uint32_t threadid, uint32_t fiberid, uint32_t time, const std::string &thread_name);

        const char *getfile() const { return m_file; }
        int32_t getline() const { return m_line; }
        uint32_t getElapse() const { return m_elapse; }
        uint32_t getThreadId() const { return m_threadId; }
        uint32_t getFiberId() const { return m_fiberId; }
        uint32_t getTime() const { return m_time; }
        const std::string& getThreadName()const{return m_threadName;}
        std::string getContent()const{return m_ss.str();}
        std::shared_ptr<Logger> getLogger()const{return m_logger;}
        std::stringstream& getSS(){return m_ss;}


        /**
         * @name: ge shi hua xie ru
         * @msg: 格式化写入日志内容
         * @param {*}
         * @return {*}
         */
        void format(const char*fmt,...);


    private:
        //file name
        const char* m_file = nullptr;
        //line number
        int32_t m_line = 0;
        //time from program starts;
        uint32_t m_elapse = 0;
        //thread id
        uint32_t m_threadId = 0;
        //Fiber  id
        uint32_t m_fiberId = 0;
        //stamp time
        uint32_t m_time = 0;
        // thread name
        std::string m_threadName;
        //stream
        std::stringstream m_ss;
        //Logger
        std::shared_ptr<Logger> m_logger;
        //Logger level
        LogLevel::Level m_level;
    };

    class LogFormatter{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
     /**
     * @brief 构造函数
     * @param[in] pattern 格式模板
     * @details 
     *  %m 消息
     *  %p 日志级别
     *  %r 累计毫秒数
     *  %c 日志名称
     *  %t 线程id
     *  %n 换行
     *  %d 时间
     *  %f 文件名
     *  %l 行号
     *  %T 制表符
     *  %F 协程id
     *  %N 线程名称
     *
     *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
     */
     LogFormatter(const std::string& pattern);
     
     /**
      * @name: 
      * @msg: 返回格式化日志文本
      * @param {shared_ptr<Logger>} logger
      * @param {Level} level
      * @param {LogEvent::ptr} ptr
      * @return {*}
      */
     std::string format(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr ptr);
     std::ostream&  format(std::ostream &os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr ptr);
     

public:
    class FormatItem{
        public:
            typedef std::shared_ptr<FormatItem> ptr;
            virtual ~FormatItem(){;}

            virtual void format(std::ostream &os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr ptr)=0;
    };


    void init();

    bool isError()const{return m_error;}

private:
    std::string m_pattern;
    std::vector<FormatItem> m_items;
    bool m_error=false;

};


class LogAppender{

public:
    std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender(){}

    virtual void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr ptr)=0;
protected:
    LogLevel::Level m_level;
    bool  m_hasFormatter=false;
    
    LogFormatter::ptr m_formatter;

};



}



#endif