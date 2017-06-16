#ifndef MIOPEN_GUARD_MIOPEN_H_
#define MIOPEN_GUARD_MIOPEN_H_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wextern-c-compat"
#endif

#include <stddef.h>

#include <miopen/export.h>
#include <miopen/config.h>

#if MIOPEN_BACKEND_OPENCL
#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#elif MIOPEN_BACKEND_HIP
#include <hip/hip_runtime_api.h>
#endif

/*! Constructs type name from a struct */
#define MIOPEN_DECLARE_OBJECT(name) \
struct name {}; \
typedef struct name * name ## _t;

#ifdef __cplusplus
extern "C" {
#endif

#if MIOPEN_BACKEND_OPENCL
typedef cl_command_queue miopenAcceleratorQueue_t;
#elif MIOPEN_BACKEND_HIP
typedef hipStream_t miopenAcceleratorQueue_t;
#endif


/*! \brief Creates the miopenHandle_t type */
MIOPEN_DECLARE_OBJECT(miopenHandle);


/*! \enum miopenStatus_t 
 * Error codes that are returned by all MIOpen API calls.
*/
typedef enum {
    miopenStatusSuccess         = 0, /*!< No errors */
    miopenStatusNotInitialized  = 1, /*!< Data not initialized. */
    miopenStatusInvalidValue    = 2, /*!< Incorrect variable value. */
    miopenStatusBadParm         = 3, /*!< Incorrect parameter detected. */
    miopenStatusAllocFailed     = 4, /*!< Memory allocation error. */
    miopenStatusInternalError   = 5, /*!< MIOpen failure. */ 
    miopenStatusNotImplemented  = 6, /*!< Use of unimplemented feature. */
    miopenStatusUnknownError    = 7, /*!< Unknown error occurred. */
} miopenStatus_t;


/*! \brief Method to create the MIOpen handle object. 
 *
 * Creates a MIOpen handle. This is called at the very start to initialize the MIOpen environment.
 * \param handle     A pointer to a MIOpen handle type
 * \return           miopenStatus_t 
*/
MIOPEN_EXPORT miopenStatus_t miopenCreate(miopenHandle_t *handle);

/*! \brief Create a MIOpen handle with an accelerator stream. 
 *
 * Create a handle with a previously created accelerator command queue
 * \param handle     A pointer to a  MIOpen handle type
 * \param streamID   An accelerator queue type
 * \return           miopenStatus_t 
*/
MIOPEN_EXPORT miopenStatus_t miopenCreateWithStream(miopenHandle_t *handle,
        miopenAcceleratorQueue_t        stream);

/*! \brief Destroys the MIOpen handle.
 *
 * This is called when breaking down the MIOpen environment.
 * \param handle     MIOpen handle
 * \return           miopenStatus_t 
*/
MIOPEN_EXPORT miopenStatus_t miopenDestroy(miopenHandle_t handle);

/*! \brief Set accelerator command queue previously created 
 *
 * Set a command queue for an accelerator device
 * \param handle     MIOpen handle
 * \param streamID   An accelerator queue type
 * \return           miopenStatus_t 
*/
MIOPEN_EXPORT miopenStatus_t miopenSetStream(miopenHandle_t handle,
        miopenAcceleratorQueue_t        streamID);

/*! \brief Get the previously created accelerator command queue  
 *
 * Creates a command queue for an accelerator device
 * \param handle     MIOpen handle
 * \param streamID   Pointer to a accelerator queue type
 * \return           miopenStatus_t 
*/
MIOPEN_EXPORT miopenStatus_t miopenGetStream(miopenHandle_t handle,
        miopenAcceleratorQueue_t        *streamID);

/*! \brief Get time for last kernel launched
 *
 *  This function is used only when profiling mode has been enabled.
 * \param handle     MIOpen handle
 * \param time       Pointer to a float type to contain kernel time in milliseconds
 * \return           miopenStatus_t 
*/
MIOPEN_EXPORT miopenStatus_t miopenGetKernelTime(miopenHandle_t handle, float* time);

/*! \brief Enable profiling to retrieve kernel time
 * 
 * Enable or disable kernel profiling. This profiling is only for kernel time.
 * \param handle     MIOpen handle
 * \param enable     Boolean to toggle profiling
 * \return           miopenStatus_t 
*/ 
MIOPEN_EXPORT miopenStatus_t miopenEnableProfiling(miopenHandle_t handle, bool enable);

/*! \brief Creates the miopenTensorDescriptor_t type */
MIOPEN_DECLARE_OBJECT(miopenTensorDescriptor);

/*! \brief Creates the miopenConvolutionDescriptor_t type */
MIOPEN_DECLARE_OBJECT(miopenConvolutionDescriptor);

/*! \brief Creates the miopenPoolingDescriptor_t type */
MIOPEN_DECLARE_OBJECT(miopenPoolingDescriptor);

/*! \brief Creates the miopenLRNDescriptor_t type */
MIOPEN_DECLARE_OBJECT(miopenLRNDescriptor);

/*! \brief Creates the miopenActivationDescriptor_t type */
MIOPEN_DECLARE_OBJECT(miopenActivationDescriptor);


/*! \enum miopenDataType_t
 * MIOpen floating point datatypes. Currently only 32-bit floats are fully supported in MIOpen.
*/ 
typedef enum {
    miopenHalf  = 0, /*!< 16-bit floating point (not supported) */
    miopenFloat = 1, /*!< 32-bit floating point (fully supported) */
} miopenDataType_t;


/*! \enum miopenTensorOp_t
 * Element-wise tensor operation modes
*/ 
typedef enum {
    miopenTensorOpAdd = 0, /*!< Add tensors element-wise */
    miopenTensorOpMul = 1, /*!< Multiply two tensors element-wise */ 
    miopenTensorOpMin = 2, /*!< Minimum of tensor element pairs */
    miopenTensorOpMax = 3, /*!< Maximum of tensor element pairs */
} miopenTensorOp_t;

/*! \enum miopenConvolutionMode_t
 * Convolution mode selection for convolution layer preference
*/ 
typedef enum {
    miopenConvolution = 0, /*!< Convolutions */
    miopenTranspose   = 1, /*!< Transpose convolutions */
} miopenConvolutionMode_t;

/*! \enum miopenPoolingMode_t
 * Pooling layer mode
*/
typedef enum {
    miopenPoolingMax     = 0, /*!< Maximum pooling */
    miopenPoolingAverage = 1, /*!< Average pooling */
} miopenPoolingMode_t;


/*! \enum miopenLRNMode_t
 * Local Response Normalization layer mode
*/ 
typedef enum {
    miopenLRNWithinChannel = 0, /*!< Channel independent */
    miopenLRNCrossChannel  = 1, /*!< Cross Channel */
} miopenLRNMode_t;

/*! \enum miopenBatchNormMode_t;
 * Batch Normalization layer mode
*/
typedef enum {
   miopenBNPerActivation = 0,/*!< Element-wise normalization for fully connected layer */
   miopenBNSpatial       = 1,/*!< Mini-batch spatial normalization for convolutional layers */
}miopenBatchNormMode_t;

/*! \enum miopenActivationMode_t
 * Activation layer modes
 */ 
typedef enum {
    miopenActivationPATHTRU     = 0, /*!< No activation, pass through the data */
    miopenActivationLOGISTIC    = 1, /*!< Sigmoid function: \f$1 / (1 + e^-x)\f$ */
    miopenActivationTANH        = 2, /*!< Tanh activation \f$ a * tanh( b * x) \f$ */
    miopenActivationRELU        = 3, /*!< Rectified Linear Unit \f$ max(0, x) \f$ */
    miopenActivationSOFTRELU    = 4, /*!< \f$log(1 + e^x)\f$ */
    miopenActivationABS         = 5, /*!< Absolute value \f$abs(x)\f$ */
    miopenActivationPOWER       = 6, /*!< Scaled and shifted power \f$(a + b * x)^p\f$ */
} miopenActivationMode_t;

/*! \brief Create a Tensor Descriptor
 *
 * API for creating an uninitialized tensor descriptor.
 * \param tensorDesc Pointer to a tensor descriptor type
 * \return           miopenStatus_t
*/ 
MIOPEN_EXPORT miopenStatus_t miopenCreateTensorDescriptor(miopenTensorDescriptor_t *tensorDesc);

/*! \brief Set shape of 4D tensor
 * 
 * Interface for setting 4-D tensor shape. MIOpen currently only implements NCHW layout.
 * \param tensorDesc Tensor descriptor type
 * \param dataType   Currently only miopenFloat is implemented
 * \param n          Mini-batch size
 * \param c          Number of channels
 * \param h          Data height dimension size
 * \param w          Data width dimension size
 * \return           miopenStatus_t
*/ 
MIOPEN_EXPORT miopenStatus_t miopenSet4dTensorDescriptor(
        miopenTensorDescriptor_t        tensorDesc,
        miopenDataType_t                dataType, 
        int                             n,
        int                             c,
        int                             h,
        int                             w);

/*! \brief Get the details of the tensor desciptor
 * 
 * Interface to query the 4-D tensor shape.
 * \param tensorDesc Tensor descriptor type
 * \param dataType   Currently only miopenFloat is implemented
 * \param n          Mini-batch size
 * \param c          Number of channels
 * \param h          Data height dimension size
 * \param w          Data width dimension size
 * \param nstride    Mini-batch dimension stride
 * \param cstride    Channel dimension stride
 * \param hstride    Height dimension stride
 * \param wstride    Width dimension stride
 * \return           miopenStatus_t
*/ 
MIOPEN_EXPORT miopenStatus_t miopenGet4dTensorDescriptor(
        miopenTensorDescriptor_t        tensorDesc,
        miopenDataType_t                *dataType,
        int                             *n,
        int                             *c, 
        int                             *h,
        int                             *w,
        int                             *nStride,
        int                             *cStride,
        int                             *hStride,
        int                             *wStride);

/*! \brief Set shape of 4D tensor
 * 
 * Interface for setting ensor shape. MIOpen only has 4-D tensors in NCHW layout.
 * \param tensorDesc   Tensor descriptor type
 * \param dataType     Currently only miopenFloat is implemented
 * \param nbDims       Number of dimensions in the dimsA array
 * \param dimsA        Array containing the size of dimensions
 * \param stridesA     Array containing the size of stride
 * \return             miopenStatus_t
*/ 
MIOPEN_EXPORT miopenStatus_t miopenSetTensorDescriptor(
        miopenTensorDescriptor_t        tensorDesc,
        miopenDataType_t                dataType,
        int                             nbDims,
        int                             *dimsA,
        int                             *stridesA);


/*! \brief Set shape of 4D tensor
 * 
 * Interface for querying tensor size. MIOpen only has 4-D tensors in NCHW layout.
 * \param tensorDesc   Tensor descriptor type
 * \param size         number of elements in tensor described by the descriptor
 * \return             miopenStatus_t
*/ 
MIOPEN_EXPORT miopenStatus_t miopenGetTensorDescriptorSize(miopenTensorDescriptor_t tensorDesc, int* size);

/*! \brief Get the details of the n-dimensional tensor descriptor.
 *
 * \param tensorDesc Tensor descriptor type
 * \param dataType   Currently only miopenFloat is implemented
 * \param dimsA      Array containing the size of dimensions
 * \param stridesA   Array containing the size of stride
 * \return           miopenStatus_t
 */  
MIOPEN_EXPORT miopenStatus_t miopenGetTensorDescriptor(
        miopenTensorDescriptor_t        tensorDesc,
        miopenDataType_t                *dataType,
        int                             *dimsA,
        int                             *stridesA);
        
/*! \brief Destroys the tensor descriptor
 * 
 * \param tensorDesc Tensor descriptor type
 * \return           miopenStatus_t
*/ 
MIOPEN_EXPORT miopenStatus_t miopenDestroyTensorDescriptor(miopenTensorDescriptor_t tensorDesc);

/*! \brief Execute element-wise tensor operations
 * 
 *  This function implements the equation \f$ C = op ( alpha1[0] * A, alpha2[0] * B * ) + beta[0] * C \f$
 *  For Forward Bias one can also use, miopenConvolutionForwardBias()
 * \param handle     MIOpen handle
 * \param tensorOp   Operation from miopenTensorOp_t
 * \param alpha1     Tensor A's scaling factor currently always 1
 * \param aDesc      Tensor descriptor for tensor A
 * \param A          Tensor A
 * \param alpha2     Tensor B's scaling factor is currently always 1
 * \param bDesc      Tensor descriptor for tensor B
 * \param B          Tensor B
 * \param beta       Tensor C's scaling factor
 * \param cDesc      Tensor descriptor for tensor C
 * \param C          Tensor C
 * \return           miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenOpTensor(miopenHandle_t handle,
        miopenTensorOp_t                tensorOp,
        const void                      *alpha1,
        const miopenTensorDescriptor_t  aDesc,
        const void                      *A,
        const void                      *alpha2,
        const miopenTensorDescriptor_t  bDesc,
        const void                      *B,
        const void                      *beta,
        const miopenTensorDescriptor_t  cDesc,
        void                            *C);

/*! \brief Fills a tensor with a single value.
 * 
 * \param handle     MIOpen handle
 * \param cDesc      Tensor descriptor for tensor y
 * \param alpha      Pointer to fill value
 * \return           miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenSetTensor(miopenHandle_t handle,
        const miopenTensorDescriptor_t  yDesc,
        void                            *y,
        const void                      *alpha );

/*! \brief Scales all elements in a tensor by a single value.
 * 
 * \param handle     MIOpen handle
 * \param cDesc      Tensor descriptor for tensor y
 * \param alpha      Pointer to scaling value
 * \return           miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenScaleTensor(miopenHandle_t handle,
        const miopenTensorDescriptor_t  yDesc,
        void                            *y,
        const void                      *alpha );

/*! \brief Creates a convolution layer descriptor
 * 
 * \param convDesc   Convolution layer descriptor
 * \return           miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenCreateConvolutionDescriptor(
        miopenConvolutionDescriptor_t *convDesc);

/*! \brief Creates a convolution layer descriptor
 * 
 * \param convDesc   Convolution layer descriptor
 * \param mode       Convolutional mode
 * \param pad_h      Height input data padding
 * \param pad_w      Width input data padding
 * \param u          Stride for the height of input data
 * \param v          Stride for the width of input data
 * \param upscalex   Dilation height (only a value of 1 is currently supported)
 * \param upscaley   Dilation widdth (only a value of 1 is currently supported)
 * \return           miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenInitConvolutionDescriptor(miopenConvolutionDescriptor_t convDesc,
        miopenConvolutionMode_t         mode,
        int                             pad_h,
        int                             pad_w,
        int                             u,
        int                             v,
        int                             dilation_h,
        int                             dilation_w);

/*! \brief Retrieves a convolution layer descriptor's details
 * 
 * \param convDesc   Convolution layer descriptor
 * \param mode       Convolutional mode
 * \param pad_h      Height input data padding
 * \param pad_w      Width input data padding
 * \param u          Stride for the height of input data
 * \param v          Stride for the width of input data
 * \param upscalex   Dilation height (only a value of 1 is currently supported)
 * \param upscaley   Dilation widdth (only a value of 1 is currently supported)
 * \return           miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenGetConvolutionDescriptor(miopenConvolutionDescriptor_t convDesc,
        miopenConvolutionMode_t         *mode,
        int                             *pad_h,
        int                             *pad_w,
        int                             *u,
        int                             *v,
        int                             *dilation_h,
        int                             *dilation_w);

/*! \brief Get the shape of  a resulting 4-D tensor from a 2-D convolution
 * 
 * This function returns the dimensions of the resulting 4D tensor of a 2D
 * convolution, given the convolution descriptor, the input tensor descriptor
 * and the filter descriptor This function can help to setup the output tensor
 * and allocate the proper amount of memory prior to launch the actual
 * convolution.
 * 
 * \param convDesc   Convolution layer descriptor
 * \param mode       Convolutional mode
 * \param n          Mini-batch size
 * \param c          Number of channels
 * \param h          Data height dimension size
 * \param w          Data width dimension size
 * \return           miopenStatus_t  
 */
MIOPEN_EXPORT miopenStatus_t miopenGetConvolutionForwardOutputDim(miopenConvolutionDescriptor_t convDesc,
        const miopenTensorDescriptor_t  inputTensorDesc,
        const miopenTensorDescriptor_t  filterDesc,
        int                             *n,
        int                             *c,
        int                             *h,
        int                             *w);
        
/*! \brief Destroys the tensor descriptor object
 * 
 * \param tensorDesc Convolution tensor descriptor type
 * \return           miopenStatus_t
*/ 
MIOPEN_EXPORT miopenStatus_t miopenDestroyConvolutionDescriptor(miopenConvolutionDescriptor_t convDesc);

/*! \enum miopenConvFwdAlgorithm_t
 * Convolutional algorithm mode for forward propagation.
 */
typedef enum {
    miopenConvolutionFwdAlgoGEMM     = 0, /*!< GEMM variant */
    miopenConvolutionFwdAlgoDirect   = 1, /*!< Direct convolutions */
    miopenConvolutionFwdAlgoFFT      = 2, /*!< Fast Fourier Transform indirect convolutions */
    miopenConvolutionFwdAlgoWinograd = 3, /*!< Winograd indirect convolutions */
} miopenConvFwdAlgorithm_t;

/*! \enum miopenConvBwdWeightsAlgorithm_t
 * Convolutional algorithm mode for back propagation on weights.
 */
typedef enum {
    miopenConvolutionBwdWeightsAlgoGEMM   = 0, /*!< GEMM variant */
    miopenConvolutionBwdWeightsAlgoDirect = 1, /*!< Direct convolution algorithm */
} miopenConvBwdWeightsAlgorithm_t;

/*! \enum miopenConvBwdDataAlgorithm_t
 * Convolutional algorithm mode for back propagation on data.
 */
typedef enum {
    miopenConvolutionBwdDataAlgoGEMM     = 0, /*!< GEMM variant */
    miopenConvolutionBwdDataAlgoDirect   = 1, /*!< Direct convolutions */
    miopenConvolutionBwdDataAlgoFFT      = 2, /*!< Fast Fourier Transform indirect convolutions */
    miopenConvolutionBwdDataAlgoWinograd = 3, /*!< Winograd indirect convolutions */
    miopenTransposeBwdDataAlgoGEMM       = 4, /*!< Transpose GEMM variant */
} miopenConvBwdDataAlgorithm_t;

/*! \struct miopenConvAlgoPerf_t
 * \brief Perf struct for forward, backward filter, or backward data algorithms
 * 
 * Contains the union to hold the selected convolution algorithm for forward, or backwards layers.
 * Also contains the time it took to run the algorithm and the workspace required to run the algorithm.
 */ 
typedef struct{
    union {
        miopenConvFwdAlgorithm_t fwd_algo; /*!< Forward convolution algorithm enum selection */
        miopenConvBwdWeightsAlgorithm_t bwd_weights_algo; /*!< Back propagation on weights convolution algorithm enum selection */
        miopenConvBwdDataAlgorithm_t bwd_data_algo;/*!< Back propagation on data convolution algorithm enum selection */
    };
    float time; /*!< Time to exectued the selected algorithm represented in the union */
    size_t memory; /*!< Workspace required to run the selected algorithm represented in the union */
} miopenConvAlgoPerf_t;


/*! \brief Query the workspace size required for a forward convolution layer
 * 
 * This call is required for running the convolutional layers and for running the findConvolution functions.
 * 
 * \param handle         MIOpen handle
 * \param wDesc          Tensor descriptor for weight tensor w
 * \param xDesc          Tensor descriptor for input data tensor x
 * \param convDesc       Convolution layer descriptor
 * \param yDesc          Tensor descriptor for output data tensor y
 * \param workSpaceSize  Pointer to memory to return size in bytes
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenConvolutionForwardGetWorkSpaceSize(
        miopenHandle_t                      handle,
        const miopenTensorDescriptor_t      wDesc,
        const miopenTensorDescriptor_t      xDesc,
        const miopenConvolutionDescriptor_t convDesc,
        const miopenTensorDescriptor_t      yDesc,
        size_t                              *workSpaceSize);

/*! \brief Search and run the forward convolutional algorithms and return a list of kernel times.
 * 
 * This function attempts all MIOpen algorithms for miopenConvolutionForward(), and outputs performance metrics to a user- allocated array of miopenConvAlgoPerf_t. 
 * These metrics are written in sorted fashion where the first element has the lowest compute time.
 * This function is mandatory before using forward convolutions. Users can chose the top-most algorithm if they only care about the fastest algorithm.
 * 
 * \param handle             MIOpen handle
 * \param xDesc              Tensor descriptor for data input tensor x
 * \param x                  Data tensor x
 * \param wDesc              Tensor descriptor for weight tensor w
 * \param w                  Weights tensor w
 * \param convDesc           Convolution layer descriptor
 * \param yDesc              Tensor descriptor for output data tensor y
 * \param y                  Data tensor y
 * \param requestAlgoCount   Number of algorithms to return kernel times
 * \param returnedAlgoCount  Pointer to number of algorithms returned
 * \param perResults         Pointer to union of best algorithm for forward and backwards
 * \param workSpace          Pointer to workspace required for the search
 * \param workSpaceSize      Size in bytes of the memory needed for find
 * \param exhaustiveSearch   A boolean to toggle a full search of all algorithms and configurations.
 * \return                   miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenFindConvolutionForwardAlgorithm(miopenHandle_t handle,
        const miopenTensorDescriptor_t      xDesc,
        const void                          *x,
        const miopenTensorDescriptor_t      wDesc,
        const void                          *w,
        const miopenConvolutionDescriptor_t convDesc,
        const miopenTensorDescriptor_t      yDesc,
        void                                *y,
        const int                           requestAlgoCount,
        int                                 *returnedAlgoCount,
        miopenConvAlgoPerf_t                *perfResults,
        void                                *workSpace,
        size_t                              workSpaceSize,
        bool                                exhaustiveSearch);

/*! \brief Execute a forward convolution layer
 * 
 * \param handle         MIOpen handle
 * \param alpha          Scaling factor, always equal to 1
 * \param xDesc          Tensor descriptor for data input tensor x
 * \param x              Data tensor x
 * \param wDesc          Tensor descriptor for weight tensor w
 * \param w              Weights tensor w
 * \param convDesc       Convolution layer descriptor
 * \param algo           Algorithm selected
 * \param beta           Shift factor, always equal to 0
 * \param yDesc          Tensor descriptor for output data tensor y
 * \param y              Data tensor y
 * \param workSpace      Pointer to workspace required for the search
 * \param workSpaceSize  Size in bytes of the memory needed for find
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenConvolutionForward(miopenHandle_t handle,
        const void                          *alpha,
        const miopenTensorDescriptor_t      xDesc,
        const void                          *x,
        const miopenTensorDescriptor_t      wDesc,
        const void                          *w,
        const miopenConvolutionDescriptor_t convDesc,
        miopenConvFwdAlgorithm_t            algo,
        const void                          *beta,
        const miopenTensorDescriptor_t      yDesc,
        void                                *y,
        void                                *workSpace,
        size_t                              workSpaceSize);

/*! \brief Calculate element-wise scale and shift of a tensor via a bias tensor
 * 
 *  This function applies an element-wise bias to a data tensor from an input bias tensor. Currently for MIOpen v1.0 alpha and beta are 1 and 0 respectively.
 * 
 * \param handle         MIOpen handle
 * \param alpha          Scaling factor, always equal to 1
 * \param bDesc         Tensor descriptor for bias tensor b
 * \param dy             Bias tensor b
 * \param beta           Shift factor, always equal to 0
 * \param dyDesc         Tensor descriptor for data tensor y
 * \param y              Data tensor y
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenConvolutionForwardBias(miopenHandle_t handle,
        const void                          *alpha,
        const miopenTensorDescriptor_t      bDesc,
        const void                          *b,
        const void                          *beta,
        const miopenTensorDescriptor_t      yDesc,
        void                                *y);

/*! \brief Search and run the backwards data convolutional algorithms and return a list of kernel times.
 * 
 * This function attempts all MIOpen algorithms for miopenConvolutionBackwardsData(), and outputs performance metrics to a user- allocated array of miopenConvAlgoPerf_t. 
 * These metrics are written in sorted fashion where the first element has the lowest compute time.
 * This function is mandatory before using backwards convolutions. Users can chose the top-most algorithm if they only care about the fastest algorithm.
 * 
 * \param handle             MIOpen handle
 * \param dyDesc             Tensor descriptor for data input tensor dy
 * \param y                  Data delta tensor dy
 * \param wDesc              Tensor descriptor for weight tensor w
 * \param w                  Weights tensor w
 * \param convDesc           Convolution layer descriptor
 * \param dxDesc             Tensor descriptor for output data tensor dx
 * \param dx                 Data delta tensor dx
 * \param requestAlgoCount   Number of algorithms to return kernel times
 * \param returnedAlgoCount  Pointer to number of algorithms returned
 * \param perResults         Pointer to union of best algorithm for forward and backwards
 * \param workSpace          Pointer to workspace required for the search
 * \param workSpaceSize      Size in bytes of the memory needed for find
 * \param exhaustiveSearch   A boolean to toggle a full search of all algorithms and configurations.
 * \return                   miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenFindConvolutionBackwardDataAlgorithm(miopenHandle_t handle,
        const miopenTensorDescriptor_t      dyDesc,
        const void                          *dy,
        const miopenTensorDescriptor_t      wDesc,
        const void                          *w,
        const miopenConvolutionDescriptor_t convDesc,
        const miopenTensorDescriptor_t      dxDesc,
        const void                          *dx,
        const int                           requestAlgoCount,
        int                                 *returnedAlgoCount,
        miopenConvAlgoPerf_t                *perfResults,
        void                                *workSpace,
        size_t                              workSpaceSize,
        bool                                exhaustiveSearch);

/*! \brief Execute a backward data convolution layer
 * 
 * \param handle         MIOpen handle
 * \param alpha          Scaling factor, always equal to 1
 * \param dyDesc         Tensor descriptor for data input tensor dy
 * \param dy             Data delta tensor dy
 * \param wDesc          Tensor descriptor for weight tensor w
 * \param w              Weights tensor w
 * \param convDesc       Convolution layer descriptor
 * \param algo           Algorithm selected
 * \param beta           Shift factor, always equal to 0
 * \param dxDesc         Tensor descriptor for output data tensor dx
 * \param dx             Data delta tensor dx
 * \param workSpace      Pointer to workspace required for the search
 * \param workSpaceSize  Size in bytes of the memory needed for find
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenConvolutionBackwardData(miopenHandle_t handle,
        const void                          *alpha,
        const miopenTensorDescriptor_t      dyDesc,
        const void                          *dy,
        const miopenTensorDescriptor_t      wDesc,
        const void                          *w,
        const miopenConvolutionDescriptor_t convDesc,
        miopenConvBwdDataAlgorithm_t        algo,
        const void                          *beta,
        const miopenTensorDescriptor_t      dxDesc,
        void                                *dx,
        void                                *workSpace,
        size_t                              workSpaceSize);


/*! \brief Get the GPU memory required for the backward data convolution algorithm.
 * 
 * For a provided tensor descriptors and algorithm selection, this function calculates and returns the workspace size required for back propagation on data.
 * 
 * \param handle         MIOpen handle
 * \param dyDesc         Tensor descriptor for data input tensor dy
 * \param wDesc          Tensor descriptor for weight tensor w
 * \param convDesc       Convolution layer descriptor
 * \param dxDesc         Tensor descriptor for output data tensor dx
 * \param workSpaceSize  Size in bytes of the memory required
 * \return               miopenStatus_t 
*/ 
MIOPEN_EXPORT miopenStatus_t miopenConvolutionBackwardDataGetWorkSpaceSize(
        miopenHandle_t                      handle,
        const miopenTensorDescriptor_t      dyDesc,
        const miopenTensorDescriptor_t      wDesc,
        const miopenConvolutionDescriptor_t convDesc,
        const miopenTensorDescriptor_t      dxDesc,
        size_t                              *workSpaceSize);


/*! \brief Get the GPU memory required for the backward weights convolution algorithm.
 * 
 * For a provided tensor descriptors and algorithm selection, this function calculates and returns the workspace size required for back propagation on weights.
 * 
 * \param handle         MIOpen handle
 * \param dyDesc         Tensor descriptor for data input tensor dy
 * \param xDesc          Tensor descriptor for data tensor x
 * \param convDesc       Convolution layer descriptor
 * \param dwDesc         Tensor descriptor for output weights tensor dw
 * \param workSpaceSize  Size in bytes of the memory required
 * \return               miopenStatus_t 
*/ 
MIOPEN_EXPORT miopenStatus_t miopenConvolutionBackwardWeightsGetWorkSpaceSize(
        miopenHandle_t                      handle,
        const miopenTensorDescriptor_t      dyDesc,
        const miopenTensorDescriptor_t      xDesc,
        const miopenConvolutionDescriptor_t convDesc,
        const miopenTensorDescriptor_t      dwDesc,
        size_t                              *workSpaceSize);


/*! \brief Search and run the backwards weights convolutional algorithms and return a list of kernel times.
 * 
 * This function attempts all MIOpen algorithms for miopenConvolutionBackwardsWeights(), and outputs performance metrics to a user- allocated array of miopenConvAlgoPerf_t. 
 * These metrics are written in sorted fashion where the first element has the lowest compute time.
 * This function is mandatory before using backwards weight convolutions. Users can chose the top-most algorithm if they only care about the fastest algorithm.
 * 
 * \param handle             MIOpen handle
 * \param dyDesc             Tensor descriptor for data input tensor dy
 * \param y                  Data delta tensor dy
 * \param xDesc              Tensor descriptor for output data tensor x
 * \param x                  Data delta tensor dx
 * \param convDesc           Convolution layer descriptor
 * \param wDesc              Tensor descriptor for weight tensor w
 * \param w                  Weights tensor w
 * \param requestAlgoCount   Number of algorithms to return kernel times
 * \param returnedAlgoCount  Pointer to number of algorithms returned
 * \param perResults         Pointer to union of best algorithm for forward and backwards
 * \param workSpace          Pointer to workspace required for the search
 * \param workSpaceSize      Size in bytes of the memory needed for find
 * \param exhaustiveSearch   A boolean to toggle a full search of all algorithms and configurations.
 * \return                   miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenFindConvolutionBackwardWeightsAlgorithm(miopenHandle_t handle,
        const miopenTensorDescriptor_t      dyDesc,
        const void                          *dy,
        const miopenTensorDescriptor_t      xDesc,
        const void                          *x,
        const miopenConvolutionDescriptor_t convDesc,
        const miopenTensorDescriptor_t      dwDesc,
        void                                *dw,
        const int                           requestAlgoCount,
        int                                 *returnedAlgoCount,
        miopenConvAlgoPerf_t                *perfResults,
        void                                *workSpace,
        size_t                              workSpaceSize,
        bool                                exhaustiveSearch);

/*! \brief Execute a backward weights convolution layer
 * 
 * \param handle         MIOpen handle
 * \param alpha          Scaling factor, always equal to 1
 * \param dyDesc         Tensor descriptor for data tensor dy
 * \param dy             Data delta tensor dy
 * \param xDesc          Tensor descriptor for data tensor x
 * \param x              Data tensor x
 * \param convDesc       Convolution layer descriptor
 * \param algo           Algorithm selected
 * \param beta           Shift factor, always equal to 0
 * \param dwDesc         Tensor descriptor for weight tensor dw
 * \param dw             Weights delta tensor dw
 * \param workSpace      Pointer to workspace required for the search
 * \param workSpaceSize  Size in bytes of the memory needed for find
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenConvolutionBackwardWeights(miopenHandle_t handle,
        const void                          *alpha,
        const miopenTensorDescriptor_t      dyDesc,
        const void                          *dy,
        const miopenTensorDescriptor_t      xDesc,
        const void                          *x,
        const miopenConvolutionDescriptor_t convDesc,
        miopenConvBwdWeightsAlgorithm_t     algo,
        const void                          *beta,
        const miopenTensorDescriptor_t      dwDesc,
        void                                *dw,
        void                                *workSpace,
        size_t                              workSpaceSize);


/*! \brief Calculates the gradient with respect to the bias.
 * 
 * Compute the convolution backwards gradient with respect to the bias tensor. Currently for MIOpen v1.0 alpha and beta are 1 and 0 respectively.
 * 
 * \param handle         MIOpen handle
 * \param alpha          Scaling factor, always equal to 1
 * \param dyDesc         Tensor descriptor for data input tensor dy
 * \param dy             Data delta tensor dy
 * \param beta           Shift factor, always equal to 0
 * \param dbDesc         Tensor descriptor for input bias tensor db
 * \param db             Bias delta tensor db
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenConvolutionBackwardBias(miopenHandle_t handle,
        const void                          *alpha,
        const miopenTensorDescriptor_t      dyDesc,
        const void                          *dy,
        const void                          *beta,
        const miopenTensorDescriptor_t      dbDesc,
        void                                *db);




// Pooling APIs

/*! \brief Creates a pooling layer descriptor
 * 
 * \param poolDesc   Pointer to a pooling layer descriptor
 * \return           miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenCreatePoolingDescriptor(miopenPoolingDescriptor_t *poolDesc);


/*! \brief Sets a 2-D pooling layer descriptor details 
 * 
 * Sets the window shape, padding, and stride for a previously created 2-D pooling descriptor
 * 
 * \param poolDesc       Pointer to a pooling layer descriptor
 * \param mode           Pooling mode enum
 * \param windowHeight   Input window height dimension
 * \param windowWidth    Input window width dimension
 * \param pad_h          Number of elements to pad height        
 * \param pad_h          Number of elements to pad width
 * \param u              Vertical stride 
 * \param v              Horizontal stride 
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenSet2dPoolingDescriptor(
        miopenPoolingDescriptor_t           poolDesc,
        miopenPoolingMode_t                 mode,
        int                                 windowHeight,
        int                                 windowWidth,
        int                                 pad_h,
        int                                 pad_w,
        int                                 u,
        int                                 v);

/*! \brief Gets a 2-D pooling layer descriptor details 
 * 
 * Gets the window shape, padding, and stride for a previously created 2-D pooling descriptor
 * 
 * \param poolDesc       Pointer to a pooling layer descriptor
 * \param mode           Pooling mode enum
 * \param windowHeight   Input window height dimension
 * \param windowWidth    Input window width dimension
 * \param pad_h          Number of elements to pad height        
 * \param pad_h          Number of elements to pad width
 * \param u              Vertical stride 
 * \param v              Horizontal stride 
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenGet2dPoolingDescriptor(
        const miopenPoolingDescriptor_t     poolDesc,
        miopenPoolingMode_t                 *mode,
        int                                 *windowHeight,
        int                                 *windowWidth,
        int                                 *pad_h,
        int                                 *pad_w,
        int                                 *u,
        int                                 *v);

/*! \brief Gets the shape of the output tensor for 2-D pooling
 * 
 * Retrieve the tensor dimensions for the forward 2-D pooling 
 * 
 * \param poolDesc       Pointer to a pooling layer descriptor
 * \param mode           Pooling mode enum
 * \param windowHeight   Input window height dimension
 * \param windowWidth    Input window width dimension
 * \param pad_h          Number of elements to pad height        
 * \param pad_h          Number of elements to pad width
 * \param u              Vertical stride 
 * \param v              Horizontal stride 
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenGetPoolingForwardOutputDim(
        const miopenPoolingDescriptor_t     poolDesc,
        const miopenTensorDescriptor_t      tensorDesc,
        int                                 *n,
        int                                 *c,
        int                                 *h,
        int                                 *w);

/*! \brief Get the amount of GPU memory required for pooling
 * 
 * Retrieves the amount of workspace in bytes require for pooling
 * 
 * \param yDesc          Descriptor for pooling layer
 * \param workSpaceSize  Pointer to workSpaceSize
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenPoolingGetWorkSpaceSize(
        const miopenTensorDescriptor_t      yDesc,
        size_t                              *workSpaceSize);

/*! \brief Execute a forward pooling layer
 * 
 * \param handle         MIOpen handle
 * \param poolDesc       Descriptor for pooling layer
 * \param alpha          Scaling factor, always equal to 1
 * \param xDesc          Tensor descriptor for data input tensor x
 * \param x              Data tensor x
 * \param beta           Shift factor, always equal to 0
 * \param yDesc          Tensor descriptor for output data tensor y
 * \param y              Data tensor y
 * \param do_backward    Boolean to toggle save data in workspace for backwards pass
 * \param workSpace      Pointer user allocated memory
 * \param workSpaceSize  Size in bytes of the memory needed 
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenPoolingForward(
        miopenHandle_t                      handle,
        const miopenPoolingDescriptor_t     poolDesc,
        const void                          *alpha,
        const miopenTensorDescriptor_t      xDesc,
        const void                          *x,
        const void                          *beta,
        const miopenTensorDescriptor_t      yDesc,
        void                                *y,
        bool                                do_backward,
        void                                *workSpace,
        size_t                              workSpaceSize);


/*! \brief Execute a backward pooling layer
 * 
 * \param handle         MIOpen handle
 * \param poolDesc       Descriptor for pooling layer
 * \param alpha          Scaling factor, always equal to 1
 * \param yDesc          Tensor descriptor for output data tensor y
 * \param y              Data tensor y
 * \param dyDesc         Tensor descriptor for data input tensor dy
 * \param dy             Data delta tensor dy
 * \param xDesc          Tensor descriptor for output data tensor x
 * \param x              Data tensor x
 * \param beta           Shift factor, always equal to 0
 * \param dxDesc         Tensor descriptor for tensor dx
 * \param dx             Weights delta tensor dx
 * \param workSpace      Pointer to user allocated workspace
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenPoolingBackward(
        miopenHandle_t                      handle,
        const miopenPoolingDescriptor_t     poolDesc,
        const void                          *alpha,
        const miopenTensorDescriptor_t      yDesc,
        const void                          *y,
        const miopenTensorDescriptor_t      dyDesc,
        const void                          *dy,
        const miopenTensorDescriptor_t      xDesc,
        const void                          *x,
        const void                          *beta,
        const miopenTensorDescriptor_t      dxDesc,
        void                                *dx,
        const void                          *workSpace);


/*! \brief Destroys the pooling descriptor object
 * 
 * \param poolDesc Pooling tensor descriptor type
 * \return           miopenStatus_t
*/ 
MIOPEN_EXPORT miopenStatus_t miopenDestroyPoolingDescriptor(miopenPoolingDescriptor_t poolDesc);





// LRN APIs
/*! \brief Creates a local response normalization (LRN) layer descriptor
 * 
 * \param lrnDesc    Pointer to a local response normalization layer descriptor type
 * \return           miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenCreateLRNDescriptor(miopenLRNDescriptor_t *lrnDesc);

/*! \brief Sets a LRN layer descriptor details 
 * 
 * Sets all of the descriptor details for the LRN layer
 * Normalization windows are calculated using \f$ [center-back, center+ahead] \f$, and \f$ back = floor( (lrnN-1)/2 ) \f$, \f$ ahead = lrnN-back-1\f$.
 * 
 * \param lrnDesc      Pointer to a LRN layer descriptor
 * \param mode         LRN mode enum
 * \param lrnN         Number of normalization window elements
 * \param lrnAlpha     Scaling factor
 * \param lrnBeta      Shift factor
 * \param lrnK         K factor
 * \return             miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenSetLRNDescriptor(
    const miopenLRNDescriptor_t             lrnDesc,
    miopenLRNMode_t                         mode,
    unsigned int                            lrnN,
    double                                  lrnAlpha,
    double                                  lrnBeta,
    double                                  lrnK);


/*! \brief Gets a LRN layer descriptor details 
 * 
 * Retrieve the LRN descriptor details
 * 
 * \param lrnDesc      Pointer to a LRN layer descriptor
 * \param mode         LRN mode enum
 * \param lrnN         Number of normalization window elements
 * \param lrnAlpha     Scaling factor
 * \param lrnBeta      Shift factor
 * \param lrnK         K factor
 * \return             miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenGetLRNDescriptor(
        const miopenLRNDescriptor_t         lrnDesc,
        miopenLRNMode_t                     *mode,
        unsigned int                        *lrnN,
        double                              *lrnAlpha,
        double                              *lrnBeta,
        double                              *lrnK);



/*! \brief Determine the workspace requirements.
 * 
 * This function determines the GPU memory allocation required to execute the LRN layer based on the LRN descriptor
 * 
 * \param yDesc           Pointer to a LRN layer descriptor
 * \param workSpaceSize   Output variable for workspace size
 * \return                miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenLRNGetWorkSpaceSize(
        const miopenTensorDescriptor_t      yDesc,
        size_t                              *workSpaceSize);


/*! \brief Execute a LRN forward layer
 * 
 * \param handle         MIOpen handle
 * \param lrnDesc       Descriptor for LRN layer
 * \param alpha          Scaling factor, always equal to 1
 * \param xDesc          Tensor descriptor for data input tensor x
 * \param x              Data tensor x
 * \param beta           Shift factor, always equal to 0
 * \param yDesc          Tensor descriptor for output data tensor y
 * \param y              Data tensor y
 * \param do_backward    Boolean to toggle save data in workspace for backwards pass
 * \param workSpace      Pointer user allocated memory
 * \param workSpaceSize  Size in bytes of the memory needed 
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenLRNForward(
        miopenHandle_t                      handle,
        const miopenLRNDescriptor_t         lrnDesc,
        const void                          *alpha,
        const miopenTensorDescriptor_t      xDesc,
        const void                          *x,
        const void                          *beta,
        const miopenTensorDescriptor_t      yDesc,
        void                                *y,
        bool                                do_backward,
        void                                *workSpace);


/*! \brief Execute a LRN backward layer
 * 
 * \param handle         MIOpen handle
 * \param lrnDesc       Descriptor for LRN layer
 * \param alpha          Scaling factor, always equal to 1
 * \param yDesc          Tensor descriptor for data input tensor y
 * \param y              Data tensor y
 * \param dyDesc         Tensor descriptor for data input tensor dy
 * \param dy             Data delta tensor dy
 * \param xDesc          Tensor descriptor for input data tensor x
 * \param x              Data tensor x
 * \param beta           Shift factor, always equal to 0
 * \param dxDesc         Tensor descriptor for output data tensor dx
 * \param dx             Data delta tensor x
 * \param workSpace      Pointer user allocated memory
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenLRNBackward(
        miopenHandle_t                      handle,
        const miopenLRNDescriptor_t         lrnDesc,
        const void                          *alpha,
        const miopenTensorDescriptor_t      yDesc,
        const void                          *y,
        const miopenTensorDescriptor_t      dyDesc,
        const void                          *dy,
        const miopenTensorDescriptor_t      xDesc,
        const void                          *x,
        const void                          *beta,
        const miopenTensorDescriptor_t      dxDesc,
        void                                *dx,
        const void                          *workSpace);


/*! \brief Destroys the LRN descriptor object
 * 
 * \param lrnDesc   LRN tensor descriptor type
 * \return          miopenStatus_t
*/ 
MIOPEN_EXPORT miopenStatus_t miopenDestroyLRNDescriptor(miopenLRNDescriptor_t lrnDesc);




// Batch-Normalization APIs

/*! \brief Derive tensor for gamma and beta from input tensor descriptor
 * 
 * This function takes the input tensor descriptor and outputs a derived tensor for the normalization scale (gamma) and shift (beta) tensors. 
 * For an input tensor NCHW the and spatial mode, the output derived tensor is 1C11, while for per-activation the derived tensor is 1CHW.
 * /*! \brief Destroys the LRN descriptor object
 * 
 * \param derivedBnDesc   Output derived tensor descriptor
 * \param xDesc           Input tensor descriptor
 * \param bn_mode         Batch Normalization mode
 * \return                miopenStatus_t
*/ 
MIOPEN_EXPORT miopenStatus_t miopenDeriveBNTensorDescriptor(
        miopenTensorDescriptor_t            derivedBnDesc,
        const miopenTensorDescriptor_t      xDesc,
        miopenBatchNormMode_t               bn_mode);

/*! \brief Execute forward training layer for batch normalization
 * 
 * Batch normalization pass for forward training pass. 
 * Takes in batch normalization mode bn_mode and input tensor x, output tensor y, bnBias and bnScale with their descriptor. 
 * If either resultSaveMean, or resultSaveInvVariance are null pointers then the values for the mean and inverse variance will not be used. 
 * Likewise, if either resultRunningMean, or resultRunningVariance are null pointers then the values for the running mean and variance will not be saved.
 * Running averages and variances are scaled using an exponential averaging factor: \f$ \mu_{old} = \mu_{new}*factor + \mu_{old}*(1-factor) \f$ where \f$factor=1/(1+iteration)\f$
 * 
 * \param handle                    MIOpen handle
 * \param bn_mode                   Batch normalization mode (spatial/ per-activation)
 * \param alpha                     Scaling factor, always equal to 1
 * \param beta                      Shift factor, always equal to 0
 * \param xDesc                     Tensor descriptor for data input tensor x
 * \param x                         Data tensor x
 * \param yDesc                     Tensor descriptor for output data tensor y
 * \param y                         Data tensor y
 * \param bnScaleBiasMeanVarDesc    Tensor descriptor for BN scaling, shifting, saved variance and mean
 * \param bnScale                   Batch norm scaling (gamma) tensor
 * \param bnBias                    Batch norm bias (beta) tensor
 * \param expAvgFactor              Exponential averaging factor
 * \param resultRunningMean         Running average saved for inference
 * \param resultRunningVariance     Running variance saved for inference
 * \param epsilon                   Value to stablize inverse variance calculation
 * \param resultSavedMean           Saved mini-batch mean for backwards pass
 * \param resultSavedInvVariance    Saved mini-bathc inverse variance for backwards pass
 * \return                          miopenStatus_t 
*/ 
MIOPEN_EXPORT miopenStatus_t miopenBatchNormalizationForwardTraining(
        miopenHandle_t                      handle,
        miopenBatchNormMode_t               bn_mode,
        void                                *alpha,
        void                                *beta,
        const miopenTensorDescriptor_t      xDesc,
        const void                          *x,
        const miopenTensorDescriptor_t      yDesc,
        void                                *y,
        const miopenTensorDescriptor_t      bnScaleBiasMeanVarDesc,
        void                                *bnScale,
        void                                *bnBias,
        double                              expAvgFactor,
        void                                *resultRunningMean,
        void                                *resultRunningVariance,
        double                              epsilon,
        void                                *resultSaveMean,
        void                                *resultSaveInvVariance);


/*! \brief Execute forward inference layer for batch normalization
 * 
 * Batch normalization pass for forward inference pass. 
 * Takes in batch normalization mode bn_mode and input tensor x, output tensor y, bnBias and bnScale with their descriptor. 
 * If either estimatedMEan, or estimatedVariance are null pointers then the values for the mean and variance will not be used. 
 * 
 * \param handle                    MIOpen handle
 * \param bn_mode                   Batch normalization mode (spatial/ per-activation)
 * \param alpha                     Scaling factor, always equal to 1
 * \param beta                      Shift factor, always equal to 0
 * \param xDesc                     Tensor descriptor for data input tensor x
 * \param x                         Data tensor x
 * \param yDesc                     Tensor descriptor for output data tensor y
 * \param y                         Data tensor y
 * \param bnScaleBiasMeanVarDesc    Tensor descriptor for BN scaling, shifting, saved variance and mean
 * \param bnScale                   Batch norm scaling (gamma) tensor
 * \param bnBias                    Batch norm bias (beta) tensor
 * \param estimatedMean             Running average saved during forward training
 * \param estimatedVariance         Running variance saved during forward training
 * \param epsilon                   Value to stablize inverse variance calculation
 * \return                          miopenStatus_t 
*/ 
MIOPEN_EXPORT miopenStatus_t miopenBatchNormalizationForwardInference(
        miopenHandle_t                      handle,
        miopenBatchNormMode_t               bn_mode,
        void                                *alpha,
        void                                *beta,
        const miopenTensorDescriptor_t      xDesc,
        const void                          *x,
        const miopenTensorDescriptor_t      yDesc,
        void                                *y,
        const miopenTensorDescriptor_t      bnScaleBiasMeanVarDesc,
        void                                *bnScale,
        void                                *bnBias,
        void                                *estimatedMean,
        void                                *estimatedVariance,
        double                              epsilon);


/*! \brief Execute backwards propagation layer for batch normalization
 * 
 * Batch normalization pass for backwards propagation training pass. 
 * The method for backwards propagation batch normalization. 
 * Takes in batch normalization mode bn_mode and input tensor data x, input activation tensor dy, output tensor dx, the learned tensors resultBNBiasDiff and resultBNScaleDiff with their descriptor. 
 * If BOTH savedMean, and savedVariance are not null pointers then the method will use the saved mean and variance calculated by the forward training phase.
 * 
 * \param handle                    MIOpen handle
 * \param bn_mode                   Batch normalization mode (spatial/ per-activation)
 * \param alpha                     Scaling factor, always equal to 1
 * \param beta                      Shift factor, always equal to 0
 * \param xDesc                     Tensor descriptor for data input tensor x
 * \param x                         Data tensor x
 * \param yDesc                     Tensor descriptor for output data tensor y
 * \param y                         Data tensor y
 * \param bnScaleBiasMeanVarDesc    Tensor descriptor for BN scaling, shifting, saved variance and mean
 * \param bnScale                   Batch norm scaling (gamma) tensor
 * \param bnBias                    Batch norm bias (beta) tensor
 * \param expAvgFactor              Exponential averaging factor
 * \param resultRunningMean         Running average saved for inference
 * \param resultRunningVariance     Running variance saved for inference
 * \param epsilon                   Value to stablize inverse variance calculation
 * \param resultSavedMean           Saved mini-batch mean for backwards pass
 * \param resultSavedInvVariance    Saved mini-bathc inverse variance for backwards pass
 * \return                          miopenStatus_t 
*/ 
MIOPEN_EXPORT miopenStatus_t miopenBatchNormalizationBackward(
        miopenHandle_t                      handle,
        miopenBatchNormMode_t               bn_mode,
        const void                          *alphaDataDiff,
        const void                          *betaDataDiff,
        const void                          *alphaParamDiff,
        const void                          *betaParamDiff,
        const miopenTensorDescriptor_t      xDesc,
        const void                          *x,
        const miopenTensorDescriptor_t      dyDesc,
        const void                          *dy,
        const miopenTensorDescriptor_t      dxDesc,
        void                                *dx,
        const miopenTensorDescriptor_t      bnScaleBiasDiffDesc,
        const void                          *bnScale,
        void                                *resultBnScaleDiff,
        void                                *resultBnBiasDiff,
        double                              epsilon,
        const void                          *savedMean,
        const void                          *savedInvVariance);




// Activation APIs


/*! \brief Creates the Activation descriptor object
 * 
 * \param activDesc Pointer to an activation tensor descriptor type
 * \return          miopenStatus_t
*/ 
MIOPEN_EXPORT miopenStatus_t miopenCreateActivationDescriptor(miopenActivationDescriptor_t *activDesc);


/*! \brief Sets the activation layer descriptor details 
 * 
 * Sets all of the descriptor details for the activation layer
 * 
 * \param activDesc    Pointer to a activation layer descriptor
 * \param mode         Activation mode enum
 * \param activAlpha   Alpha value for some activation modes
 * \param activeBeta   Beta value for some activation modes
 * \param activPower   Power exponent value for some activation modes
 * \return             miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenSetActivationDescriptor(
    const miopenActivationDescriptor_t      activDesc,
    miopenActivationMode_t                  mode,
    double                                  activAlpha,
    double                                  activBeta,
    double                                  activPower);

/*! \brief Gets the activation layer descriptor details 
 * 
 * Retrieves all of the descriptor details for the activation layer
 * 
 * \param activDesc    Pointer to a activation layer descriptor
 * \param mode         Activation mode enum
 * \param activAlpha   Alpha value for some activation modes
 * \param activeBeta   Beta value for some activation modes
 * \param activPower   Power exponent value for some activation modes
 * \return             miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenGetActivationDescriptor(
    const miopenActivationDescriptor_t      activDesc,
    miopenActivationMode_t                  *mode,
    double                                  *activAlpha,
    double                                  *activBeta,
    double                                  *activPower);

/*! \brief Execute an activation forward layer
 * 
 * \param handle         MIOpen handle
 * \param poolDesc       Descriptor for LRN layer
 * \param alpha          Scaling factor, always equal to 1
 * \param xDesc          Tensor descriptor for data input tensor x
 * \param x              Data tensor x
 * \param beta           Shift factor, always equal to 0
 * \param yDesc          Tensor descriptor for output data tensor y
 * \param y              Data tensor y
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenActivationForward(
    miopenHandle_t                          handle,
    const miopenActivationDescriptor_t      activDesc,
    const void                              *alpha,
    const miopenTensorDescriptor_t          xDesc,
    const void                              *x,
    const void                              *beta,
    const miopenTensorDescriptor_t          yDesc,
    void                                    *y);

/*! \brief Execute a activation backwards layer
 * 
 * \param handle         MIOpen handle
 * \param activDesc      Descriptor for activation layer
 * \param alpha          Scaling factor, always equal to 1
 * \param yDesc          Tensor descriptor for input data tensor y
 * \param y              Data tensor y
 * \param dyDesc         Tensor descriptor for input data tensor dy
 * \param dy             Data delta tensor dy
 * \param xDesc          Tensor descriptor for data input tensor x
 * \param x              Data tensor x
 * \param beta           Shift factor, always equal to 0
 * \param dxDesc         Tensor descriptor for data output tensor dx
 * \param dx             Output data delta tensor dx
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenActivationBackward(
    miopenHandle_t                          handle,
    const miopenActivationDescriptor_t      activDesc,
    const void                              *alpha,
    const miopenTensorDescriptor_t          yDesc,
    const void                              *y,
    const miopenTensorDescriptor_t          dyDesc,
    const void                              *dy,
    const miopenTensorDescriptor_t          xDesc,
    const void                              *x,
    const void                              *beta,
    const miopenTensorDescriptor_t          dxDesc,
    void                                    *dx);


/*! \brief Destroys the activation descriptor object
 * 
 * \param activDesc   Activation tensor descriptor type
 * \return            miopenStatus_t
*/ 
MIOPEN_EXPORT miopenStatus_t miopenDestroyActivationDescriptor(miopenActivationDescriptor_t activDesc);




// Softmax APIs


/*! \brief Execute a softmax forward layer
 * 
 * MIOpen does not support Softmax modes. MIOpen implements the SOFTMAX_MODE_CHANNEL flavor.
 * 
 * \param handle         MIOpen handle
 * \param alpha          Scaling factor, always equal to 1
 * \param xDesc          Tensor descriptor for data input tensor x
 * \param x              Data tensor x
 * \param beta           Shift factor, always equal to 0
 * \param yDesc          Tensor descriptor for output data tensor y
 * \param y              Data tensor y
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenSoftmaxForward(
    miopenHandle_t                          handle,
    const void                              *alpha,
    const miopenTensorDescriptor_t          xDesc,
    const void                              *x,
    const void                              *beta,
    const miopenTensorDescriptor_t          yDesc,
    void                                    *y);


/*! \brief Execute a softmax backwards layer
 * 
 * MIOpen does not support Softmax modes. MIOpen implements the SOFTMAX_MODE_CHANNEL flavor.
 * 
 * \param handle         MIOpen handle
 * \param alpha          Scaling factor, always equal to 1
 * \param yDesc          Tensor descriptor for input data tensor y
 * \param y              Data tensor y
 * \param dyDesc         Tensor descriptor for input data tensor dy
 * \param dy             Data delta tensor dy
 * \param beta           Shift factor, always equal to 0
 * \param dxDesc         Tensor descriptor for data output tensor dx
 * \param dx             Output data delta tensor dx
 * \return               miopenStatus_t 
 */
MIOPEN_EXPORT miopenStatus_t miopenSoftmaxBackward(
    miopenHandle_t                          handle,
    const void                              *alpha,
    const miopenTensorDescriptor_t          yDesc,
    const void                              *y,
    const miopenTensorDescriptor_t          dyDesc,
    const void                              *dy,
    const void                              *beta,
    const miopenTensorDescriptor_t          dxDesc,
    void                                    *dx);

// GEMM API
/*! \brief Interface for GEMM
 * 
 * Executes \f$C = \alpha*op(A)*op(B) + \beta*C \f$with transposed options (op()) on A and B, but not Hermitian-Transpose. 
 * Data can be represented in column major format.
 * 
 * 
 * \param handle         MIOpen handle
 * \param isDataColMajor Indicate column major mode
 * \param transA         Transpose matrix A
 * \param transB         Transpose matrix B
 * \param M              Number of rows in matrix op(A) and C
 * \param N              Number of columns in matrix op(B) and C
 * \param K              Number of columns in matrix op(A) and C
 * \param alpha          Scaling factor, always equal to 1
 * \param A              Pointer to matrix A
 * \param lda            leading dimension of A depending on isDataColMajor and transA
 * \param B              Pointer to matrix B
 * \param ldb            leading dimension of B depending on isDataColMajor and transB
 * \param beta           Shift factor, always equal to 0
 * \param C              Pointer to matrix C
 * \param ldc            leading dimension of C depending on isDataColMajor
 * \return               miopenStatus_t 
 */ 
MIOPEN_EXPORT miopenStatus_t miopenGemm(
        miopenHandle_t                  handle,
        bool                            isDataColMajor,
        bool                            transA, 
        bool                            transB, 
        int                             M, 
        int                             N, 
        int                             K, 
        const void                      *alpha, 
        const void                      *A, 
        int                             lda, 
        const void                      *B, 
        int                             ldb, 
        const void                      *beta, 
        void                            *C, 
        int                             ldc);

#ifdef __cplusplus
}
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // MIOPEN_GUARD_MIOPEN_H_