// Ray class
#include "Ray.h"

// stream output
std::ostream& operator << (std::ostream& outStream, const Ray& value)
{ // stream output
    outStream << "Ray Origin: " << value.getOrigin() << " Ray Direction: " << value.getDirection();
    return outStream;
}