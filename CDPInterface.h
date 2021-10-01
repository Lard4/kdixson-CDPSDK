#include <exception>
#include <string>

class CDPInterface {
   class NotImplementedException: public std::exception {
   public:
      /**
       * Constructor (for C strings).
       * @param message C-style string error message.
       *
       * The string contents are copied upon construction. Hence, responsibility for deleting the
       * char* lies with the caller.
       */
      explicit NotImplementedException(const char* message): msg_(message) {}

      /**
       * Constructor (C++ STL strings).
       * @param message The error message.
       */
      explicit NotImplementedException(const std::string& message): msg_(message) { }

   protected:
      std::string msg_;
   };

public:
   virtual void setupUART();
};