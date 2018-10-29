# AMD DGtest

The AMD ParseDigit is a useful tool for preparing image dataset for [MNIST](http://yann.lecun.com/exdb/mnist/). 
The program will automatically detect digits from the input image, crop, and preprocess the images.

### Explanation
The program uses opencv [MSER function](https://docs.opencv.org/3.1.0/d3/d28/classcv_1_1MSER.html) for detecting digits.
After detecting and cropping the images, it preprocesses the images so that it can be used for MNIST dataset.
The detailed explanation of preprocessing the images can be found at http://opensourc.es/blog/tensorflow-mnist


### Pre-requisites
1. ubuntu 16.04
2. [rocm supported hardware](https://github.io/hardware.html)
3. [rocm](https://github.com/RadeonOpenCompute/ROCm#installing-from-amd-rocm-repositories)
4. [MIOpen](https://github.com/ROCmSoftwarePlatform/MIOpen) with OpenCL backend
   Alternatively, you can simply run the [radeonInferenceInstaller](https://github.com/kiritigowda/help/tree/master/radeonInferenceInstaller#installer) which will install all the dependencies for MIOpen
5. [OpenCV 3.1](https://opencv.org/opencv-3-1.html) or higher
6. cmake git 
             sudo apt-get install cmake git

### Build using Cmake on Linux (Ubuntu 16.04 64bit)
     git clone https://github.com/hansely/DGtest.git
     cd DGtest
     mkdir build
     cd build
     cmake ..
     make

### Usage
     Usage: ./DGtest <weights.bin> <input-tensor> <output-tensor> <labels.txt> <imagetag.txt> <batch-size> \n"
     
     1. [weights.bin]
         The name of the weights file to be used for the inference. It is created by running caffemodel converter.
     2. [input-tensor]
         The name of the input tensor to run the inference on. It is created by the img2tensor.py
     3. [output-tensor]
         The name of the ouput tensor that will contian the inference result.
     4. [labels.txt]
         The text file containing the labels of each classes.
     5. [imagetag.txt]
         The text file containing each images' directories. It is created by the img2tensor.py. 
     6. [batch-size]
  
            
### Guideline for Image Preparation & Converting it to tensor
    You can prepare your own handwritten digits by using the [ParseDigit](https://github.com/hansely/ParseDigit) program.
    After the image preparation is done, convert it to a tensor using the python script in this repository
         python img2tensor.py -d <image_directory> -i <imagetag.txt> -o <output_tensor.f32>
         
         1. <image_directory>
              Path to the diectory containing prepared images.
         2. <imagetag.txt>
              The name of the text file that will be created.
              It contains each images' directories which will be used for DGtest's result.
         3. <output_tensor>
              The name of the tensor that will be created.
     
### Testing with your own Caffemodel
    You can test your own trained MNIST caffemodel using the [model compiler](https://github.com/GPUOpen-ProfessionalCompute-Libraries/amdovx-modules/tree/develop/utils/model_compiler)
    
    1. Convert your caffemodel->NNIR->openvx using the model compiler.
    2. From the generated files, copy 
    
         annmodule.cpp
         annmodule.h
         weights.bin
         
       to the DGtest folder.
    3. Build the program again.
         make
         
### Example
    ./ParseDigit ../Examples/image.jpg ../Examples/Cropped/ digits
    
    The output images will be stored in ../Examples/Cropped folder as digits001-1.jpg, digits001-2.jpg, ... digits009-5.jpg.
    Make sure the destination folder is created before running the program.
   
