#ifndef SERIALCOMMANDADAPTER_H
#define SERIALCOMMANDADAPTER_H

#include "datacommand.h"
#include "serial.h"
#include <memory>

class SerialCommandAdapter : public DataCommand<float>
{
public:
    SerialCommandAdapter(std::shared_ptr<Serial> serial, DataType type);
    virtual ~SerialCommandAdapter() = default;
    float runCommand(float data) override;
private:
    std::shared_ptr<Serial> serialPtr;
    DataType dataType;
};

#endif // SERIALCOMMANDADAPTER_H
