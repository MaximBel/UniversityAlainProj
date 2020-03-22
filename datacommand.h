#ifndef DATACOMMAND_H
#define DATACOMMAND_H

template<typename T>
class DataCommand
{
public:
    DataCommand() = default;
    virtual ~DataCommand() = default;

    virtual T runCommand(T data) = 0;
};

#endif // DATACOMMAND_H
