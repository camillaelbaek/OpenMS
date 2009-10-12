// -*- mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework 
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2009 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Chris Bielow, Stephan Aiche, Andreas Bertsch $
// $Authors: $
// --------------------------------------------------------------------------

#ifndef OPENMS_CONCEPT_LOGSTREAM_H
#define OPENMS_CONCEPT_LOGSTREAM_H

#include <OpenMS/DATASTRUCTURES/String.h>


#include <sstream>
#include <iostream>
#include <list>
#include <vector>
#include <ctime>
#include <map>

namespace OpenMS 
{

	/**	@name Log streams
			Logging, filtering, and storing messages.
			Many programs emit warning messages, error messages, or simply
			informations and remarks to their users. The  LogStream  
			class provides a convenient and straight-forward interface 
			to classify these messages according to their importance 
			(via the loglevel), filter and store them in files or
			write them to streams. \par
			As the LogStream class is derived from ostream, it behaves 
			as any ostream object. Additionally you may associate
			streams with each LogStream object that catch only 
			messages of certain loglevels. So the user might decide to
			redirect all error messages to cerr, all warning messages
			to cout and all informations to a file. \par
			Along with each message its time of creation and its loglevel
			is stored. So the user might also decide to store all 
			errors he got in the last two hours or alike. \par
			The  \link LogStream LogStream \endlink  class heavily relies on the  \link LogStreamBuf LogStreamBuf \endlink 
			class, which does the actual buffering and storing, but is only
			of interest if you want to implement a derived class, as the 
			actual user interface is implemented in the LogStream class.
	* 	\ingroup Common
	*/
	//@{

	// forward declarations
	class LogStream;
	class LogStreamNotifier;

	/** Stream buffer used by LogStream.
			This class implements the low level behaviour of
			 \link LogStream LogStream \endlink . It takes care of the buffers and stores
			the lines written into the  \link LogStream LogStream \endlink  object.
			It also contains a list of streams that are associated with
			the LogStream object. This list contains pointers to the
			streams and their minimum and maximum log level.
			Each line entered in the  \link LogStream LogStream \endlink  is marked with its
			time (in fact, the time  \link LogStreamBuf::sync sync \endlink  was called) and its
			loglevel. The loglevel is determined by either the current
			loglevel (as set by  \link LogStream::setLevel LogStream::setLevel \endlink  or a temporary
			level (as set by  \link LogStream::level LogStream::level \endlink  for a single line only).
			For each line stored, the list of associated streams is checked
			whether the loglevel falls into the range declared by the 
			stream's minimum and maximum level. If this condition is met,
			the logline (with its prefix, see  \link LogStream::setPrefix LogStream::setPrefix \endlink )
			is also copied to the associated stream and this stream is 
			flushed, too.
	*/
	class OPENMS_DLLAPI LogStreamBuf
		: public std::streambuf
	{

		friend class LogStream;

		public:

		/**	@name	Constants
		*/
		//@{
		static const int MAX_LEVEL;
		static const int MIN_LEVEL;
		static const time_t MAX_TIME;
		//@}

		/**	@name Constructors and Destructors
		*/
		//@{
		
		/** Default constructor.
				Create a new LogStreamBuf object.
		*/
		LogStreamBuf();

		/** Destructor.
				Destruct the buffer and free all stored messages strings.
		*/
		virtual ~LogStreamBuf();
		
		//@}
		
		/**	@name	Stream methods 
		*/
		//@{

		/**	Sync method.
				This method is called as soon as the ostream is flushed
				(especially this method is called by flush or endl).
				It transfers the contents of the streambufs putbuffer 
				into a logline if a newline or linefeed character 
				is found in the buffer ("\n" or "\r" resp.).
				The line is then removed from the putbuffer.
				Incomplete lines (not terminated by "\n" / "\r" are
				stored in incomplete_line_.
		*/
		virtual int sync();

		/**	Overflow method.
				This method calls sync and <tt>streambuf::overflow(c)</tt> to 
				prevent a buffer overflow.
		*/
		virtual int overflow(int c = -1);
		//@}

    /**
     * @brief Holds a stream that is connected to the LogStream incl. the minimum and maximum
     * level at which the LogStream redirects messages to this stream.
     */
		OPENMS_DLLAPI struct StreamStruct
		{
			std::ostream*				stream;
      std::string         prefix;
			int									min_level;
			int									max_level;
			LogStreamNotifier*	target;
		
			StreamStruct()
				:	stream(0),
					min_level(MIN_LEVEL),
					max_level(MAX_LEVEL),
					target(0)
			{
			}
			
			// Delete the notification target.
			~StreamStruct()
			{
			}
		};


		protected:

		// interpret the prefix format string and return the expanded prefix
		std::string expandPrefix_(const std::string& prefix, int level, time_t time) const;

		char* 									pbuf_;
	
		int											level_;

		int											tmp_level_;
		
		std::list<StreamStruct>	stream_list_;

		std::string             incomplete_line_;

 		/**	@name Caching
		*/
		//@{

    /**
     * @brief Holds a counter of occurences and an index for the occurence sequence 
     * of the corresponding log message
     */
    struct LogCacheStruct 
    {
      Size timestamp;
      int counter;
    };

    /**
     * Sequential counter to remember the sequence of occurence 
     * of the cached log messages
     */
    Size log_cache_counter_;

    /// Cache of the last two log messages
    std::map<std::string, LogCacheStruct> log_cache_;
    /// Cache of the occurence sequence of the last two log messages
    std::map<Size, std::string > log_time_cache_;

    /// Checks if the line is already in the cache
    bool isInCache_(std::string const & line);

    /**
       Adds the new line to the cache and removes an old one
       if necessary

       @param line The Log message that should be added to the cache
       @return An additional massage if a reoccuring message was removed
       from the cache
     */
    std::string addToCache_(std::string const & line);

    /**
     * Returns the next free index for a log message
     */
    Size getNextLogCounter_();

    /**
     * Checks if some of the cached entries where sent more then once
     * to the LogStream and (if necessary) prints a corresponding messages
     * into all affected Logs
     */
    void clearCache_();
		//@}

	};


	///
	class OPENMS_DLLAPI LogStreamNotifier
	{
		public:
		
		///
		LogStreamNotifier();
			
		///
		virtual ~LogStreamNotifier();

		///
		virtual void logNotify();

		///
		void registerAt(LogStream& log_stream,
										int min_level = LogStreamBuf::MIN_LEVEL, 
										int max_level = LogStreamBuf::MAX_LEVEL);
		///
		void unregister();

		protected:

		std::stringstream stream_;

		LogStream* registered_at_;
	};



	/**	Log Stream Class.
			 \par
			
			 \par
	*/
	class OPENMS_DLLAPI LogStream
		: public std::ostream
	{
		public:


		/**	@name Enums
		*/
		//@{
			
		/** Log levels.
				Constants for the different predefined log levels.
				Use  LogStream::ERROR to indicate a severe error,  LogStream::WARNING to 
				indicate a problem that could be fixed or is of minor importance, 
				and  LogStream::INFORMATION for messages that do not indicate any problem 
				(e.g. progress messages).
		*/
		enum LogStreamLevel
		{
			/** Loglevels >= ERROR should be used to indicate errors
			*/

			ERROR_LEVEL = 2000 ,
			
			/** Loglevels >= WARNING should be used to indicate warnings
			*/
			WARNING_LEVEL = 1000,
			/** Loglevels >= INFORMATION indicate information messages
			*/
			INFORMATION_LEVEL = 0
		};

		//@}

		/* TODO
			 replace log level by this: 
			 - The higher the level, the more critical are the errors
       - Error level 0 throws out everything
					
    enum LogStreamLevel
    {
      /// Loglevels >= ERROR should be used to indicate errors
      ERROR_LEVEL = 3000 ,

      /// Loglevels >= WARNING should be used to indicate warnings
      WARNING_LEVEL = 2000,

      /// Loglevels >= INFORMATION indicate information messages
      INFORMATION_LEVEL = 1000,

      /// LogLevels >= DEBUG indicate debug messages
      DEBUG_LEVEL = 0
    };
		*/

	
		/**	@name	Constructors and Destructors
		*/
		//@{

		/** Constructor.
				Creates a new LogStream object that is not associated with any stream.
				If the argument <tt>associate_stdio</tt> is set to <b>true</b>,
				<tt>cout</tt> is associated with all messages of levels  LogStream::INFORMATION   
				and  LogStream::WARNING , and <tt>cerr</tt> is associated with all messages
				of level  LogStream::ERROR .
				@param	buf
				@param  delete_buf
				@param	associate_stdio bool, default is false
		*/
		LogStream(LogStreamBuf* buf = 0, bool delete_buf = true, bool associate_stdio = false);

		/** Destructor.
				Clears all message buffers.
		*/
		virtual ~LogStream();
	
		//@}		

		/**	@name	Stream Methods
		*/
		//@{

		/**	<tt>rdbuf</tt> method of ostream.
				This method is needed to access the LogStreamBuf object.
		*/
		LogStreamBuf* rdbuf();

		/** Arrow operator.
		*/
		LogStreamBuf* operator -> ();
		//@}

		/**	@name Loglevel management 
		*/
		//@{

		/**	Assign a new log level.
				This method assigns a new loglevel which will be used
				for all messages sent to the LogStream after that call
				(except for messages which use the temporary loglevel
				set by LogStream::level ).
		*/
		void setLevel(int level);

		/**	Return the current log level.
				The LogStreamBuf object has an internal current log level (<tt>level_</tt>).
				It is set to 0 by the LogStreamBuf default constructor.
				This method returns <tt>rdbuf()->level_</tt> if rdbuf() does not
				return a null pointer, 0 otherwise.
				@return		int the current log level
		*/
		int getLevel();

		/**	Set a temporary log level.
				Using <b>level</b>, a temporary loglevel may be defined.
				It is valid unly until the next <b>flush</b> or <b>endl</b> is issued. \par
				Use this command to log a single line with a certain log level. \par
				<b>Example:</b>
					<tt>log << "log message 1" << endl;</tt> \par
					<tt>log.level(4) << "log message 2" << endl;</tt> \par
					<tt>log << "log message 3" << endl;</tt> \par
				In this example, only the second message will be logged using level 4.
				
				@return	LogStream the log stream
				@param	level the temporary log level
		*/
		LogStream& level(int level);

		/**	Log an information message.
				This method is equivalent to LogStream::level (LogStream::INFORMATION + n). 
				@param	n the channel 
		*/
		LogStream& info(int n = 0);

		/**	Log an error message.
				This method is equivalent to  LogStream::level (LogStream::ERROR + n). 
				@param	n the channel 
		*/
		LogStream& error(int n = 0);

		/**	Log an information message.
				This method is equivalent to LogStream::level (LogStream::WARNING + n). 
				@param	n the channel 
		*/
		LogStream& warn(int n = 0);

		// TODO
		//LogStream& debug(int n = 0);

		//@}

		/**	@name Associating Streams 
		*/
		//@{

		/**	Associate a new stream with this logstream.
				This method inserts a new stream into the list of 
				associated streams and sets the corresponding minimum
				and maximum log levels.
				Any message that is subsequently logged, will be copied
				to this stream if its log level is between <tt>min_level</tt>
				and <tt>max_level</tt>. If <tt>min_level</tt> and <tt>max_level</tt>
				are omitted, all messages are copied to this stream.
				If <tt>min_level</tt>	and <tt>max_level</tt> are equal, this function can be used
				to listen to a specified channel.
				@param	s a reference to the stream to be associated
				@param	min_level the minimum level of messages copied to this stream
				@param	max_level the maximum level of messages copied to this stream
		*/
		void insert
			(std::ostream& s, int min_level = LogStreamBuf::MIN_LEVEL, 
			 int max_level = LogStreamBuf::MAX_LEVEL);

		/**	Remove an association with a stream.
				Remove a stream from the stream list and avoid the copying of new messages to
				this stream. \par
				If the stream was not in the list of associated streams nothing will
				happen.
				@param	s the stream to be removed
		*/
		void remove(std::ostream& s);

		/**	Add a notification target
		*/
		void insertNotification(std::ostream& s, 
														LogStreamNotifier& target,
														int min_level = LogStreamBuf::MIN_LEVEL, 
														int max_level = LogStreamBuf::MAX_LEVEL);

		/**	Set the minimum log level of an associated stream.
				This method changes the minimum log level of an already
				associated stream. However, if the stream is not
				associated, nothing will happen.
				@param	s the associated stream
				@param	min_level the new minimum level
		*/
		void setMinLevel(const std::ostream& s, int min_level);
		
		/**	Set the maximum log level of an associated stream.
				This method changes the maximum log level of an already
				associated stream. However, if the stream is not
				associated, nothing will happen.
				@param	s the associated stream
				@param	max_level the new minimum level
		*/
		void setMaxLevel(const std::ostream& s, int max_level);

		/**	Set prefix for output to this stream.
				Each line written to the stream will be prefixed by
				this string. The string may also contain trivial 
				format specifiers to include loglevel and time/date 
				of the logged message. \par
				The following format tags are recognized:

					- <b>%l</b>	loglevel
					- <b>%y</b>	message type ("Error", "Warning", "Information", "-")
					- <b>%T</b>  time (HH:MM:SS)
					- <b>%t</b>  time in short format (HH:MM)
					- <b>%D</b>	date (DD.MM.YYYY)
					- <b>%d</b>  date in short format (DD.MM.)
					- <b>%S</b>  time and date (DD.MM.YYYY, HH:MM:SS)
					- <b>%s</b>  time and date in short format (DD.MM., HH:MM)
					- <b>%%</b>	percent sign (escape sequence)
				
		*/
		void setPrefix(const std::ostream& s, const std::string& prefix);

		/// Disable all output
		void disableOutput();

		/// Enable all output
		void enableOutput();

		/// Is Output enabled?
		bool outputEnabled() const;

		///
		void flush();
		//@}		

		private:

		typedef std::list<LogStreamBuf::StreamStruct>::iterator StreamIterator;
		
		StreamIterator findStream_(const std::ostream& stream);
		bool hasStream_(std::ostream& stream);
		bool bound_() const;

		// flag needed by the destructor to decide whether the streambuf
		// has to be deleted. If the default ctor is used to create
		// the LogStreamBuf, delete_buffer_ is set to true and the ctor
		// also deletes the buffer.
		bool	delete_buffer_;
		bool  disable_output_;
	};


	/** Global static instance of a logstream.
			This instance of LogStream is by default bound to <b>cout</b> <b>cerr</b> by calling
			the default constructor.
	*/
	OPENMS_DLLAPI extern LogStream	Log;

	/* TODO
	Macros to define the log-level

	#define LOG_ERROR \
  Log.error()

  #define LOG_ERROR_N(error_level) \
  Log.error(error_level)

  #define LOG_INFO \
  Log.info()

  #define LOG_INFO_N(info_level) \
  Log.info(info_level) \

  #define LOG_WARN \
  Log.warn()

  #define LOG_WARN_N(warn_level) \
  Log.warn(warn_level)

  #define LOG_DEBUG \
  Log.debug()

  #define LOG_DEBUG_N(debug_level) \
  Log.debug(debug_level)
	*/

	//@}
	
} // namespace OpenMS

#endif // OPENMS_CONCEPT_LOGSTREAM_H
