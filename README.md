
#  **Grass Real-Time Rendering with Wind**

### **[Project Log](https://docs.google.com/document/d/1lhBNQFNMpAo0fFtIDtezeUoxU2dB6yr0HxkB_vA13ak/edit)**

### **Summary**:
In this project, we create a realistic and visually appealing simulation of grass using OpenGL, which have a variety of potential application both in video games and virtual reality experiences. It involve a conbination of geometry generation, simulation, shading and light techniques.

### **Contents**:
src/  
...

### **Running the code**:

```
cd yourCodeDirectory
git clone https://github.com/KaiyangYao/COMP465_GrassProject.git
cd COMP465_GrassProject
mkdir build
cd build
cmake-gui ..
```
## **Grass Rendering**:

#### **Method 1**:

The traditional approach of passing model data from the CPU to the GPU and the GPU rendering based on that data tends to ignore the model details of individual grasses when rendering large-scale grasses. If the modeling of individual grasses is too detailed, many polygons need to be passed to render large grasses, resulting in performance degradation.

Therefore, a scheme that renders a large area of grass often needs to satisfy the following conditions:

- There must not be too many polygons of individual grasses, preferably a single grass is represented by only one quad
- The grass must appear dense when viewed from different angles
- The grass must not be arranged too regularly, otherwise it will be unnatural


To sum up, the classic structure when rendering grasses - the star shape - appears. In this way, the simple star structure satisfies both the low number of faces of a single grass and the fact that it can look dense when viewed from different angles. It is also easy to make the grass move with the wind, just find out the top vertices according to their uv information and make the vertices move according to your own rules.

#### **Method 2**:

#### **Method 3**:



## **Level of Detail**:



### **References**:
