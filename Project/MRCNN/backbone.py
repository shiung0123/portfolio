import math
import numpy as np
import tensorflow as tf

from keras import layers
from keras.layers import ZeroPadding2D
from keras.layers import Conv2D
from keras.layers import BatchNormalization
from keras.layers import Activation
from keras.layers import MaxPooling2D
from keras.layers import SeparableConv2D
from keras.layers import AveragePooling2D
from keras.layers import Concatenate
from keras.layers import Lambda
from keras import backend as K
from mrcnn.model import PyramidROIAlign
from keras.layers import TimeDistributed
from keras.layers import Conv2DTranspose

# BN default axis = -1
#     1: first channel [batch, channel, height, weith]
#     3, -1: last channel [batch, height, weith, channel]
# BN default training = None
#     None: Train BN layers. This is the normal mode
#     False: Freeze BN layers. Good when using a small batch size
#     True: (don't use). Set layer in training mode even when predicting
# Conv default use_bias = True
#     False: Reduce model burden. Good when using a small/big model
#     True: Increase model diversity. Good when using a middle model

############################################################
#  ResNet50
############################################################

"""
from keras import layers
from keras.layers import ZeroPadding2D
from keras.layers import Conv2D
from keras.layers import BatchNormalization
from keras.layers import Activation
from keras.layers import MaxPooling2D
"""

def identity_block(input_tensor, kernel_size, filters, stage, block, train_bn=None):
    """The identity block is the block that has no conv layer at shortcut.

    # Arguments
        input_tensor: input tensor
        kernel_size: defualt 3, the kernel size of middle conv layer at main path
        filters: list of integers, the filterss of 3 conv layer at main path
        stage: integer, current stage label, used for generating layer names
        block: 'a','b'..., current block label, used for generating layer names

    # Returns
        Output tensor for the block.
    """
    filters1, filters2, filters3 = filters
    bn_axis = -1
    conv_name_base = 'res' + str(stage) + block + '_branch'
    bn_name_base = 'bn' + str(stage) + block + '_branch'

    x = Conv2D(filters1, (1, 1), name=conv_name_base + '2a')(input_tensor)
    x = BatchNormalization(axis=bn_axis, name=bn_name_base + '2a')(x, training=train_bn)
    x = Activation('relu')(x)

    x = Conv2D(filters2, kernel_size,
               padding='same', name=conv_name_base + '2b')(x)
    x = BatchNormalization(axis=bn_axis, name=bn_name_base + '2b')(x, training=train_bn)
    x = Activation('relu')(x)

    x = Conv2D(filters3, (1, 1), name=conv_name_base + '2c')(x)
    x = BatchNormalization(axis=bn_axis, name=bn_name_base + '2c')(x, training=train_bn)

    x = layers.add([x, input_tensor])
    x = Activation('relu')(x)
    return x

def conv_block(input_tensor, kernel_size, filters, stage, block, strides=(2, 2), train_bn=None):
    """conv_block is the block that has a conv layer at shortcut

    # Arguments
        input_tensor: input tensor
        kernel_size: defualt 3, the kernel size of middle conv layer at main path
        filters: list of integers, the filterss of 3 conv layer at main path
        stage: integer, current stage label, used for generating layer names
        block: 'a','b'..., current block label, used for generating layer names

    # Returns
        Output tensor for the block.

    Note that from stage 3, the first conv layer at main path is with strides=(2,2)
    And the shortcut should have strides=(2,2) as well
    """
    filters1, filters2, filters3 = filters
    bn_axis = -1
    conv_name_base = 'res' + str(stage) + block + '_branch'
    bn_name_base = 'bn' + str(stage) + block + '_branch'

    x = Conv2D(filters1, (1, 1), strides=strides,
               name=conv_name_base + '2a')(input_tensor)
    x = BatchNormalization(axis=bn_axis, name=bn_name_base + '2a')(x, training=train_bn)
    x = Activation('relu')(x)

    x = Conv2D(filters2, kernel_size, padding='same',
               name=conv_name_base + '2b')(x)
    x = BatchNormalization(axis=bn_axis, name=bn_name_base + '2b')(x, training=train_bn)
    x = Activation('relu')(x)

    x = Conv2D(filters3, (1, 1), name=conv_name_base + '2c')(x)
    x = BatchNormalization(axis=bn_axis, name=bn_name_base + '2c')(x, training=train_bn)

    shortcut = Conv2D(filters3, (1, 1), strides=strides,
                      name=conv_name_base + '1')(input_tensor)
    shortcut = BatchNormalization(axis=bn_axis, name=bn_name_base + '1')(shortcut, training=train_bn)

    x = layers.add([x, shortcut])
    x = Activation('relu')(x)
    return x

def ResNet50(self, input_image, stage5=True, train_bn=None):

    bn_axis = -1

    # get strides used
    # test = np.ones([10, 512, 512, 3], dtype='float32')
    # input_image = tf.Variable(test) # Use variable 

    x = ZeroPadding2D((3, 3))(input_image)
    x = Conv2D(64, (7, 7), strides=(2, 2), name='conv1')(x)
    x = BatchNormalization(axis=bn_axis, name='bn_conv1')(x, training=train_bn)
    x = Activation('relu')(x)
    C1 = x = MaxPooling2D((3, 3), strides=(2, 2), padding="same")(x)

    x = conv_block(x, 3, [64, 64, 256], stage=2, block='a', strides=(1, 1), train_bn=train_bn)
    x = identity_block(x, 3, [64, 64, 256], stage=2, block='b', train_bn=train_bn)
    C2 = x = identity_block(x, 3, [64, 64, 256], stage=2, block='c', train_bn=train_bn)

    x = conv_block(x, 3, [128, 128, 512], stage=3, block='a', train_bn=train_bn)
    x = identity_block(x, 3, [128, 128, 512], stage=3, block='b', train_bn=train_bn)
    x = identity_block(x, 3, [128, 128, 512], stage=3, block='c', train_bn=train_bn)
    C3 = x = identity_block(x, 3, [128, 128, 512], stage=3, block='d', train_bn=train_bn)

    x = conv_block(x, 3, [256, 256, 1024], stage=4, block='a', train_bn=train_bn)
    x = identity_block(x, 3, [256, 256, 1024], stage=4, block='b', train_bn=train_bn)
    x = identity_block(x, 3, [256, 256, 1024], stage=4, block='c', train_bn=train_bn)
    x = identity_block(x, 3, [256, 256, 1024], stage=4, block='d', train_bn=train_bn)
    x = identity_block(x, 3, [256, 256, 1024], stage=4, block='e', train_bn=train_bn)
    C4 = x = identity_block(x, 3, [256, 256, 1024], stage=4, block='f', train_bn=train_bn)

    x = conv_block(x, 3, [512, 512, 2048], stage=5, block='a', train_bn=train_bn)
    x = identity_block(x, 3, [512, 512, 2048], stage=5, block='b', train_bn=train_bn)
    C5 = x = identity_block(x, 3, [512, 512, 2048], stage=5, block='c', train_bn=train_bn)

    return [C1, C2, C3, C4, C5]

def ResNet50_shape(self, image_shape):
    return np.array(
        [[int(math.ceil(image_shape[0] / stride)),
            int(math.ceil(image_shape[1] / stride))]
            for stride in [4, 8, 16, 32, 64]])

def ResNet50_strides():
    strides = [4, 8, 16, 32, 64]
    return strides

def ResNet50_fpn_mask(self, rois, feature_maps, image_meta, pool_size, num_classes, train_bn=None):
    """Builds the computation graph of the mask head of Feature Pyramid Network.
    rois: [batch, num_rois, (y1, x1, y2, x2)] Proposal boxes in normalized
          coordinates.
    feature_maps: List of feature maps from diffent layers of the pyramid,
                  [P2, P3, P4, P5]. Each has a different resolution.
    image_meta: [batch, (meta data)] Image details. See compose_image_meta()
    pool_size: The width of the square feature map generated from ROI Pooling.
    num_classes: number of classes, which determines the depth of the results
    train_bn: Boolean. Train or freeze Batch Norm layres
    Returns: Masks [batch, roi_count, height, width, num_classes]
    """

    bn_axis = -1

    # ROI Pooling
    # Shape: [batch, boxes, pool_height, pool_width, channels]
    x = PyramidROIAlign([pool_size, pool_size],
                        name="roi_align_mask")([rois, image_meta] + feature_maps)

    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv1")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn1')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv2")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn2')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv3")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn3')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv4")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn4')(x, training=train_bn)
    x = Activation('relu')(x)

    x = TimeDistributed(Conv2DTranspose(256, (2, 2), strides=2, activation="relu"),
                           name="mrcnn_mask_deconv")(x)
    x = TimeDistributed(Conv2D(num_classes, (1, 1), strides=1, activation="sigmoid"),
                           name="mrcnn_mask")(x)
    return x

def ResNet50_layer_reg():
    reg = {
        # all layers but the backbone
        "heads": r"(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # From a specific Resnet stage and up
        "3+": r"(res3.*)|(bn3.*)|(res4.*)|(bn4.*)|(res5.*)|(bn5.*)|(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "4+": r"(res4.*)|(bn4.*)|(res5.*)|(bn5.*)|(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "5+": r"(res5.*)|(bn5.*)|(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # All layers
        "all": ".*",
    }
    return reg

############################################################
#  Xception
############################################################

"""
from keras import layers
from keras.layers import Conv2D
from keras.layers import BatchNormalization
from keras.layers import Activation
from keras.layers import MaxPooling2D
from keras.layers import SeparableConv2D
"""

def Xception(self, input_image, stage5=True, train_bn=None):

    # get strides used
    # test = np.ones([10, 512, 512, 3], dtype='float32')
    # input_image = tf.Variable(test) # Use variable

    x = Conv2D(32, (3, 3), strides=(2, 2), padding='same', use_bias=False, name='block1_conv1')(input_image)
    x = BatchNormalization(name='block1_conv1_bn')(x, training=train_bn)
    x = Activation('relu', name='block1_conv1_act')(x)
    x = Conv2D(64, (3, 3), padding='same', use_bias=False, name='block1_conv2')(x)
    x = BatchNormalization(name='block1_conv2_bn')(x, training=train_bn)
    C1 = x = Activation('relu', name='block1_conv2_act')(x)

    residual = Conv2D(128, (1, 1), strides=(2, 2),
                      padding='same', use_bias=False)(x)
    residual = BatchNormalization()(residual, training=train_bn)

    x = SeparableConv2D(128, (3, 3), padding='same', use_bias=False, name='block2_sepconv1')(x)
    x = BatchNormalization(name='block2_sepconv1_bn')(x, training=train_bn)
    x = Activation('relu', name='block2_sepconv2_act')(x)
    x = SeparableConv2D(128, (3, 3), padding='same', use_bias=False, name='block2_sepconv2')(x)
    x = BatchNormalization(name='block2_sepconv2_bn')(x, training=train_bn)

    x = MaxPooling2D((3, 3), strides=(2, 2), padding='same', name='block2_pool')(x)
    C2 = x = layers.add([x, residual])

    residual = Conv2D(256, (1, 1), strides=(2, 2),
                      padding='same', use_bias=False)(x)
    residual = BatchNormalization()(residual, training=train_bn)

    x = Activation('relu', name='block3_sepconv1_act')(x)
    x = SeparableConv2D(256, (3, 3), padding='same', use_bias=False, name='block3_sepconv1')(x)
    x = BatchNormalization(name='block3_sepconv1_bn')(x, training=train_bn)
    x = Activation('relu', name='block3_sepconv2_act')(x)
    x = SeparableConv2D(256, (3, 3), padding='same', use_bias=False, name='block3_sepconv2')(x)
    x = BatchNormalization(name='block3_sepconv2_bn')(x, training=train_bn)

    x = MaxPooling2D((3, 3), strides=(2, 2), padding='same', name='block3_pool')(x)
    C3 = x = layers.add([x, residual])

    residual = Conv2D(728, (1, 1), strides=(2, 2),
                      padding='same', use_bias=False)(x)
    residual = BatchNormalization()(residual, training=train_bn)

    x = Activation('relu', name='block4_sepconv1_act')(x)
    x = SeparableConv2D(728, (3, 3), padding='same', use_bias=False, name='block4_sepconv1')(x)
    x = BatchNormalization(name='block4_sepconv1_bn')(x, training=train_bn)
    x = Activation('relu', name='block4_sepconv2_act')(x)
    x = SeparableConv2D(728, (3, 3), padding='same', use_bias=False, name='block4_sepconv2')(x)
    x = BatchNormalization(name='block4_sepconv2_bn')(x, training=train_bn)

    x = MaxPooling2D((3, 3), strides=(2, 2), padding='same', name='block4_pool')(x)
    x = layers.add([x, residual])

    for i in range(8):
        residual = x
        prefix = 'block' + str(i + 5)

        x = Activation('relu', name=prefix + '_sepconv1_act')(x)
        x = SeparableConv2D(728, (3, 3), padding='same', use_bias=False, name=prefix + '_sepconv1')(x)
        x = BatchNormalization(name=prefix + '_sepconv1_bn')(x, training=train_bn)
        x = Activation('relu', name=prefix + '_sepconv2_act')(x)
        x = SeparableConv2D(728, (3, 3), padding='same', use_bias=False, name=prefix + '_sepconv2')(x)
        x = BatchNormalization(name=prefix + '_sepconv2_bn')(x, training=train_bn)
        x = Activation('relu', name=prefix + '_sepconv3_act')(x)
        x = SeparableConv2D(728, (3, 3), padding='same', use_bias=False, name=prefix + '_sepconv3')(x)
        x = BatchNormalization(name=prefix + '_sepconv3_bn')(x, training=train_bn)

        x = layers.add([x, residual])

    C4 = x

    residual = Conv2D(1024, (1, 1), strides=(2, 2),
                      padding='same', use_bias=False)(x)
    residual = BatchNormalization()(residual, training=train_bn)

    x = Activation('relu', name='block13_sepconv1_act')(x)
    x = SeparableConv2D(728, (3, 3), padding='same', use_bias=False, name='block13_sepconv1')(x)
    x = BatchNormalization(name='block13_sepconv1_bn')(x, training=train_bn)
    x = Activation('relu', name='block13_sepconv2_act')(x)
    x = SeparableConv2D(1024, (3, 3), padding='same', use_bias=False, name='block13_sepconv2')(x)
    x = BatchNormalization(name='block13_sepconv2_bn')(x, training=train_bn)

    x = MaxPooling2D((3, 3), strides=(2, 2), padding='same', name='block13_pool')(x)
    x = layers.add([x, residual])

    x = SeparableConv2D(1536, (3, 3), padding='same', use_bias=False, name='block14_sepconv1')(x)
    x = BatchNormalization(name='block14_sepconv1_bn')(x, training=train_bn)
    x = Activation('relu', name='block14_sepconv1_act')(x)

    x = SeparableConv2D(2048, (3, 3), padding='same', use_bias=False, name='block14_sepconv2')(x)
    x = BatchNormalization(name='block14_sepconv2_bn')(x, training=train_bn)
    C5 = x = Activation('relu', name='block14_sepconv2_act')(x)

    return [C1, C2, C3, C4, C5]

def Xception_shape(self, image_shape):
    return np.array(
        [[int(math.ceil(image_shape[0] / stride)),
            int(math.ceil(image_shape[1] / stride))]
            for stride in [4, 8, 16, 32, 64]])

def Xception_strides():
    strides = [4, 8, 16, 32, 64]
    return strides

def Xception_fpn_mask(self, rois, feature_maps, image_meta, pool_size, num_classes, train_bn=None):
    """Builds the computation graph of the mask head of Feature Pyramid Network.
    rois: [batch, num_rois, (y1, x1, y2, x2)] Proposal boxes in normalized
          coordinates.
    feature_maps: List of feature maps from diffent layers of the pyramid,
                  [P2, P3, P4, P5]. Each has a different resolution.
    image_meta: [batch, (meta data)] Image details. See compose_image_meta()
    pool_size: The width of the square feature map generated from ROI Pooling.
    num_classes: number of classes, which determines the depth of the results
    train_bn: Boolean. Train or freeze Batch Norm layres
    Returns: Masks [batch, roi_count, height, width, num_classes]
    """

    bn_axis = -1

    # ROI Pooling
    # Shape: [batch, boxes, pool_height, pool_width, channels]
    x = PyramidROIAlign([pool_size, pool_size],
                        name="roi_align_mask")([rois, image_meta] + feature_maps)

    x = TimeDistributed(SeparableConv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv1")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn1')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(SeparableConv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv2")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn2')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(SeparableConv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv3")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn3')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(SeparableConv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv4")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn4')(x, training=train_bn)
    x = Activation('relu')(x)

    x = TimeDistributed(Conv2DTranspose(256, (2, 2), strides=2, activation="relu"),
                           name="mrcnn_mask_deconv")(x)
    x = TimeDistributed(Conv2D(num_classes, (1, 1), strides=1, activation="sigmoid"),
                           name="mrcnn_mask")(x)
    return x

def Xception_layer_reg():
    reg = {
        # all layers but the backbone
        "heads": r"(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # From a specific Resnet stage and up
        "3+": r"(block[3-9].*)|(block1[0-4].*)|(conv2d_[2-4])|(batch_normalization_[2-4])|" \
               "(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "4+": r"(block[4-9].*)|(block1[0-4].*)|(conv2d_[3-4])|(batch_normalization_[3-4])|" \
               "(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "5+": r"(block1[3-4].*)|(conv2d_4)|(batch_normalization_4)|" \
               "(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # All layers
        "all": ".*",
    }
    return reg

############################################################
#  MobileNet V1
############################################################

"""
from keras import layers
from keras.layers import Conv2D
from keras.layers import BatchNormalization
from keras.layers import Activation
from keras import backend as K
from keras import initializers
from keras import regularizers
from keras import constraints
from keras.engine import InputSpec
from keras.utils import conv_utils
"""

from keras import initializers
from keras import regularizers
from keras import constraints
from keras.engine import InputSpec
from keras.utils import conv_utils

def relu6(x):
    # custom relu
    return K.relu(x, max_value=6)

class DepthwiseConv2D(Conv2D):
    """Depthwise separable 2D convolution.

    Depthwise Separable convolutions consists in performing
    just the first step in a depthwise spatial convolution
    (which acts on each input channel separately).
    The `depth_multiplier` argument controls how many
    output channels are generated per input channel in the depthwise step.

    # Arguments
        kernel_size: An integer or tuple/list of 2 integers, specifying the
            width and height of the 2D convolution window.
            Can be a single integer to specify the same value for
            all spatial dimensions.
        strides: An integer or tuple/list of 2 integers,
            specifying the strides of the convolution along the width and height.
            Can be a single integer to specify the same value for
            all spatial dimensions.
            Specifying any stride value != 1 is incompatible with specifying
            any `dilation_rate` value != 1.
        padding: one of `"valid"` or `"same"` (case-insensitive).
        depth_multiplier: The number of depthwise convolution output channels
            for each input channel.
            The total number of depthwise convolution output
            channels will be equal to `filters_in * depth_multiplier`.
        data_format: A string,
            one of `channels_last` (default) or `channels_first`.
            The ordering of the dimensions in the inputs.
            `channels_last` corresponds to inputs with shape
            `(batch, height, width, channels)` while `channels_first`
            corresponds to inputs with shape
            `(batch, channels, height, width)`.
            It defaults to the `image_data_format` value found in your
            Keras config file at `~/.keras/keras.json`.
            If you never set it, then it will be "channels_last".
        activation: Activation function to use
            (see [activations](keras./activations.md)).
            If you don't specify anything, no activation is applied
            (ie. "linear" activation: `a(x) = x`).
        use_bias: Boolean, whether the layer uses a bias vector.
        depthwise_initializer: Initializer for the depthwise kernel matrix
            (see [initializers](keras./initializers.md)).
        bias_initializer: Initializer for the bias vector
            (see [initializers](keras./initializers.md)).
        depthwise_regularizer: Regularizer function applied to
            the depthwise kernel matrix
            (see [regularizer](keras./regularizers.md)).
        bias_regularizer: Regularizer function applied to the bias vector
            (see [regularizer](keras./regularizers.md)).
        activity_regularizer: Regularizer function applied to
            the output of the layer (its "activation").
            (see [regularizer](keras./regularizers.md)).
        depthwise_constraint: Constraint function applied to
            the depthwise kernel matrix
            (see [constraints](keras./constraints.md)).
        bias_constraint: Constraint function applied to the bias vector
            (see [constraints](keras./constraints.md)).

    # Input shape
        4D tensor with shape:
        `[batch, channels, rows, cols]` if data_format='channels_first'
        or 4D tensor with shape:
        `[batch, rows, cols, channels]` if data_format='channels_last'.

    # Output shape
        4D tensor with shape:
        `[batch, filters, new_rows, new_cols]` if data_format='channels_first'
        or 4D tensor with shape:
        `[batch, new_rows, new_cols, filters]` if data_format='channels_last'.
        `rows` and `cols` values might have changed due to padding.
    """

    def __init__(self,
                 kernel_size,
                 strides=(1, 1),
                 padding='valid',
                 depth_multiplier=1,
                 data_format=None,
                 activation=None,
                 use_bias=True,
                 depthwise_initializer='glorot_uniform',
                 bias_initializer='zeros',
                 depthwise_regularizer=None,
                 bias_regularizer=None,
                 activity_regularizer=None,
                 depthwise_constraint=None,
                 bias_constraint=None,
                 **kwargs):
        super(DepthwiseConv2D, self).__init__(
            filters=None,
            kernel_size=kernel_size,
            strides=strides,
            padding=padding,
            data_format=data_format,
            activation=activation,
            use_bias=use_bias,
            bias_regularizer=bias_regularizer,
            activity_regularizer=activity_regularizer,
            bias_constraint=bias_constraint,
            **kwargs)
        self.depth_multiplier = depth_multiplier
        self.depthwise_initializer = initializers.get(depthwise_initializer)
        self.depthwise_regularizer = regularizers.get(depthwise_regularizer)
        self.depthwise_constraint = constraints.get(depthwise_constraint)
        self.bias_initializer = initializers.get(bias_initializer)

    def build(self, input_shape):
        if len(input_shape) < 4:
            raise ValueError('Inputs to `DepthwiseConv2D` should have rank 4. '
                             'Received input shape:', str(input_shape))
        if self.data_format == 'channels_first':
            channel_axis = 1
        else:
            channel_axis = 3
        if input_shape[channel_axis] is None:
            raise ValueError('The channel dimension of the inputs to '
                             '`DepthwiseConv2D` '
                             'should be defined. Found `None`.')
        input_dim = int(input_shape[channel_axis])
        depthwise_kernel_shape = (self.kernel_size[0],
                                  self.kernel_size[1],
                                  input_dim,
                                  self.depth_multiplier)

        self.depthwise_kernel = self.add_weight(
            shape=depthwise_kernel_shape,
            initializer=self.depthwise_initializer,
            name='depthwise_kernel',
            regularizer=self.depthwise_regularizer,
            constraint=self.depthwise_constraint)

        if self.use_bias:
            self.bias = self.add_weight(shape=(input_dim * self.depth_multiplier,),
                                        initializer=self.bias_initializer,
                                        name='bias',
                                        regularizer=self.bias_regularizer,
                                        constraint=self.bias_constraint)
        else:
            self.bias = None
        # Set input spec.
        self.input_spec = InputSpec(ndim=4, axes={channel_axis: input_dim})
        self.built = True

    def call(self, inputs, training=None):
        outputs = K.depthwise_conv2d(
            inputs,
            self.depthwise_kernel,
            strides=self.strides,
            padding=self.padding,
            dilation_rate=self.dilation_rate,
            data_format=self.data_format)

        if self.bias:
            outputs = K.bias_add(
                outputs,
                self.bias,
                data_format=self.data_format)

        if self.activation is not None:
            return self.activation(outputs)

        return outputs

    def compute_output_shape(self, input_shape):
        if self.data_format == 'channels_first':
            rows = input_shape[2]
            cols = input_shape[3]
            out_filters = input_shape[1] * self.depth_multiplier
        elif self.data_format == 'channels_last':
            rows = input_shape[1]
            cols = input_shape[2]
            out_filters = input_shape[3] * self.depth_multiplier

        rows = conv_utils.conv_output_length(rows, self.kernel_size[0],
                                             self.padding,
                                             self.strides[0])
        cols = conv_utils.conv_output_length(cols, self.kernel_size[1],
                                             self.padding,
                                             self.strides[1])

        if self.data_format == 'channels_first':
            return (input_shape[0], out_filters, rows, cols)
        elif self.data_format == 'channels_last':
            return (input_shape[0], rows, cols, out_filters)

    def get_config(self):
        config = super(DepthwiseConv2D, self).get_config()
        config.pop('filters')
        config.pop('kernel_initializer')
        config.pop('kernel_regularizer')
        config.pop('kernel_constraint')
        config['depth_multiplier'] = self.depth_multiplier
        config['depthwise_initializer'] = initializers.serialize(self.depthwise_initializer)
        config['depthwise_regularizer'] = regularizers.serialize(self.depthwise_regularizer)
        config['depthwise_constraint'] = constraints.serialize(self.depthwise_constraint)
        return config

def _conv_block(inputs, filters, alpha, kernel=(3, 3), strides=(1, 1), train_bn=None):
    """Adds an initial convolution layer (with batch normalization and relu6).

    # Arguments
        inputs: Input tensor of shape `(rows, cols, 3)`
            (with `channels_last` data format) or
            (3, rows, cols) (with `channels_first` data format).
            It should have exactly 3 inputs channels,
            and width and height should be no smaller than 32.
            E.g. `(224, 224, 3)` would be one valid value.
        filters: Integer, the dimensionality of the output space
            (i.e. the number output of filters in the convolution).
        alpha: controls the width of the network.
            - If `alpha` < 1.0, proportionally decreases the number
                of filters in each layer.
            - If `alpha` > 1.0, proportionally increases the number
                of filters in each layer.
            - If `alpha` = 1, default number of filters from the paper
                 are used at each layer.
        kernel: An integer or tuple/list of 2 integers, specifying the
            width and height of the 2D convolution window.
            Can be a single integer to specify the same value for
            all spatial dimensions.
        strides: An integer or tuple/list of 2 integers,
            specifying the strides of the convolution along the width and height.
            Can be a single integer to specify the same value for
            all spatial dimensions.
            Specifying any stride value != 1 is incompatible with specifying
            any `dilation_rate` value != 1.

    # Input shape
        4D tensor with shape:
        `(samples, channels, rows, cols)` if data_format='channels_first'
        or 4D tensor with shape:
        `(samples, rows, cols, channels)` if data_format='channels_last'.

    # Output shape
        4D tensor with shape:
        `(samples, filters, new_rows, new_cols)` if data_format='channels_first'
        or 4D tensor with shape:
        `(samples, new_rows, new_cols, filters)` if data_format='channels_last'.
        `rows` and `cols` values might have changed due to stride.

    # Returns
        Output tensor of block.
    """
    bn_axis = -1
    filters = int(filters * alpha)
    x = Conv2D(filters, kernel,
               padding='same',
               use_bias=False,
               strides=strides,
               name='conv1')(inputs)
    x = BatchNormalization(axis=bn_axis, name='conv1_bn')(x, training=train_bn)
    return Activation(relu6, name='conv1_relu')(x)

def _depthwise_conv_block(inputs, pointwise_conv_filters, alpha,
                          depth_multiplier=1, strides=(1, 1), block_id=1, train_bn=None):
    """Adds a depthwise convolution block.

    A depthwise convolution block consists of a depthwise conv,
    batch normalization, relu6, pointwise convolution,
    batch normalization and relu6 activation.

    # Arguments
        inputs: Input tensor of shape `(rows, cols, channels)`
            (with `channels_last` data format) or
            (channels, rows, cols) (with `channels_first` data format).
        pointwise_conv_filters: Integer, the dimensionality of the output space
            (i.e. the number output of filters in the pointwise convolution).
        alpha: controls the width of the network.
            - If `alpha` < 1.0, proportionally decreases the number
                of filters in each layer.
            - If `alpha` > 1.0, proportionally increases the number
                of filters in each layer.
            - If `alpha` = 1, default number of filters from the paper
                 are used at each layer.
        depth_multiplier: The number of depthwise convolution output channels
            for each input channel.
            The total number of depthwise convolution output
            channels will be equal to `filters_in * depth_multiplier`.
        strides: An integer or tuple/list of 2 integers,
            specifying the strides of the convolution along the width and height.
            Can be a single integer to specify the same value for
            all spatial dimensions.
            Specifying any stride value != 1 is incompatible with specifying
            any `dilation_rate` value != 1.
        block_id: Integer, a unique identification designating the block number.

    # Input shape
        4D tensor with shape:
        `(batch, channels, rows, cols)` if data_format='channels_first'
        or 4D tensor with shape:
        `(batch, rows, cols, channels)` if data_format='channels_last'.

    # Output shape
        4D tensor with shape:
        `(batch, filters, new_rows, new_cols)` if data_format='channels_first'
        or 4D tensor with shape:
        `(batch, new_rows, new_cols, filters)` if data_format='channels_last'.
        `rows` and `cols` values might have changed due to stride.

    # Returns
        Output tensor of block.
    """
    bn_axis = -1
    pointwise_conv_filters = int(pointwise_conv_filters * alpha)

    x = DepthwiseConv2D((3, 3),
                        padding='same',
                        depth_multiplier=depth_multiplier,
                        strides=strides,
                        use_bias=False,
                        name='conv_dw_%d' % block_id)(inputs)
    x = BatchNormalization(axis=bn_axis, name='conv_dw_%d_bn' % block_id)(x, training=train_bn)
    x = Activation(relu6, name='conv_dw_%d_relu' % block_id)(x)

    x = Conv2D(pointwise_conv_filters, (1, 1),
               padding='same',
               use_bias=False,
               strides=(1, 1),
               name='conv_pw_%d' % block_id)(x)
    x = BatchNormalization(axis=bn_axis, name='conv_pw_%d_bn' % block_id)(x, training=train_bn)
    return Activation(relu6, name='conv_pw_%d_relu' % block_id)(x)

def MobileNetV1(self, input_image, stage5=True, train_bn=None):

    alpha = 1.0
    depth_multiplier = 1

    # get strides used
    # test = np.ones([10, 512, 512, 3], dtype='float32')
    # input_image = tf.Variable(test) # Use variable

    x = _conv_block(input_image, 32, alpha, strides=(2, 2), train_bn=train_bn)
    C1 = x = _depthwise_conv_block(x, 64, alpha, depth_multiplier, block_id=1, train_bn=train_bn)

    x = _depthwise_conv_block(x, 128, alpha, depth_multiplier,
                              strides=(2, 2), block_id=2, train_bn=train_bn)
    C2 = x = _depthwise_conv_block(x, 128, alpha, depth_multiplier, block_id=3, train_bn=train_bn)

    x = _depthwise_conv_block(x, 256, alpha, depth_multiplier,
                              strides=(2, 2), block_id=4, train_bn=train_bn)
    C3 = x = _depthwise_conv_block(x, 256, alpha, depth_multiplier, block_id=5, train_bn=train_bn)

    x = _depthwise_conv_block(x, 512, alpha, depth_multiplier,
                              strides=(2, 2), block_id=6, train_bn=train_bn)
    x = _depthwise_conv_block(x, 512, alpha, depth_multiplier, block_id=7, train_bn=train_bn)
    x = _depthwise_conv_block(x, 512, alpha, depth_multiplier, block_id=8, train_bn=train_bn)
    x = _depthwise_conv_block(x, 512, alpha, depth_multiplier, block_id=9, train_bn=train_bn)
    x = _depthwise_conv_block(x, 512, alpha, depth_multiplier, block_id=10, train_bn=train_bn)
    C4 = x = _depthwise_conv_block(x, 512, alpha, depth_multiplier, block_id=11, train_bn=train_bn)

    x = _depthwise_conv_block(x, 1024, alpha, depth_multiplier,
                              strides=(2, 2), block_id=12, train_bn=train_bn)
    C5 = x = _depthwise_conv_block(x, 1024, alpha, depth_multiplier, block_id=13, train_bn=train_bn)

    return [C1, C2, C3, C4, C5]

def MobileNetV1_shape(self, image_shape):
    return np.array(
        [[int(math.ceil(image_shape[0] / stride)),
            int(math.ceil(image_shape[1] / stride))]
            for stride in [4, 8, 16, 32, 64]])

def MobileNetV1_strides():
    strides = [4, 8, 16, 32, 64]
    return strides

def _timedistributed_depthwise_conv_block(inputs, pointwise_conv_filters, strides=(1, 1), block_id=1, train_bn=None):
    """Similiar to the _depthwise_conv_block used in the Backbone,
    but with each layer wrapped in a TimeDistributed layer,
    used to build the computation graph of the mask head of the FPN.
    """

    bn_axis = -1

    # Depthwise
    x = TimeDistributed(DepthwiseConv2D(
                    (3, 3),
                    padding='same',
                    depth_multiplier=1,
                    strides=strides,
                    use_bias=False),
                    name='mrcnn_mask_conv_dw_{}'.format(block_id))(inputs)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),
                    name='mrcnn_mask_conv_dw_{}_bn'.format(block_id))(x, training=train_bn)
    x = Activation(relu6, name='mrcnn_mask_conv_dw_{}_relu'.format(block_id))(x)
    # Pointwise
    x = TimeDistributed(Conv2D(pointwise_conv_filters,
                    (1, 1),
                    padding='same',
                    use_bias=False,
                    strides=(1, 1)),
                    name='mrcnn_mask_conv_pw_{}'.format(block_id))(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),
                    name='mrcnn_mask_conv_pw_{}_bn'.format(block_id))(x, training=train_bn)

    return Activation(relu6, name='mrcnn_mask_conv_pw_{}_relu'.format(block_id))(x)

def MobileNetV1_fpn_mask(self, rois, feature_maps, image_meta, pool_size, num_classes, train_bn=None):
    """Builds the computation graph of the mask head of Feature Pyramid Network.
    rois: [batch, num_rois, (y1, x1, y2, x2)] Proposal boxes in normalized
          coordinates.
    feature_maps: List of feature maps from diffent layers of the pyramid,
                  [P2, P3, P4, P5]. Each has a different resolution.
    image_meta: [batch, (meta data)] Image details. See compose_image_meta()
    pool_size: The width of the square feature map generated from ROI Pooling.
    num_classes: number of classes, which determines the depth of the results
    train_bn: Boolean. Train or freeze Batch Norm layres
    Returns: Masks [batch, roi_count, height, width, num_classes]
    """

    # ROI Pooling
    # Shape: [batch, boxes, pool_height, pool_width, channels]
    x = PyramidROIAlign([pool_size, pool_size],
                        name="roi_align_mask")([rois, image_meta] + feature_maps)

    x = _timedistributed_depthwise_conv_block(x, 256, block_id = 1, train_bn=train_bn)
    x = _timedistributed_depthwise_conv_block(x, 256, block_id = 2, train_bn=train_bn)
    x = _timedistributed_depthwise_conv_block(x, 256, block_id = 3, train_bn=train_bn)
    x = _timedistributed_depthwise_conv_block(x, 256, block_id = 4, train_bn=train_bn)

    x = TimeDistributed(Conv2DTranspose(256, (2, 2), strides=2, activation="relu"),
                           name="mrcnn_mask_deconv")(x)
    x = TimeDistributed(Conv2D(num_classes, (1, 1), strides=1, activation="sigmoid"),
                           name="mrcnn_mask")(x)
    return x

def MobileNetV1_layer_reg():
    reg = {
        # all layers but the backbone
        "heads": r"(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # From a specific Resnet stage and up
        "3+": r"(conv\_(p|d)w\_[4-9].*)|(conv\_(p|d)w\_1[0-3].*)|(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "4+": r"(conv\_(p|d)w\_[6-9].*)|(conv\_(p|d)w\_1[0-3].*)|(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "5+": r"(conv\_(p|d)w\_1[2-3].*)|(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # All layers
        "all": ".*",
    }
    return reg

############################################################
#  Inception V3
############################################################

"""
from keras import layers
from keras.layers import Conv2D
from keras.layers import BatchNormalization
from keras.layers import Activation
from keras.layers import MaxPooling2D
from keras.layers import AveragePooling2D
"""

def conv2d_bn(x, filters, num_row, num_col, padding='same', strides=(1, 1), name=None, train_bn=None):
    """Utility function to apply conv + BN.

    Arguments:
        x: input tensor.
        filters: filters in `Conv2D`.
        num_row: height of the convolution kernel.
        num_col: width of the convolution kernel.
        padding: padding mode in `Conv2D`.
        strides: strides in `Conv2D`.
        name: name of the ops; will become `name + '_conv'`
            for the convolution and `name + '_bn'` for the
            batch norm layer.

    Returns:
        Output tensor after applying `Conv2D` and `BatchNormalization`.
    """
    if name is not None:
        bn_name = name + '_bn'
        conv_name = name + '_conv'
    else:
        bn_name = None
        conv_name = None

    bn_axis = -1

    x = Conv2D(
        filters, (num_row, num_col),
        strides=strides,
        padding=padding,
        use_bias=False,
        name=conv_name)(x)
    x = BatchNormalization(axis=bn_axis, scale=False, name=bn_name)(x, training=train_bn)
    x = Activation('relu', name=name)(x)

    return x

def InceptionV3(self, input_image, stage5=True, train_bn=None):

    channel_axis = -1

    # get strides used
    # test = np.ones([10, 512, 512, 3], dtype='float32')
    # input_image = tf.Variable(test) # Use variable

    x = conv2d_bn(input_image, 32, 3, 3, strides=(2, 2), padding='same', train_bn=train_bn)
    x = conv2d_bn(x, 32, 3, 3, padding='same', train_bn=train_bn)
    x = conv2d_bn(x, 64, 3, 3, train_bn=train_bn)
    x = MaxPooling2D((3, 3), strides=(2, 2), padding='same')(x)

    x = conv2d_bn(x, 80, 1, 1, padding='same', train_bn=train_bn)
    x = conv2d_bn(x, 192, 3, 3, padding='same', train_bn=train_bn)
    C1 = x = MaxPooling2D((3, 3), strides=(2, 2), padding='same')(x)

    # mixed 0, 1, 2: 35 x 35 x 256
    branch1x1 = conv2d_bn(x, 64, 1, 1, train_bn=train_bn)

    branch5x5 = conv2d_bn(x, 48, 1, 1, train_bn=train_bn)
    branch5x5 = conv2d_bn(branch5x5, 64, 5, 5, train_bn=train_bn)

    branch3x3dbl = conv2d_bn(x, 64, 1, 1, train_bn=train_bn)
    branch3x3dbl = conv2d_bn(branch3x3dbl, 96, 3, 3, train_bn=train_bn)
    branch3x3dbl = conv2d_bn(branch3x3dbl, 96, 3, 3, train_bn=train_bn)

    branch_pool = AveragePooling2D((3, 3), strides=(1, 1), padding='same')(x)
    branch_pool = conv2d_bn(branch_pool, 32, 1, 1, train_bn=train_bn)
    x = layers.concatenate(
        [branch1x1, branch5x5, branch3x3dbl, branch_pool],
        axis=channel_axis,
        name='mixed0')

    # mixed 1: 35 x 35 x 256
    branch1x1 = conv2d_bn(x, 64, 1, 1, train_bn=train_bn)

    branch5x5 = conv2d_bn(x, 48, 1, 1, train_bn=train_bn)
    branch5x5 = conv2d_bn(branch5x5, 64, 5, 5, train_bn=train_bn)

    branch3x3dbl = conv2d_bn(x, 64, 1, 1, train_bn=train_bn)
    branch3x3dbl = conv2d_bn(branch3x3dbl, 96, 3, 3, train_bn=train_bn)
    branch3x3dbl = conv2d_bn(branch3x3dbl, 96, 3, 3, train_bn=train_bn)

    branch_pool = AveragePooling2D((3, 3), strides=(1, 1), padding='same')(x)
    branch_pool = conv2d_bn(branch_pool, 64, 1, 1, train_bn=train_bn)
    x = layers.concatenate(
        [branch1x1, branch5x5, branch3x3dbl, branch_pool],
        axis=channel_axis,
        name='mixed1')

    # mixed 2: 35 x 35 x 256
    branch1x1 = conv2d_bn(x, 64, 1, 1, train_bn=train_bn)

    branch5x5 = conv2d_bn(x, 48, 1, 1, train_bn=train_bn)
    branch5x5 = conv2d_bn(branch5x5, 64, 5, 5, train_bn=train_bn)

    branch3x3dbl = conv2d_bn(x, 64, 1, 1, train_bn=train_bn)
    branch3x3dbl = conv2d_bn(branch3x3dbl, 96, 3, 3, train_bn=train_bn)
    branch3x3dbl = conv2d_bn(branch3x3dbl, 96, 3, 3, train_bn=train_bn)

    branch_pool = AveragePooling2D((3, 3), strides=(1, 1), padding='same')(x)
    branch_pool = conv2d_bn(branch_pool, 64, 1, 1, train_bn=train_bn)
    C2 = x = layers.concatenate(
        [branch1x1, branch5x5, branch3x3dbl, branch_pool],
        axis=channel_axis,
        name='mixed2')

    # mixed 3: 17 x 17 x 768
    branch3x3 = conv2d_bn(x, 384, 3, 3, strides=(2, 2), padding='same', train_bn=train_bn)

    branch3x3dbl = conv2d_bn(x, 64, 1, 1, train_bn=train_bn)
    branch3x3dbl = conv2d_bn(branch3x3dbl, 96, 3, 3, train_bn=train_bn)
    branch3x3dbl = conv2d_bn(
        branch3x3dbl, 96, 3, 3, strides=(2, 2), padding='same', train_bn=train_bn)

    branch_pool = MaxPooling2D((3, 3), strides=(2, 2), padding='same')(x)
    x = layers.concatenate(
        [branch3x3, branch3x3dbl, branch_pool], axis=channel_axis, name='mixed3')

    # mixed 4: 17 x 17 x 768
    branch1x1 = conv2d_bn(x, 192, 1, 1, train_bn=train_bn)

    branch7x7 = conv2d_bn(x, 128, 1, 1, train_bn=train_bn)
    branch7x7 = conv2d_bn(branch7x7, 128, 1, 7, train_bn=train_bn)
    branch7x7 = conv2d_bn(branch7x7, 192, 7, 1, train_bn=train_bn)

    branch7x7dbl = conv2d_bn(x, 128, 1, 1, train_bn=train_bn)
    branch7x7dbl = conv2d_bn(branch7x7dbl, 128, 7, 1, train_bn=train_bn)
    branch7x7dbl = conv2d_bn(branch7x7dbl, 128, 1, 7, train_bn=train_bn)
    branch7x7dbl = conv2d_bn(branch7x7dbl, 128, 7, 1, train_bn=train_bn)
    branch7x7dbl = conv2d_bn(branch7x7dbl, 192, 1, 7, train_bn=train_bn)

    branch_pool = AveragePooling2D((3, 3), strides=(1, 1), padding='same')(x)
    branch_pool = conv2d_bn(branch_pool, 192, 1, 1, train_bn=train_bn)
    C3 = x = layers.concatenate(
        [branch1x1, branch7x7, branch7x7dbl, branch_pool],
        axis=channel_axis,
        name='mixed4')

    # mixed 5, 6: 17 x 17 x 768
    for i in range(2):
        branch1x1 = conv2d_bn(x, 192, 1, 1, train_bn=train_bn)

        branch7x7 = conv2d_bn(x, 160, 1, 1, train_bn=train_bn)
        branch7x7 = conv2d_bn(branch7x7, 160, 1, 7, train_bn=train_bn)
        branch7x7 = conv2d_bn(branch7x7, 192, 7, 1, train_bn=train_bn)

        branch7x7dbl = conv2d_bn(x, 160, 1, 1, train_bn=train_bn)
        branch7x7dbl = conv2d_bn(branch7x7dbl, 160, 7, 1, train_bn=train_bn)
        branch7x7dbl = conv2d_bn(branch7x7dbl, 160, 1, 7, train_bn=train_bn)
        branch7x7dbl = conv2d_bn(branch7x7dbl, 160, 7, 1, train_bn=train_bn)
        branch7x7dbl = conv2d_bn(branch7x7dbl, 192, 1, 7, train_bn=train_bn)

        branch_pool = AveragePooling2D(
            (3, 3), strides=(1, 1), padding='same')(x)
        branch_pool = conv2d_bn(branch_pool, 192, 1, 1, train_bn=train_bn)
        x = layers.concatenate(
            [branch1x1, branch7x7, branch7x7dbl, branch_pool],
            axis=channel_axis,
            name='mixed' + str(5 + i))

    # mixed 7: 17 x 17 x 768
    branch1x1 = conv2d_bn(x, 192, 1, 1, train_bn=train_bn)

    branch7x7 = conv2d_bn(x, 192, 1, 1, train_bn=train_bn)
    branch7x7 = conv2d_bn(branch7x7, 192, 1, 7, train_bn=train_bn)
    branch7x7 = conv2d_bn(branch7x7, 192, 7, 1, train_bn=train_bn)

    branch7x7dbl = conv2d_bn(x, 192, 1, 1, train_bn=train_bn)
    branch7x7dbl = conv2d_bn(branch7x7dbl, 192, 7, 1, train_bn=train_bn)
    branch7x7dbl = conv2d_bn(branch7x7dbl, 192, 1, 7, train_bn=train_bn)
    branch7x7dbl = conv2d_bn(branch7x7dbl, 192, 7, 1, train_bn=train_bn)
    branch7x7dbl = conv2d_bn(branch7x7dbl, 192, 1, 7, train_bn=train_bn)

    branch_pool = AveragePooling2D((3, 3), strides=(1, 1), padding='same')(x)
    branch_pool = conv2d_bn(branch_pool, 192, 1, 1, train_bn=train_bn)
    C4 = x = layers.concatenate(
        [branch1x1, branch7x7, branch7x7dbl, branch_pool],
        axis=channel_axis,
        name='mixed7')

    # mixed 8: 8 x 8 x 1280
    branch3x3 = conv2d_bn(x, 192, 1, 1, train_bn=train_bn)
    branch3x3 = conv2d_bn(branch3x3, 320, 3, 3,
                          strides=(2, 2), padding='same', train_bn=train_bn)

    branch7x7x3 = conv2d_bn(x, 192, 1, 1, train_bn=train_bn)
    branch7x7x3 = conv2d_bn(branch7x7x3, 192, 1, 7, train_bn=train_bn)
    branch7x7x3 = conv2d_bn(branch7x7x3, 192, 7, 1, train_bn=train_bn)
    branch7x7x3 = conv2d_bn(
        branch7x7x3, 192, 3, 3, strides=(2, 2), padding='same', train_bn=train_bn)

    branch_pool = MaxPooling2D((3, 3), strides=(2, 2), padding='same')(x)
    x = layers.concatenate(
        [branch3x3, branch7x7x3, branch_pool], axis=channel_axis, name='mixed8')

    # mixed 9: 8 x 8 x 2048
    for i in range(2):
        branch1x1 = conv2d_bn(x, 320, 1, 1, train_bn=train_bn)

        branch3x3 = conv2d_bn(x, 384, 1, 1, train_bn=train_bn)
        branch3x3_1 = conv2d_bn(branch3x3, 384, 1, 3, train_bn=train_bn)
        branch3x3_2 = conv2d_bn(branch3x3, 384, 3, 1, train_bn=train_bn)
        branch3x3 = layers.concatenate(
            [branch3x3_1, branch3x3_2], axis=channel_axis, name='mixed9_' + str(i))

        branch3x3dbl = conv2d_bn(x, 448, 1, 1, train_bn=train_bn)
        branch3x3dbl = conv2d_bn(branch3x3dbl, 384, 3, 3, train_bn=train_bn)
        branch3x3dbl_1 = conv2d_bn(branch3x3dbl, 384, 1, 3, train_bn=train_bn)
        branch3x3dbl_2 = conv2d_bn(branch3x3dbl, 384, 3, 1, train_bn=train_bn)
        branch3x3dbl = layers.concatenate(
            [branch3x3dbl_1, branch3x3dbl_2], axis=channel_axis)

        branch_pool = AveragePooling2D(
            (3, 3), strides=(1, 1), padding='same')(x)
        branch_pool = conv2d_bn(branch_pool, 192, 1, 1, train_bn=train_bn)
        x = layers.concatenate(
            [branch1x1, branch3x3, branch3x3dbl, branch_pool],
            axis=channel_axis,
            name='mixed' + str(9 + i))

    C5 = x

    return [C1, C2, C3, C4, C5]

def InceptionV3_shape(self, image_shape):
    return np.array(
        [[int(math.ceil(image_shape[0] / stride)),
            int(math.ceil(image_shape[1] / stride))]
            for stride in [8, 16, 16, 32, 64]])

def InceptionV3_strides():
    strides = [8, 16, 16, 32, 64]
    return strides

def InceptionV3_fpn_mask(self, rois, feature_maps, image_meta, pool_size, num_classes, train_bn=None):
    """Builds the computation graph of the mask head of Feature Pyramid Network.
    rois: [batch, num_rois, (y1, x1, y2, x2)] Proposal boxes in normalized
          coordinates.
    feature_maps: List of feature maps from diffent layers of the pyramid,
                  [P2, P3, P4, P5]. Each has a different resolution.
    image_meta: [batch, (meta data)] Image details. See compose_image_meta()
    pool_size: The width of the square feature map generated from ROI Pooling.
    num_classes: number of classes, which determines the depth of the results
    train_bn: Boolean. Train or freeze Batch Norm layres
    Returns: Masks [batch, roi_count, height, width, num_classes]
    """

    bn_axis = -1

    # ROI Pooling
    # Shape: [batch, boxes, pool_height, pool_width, channels]
    x = PyramidROIAlign([pool_size, pool_size],
                        name="roi_align_mask")([rois, image_meta] + feature_maps)

    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv1")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn1')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv2")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn2')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv3")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn3')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv4")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn4')(x, training=train_bn)
    x = Activation('relu')(x)

    x = TimeDistributed(Conv2DTranspose(256, (2, 2), strides=2, activation="relu"),
                           name="mrcnn_mask_deconv")(x)
    x = TimeDistributed(Conv2D(num_classes, (1, 1), strides=1, activation="sigmoid"),
                           name="mrcnn_mask")(x)
    return x

def InceptionV3_layer_reg():
    reg = {
        # all layers but the backbone
        "heads": r"(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # From a specific Resnet stage and up
        "3+": r"(batch\_normalization|conv2d)\_(2[7-9]|[3-8][0-9]|9[0-4])|" \
               "(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "4+": r"(batch\_normalization|conv2d)\_(4[1-9]|[5-8][0-9]|9[0-4])|" \
               "(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "5+": r"(batch\_normalization|conv2d)\_(7[1-9]|8[0-9]|9[0-4])|" \
               "(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # All layers
        "all": ".*",
    }
    return reg

############################################################
#  Inception ResNet V2
############################################################

"""
from keras import layers
from keras.layers import Conv2D
from keras.layers import BatchNormalization
from keras.layers import Activation
from keras.layers import MaxPooling2D
from keras.layers import AveragePooling2D
from keras.layers import Concatenate
from keras.layers import Lambda
"""

def InceptionResNetV2_conv2d_bn(x, filters, kernel_size, strides=1, padding='same', activation='relu',
              use_bias=False, name=None, train_bn=None):
    """Utility function to apply conv + BN.

    # Arguments
        x: input tensor.
        filters: filters in `Conv2D`.
        kernel_size: kernel size as in `Conv2D`.
        padding: padding mode in `Conv2D`.
        activation: activation in `Conv2D`.
        strides: strides in `Conv2D`.
        name: name of the ops; will become `name + '_ac'` for the activation
            and `name + '_bn'` for the batch norm layer.

    # Returns
        Output tensor after applying `Conv2D` and `BatchNormalization`.
    """
    x = Conv2D(filters, kernel_size, strides=strides, padding=padding,
               use_bias=use_bias, name=name)(x)

    if not use_bias:
        bn_axis = -1
        bn_name = None if name is None else name + '_bn'
        x = BatchNormalization(axis=bn_axis, scale=False, name=bn_name)(x, training=train_bn)

    if activation is not None:
        ac_name = None if name is None else name + '_ac'
        x = Activation(activation, name=ac_name)(x)

    return x

def inception_resnet_block(x, scale, block_type, block_idx, activation='relu', train_bn=None):
    """Adds a Inception-ResNet block.

    This function builds 3 types of Inception-ResNet blocks mentioned
    in the paper, controlled by the `block_type` argument (which is the
    block name used in the official TF-slim implementation):
        - Inception-ResNet-A: `block_type='block35'`
        - Inception-ResNet-B: `block_type='block17'`
        - Inception-ResNet-C: `block_type='block8'`

    # Arguments
        x: input tensor.
        scale: scaling factor to scale the residuals (i.e., the output of
            passing `x` through an inception module) before adding them
            to the shortcut branch. Let `r` be the output from the residual branch,
            the output of this block will be `x + scale * r`.
        block_type: `'block35'`, `'block17'` or `'block8'`, determines
            the network structure in the residual branch.
        block_idx: an `int` used for generating layer names. The Inception-ResNet blocks
            are repeated many times in this network. We use `block_idx` to identify
            each of the repetitions. For example, the first Inception-ResNet-A block
            will have `block_type='block35', block_idx=0`, ane the layer names will have
            a common prefix `'block35_0'`.
        activation: activation function to use at the end of the block
            (see [activations](keras./activations.md)).
            When `activation=None`, no activation is applied
            (i.e., "linear" activation: `a(x) = x`).

    # Returns
        Output tensor for the block.

    # Raises
        ValueError: if `block_type` is not one of `'block35'`,
            `'block17'` or `'block8'`.
    """
    if block_type == 'block35':
        branch_0 = InceptionResNetV2_conv2d_bn(x, 32, 1, train_bn=train_bn)
        branch_1 = InceptionResNetV2_conv2d_bn(x, 32, 1, train_bn=train_bn)
        branch_1 = InceptionResNetV2_conv2d_bn(branch_1, 32, 3, train_bn=train_bn)
        branch_2 = InceptionResNetV2_conv2d_bn(x, 32, 1, train_bn=train_bn)
        branch_2 = InceptionResNetV2_conv2d_bn(branch_2, 48, 3, train_bn=train_bn)
        branch_2 = InceptionResNetV2_conv2d_bn(branch_2, 64, 3, train_bn=train_bn)
        branches = [branch_0, branch_1, branch_2]
    elif block_type == 'block17':
        branch_0 = InceptionResNetV2_conv2d_bn(x, 192, 1, train_bn=train_bn)
        branch_1 = InceptionResNetV2_conv2d_bn(x, 128, 1, train_bn=train_bn)
        branch_1 = InceptionResNetV2_conv2d_bn(branch_1, 160, [1, 7], train_bn=train_bn)
        branch_1 = InceptionResNetV2_conv2d_bn(branch_1, 192, [7, 1], train_bn=train_bn)
        branches = [branch_0, branch_1]
    elif block_type == 'block8':
        branch_0 = InceptionResNetV2_conv2d_bn(x, 192, 1, train_bn=train_bn)
        branch_1 = InceptionResNetV2_conv2d_bn(x, 192, 1, train_bn=train_bn)
        branch_1 = InceptionResNetV2_conv2d_bn(branch_1, 224, [1, 3], train_bn=train_bn)
        branch_1 = InceptionResNetV2_conv2d_bn(branch_1, 256, [3, 1], train_bn=train_bn)
        branches = [branch_0, branch_1]
    else:
        raise ValueError('Unknown Inception-ResNet block type. '
                         'Expects "block35", "block17" or "block8", '
                         'but got: ' + str(block_type))

    block_name = block_type + '_' + str(block_idx)
    channel_axis = -1
    mixed = Concatenate(axis=channel_axis, name=block_name + '_mixed')(branches)
    up = InceptionResNetV2_conv2d_bn(mixed,
                   K.int_shape(x)[channel_axis],
                   1,
                   activation=None,
                   use_bias=True,
                   name=block_name + '_conv',
                   train_bn=train_bn)

    x = Lambda(lambda inputs, scale: inputs[0] + inputs[1] * scale,
               output_shape=K.int_shape(x)[1:],
               arguments={'scale': scale},
               name=block_name)([x, up])

    if activation is not None:
        x = Activation(activation, name=block_name + '_ac')(x)

    return x

def InceptionResNetV2(self, input_image, stage5=True, train_bn=None):

    channel_axis = -1

    # get strides used
    # test = np.ones([10, 512, 512, 3], dtype='float32')
    # input_image = tf.Variable(test) # Use variable

    # Stem block: 35 x 35 x 192
    x = InceptionResNetV2_conv2d_bn(input_image, 32, 3, strides=2, padding='same', train_bn=train_bn)
    x = InceptionResNetV2_conv2d_bn(x, 32, 3, padding='same', train_bn=train_bn)
    x = InceptionResNetV2_conv2d_bn(x, 64, 3, train_bn=train_bn)
    x = MaxPooling2D(3, strides=2, padding='same')(x)
    x = InceptionResNetV2_conv2d_bn(x, 80, 1, padding='same', train_bn=train_bn)
    x = InceptionResNetV2_conv2d_bn(x, 192, 3, padding='same', train_bn=train_bn)
    C1 = x = MaxPooling2D(3, strides=2, padding='same')(x)

    # Mixed 5b (Inception-A block): 35 x 35 x 320
    branch_0 = InceptionResNetV2_conv2d_bn(x, 96, 1, train_bn=train_bn)
    branch_1 = InceptionResNetV2_conv2d_bn(x, 48, 1, train_bn=train_bn)
    branch_1 = InceptionResNetV2_conv2d_bn(branch_1, 64, 5, train_bn=train_bn)
    branch_2 = InceptionResNetV2_conv2d_bn(x, 64, 1, train_bn=train_bn)
    branch_2 = InceptionResNetV2_conv2d_bn(branch_2, 96, 3, train_bn=train_bn)
    branch_2 = InceptionResNetV2_conv2d_bn(branch_2, 96, 3, train_bn=train_bn)
    branch_pool = AveragePooling2D(3, strides=1, padding='same')(x)
    branch_pool = InceptionResNetV2_conv2d_bn(branch_pool, 64, 1, train_bn=train_bn)
    branches = [branch_0, branch_1, branch_2, branch_pool]
    
    x = Concatenate(axis=channel_axis, name='mixed_5b')(branches)

    # 10x block35 (Inception-ResNet-A block): 35 x 35 x 320
    for block_idx in range(1, 11):
        x = inception_resnet_block(x,
                                   scale=0.17,
                                   block_type='block35',
                                   block_idx=block_idx,
                                   train_bn=train_bn)

    C2 = x

    # Mixed 6a (Reduction-A block): 17 x 17 x 1088
    branch_0 = InceptionResNetV2_conv2d_bn(x, 384, 3, strides=2, padding='same', train_bn=train_bn)
    branch_1 = InceptionResNetV2_conv2d_bn(x, 256, 1, train_bn=train_bn)
    branch_1 = InceptionResNetV2_conv2d_bn(branch_1, 256, 3, train_bn=train_bn)
    branch_1 = InceptionResNetV2_conv2d_bn(branch_1, 384, 3, strides=2, padding='same', train_bn=train_bn)
    branch_pool = MaxPooling2D(3, strides=2, padding='same')(x)
    branches = [branch_0, branch_1, branch_pool]
    x = Concatenate(axis=channel_axis, name='mixed_6a')(branches)

    # 20x block17 (Inception-ResNet-B block): 17 x 17 x 1088
    for block_idx in range(1, 21):
        x = inception_resnet_block(x,
                                   scale=0.1,
                                   block_type='block17',
                                   block_idx=block_idx,
                                   train_bn=train_bn)

    C3 = x

    # Mixed 7a (Reduction-B block): 8 x 8 x 2080
    branch_0 = InceptionResNetV2_conv2d_bn(x, 256, 1, train_bn=train_bn)
    branch_0 = InceptionResNetV2_conv2d_bn(branch_0, 384, 3, strides=2, padding='same', train_bn=train_bn)
    branch_1 = InceptionResNetV2_conv2d_bn(x, 256, 1, train_bn=train_bn)
    branch_1 = InceptionResNetV2_conv2d_bn(branch_1, 288, 3, strides=2, padding='same', train_bn=train_bn)
    branch_2 = InceptionResNetV2_conv2d_bn(x, 256, 1, train_bn=train_bn)
    branch_2 = InceptionResNetV2_conv2d_bn(branch_2, 288, 3, train_bn=train_bn)
    branch_2 = InceptionResNetV2_conv2d_bn(branch_2, 320, 3, strides=2, padding='same', train_bn=train_bn)
    branch_pool = MaxPooling2D(3, strides=2, padding='same')(x)
    branches = [branch_0, branch_1, branch_2, branch_pool]
    x = Concatenate(axis=channel_axis, name='mixed_7a')(branches)

    # 10x block8 (Inception-ResNet-C block): 8 x 8 x 2080
    for block_idx in range(1, 10):
        x = inception_resnet_block(x,
                                   scale=0.2,
                                   block_type='block8',
                                   block_idx=block_idx,
                                   train_bn=train_bn)
    x = inception_resnet_block(x,
                               scale=1.,
                               activation=None,
                               block_type='block8',
                               block_idx=10,
                               train_bn=train_bn)

    C4 = x

    # Final convolution block: 8 x 8 x 1536
    C5 = x = InceptionResNetV2_conv2d_bn(x, 1536, 1, name='conv_7b', train_bn=train_bn)

    return [C1, C2, C3, C4, C5]

def InceptionResNetV2_shape(self, image_shape):
    return np.array(
        [[int(math.ceil(image_shape[0] / stride)),
            int(math.ceil(image_shape[1] / stride))]
            for stride in [8, 16, 32, 32, 64]])

def InceptionResNetV2_strides():
    strides = [8, 16, 32, 32, 64]
    return strides

def InceptionResNetV2_fpn_mask(self, rois, feature_maps, image_meta, pool_size, num_classes, train_bn=None):
    """Builds the computation graph of the mask head of Feature Pyramid Network.
    rois: [batch, num_rois, (y1, x1, y2, x2)] Proposal boxes in normalized
          coordinates.
    feature_maps: List of feature maps from diffent layers of the pyramid,
                  [P2, P3, P4, P5]. Each has a different resolution.
    image_meta: [batch, (meta data)] Image details. See compose_image_meta()
    pool_size: The width of the square feature map generated from ROI Pooling.
    num_classes: number of classes, which determines the depth of the results
    train_bn: Boolean. Train or freeze Batch Norm layres
    Returns: Masks [batch, roi_count, height, width, num_classes]
    """

    bn_axis = -1

    # ROI Pooling
    # Shape: [batch, boxes, pool_height, pool_width, channels]
    x = PyramidROIAlign([pool_size, pool_size],
                        name="roi_align_mask")([rois, image_meta] + feature_maps)

    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv1")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn1')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv2")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn2')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv3")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn3')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv4")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn4')(x, training=train_bn)
    x = Activation('relu')(x)

    x = TimeDistributed(Conv2DTranspose(256, (2, 2), strides=2, activation="relu"),
                           name="mrcnn_mask_deconv")(x)
    x = TimeDistributed(Conv2D(num_classes, (1, 1), strides=1, activation="sigmoid"),
                           name="mrcnn_mask")(x)
    return x

def InceptionResNetV2_layer_reg():
    reg = {
        # all layers but the backbone
        "heads": r"(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # From a specific Resnet stage and up
        "3+": r"(batch\_normalization|conv2d)\_(7[3-9]|[8-9][0-9]|1[0-9][0-9]|20[0-3])|" \
               "(conv\_7b.*)|(block8.*)|(block17.*)|" \
               "(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "4+": r"(batch\_normalization|conv2d)\_(15[7-9]|1[6-9][0-9]|20[0-3])|" \
               "(conv\_7b.*)|(block8.*)|" \
               "(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "5+": r"(conv\_7b.*)|" \
               "(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # All layers
        "all": ".*",
    }
    return reg

############################################################
#  DenseNet201
############################################################

"""
from keras import layers
from keras.layers import Conv2D
from keras.layers import BatchNormalization
from keras.layers import Activation
from keras.layers import MaxPooling2D
from keras.layers import AveragePooling2D
from keras.layers import ZeroPadding2D
"""

def DenseNet_conv_block(x, growth_rate, name, train_bn=None):
    """A building block for a dense block.

    # Arguments
        x: input tensor.
        growth_rate: float, growth rate at dense layers.
        name: string, block label.

    # Returns
        Output tensor for the block.
    """
    bn_axis = -1
    x1 = layers.BatchNormalization(axis=bn_axis,
                                   epsilon=1.001e-5,
                                   name=name + '_0_bn')(x, training=train_bn)
    x1 = layers.Activation('relu', name=name + '_0_relu')(x1)
    x1 = layers.Conv2D(4 * growth_rate, 1,
                       use_bias=False,
                       name=name + '_1_conv')(x1)
    x1 = layers.BatchNormalization(axis=bn_axis, epsilon=1.001e-5,
                                   name=name + '_1_bn')(x1, training=train_bn)
    x1 = layers.Activation('relu', name=name + '_1_relu')(x1)
    x1 = layers.Conv2D(growth_rate, 3,
                       padding='same',
                       use_bias=False,
                       name=name + '_2_conv')(x1)
    x = layers.Concatenate(axis=bn_axis, name=name + '_concat')([x, x1])
    return x

def dense_block(x, blocks, name, train_bn=None):
    """A dense block.

    # Arguments
        x: input tensor.
        blocks: integer, the number of building blocks.
        name: string, block label.

    # Returns
        output tensor for the block.
    """
    for i in range(blocks):
        x = DenseNet_conv_block(x, 32, name=name + '_block' + str(i + 1), train_bn=train_bn)
    return x

def transition_block(x, reduction, name, train_bn=None):
    """A transition block.

    # Arguments
        x: input tensor.
        reduction: float, compression rate at transition layers.
        name: string, block label.

    # Returns
        output tensor for the block.
    """
    bn_axis = -1
    x = layers.BatchNormalization(axis=bn_axis, epsilon=1.001e-5,
                                  name=name + '_bn')(x, training=train_bn)
    x = layers.Activation('relu', name=name + '_relu')(x)
    x = layers.Conv2D(int(K.int_shape(x)[bn_axis] * reduction), 1,
                      use_bias=False,
                      name=name + '_conv')(x)
    x = layers.AveragePooling2D(2, strides=2, name=name + '_pool')(x)
    return x

def DenseNet201(self, input_image, stage5=True, train_bn=None):

    bn_axis = -1
    blocks = [6, 12, 48, 32]

    # get strides used
    # test = np.ones([10, 512, 512, 3], dtype='float32')
    # input_image = tf.Variable(test) # Use variable

    x = layers.ZeroPadding2D(padding=((3, 3), (3, 3)))(input_image)
    x = layers.Conv2D(64, 7, strides=2, use_bias=False, name='conv1/conv')(x)
    x = layers.BatchNormalization(
        axis=bn_axis, epsilon=1.001e-5, name='conv1/bn')(x, training=train_bn)
    x = layers.Activation('relu', name='conv1/relu')(x)
    x = layers.ZeroPadding2D(padding=((1, 1), (1, 1)))(x)
    C1 = x = layers.MaxPooling2D(3, strides=2, name='pool1')(x)

    x = dense_block(x, blocks[0], name='conv2', train_bn=train_bn)
    C2 = x = transition_block(x, 0.5, name='pool2', train_bn=train_bn)
    x = dense_block(x, blocks[1], name='conv3', train_bn=train_bn)
    C3 = x = transition_block(x, 0.5, name='pool3', train_bn=train_bn)
    x = dense_block(x, blocks[2], name='conv4', train_bn=train_bn)
    C4 = x = transition_block(x, 0.5, name='pool4', train_bn=train_bn)
    x = dense_block(x, blocks[3], name='conv5', train_bn=train_bn)

    x = layers.BatchNormalization(
        axis=bn_axis, epsilon=1.001e-5, name='bn')(x, training=train_bn)
    C5 = x = layers.Activation('relu', name='relu')(x)

    return [C1, C2, C3, C4, C5]

def DenseNet201_shape(self, image_shape):
    return np.array(
        [[int(math.ceil(image_shape[0] / stride)),
            int(math.ceil(image_shape[1] / stride))]
            for stride in [8, 16, 32, 32, 64]])

def DenseNet201_strides():
    strides = [8, 16, 32, 32, 64]
    return strides

def DenseNet201_fpn_mask(self, rois, feature_maps, image_meta, pool_size, num_classes, train_bn=None):
    """Builds the computation graph of the mask head of Feature Pyramid Network.
    rois: [batch, num_rois, (y1, x1, y2, x2)] Proposal boxes in normalized
          coordinates.
    feature_maps: List of feature maps from diffent layers of the pyramid,
                  [P2, P3, P4, P5]. Each has a different resolution.
    image_meta: [batch, (meta data)] Image details. See compose_image_meta()
    pool_size: The width of the square feature map generated from ROI Pooling.
    num_classes: number of classes, which determines the depth of the results
    train_bn: Boolean. Train or freeze Batch Norm layres
    Returns: Masks [batch, roi_count, height, width, num_classes]
    """

    bn_axis = -1

    # ROI Pooling
    # Shape: [batch, boxes, pool_height, pool_width, channels]
    x = PyramidROIAlign([pool_size, pool_size],
                        name="roi_align_mask")([rois, image_meta] + feature_maps)

    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv1")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn1')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv2")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn2')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv3")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn3')(x, training=train_bn)
    x = Activation('relu')(x)
    x = TimeDistributed(Conv2D(256, (3, 3), padding="same"),name="mrcnn_mask_conv4")(x)
    x = TimeDistributed(BatchNormalization(axis=bn_axis),name='mrcnn_mask_bn4')(x, training=train_bn)
    x = Activation('relu')(x)

    x = TimeDistributed(Conv2DTranspose(256, (2, 2), strides=2, activation="relu"),
                           name="mrcnn_mask_deconv")(x)
    x = TimeDistributed(Conv2D(num_classes, (1, 1), strides=1, activation="sigmoid"),
                           name="mrcnn_mask")(x)
    return x

def DenseNet201_layer_reg():
    reg = {
        # all layers but the backbone
        "heads": r"(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # From a specific Resnet stage and up
        "3+": r"(bn)|(conv5\_.*)|(conv4\_.*)|(conv3\_.*)|" \
               "(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "4+": r"(bn)|(conv5\_.*)|(conv4\_.*)|" \
               "(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "5+": r"(bn)|(conv5\_.*)|" \
               "(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # All layers
        "all": ".*",
    }
    return reg

############################################################
#  Function
############################################################

def Package(name):
    if name == "ResNet50":
        return ResNet50, ResNet50_fpn_mask, ResNet50_layer_reg(), \
               ResNet50_shape, ResNet50_strides()
    elif name == "Xception":
        return Xception, Xception_fpn_mask, Xception_layer_reg(), \
               Xception_shape, Xception_strides()
    elif name == "MobileNetV1":
        return MobileNetV1, MobileNetV1_fpn_mask, MobileNetV1_layer_reg(), \
               MobileNetV1_shape, MobileNetV1_strides()
    elif name == "InceptionV3":
        return InceptionV3, InceptionV3_fpn_mask, InceptionV3_layer_reg(), \
               InceptionV3_shape, InceptionV3_strides()
    elif name == "InceptionResNetV2":
        return InceptionResNetV2, InceptionResNetV2_fpn_mask, InceptionResNetV2_layer_reg(), \
               InceptionResNetV2_shape, InceptionResNetV2_strides()
    elif name == "DenseNet201":
        return DenseNet201, DenseNet201_fpn_mask, DenseNet201_layer_reg(), \
               DenseNet201_shape, DenseNet201_strides()
    else:
        raise Exception("No such that backbone.")

