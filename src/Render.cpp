#include "Render.h"

namespace Render {

    Model* model = new Model("./obj/african_head.obj");;
    float ang = 0;

    void Renderize()
    {
        Window::GetInstance().CleanWindow();
        ang += 0.5f;

        for (int i = 0; i < model->nfaces(); i++) {
            std::vector<int> face = model->face(i);
            Triangle t(model->vert(face[0]), model->vert(face[1]), model->vert(face[2]));
            

            t.RotateY(toRad(ang));
            
            t.v1.z += 2;
            t.v2.z += 2;
            t.v3.z += 2;

            Window::GetInstance().TransformTri(t);
            Draw(t, 0xFFFFFF, true);
        }
        
    }



    /// Linea

    void bresenham(const Line& l, const uint32_t& color) {
        int x_ini = l.a.x, x_end = l.b.x;
        int y_ini = l.a.y, y_end = l.b.y;

        if (sunderCoen(x_ini, y_ini, x_end, y_end))
        {
            // variable que sirve para saber si es mas vertical la cuesta
            bool _steep = false;

            // comprobamos si es mas vertical que horizontal
            // en caso de serlo se debe iterar a traves de las y por lo que se cambian
            if (std::abs(x_ini - x_end) < std::abs(y_ini - y_end)) {
                std::swap(x_ini, y_ini);
                std::swap(x_end, y_end);
                _steep = true;
            }
            // comprobamos que se dibuja de izquierda a derecha para evitar problemas
            if (x_ini > x_end) {
                std::swap(x_ini, x_end);
                std::swap(y_ini, y_end);
            }

            int _dx = x_end - x_ini;
            int _dy = y_end - y_ini;

            int _pk = std::abs(_dy) * 2;
            int _pnext = 0;

            int _y = y_ini;
            for (int x = x_ini; x <= x_end; x++) {
                if (_steep) {
                    Window::GetInstance().PaintPixel(_y, x, color);
                }
                else {
                    Window::GetInstance().PaintPixel(x, _y, color);
                }

                _pnext += _pk;
                if (_pnext > _dx) {
                    _y += (y_end > y_ini ? 1 : -1);
                    _pnext -= _dx * 2;
                }
            }
        }
    }

    void dda(const Line& l, const uint32_t& color)
    {
        int x_ini = l.a.x, x_end = l.b.x;
        int y_ini = l.a.y, y_end = l.b.y;

        if (sunderCoen(x_ini, y_ini, x_end, y_end))
        {
            // variable que sirve para saber si es mas vertical la cuesta
            bool _steep = false;

            // comprobamos si es mas vertical que horizontal
            // en caso de serlo se debe iterar a traves de las y por lo que se cambian
            if (std::abs(x_ini - x_end) < std::abs(y_ini - y_end)) {
                std::swap(x_ini, y_ini);
                std::swap(x_end, y_end);
                _steep = true;
            }
            // comprobamos que se dibuja de izquierda a derecha para evitar problemas
            if (x_ini > x_end) {
                std::swap(x_ini, x_end);
                std::swap(y_ini, y_end);
            }

            if (x_ini - x_end == 0)
                return;

            float _m = (float)(y_end - y_ini) / (float)(x_end - x_ini);

            float _y = y_ini;

            for (int x = x_ini; x <= x_end; x++) {
                _y += _m;

                if (_steep) {
                    Window::GetInstance().PaintPixel((int)_y, x, color);
                }
                else {
                    Window::GetInstance().PaintPixel(x, (int)_y, color);
                }
            }
        }
    }

    bool sunderCoen(int& x0, int& y0, int& x1, int& y1)
    {
        // Computamos la region para ambos puntos de la recta
        int code1 = compute(x0, y0);
        int code2 = compute(x1, y1);

        // Initialize line as outside the rectangular window
        bool accept = false;

        while (true) {
            // si ambos puntos son 0 estan dentro del rectangulo
            if ((code1 == 0) && (code2 == 0)) {
                accept = true;
                break;
            }
            // si ambos puntos estan fuera pero en la misma region damos por hecho que estan fuera
            else if (code1 & code2) {
                break;
            }
            else {
                int code_out;
                double x, y;

                // Cogemos uno de los puntos que este fuera todavia
                code_out = (code1 != 0) ? code1 : code2;

                // Find intersection point;
                // using formulas y = y1 + slope * (x - x1),
                // x = x1 + (1 / slope) * (y - y1)
                // 8 = 1000
                if (code_out & 8) {
                    // point is above the clip rectangle
                    x = x0 + (x1 - x0) * (Window::GetInstance().GetHeight() - 1 - y0) / (y1 - y0);
                    y = Window::GetInstance().GetHeight() - 1;
                }
                // 4 = 0100
                else if (code_out & 4) {
                    // point is below the rectangle
                    x = x0 + (x1 - x0) * (0 - y0) / (y1 - y0);
                    y = 0;
                }
                // 2 = 0010
                else if (code_out & 2) {
                    // point is to the right of rectangle
                    y = y0 + (y1 - y0) * (Window::GetInstance().GetWidth() - 1 - x0) / (x1 - x0);
                    x = Window::GetInstance().GetWidth() - 1;
                }
                // 1 = 0001
                else if (code_out & 1) {
                    // point is to the left of rectangle
                    y = y0 + (y1 - y0) * (0 - x0) / (x1 - x0);
                    x = 0;
                }

                // Now intersection point x, y is found
                // We replace point outside rectangle
                // by intersection point
                if (code_out == code1) {
                    x0 = x;
                    y0 = y;
                    code1 = compute(x0, y0);
                }
                else {
                    x1 = x;
                    y1 = y;
                    code2 = compute(x1, y1);
                }
            }
        }

        return accept;
    }

    int compute(const int& x, const int& y)
    {
        // 0000 es que esta dentro
        int code = 0000;

        // 1000 es que esta por encima
        if (y > Window::GetInstance().GetHeight() - 1)
            code |= 8;

        // abajo
        if (y < 0)
            code |= 4;

        // derecha
        if (x > Window::GetInstance().GetWidth() - 1)
            code |= 2;

        //izquierda
        if (x < 0)
            code |= 1;

        return code;
    }

    
    
    /// TRIANGULO

    void Draw(Triangle& t, const uint32_t& color, bool Wireframe) {
        if (!Wireframe)
        {
            BoundingBox box = t.GetBoundingBox();
            Clamp_bBox(box);

            for (int y = box.p_min.y; y < box.p_max.y; y++)
            {
                for (int x = box.p_min.x; x < box.p_max.x; x++)
                {
                    // esto se puede paralelizar, supongo que habria que quitar el break
                    if (t.isInside({ x,y }))
                    {
                        Window::GetInstance().PaintPixel(x, y, color);
                    }
                }
            }
        }
        else {
            Draw({ { t.a.x,t.a.y },{ t.b.x,t.b.y } }, color);
            Draw({ { t.b.x,t.b.y },{ t.c.x,t.c.y } }, color);
            Draw({ { t.c.x,t.c.y },{ t.a.x,t.a.y } }, color);
        }
    }

    void Clamp_bBox(BoundingBox& b) {
        b.p_min.x = max(b.p_min.x, 0);
        b.p_min.y = max(b.p_min.y, 0);
        b.p_max.x = min(b.p_max.x, Window::GetInstance().GetWidth());
        b.p_max.y = max(b.p_max.y, Window::GetInstance().GetHeight());
    }
}
