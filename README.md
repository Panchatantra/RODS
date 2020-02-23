# RODS
***

**R**esponse solver for **O**scillated **D**ynamic **S**ystem based on finite element method

## About the name

* Abbreviation for
"**R**esponse solver for **O**scillated **D**ynamic **S**ystem"
* Most of the elements are **rod**-like elements
* Salute to the famous ancient Chinese calculator — counting **rods**


## Element types
### 1D elements

* Inerter
* Spring
* Dashpot
* SPIS2
* TVMD
* SpringBilinear
* SpringBoucWen
* SpringNonlinear
* DashpotExp
* DashpotMaxwell
* Slider
* ...
  
### 2D elements

* Inerter2D
* Spring2D
* Dashpot2D
* SpringBilinear2D
* SpringBoucWen2D
* SpringNonlinear2D
* DashpotExp2D
* DashpotMaxwell2D
* TrussElastic2D
* FrameElastic2D
* Truss2D
* Frame2D
* FramePDelta2D
* Quad4Elastic
* ...

### 3D elements

* ...


## Nonlinear materials
### 1D materials

* Elastoplastic
* ConcreteTrilinear
* SteelBilinear
* SMABilinear
* ...

### Section

* Fiber
* SectionTruss (Fiber-based)
* SectionFrame2D (Fiber-based)
* ...


## Solvers

* Eigen solver
* Static solver
    * Linear solver
    * Nonlinear solver
        * Load Control
        * Displacement Control
* Dynamic solver
    * Linear solver
        * Newmark-β solver
        * State space solver 
    * Nonlinear solver
        * Newmark-β solver with Newton Iteration
        * State space solver 
* Stochastic solver


## Response Recorders

* DOF Recorder
* Element Recorder


## Visualization

* Gmsh

## API Documentation

[Link](RODS/html/index.html)