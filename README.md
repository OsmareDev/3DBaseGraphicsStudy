# VoidEngine
![a_final](https://github.com/OsmareDev/VoidEngine/assets/50903643/0e257aec-5aa4-47f3-8e1e-46b78508619d)

## Camera.cpp
Camera implementation for computer graphics

1. SetMatrix() Function:
        This function sets up the projection matrix for the camera.
        It initializes the Projection matrix as the identity matrix.
        It modifies specific elements of the Projection matrix based on the window dimensions (window_width and window_height), field of view (fov), and the near and far clipping planes (z_near and z_far).
        The calculations are performed to set up the perspective projection matrix.

2. Homo() Function:
        This function performs homogenization on a 4x1 matrix (Matrix m).
        The homogenization process involves dividing each element of the matrix by its fourth element.
        The result is a 3x1 matrix.

3. to_viewport() Function:
        This function converts a 3D point (Vec3f v) to viewport coordinates (Vec2i).
        The viewport coordinates are calculated based on the window dimensions (window_width and window_height).
        The 3D point is transformed into 2D viewport coordinates.

4. TransformTriangle() Function:
        This function transforms a 3D triangle (Triangle& t) using the projection matrix.
        It calls the Homo(), v2m(), m2v(), and to_viewport() functions to transform the vertices of the triangle.
        The resulting transformed vertices are updated in the original triangle.

## Geometry.cpp
Defines a set of classes and functions related to 3D geometry and matrix operations.

1. Matrix Class:

    Represents a 4x4 matrix.
    Constructor initializes the matrix with given dimensions and fills it with zeros.
    identity method creates an identity matrix of a specified dimension.
    RotX, RotY, and RotZ methods create rotation matrices around the X, Y, and Z axes, respectively.
    Overloads [] operator for matrix element access.
    Overloads * operator for matrix multiplication.
    transpose method returns the transpose of the matrix.
    inverse method calculates and returns the inverse of the matrix.

2. Triangle Class:

    Represents a 3D triangle.
    RotateX, RotateY, and RotateZ methods rotate the triangle around the X, Y, and Z axes, respectively.

3. Helper Functions:

    m2v converts a 4x1 matrix to a 3D vector (Vec3f).
    v2m converts a 3D vector (Vec3f) to a 4x1 matrix.

4. Vec3f Struct:

    Represents a 3D vector with x, y, and z components.

## Window.cpp
Defines a Window class for managing a simple graphical window in a Windows environment. Uses the Windows API to create a window, handle messages, and perform basic rendering.

1. Static Instance:
        Window::main_window: A static instance of the Window class.

2. Window Procedure:
        WindowProc: A callback function that processes messages for the window. It handles WM_CLOSE and WM_DESTROY messages to set a flag (window_running) to stop the window loop. Additionally, it handles WM_SIZE to recalculate the window size and update memory accordingly.

3. Memory Management:
        RecalculateMemory: Recalculates the memory buffer size based on the window dimensions. Allocates or reallocates memory using VirtualAlloc and updates the BITMAPINFO structure.

4. Window Loop:
        StartLoop: Initiates the main window loop by calling RunningLoop.

5. Constructor:
        Window::Window: Initializes the window instance, defines the window class, and creates the window.

6. Window Class Definition:
        DefineClass: Registers the window class using RegisterClass.

7. Window Creation:
        WindowCreate: Creates the window using CreateWindowEx and sets up the device context (hdc).

8. Main Loop:
        RunningLoop: Implements the main message loop and drawing loop. It continuously processes messages using PeekMessage and renders using Render.

9. Rendering:
        Render: The rendering logic, including using StretchDIBits to display the memory buffer on the window.

10. Pixel Drawing and Clearing:
        PaintPixel: Paints a pixel in the memory buffer at a specified position.
        CleanWindow: Clears the entire window by setting all pixels to black (0x000000).

## Render.cpp
Simple rendering engine for drawing 3D models in a graphical window.

1. Model Initialization:
        A global Model object is created using a constructor that takes the path to an OBJ file ("./obj/african_head.obj").

2. Rendering Function:
        Renderize: Clears the window, rotates each triangle of the model around the Y-axis, adjusts their Z-coordinate, transforms the triangles, and then draws them using the Draw function. The color used is white (0xFFFFFF).

3. Line Drawing Functions:
        bresenham: Implements the Bresenham line-drawing algorithm to draw a line between two points.
        dda: Implements the Digital Differential Analyzer (DDA) line-drawing algorithm to draw a line between two points.
        sunderCoen: Checks if two points are inside the clipping rectangle and performs line clipping if necessary.
        compute: Computes the region code for a point relative to the clipping rectangle.

4. Triangle Drawing Functions:
        Draw: Draws a filled or wireframe triangle on the window. For filled triangles, it computes a bounding box and fills in pixels inside the triangle.
        Clamp_bBox: Clamps the bounding box coordinates to ensure they are within the window boundaries.

## Model.cpp
This code defines the implementation of a Model class constructor and member functions for loading 3D models in Wavefront OBJ format.

