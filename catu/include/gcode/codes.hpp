#pragma once

#include <optional>
#include <vector>
#include <memory>

#include "../math/vec.hpp"

namespace GCode{

using coord = std::optional<float>;

struct Movement {
    coord x;
    coord y;
    coord z;
    Movement(coord x, coord y, coord z) : x(x), y(y), z(z){}
};


class Command{
    public:
        virtual void Execute() const =0;
        virtual float GetLength() const=0;
        virtual Movement GetMovement() const=0;
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
        virtual float GetLength() const override;
        virtual Movement GetMovement() const override;
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
