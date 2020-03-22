#include "serialcommandadapter.h"

SerialCommandAdapter::SerialCommandAdapter(std::shared_ptr<Serial> serial, DataType type):
    serialPtr(serial), dataType(type) {}

float SerialCommandAdapter::runCommand(float) {
    return serialPtr->getData(dataType);
}
