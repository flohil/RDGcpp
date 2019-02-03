#ifndef DEBUG_PRINT_INCLUDE
#define DEBUG_PRINT_INCLUDE


// prints all variables and their values of an object
class DebugPrintObject
{
public:

	virtual void debugPrint() const = 0;
};
#endif // DEBUG_PRINT_INCLUDE