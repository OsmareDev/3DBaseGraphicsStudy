#include "Render.h"

namespace Render {

    Model* model = new Model("./obj/african_head.obj");

    void Renderize()
    {
        Vec2i a(100,10), b(70,80), c(10,10);
        Vec2i d(100, 10), e(30, 60), f(-10, -10);
        
        

        Triangle2Di t1(a, b, c, 0xFF0000);
        Line l1(a, b, 0xFFFFFF), l2(b, c, 0xFFFFFF), l3(c,a, 0xFFFFFF);
        


        Triangle2Di t2(d, e, f);
        t2.Draw(0x00FF00);

        Line l4(d, e), l5(e, f), l6(f, d);
        l4.Draw(0xFFFFFF);
        l5.Draw(0xFFFFFF);
        l6.Draw(0xFFFFFF);
    }
}