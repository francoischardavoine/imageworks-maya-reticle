=======================================
spReticle: Build Notes and Instructions
=======================================

Originally developed at Sony Pictures Imageworks, imageworks-maya-reticle
(spReticle) is a Maya C++ plugin plus MEL code which creates a reticle for a
camera. It allows for various camera reference masks to be displayed when
looking through the camera, such as filmback, projection gate, and
pan-and-scan attributes.  The value of predefined parameters can be displayed
in selectable areas, such as the camera focal length and name, current aspect
ratio; frame number, name of the show and shot, Maya scene file name, current
user name, etc. Finally, arbitrary textual information can be displayed as well.
You can see a short video on how to create a spReticle in Maya and set some of
the parameters here: http://vimeo.com/6186489
spReticle is a camera reticle for Maya.

Project Home:
-------------
    https://github.com/imageworks/spReticle 


Files:
------
    README.txt
    CHANGES.txt
    Makefile
    spReticleLoc     - Main classes
    GPURenderer      - Abstract class for handling GPU Rendering
    OpenGLRenderer   - Handles OGL renderering for VP1.0 and possibly VP2.0 (default)
    V2Renderer       - Handles VP2.0 rendering
    util.h           - Utility classes
    defines.h        - Defines to drive compilation/options
    font.h           - Font Texture Atlas used for OGL font rendering
    AEspReticleLocTemplate.mel - Attribute Editor Template for spReticle
    spReticleLocLinux.mel - MEL code to create a spReticle node on Linux

    Additional files for Windows:
        spReticleLoc.vcxproj 
        spReticleLoc.vcxproj.filters
        spReticleLoc.vcxproj.user
        spReticleLoc.sln
        spReticleLocWindows.mel - MEL code to create a spReticle node on Windows

    Additional SPI internal files which can be ignored:
        Makefile.spi
        CMakeLists.txt
        .gitlab-ci.yml
        .spdev.yaml
        spreticleloc.spk.yaml
        spReticleLoc.mel


Configure Options:
------------------

The defines.h file concentrates all of the defines that can be used to
tailor the spReticle to your environment.


Compile Information:
--------------------

This is how to compile for 64-bit Maya on Linux using g++.  For other
architectures and compilers modify Makefile accordingly. 

Define the actual path to your compiler in CPP and the path of your Maya install
directory in MAYA_LOCATION on the command line. For example:

make opt CPP=/opt/rh/devtoolset-6/root/usr/bin/g++ MAYA_LOCATION=/usr/autodesk/maya2020


Usage information:
------------------

In the Maya script editor, do this to create a maya-reticle.  Note that 
you might have to put in the abolute path to the plugin and MEL files:

// Load plugin
loadPlugin "spReticleLoc.so";

// Load MEL code
source "spReticleLoc.mel"; 

// Load attribute editor template; note that this should not be necessary 
// if you installed it in the proper place so Maya finds it and loads it 
// automatically when you create a spReticleLoc node.
source "AEspReticleLocTemplate.mel"; 

// Create a spReticleLoc node
spReticleLocCreate;


Texture Font Information:
-------------------------
The freetype-gl library was used to generate most of font.h which contains
a texture font atlas.  The original copyright information is contained within
the header of the file.

