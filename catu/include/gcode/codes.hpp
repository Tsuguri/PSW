#pragma once

#include <optional>
#include <vector>
#include <memory>

#include "../math/vec.hpp"

namespace GCode{

using coord = std::optional<float>;

class Command{
    public:
        virtual void Execute() const =0;
        virtual ~Command();
};

class G01Command : public Command{
    // data:
    coord _x;
    coord _y;
    coord _z;
    public:
        G01Command(coord x, coord y, coord z);
        virtual void Execute() const override;
        virtual ~G01Command() override;
};

/*class G00Command : public Command{
    // data:
    coord _x;
    coord _y;
    coord _z;
    public:
        G00Command(coord x, coord y, coord z);
        virtual void Execute() const override;
        virtual ~G00Command() override;
};*/

using Program = std::vector<std::unique_ptr<Command>>;

class Loader {
    public:
        static Program Load(const std::string& filePath);
};

} // namespace GCode
