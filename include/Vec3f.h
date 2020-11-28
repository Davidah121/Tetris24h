
class Vec3f
{
public:
    Vec3f()
    {

    }
    Vec3f(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    ~Vec3f()
    {
        
    }

    double x = 0;
    double y = 0;
    double z = 0;
};