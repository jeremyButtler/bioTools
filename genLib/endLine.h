/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' endLine SOF: Start Of File
'   - has line ending string for different OS's
'   - this allows for a bit more portability
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef WINDOWS
   #ifdef OLD_MAC_BREAK
      static signed char *str_endLine =
         (signed char *) "\r";
   #else
     #ifdef UNIX_BREAK
        static signed char *str_endLine =
           (signed char *) "\n";
     #else
        static signed char *str_endLine =
           (signed char *) "\r\n";
      #endif
   #endif
#else
#ifdef OLD_MAC
   #ifdef WIN_BREAK
      static signed char *str_endLine =
         (signed char *) "\r\n";
   #else
      #ifdef UNIX_BREAK
         static signed char *str_endLine =
            (signed char *) "\n";
      #else
         static signed char *str_endLine =
            (signed char *) "\r";
      #endif
   #endif
#else
#ifdef UNIX
   #ifdef WIN_BREAK
      static signed char *str_endLine =
         (signed char *) "\r\n";
   #else
      #ifdef OLD_MAC_BREAK
         static signed char *str_endLine =
            (signed char *) "\r";
      #else
         static signed char *str_endLine =
            (signed char *) "\n";
      #endif
   #endif
#else
   #ifdef WIN_BREAK
      static signed char *str_endLine =
         (signed char *) "\r\n";
   #else
   #ifdef OLD_MAC_BREAK
      static signed char *str_endLine =
          (signed char *) "\r";
   #else
   #ifdef UNIX_BREAK
      static signed char *str_endLine =
         (signed char *) "\n";
   #else
      static signed char *str_endLine =
         (signed char *) "\n";
   #endif /*linux/default break*/
   #endif /*mac break*/
   #endif /*windows break*/
#endif /*linux*/
#endif /*mac*/
#endif /*windows*/
