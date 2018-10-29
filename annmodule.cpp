/*
MIT License

Copyright (c) 2018 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/* This file is generated by nnir2openvx.py on 2018-10-29T09:53:27.146287-07:00 */

#include "annmodule.h"
#include <VX/vx_khr_nn.h>
#include <vx_amd_nn.h>
#include <vx_ext_amd.h>
#include <stdio.h>

#define ERROR_CHECK_OBJECT(obj) { vx_status status = vxGetStatus((vx_reference)(obj)); if(status != VX_SUCCESS) { vxAddLogEntry((vx_reference)context, status     , "ERROR: failed with status = (%d) at " __FILE__ "#%d\n", status, __LINE__); return status; } }
#define ERROR_CHECK_STATUS(call) { vx_status status = (call); if(status != VX_SUCCESS) { vxAddLogEntry((vx_reference)context, status, "ERROR: failed with status = (%d) at " __FILE__ "#%d\n", status, __LINE__); return status; } }

static vx_status initializeTensor(vx_context context, vx_tensor tensor, FILE * fp, const char * binaryFilename)
{
    vx_enum data_type = VX_TYPE_FLOAT32;
    vx_size num_of_dims = 4, dims[4] = { 1, 1, 1, 1 }, stride[4];
    ERROR_CHECK_STATUS(vxQueryTensor(tensor, VX_TENSOR_DATA_TYPE, &data_type, sizeof(vx_enum)));
    ERROR_CHECK_STATUS(vxQueryTensor(tensor, VX_TENSOR_NUMBER_OF_DIMS, &num_of_dims, sizeof(vx_size)));
    ERROR_CHECK_STATUS(vxQueryTensor(tensor, VX_TENSOR_DIMS, &dims, num_of_dims * sizeof(vx_size)));
    vx_size itemsize = sizeof(float);
    if(data_type == VX_TYPE_UINT8 || data_type == VX_TYPE_INT8) {
        itemsize = sizeof(vx_uint8);
    }
    else if(data_type == VX_TYPE_UINT16 || data_type == VX_TYPE_INT16 || data_type == VX_TYPE_FLOAT16) {
        itemsize = sizeof(vx_uint16);
    }
    vx_size count = dims[0] * dims[1] * dims[2] * dims[3];

    vx_uint32 h[2] = { 0 };
    fread(h, 1, sizeof(h), fp);
    if(h[0] != 0xf00dd1e1 || (vx_size)h[1] != (count*itemsize)) {
      vxAddLogEntry((vx_reference)tensor, VX_FAILURE, "ERROR: invalid data (magic,size)=(0x%x,%d) in %s at byte position %d -- expected size is %ld\n", h[0], h[1], binaryFilename, ftell(fp)-sizeof(h), count*itemsize);
      return VX_FAILURE;
    }

    vx_map_id map_id;
    float * ptr;
    ERROR_CHECK_STATUS(vxMapTensorPatch(tensor, num_of_dims, nullptr, nullptr, &map_id, stride, (void **)&ptr, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0));
    vx_size n = fread(ptr, itemsize, count, fp);
    if(n != count) {
        vxAddLogEntry((vx_reference)tensor, VX_FAILURE, "ERROR: expected char[%ld], but got char[%ld] in %s\n", count*itemsize, n*itemsize, binaryFilename);
        return VX_FAILURE;
    }
    ERROR_CHECK_STATUS(vxUnmapTensorPatch(tensor, map_id));

    return VX_SUCCESS;
}

VX_API_ENTRY vx_status VX_API_CALL annAddToGraph(vx_graph graph, vx_tensor data, vx_tensor loss, const char * binaryFilename)
{
    vx_context context = vxGetContext((vx_reference)graph);
    ERROR_CHECK_OBJECT(context);
    ERROR_CHECK_STATUS(vxLoadKernels(context, "vx_nn"));

    // create variables
    vx_size dims_conv1_w[4] = { 5, 5, 1, 20 };
    vx_tensor conv1_w = vxCreateTensor(context, 4, dims_conv1_w, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(conv1_w);
    vx_size dims_conv1_b[2] = { 20, 1 };
    vx_tensor conv1_b = vxCreateTensor(context, 2, dims_conv1_b, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(conv1_b);
    vx_size dims_conv2_w[4] = { 5, 5, 20, 50 };
    vx_tensor conv2_w = vxCreateTensor(context, 4, dims_conv2_w, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(conv2_w);
    vx_size dims_conv2_b[2] = { 50, 1 };
    vx_tensor conv2_b = vxCreateTensor(context, 2, dims_conv2_b, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(conv2_b);
    vx_size dims_ip1_w[4] = { 4, 4, 50, 500 };
    vx_tensor ip1_w = vxCreateTensor(context, 4, dims_ip1_w, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(ip1_w);
    vx_size dims_ip1_b[2] = { 500, 1 };
    vx_tensor ip1_b = vxCreateTensor(context, 2, dims_ip1_b, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(ip1_b);
    vx_size dims_ip2_w[4] = { 1, 1, 500, 10 };
    vx_tensor ip2_w = vxCreateTensor(context, 4, dims_ip2_w, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(ip2_w);
    vx_size dims_ip2_b[2] = { 10, 1 };
    vx_tensor ip2_b = vxCreateTensor(context, 2, dims_ip2_b, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(ip2_b);

    // initialize variables
    FILE * fp__variables = fopen(binaryFilename, "rb");
    if(!fp__variables) {
        vxAddLogEntry((vx_reference)context, VX_FAILURE, "ERROR: unable to open: %s\n", binaryFilename);
        return VX_FAILURE;
    }
    { vx_uint32 magic = 0;
      fread(&magic, 1, sizeof(magic), fp__variables);
      if(magic != 0xf00dd1e0) {
        vxAddLogEntry((vx_reference)context, VX_FAILURE, "ERROR: invalid file magic in %s\n", binaryFilename);
        return VX_FAILURE;
      }
    }
    ERROR_CHECK_STATUS(initializeTensor(context, conv1_w, fp__variables, binaryFilename));
    ERROR_CHECK_STATUS(initializeTensor(context, conv1_b, fp__variables, binaryFilename));
    ERROR_CHECK_STATUS(initializeTensor(context, conv2_w, fp__variables, binaryFilename));
    ERROR_CHECK_STATUS(initializeTensor(context, conv2_b, fp__variables, binaryFilename));
    ERROR_CHECK_STATUS(initializeTensor(context, ip1_w, fp__variables, binaryFilename));
    ERROR_CHECK_STATUS(initializeTensor(context, ip1_b, fp__variables, binaryFilename));
    ERROR_CHECK_STATUS(initializeTensor(context, ip2_w, fp__variables, binaryFilename));
    ERROR_CHECK_STATUS(initializeTensor(context, ip2_b, fp__variables, binaryFilename));
    { vx_uint32 magic = 0;
      fread(&magic, 1, sizeof(magic), fp__variables);
      if(magic != 0xf00dd1e2) {
        vxAddLogEntry((vx_reference)context, VX_FAILURE, "ERROR: invalid eoff magic in %s\n", binaryFilename);
        return VX_FAILURE;
      }
      fclose(fp__variables);
    }

    // create local tensors used in graph
    vx_size dims_conv1[4] = { 24, 24, 20, 4 };
    vx_tensor conv1 = vxCreateVirtualTensor(graph, 4, dims_conv1, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(conv1);
    vx_size dims_pool1[4] = { 12, 12, 20, 4 };
    vx_tensor pool1 = vxCreateVirtualTensor(graph, 4, dims_pool1, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(pool1);
    vx_size dims_conv2[4] = { 8, 8, 50, 4 };
    vx_tensor conv2 = vxCreateVirtualTensor(graph, 4, dims_conv2, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(conv2);
    vx_size dims_pool2[4] = { 4, 4, 50, 4 };
    vx_tensor pool2 = vxCreateVirtualTensor(graph, 4, dims_pool2, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(pool2);
    vx_size dims_ip1[4] = { 1, 1, 500, 4 };
    vx_tensor ip1 = vxCreateVirtualTensor(graph, 4, dims_ip1, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(ip1);
    vx_size dims_relu1[4] = { 1, 1, 500, 4 };
    vx_tensor relu1 = vxCreateVirtualTensor(graph, 4, dims_relu1, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(relu1);
    vx_size dims_ip2[4] = { 1, 1, 10, 4 };
    vx_tensor ip2 = vxCreateVirtualTensor(graph, 4, dims_ip2, VX_TYPE_FLOAT32, 0);
    ERROR_CHECK_OBJECT(ip2);

    // create nodes in graph

    { vx_nn_convolution_params_t conv_params = { 0 };
      conv_params.padding_x = 0;
      conv_params.padding_y = 0;
      conv_params.overflow_policy = VX_CONVERT_POLICY_SATURATE;
      conv_params.rounding_policy = VX_ROUND_POLICY_TO_NEAREST_EVEN;
      conv_params.down_scale_size_rounding = VX_NN_DS_SIZE_ROUNDING_FLOOR;
      conv_params.dilation_x = 0;
      conv_params.dilation_y = 0;
      vx_node node = vxConvolutionLayer(graph, data, conv1_w, conv1_b, &conv_params, sizeof(conv_params), conv1);
      ERROR_CHECK_OBJECT(node);
      ERROR_CHECK_STATUS(vxReleaseNode(&node));
    }

    { vx_node node = vxPoolingLayer(graph, conv1, VX_NN_POOLING_MAX, 2, 2, 0, 0, VX_ROUND_POLICY_TO_NEAREST_EVEN, pool1);
      ERROR_CHECK_OBJECT(node);
      vx_enum border_mode = 0;
      vx_scalar s_border_mode = vxCreateScalarWithSize(context, VX_TYPE_ENUM, &border_mode, sizeof(border_mode));
      ERROR_CHECK_OBJECT(s_border_mode);
      ERROR_CHECK_STATUS(vxSetParameterByIndex(node, 8, (vx_reference) s_border_mode));
      ERROR_CHECK_STATUS(vxReleaseScalar(&s_border_mode));
      ERROR_CHECK_STATUS(vxReleaseNode(&node));
    }

    { vx_nn_convolution_params_t conv_params = { 0 };
      conv_params.padding_x = 0;
      conv_params.padding_y = 0;
      conv_params.overflow_policy = VX_CONVERT_POLICY_SATURATE;
      conv_params.rounding_policy = VX_ROUND_POLICY_TO_NEAREST_EVEN;
      conv_params.down_scale_size_rounding = VX_NN_DS_SIZE_ROUNDING_FLOOR;
      conv_params.dilation_x = 0;
      conv_params.dilation_y = 0;
      vx_node node = vxConvolutionLayer(graph, pool1, conv2_w, conv2_b, &conv_params, sizeof(conv_params), conv2);
      ERROR_CHECK_OBJECT(node);
      ERROR_CHECK_STATUS(vxReleaseNode(&node));
    }

    { vx_node node = vxPoolingLayer(graph, conv2, VX_NN_POOLING_MAX, 2, 2, 0, 0, VX_ROUND_POLICY_TO_NEAREST_EVEN, pool2);
      ERROR_CHECK_OBJECT(node);
      vx_enum border_mode = 0;
      vx_scalar s_border_mode = vxCreateScalarWithSize(context, VX_TYPE_ENUM, &border_mode, sizeof(border_mode));
      ERROR_CHECK_OBJECT(s_border_mode);
      ERROR_CHECK_STATUS(vxSetParameterByIndex(node, 8, (vx_reference) s_border_mode));
      ERROR_CHECK_STATUS(vxReleaseScalar(&s_border_mode));
      ERROR_CHECK_STATUS(vxReleaseNode(&node));
    }

    { vx_node node = vxFullyConnectedLayer(graph, pool2, ip1_w, ip1_b, VX_CONVERT_POLICY_SATURATE, VX_ROUND_POLICY_TO_NEAREST_EVEN, ip1);
      ERROR_CHECK_OBJECT(node);
      ERROR_CHECK_STATUS(vxReleaseNode(&node));
    }

    { vx_node node = vxActivationLayer(graph, ip1, VX_NN_ACTIVATION_RELU, 0.0f, 0.0f, relu1);
      ERROR_CHECK_OBJECT(node);
      ERROR_CHECK_STATUS(vxReleaseNode(&node));
    }

    { vx_node node = vxFullyConnectedLayer(graph, relu1, ip2_w, ip2_b, VX_CONVERT_POLICY_SATURATE, VX_ROUND_POLICY_TO_NEAREST_EVEN, ip2);
      ERROR_CHECK_OBJECT(node);
      ERROR_CHECK_STATUS(vxReleaseNode(&node));
    }

    { vx_node node = vxSoftmaxLayer(graph, ip2, loss);
      ERROR_CHECK_OBJECT(node);
      ERROR_CHECK_STATUS(vxReleaseNode(&node));
    }

    // release local tensors
    ERROR_CHECK_STATUS(vxReleaseTensor(&conv1));
    ERROR_CHECK_STATUS(vxReleaseTensor(&pool1));
    ERROR_CHECK_STATUS(vxReleaseTensor(&conv2));
    ERROR_CHECK_STATUS(vxReleaseTensor(&pool2));
    ERROR_CHECK_STATUS(vxReleaseTensor(&ip1));
    ERROR_CHECK_STATUS(vxReleaseTensor(&relu1));
    ERROR_CHECK_STATUS(vxReleaseTensor(&ip2));

    // release initializer tensors
    ERROR_CHECK_STATUS(vxReleaseTensor(&conv1_w));
    ERROR_CHECK_STATUS(vxReleaseTensor(&conv1_b));
    ERROR_CHECK_STATUS(vxReleaseTensor(&conv2_w));
    ERROR_CHECK_STATUS(vxReleaseTensor(&conv2_b));
    ERROR_CHECK_STATUS(vxReleaseTensor(&ip1_w));
    ERROR_CHECK_STATUS(vxReleaseTensor(&ip1_b));
    ERROR_CHECK_STATUS(vxReleaseTensor(&ip2_w));
    ERROR_CHECK_STATUS(vxReleaseTensor(&ip2_b));

    return VX_SUCCESS;
}
