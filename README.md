
#  **Grass Real-Time Rendering with Wind**
Author: Kaiyang Yao, Shengyuan Wang


### **Introduction**:
In this project, we create a realistic and visually appealing simulation of grass using OpenGL, which have a variety of potential application both in video games and virtual reality experiences. It involve the use of billboarding method, grass simulation, and wind simulation to create a realistic and dynamic grass landscape.

### **Requirements**:
To run this project, you need the following:
- C++ compiler
- OpengGL library
- GLFW library
- GLM library

### **Installation**:
To install and run the project, you should following these steps:
```
cd <yourCodeDirectory>
git clone https://github.com/KaiyangYao/COMP465_GrassProject.git
cd COMP465_GrassProject
mkdir build
cd build
cmake-gui ..
```

### **Usage**

Once you have installed and run the project, you can use the following controls:
- WASD keys to move the camera
- Mouse to look around


## **Implementation Details**:

### **Grass Simulation**:

Billboard rendering utilizes a two-stage approach. The first stage involves constructing a simple geometric mesh comprising rectangular shapes using two triangles as building blocks. The second stage involves applying the grass texture to each rectangle in the mesh, providing the appearance of grass blades.

- **Creating Geometry Mesh**:  
When creating a geometric mesh for billboard rendering, the first step is to determine the position of each rectangle box within the 3D environment. We add a base point for each rectangle box, serving as a reference point to generate the boxes. After establishing the base points, we move to the next step to generate a rectangle box for each reference point. To create a rectangle box, we need to establish four additional points that define the dimensions of the box. These points are typically located at the corners of the rectangle box and are calculated based on the width and height of the box. Once we have calculated the position of each corner point, we can append them to the vertex array for rendering purposes.

- **Adding Grass Texture**:  
The next step involves applying the grass texture overlay onto the boxes to render the grass on the screen. However, simply overlaying the texture onto each box is not adequate to produce a realistic-looking scene. Therefore, we need to make certain modifications. One potential solution is rotating billboards to generate multiple copies of the grass texture with various orientations. By repeating this process, we can create several grass quads in the same space with different directions, contributing to a more natural-looking environment.

### **Wind Simulation**:

### **Future Work**:
This project has the potential for further development in the future to enhance the realism and interactivity of the grass rendering simulation. Here are some potential areas for investigation:

- **Advanced shadowing animation techniques**:   
Currently, the simulation only accounts for the movement of the sun and the resulting shadow patterns. However, shadows cast by nearby objects such as trees or buildings could be simulated, and the interaction between the grass and these shadows could be more accurately modeled. This would create a more immersive and dynamic environment for users to explore.

- **Research into other wind simulation methods**:   
Our current approach uses a simple sine wave function to generate wind patterns, but other techniques could be explored, such as fluid dynamics simulations. This would result in a more natural and fluid movement of the grass, enhancing the realism of the simulation.

- **Collision detection with other objects**:  
 The grass is currently simulated as if it exists in a vacuum, with no interaction with other objects in the environment. By implementing collision detection algorithms, the grass could interact with other objects such as rocks, trees, and buildings. This would create a more dynamic and interactive environment, opening up new possibilities in video games and other applications.


### **Acknowledgements**:

We would like to express our sincere gratitude to Bret Jackson for the support and guidance throughout the project. The help in debugging was particularly appreeciated and played a critical role in achieving our project goals. The valuable insights and feedback provided also helped us to identify areas for improvement, which we will continue to work in the future. Thank you once again for your unwavering support!


### **References**:
