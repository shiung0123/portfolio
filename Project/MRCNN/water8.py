"""
Mask R-CNN

Source Code:
    https://github.com/matterport/Mask_RCNN/tree/master/mrcnn
    3deaec5 on 1 Apr 2019

Dataset: 
    Customized(read image)
    Read data into cache

Usage:
    // 進行訓練，分資料集為train/val
    python XX.py train --dataset=XX --weights=XX.h5
    python XX.py train --dataset=XX --backbone_weights=XX.h5

    // 輸出預測結果，選擇一個資料夾之所有圖片
    python XX.py splash --image=XX\*.jpg --weights=XX.h5

    // 計算準確度，並儲存預測結果(不用分train/val)
    python XX.py evaluate --dataset=XX --weights=XX.h5 
"""

import os
import glob
import sys
import json
import datetime
import numpy as np
import skimage.io
import skimage.color
from mrcnn.config import Config
from mrcnn.model import MaskRCNN
from mrcnn import visualize, utils
from mrcnn import model as modellib
import tensorflow as tf
import backbone as bb

config = tf.ConfigProto()
config.gpu_options.allow_growth = True
session = tf.Session(config=config)

############################################################
# Path
############################################################

# Root directory of the project
ROOT_DIR = os.path.abspath("./")

# Import Mask RCNN
sys.path.append(ROOT_DIR)  # To find local version of the library

# Path to trained weights file
COCO_WEIGHTS_PATH = os.path.join(ROOT_DIR, "mask_rcnn_coco.h5")

# Directory to save logs and model checkpoints, if not provided
# through the command line argument --logs
DEFAULT_LOGS_DIR = os.path.join(ROOT_DIR, "logs")

############################################################
#  Fix MRCNN
###########################################################

# show predict image
# fix mrcnn.visualize.display_instances 83
"""
    def display_instances(image, boxes, masks, class_ids, class_names,
                      scores=None, title="",
                      figsize=(16, 16), ax=None,
                      show_mask=True, show_bbox=True,
                      colors=None, captions=None, save_name=None):
"""
# fix mrcnn.visualize.display_instances 166
"""
    if auto_show:
        plt.savefig(save_name)
        # plt.show()
"""

# change tensorboard save args, checkpoint save args
# fix mrcnn.model.MaskRCNN.train 2612
"""
    # Callbacks
    callbacks = [
        keras.callbacks.TensorBoard(log_dir=self.log_dir,
                                    histogram_freq=0, write_graph=False, write_images=True),
        keras.callbacks.ModelCheckpoint(self.checkpoint_path,
                                        verbose=0, save_weights_only=True, period=10),
    ]
"""

# cache image
# fix mrcnn.model.load_image_gt 1212
"""
    # Load image and mask
    image, window, scale = dataset.load_image(image_id)
    mask, class_ids = dataset.load_mask(image_id)
    original_shape = image.shape
    \""" this area need annotation, start
    image, window, scale, padding, crop = utils.resize_image(
        image,
        min_dim=config.IMAGE_MIN_DIM,
        min_scale=config.IMAGE_MIN_SCALE,
        max_dim=config.IMAGE_MAX_DIM,
        mode=config.IMAGE_RESIZE_MODE)
    mask = utils.resize_mask(mask, scale, padding, crop)
    \""" this area need annotation, end
"""

# layer regular expression
# fix mrcnn config.Config 57
"""
    BACKBONE_NAME = "resnet101"
    BACKBONE = "resnet101"
    FPN_MASK = "resnet101"
    LAYER_REG = \
    {
        # all layers but the backbone
        "heads": r"(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # From a specific Resnet stage and up
        "3+": r"(res3.*)|(bn3.*)|(res4.*)|(bn4.*)|(res5.*)|(bn5.*)|(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "4+": r"(res4.*)|(bn4.*)|(res5.*)|(bn5.*)|(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        "5+": r"(res5.*)|(bn5.*)|(mrcnn\_.*)|(rpn\_.*)|(fpn\_.*)",
        # All layers
        "all": ".*",
    }
"""
# fix mrcnn model.MaskRCNN.train 2595
"""
    # Pre-defined layer regular expressions
    layer_regex = self.config.LAYER_REG
    if layers in layer_regex.keys():
        layers = layer_regex[layers]
"""

# change build function (pyramid feature, fpn mask)
# fix mrcnn model.MaskRCNN.build 1843
"""
    def build(self, mode, config):
        \""" this area need annotation, start
        Build Mask R-CNN architecture.
            input_shape: The shape of the input image.
            mode: Either "training" or "inference". The inputs and
                outputs of the model differ accordingly.
        \""" this area need annotation, end
        assert mode in ['training', 'inference']

        # Image size must be dividable by 2 multiple times
        h, w = config.IMAGE_SHAPE[:2]
        if h / 2**6 != int(h / 2**6) or w / 2**6 != int(w / 2**6):
            raise Exception("Image size must be dividable by 2 at least 6 times "
                            "to avoid fractions when downscaling and upscaling."
                            "For example, use 256, 320, 384, 448, 512, ... etc. ")

        # Inputs
        input_image = KL.Input(
            shape=[None, None, config.IMAGE_SHAPE[2]], name="input_image")
        input_image_meta = KL.Input(shape=[config.IMAGE_META_SIZE],
                                    name="input_image_meta")
        if mode == "training":
            # RPN GT
            input_rpn_match = KL.Input(
                shape=[None, 1], name="input_rpn_match", dtype=tf.int32)
            input_rpn_bbox = KL.Input(
                shape=[None, 4], name="input_rpn_bbox", dtype=tf.float32)

            # Detection GT (class IDs, bounding boxes, and masks)
            # 1. GT Class IDs (zero padded)
            input_gt_class_ids = KL.Input(
                shape=[None], name="input_gt_class_ids", dtype=tf.int32)
            # 2. GT Boxes in pixels (zero padded)
            # [batch, MAX_GT_INSTANCES, (y1, x1, y2, x2)] in image coordinates
            input_gt_boxes = KL.Input(
                shape=[None, 4], name="input_gt_boxes", dtype=tf.float32)
            # Normalize coordinates
            gt_boxes = KL.Lambda(lambda x: norm_boxes_graph(
                x, K.shape(input_image)[1:3]))(input_gt_boxes)
            # 3. GT Masks (zero padded)
            # [batch, height, width, MAX_GT_INSTANCES]
            if config.USE_MINI_MASK:
                input_gt_masks = KL.Input(
                    shape=[config.MINI_MASK_SHAPE[0],
                           config.MINI_MASK_SHAPE[1], None],
                    name="input_gt_masks", dtype=bool)
            else:
                input_gt_masks = KL.Input(
                    shape=[config.IMAGE_SHAPE[0], config.IMAGE_SHAPE[1], None],
                    name="input_gt_masks", dtype=bool)
        elif mode == "inference":
            # Anchors in normalized coordinates
            input_anchors = KL.Input(shape=[None, 4], name="input_anchors")

        # Build the shared convolutional layers.
        # Bottom-up Layers
        # Returns a list of the last layers of each stage, 5 in total.
        # Don't create the thead (stage 5), so we pick the 4th item in the list.
        if callable(config.BACKBONE):
            _, C2, C3, C4, C5 = config.BACKBONE(input_image, stage5=True,
                                                train_bn=config.TRAIN_BN)
        else:
            _, C2, C3, C4, C5 = resnet_graph(input_image, config.BACKBONE,
                                             stage5=True, train_bn=config.TRAIN_BN)
        
        # Top-down Layers
        # TODO: add assert to varify feature map sizes match what's in config
        if config.BACKBONE_NAME == "InceptionV3":
            P5 = KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (1, 1), name='fpn_c5p5')(C5)
            P4 = KL.Add(name="fpn_p4add")([
                KL.UpSampling2D(size=(2, 2), name="fpn_p5upsampled")(P5),
                KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (1, 1), name='fpn_c4p4')(C4)])
            P3 = KL.Add(name="fpn_p3add")([
                KL.UpSampling2D(size=(1, 1), name="fpn_p4upsampled")(P4),
                KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (1, 1), name='fpn_c3p3')(C3)])
            P2 = KL.Add(name="fpn_p2add")([
                KL.UpSampling2D(size=(2, 2), name="fpn_p3upsampled")(P3),
                KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (1, 1), name='fpn_c2p2')(C2)])

        elif config.BACKBONE_NAME == "InceptionResNetV2":
            P5 = KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (1, 1), name='fpn_c5p5')(C5)
            P4 = KL.Add(name="fpn_p4add")([
                KL.UpSampling2D(size=(1, 1), name="fpn_p5upsampled")(P5),
                KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (1, 1), name='fpn_c4p4')(C4)])
            P3 = KL.Add(name="fpn_p3add")([
                KL.UpSampling2D(size=(2, 2), name="fpn_p4upsampled")(P4),
                KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (1, 1), name='fpn_c3p3')(C3)])
            P2 = KL.Add(name="fpn_p2add")([
                KL.UpSampling2D(size=(2, 2), name="fpn_p3upsampled")(P3),
                KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (1, 1), name='fpn_c2p2')(C2)])

        else:
            P5 = KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (1, 1), name='fpn_c5p5')(C5)
            P4 = KL.Add(name="fpn_p4add")([
                KL.UpSampling2D(size=(2, 2), name="fpn_p5upsampled")(P5),
                KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (1, 1), name='fpn_c4p4')(C4)])
            P3 = KL.Add(name="fpn_p3add")([
                KL.UpSampling2D(size=(2, 2), name="fpn_p4upsampled")(P4),
                KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (1, 1), name='fpn_c3p3')(C3)])
            P2 = KL.Add(name="fpn_p2add")([
                KL.UpSampling2D(size=(2, 2), name="fpn_p3upsampled")(P3),
                KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (1, 1), name='fpn_c2p2')(C2)])

        # Attach 3x3 conv to all P layers to get the final feature maps.
        P2 = KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (3, 3), padding="SAME", name="fpn_p2")(P2)
        P3 = KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (3, 3), padding="SAME", name="fpn_p3")(P3)
        P4 = KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (3, 3), padding="SAME", name="fpn_p4")(P4)
        P5 = KL.Conv2D(config.TOP_DOWN_PYRAMID_SIZE, (3, 3), padding="SAME", name="fpn_p5")(P5)
        # P6 is used for the 5th anchor scale in RPN. Generated by
        # subsampling from P5 with stride of 2.
        P6 = KL.MaxPooling2D(pool_size=(1, 1), strides=2, name="fpn_p6")(P5)

        # Note that P6 is used in RPN, but not in the classifier heads.
        rpn_feature_maps = [P2, P3, P4, P5, P6]
        mrcnn_feature_maps = [P2, P3, P4, P5]

        # Anchors
        if mode == "training":
            anchors = self.get_anchors(config.IMAGE_SHAPE)
            # Duplicate across the batch dimension because Keras requires it
            # TODO: can this be optimized to avoid duplicating the anchors?
            anchors = np.broadcast_to(anchors, (config.BATCH_SIZE,) + anchors.shape)
            # A hack to get around Keras's bad support for constants
            anchors = KL.Lambda(lambda x: tf.Variable(anchors), name="anchors")(input_image)
        else:
            anchors = input_anchors

        # RPN Model
        rpn = build_rpn_model(config.RPN_ANCHOR_STRIDE,
                              len(config.RPN_ANCHOR_RATIOS), config.TOP_DOWN_PYRAMID_SIZE)
        # Loop through pyramid layers
        layer_outputs = []  # list of lists
        for p in rpn_feature_maps:
            layer_outputs.append(rpn([p]))
        # Concatenate layer outputs
        # Convert from list of lists of level outputs to list of lists
        # of outputs across levels.
        # e.g. [[a1, b1, c1], [a2, b2, c2]] => [[a1, a2], [b1, b2], [c1, c2]]
        output_names = ["rpn_class_logits", "rpn_class", "rpn_bbox"]
        outputs = list(zip(*layer_outputs))
        outputs = [KL.Concatenate(axis=1, name=n)(list(o))
                   for o, n in zip(outputs, output_names)]

        rpn_class_logits, rpn_class, rpn_bbox = outputs

        # Generate proposals
        # Proposals are [batch, N, (y1, x1, y2, x2)] in normalized coordinates
        # and zero padded.
        proposal_count = config.POST_NMS_ROIS_TRAINING if mode == "training"\
            else config.POST_NMS_ROIS_INFERENCE
        rpn_rois = ProposalLayer(
            proposal_count=proposal_count,
            nms_threshold=config.RPN_NMS_THRESHOLD,
            name="ROI",
            config=config)([rpn_class, rpn_bbox, anchors])

        if mode == "training":
            # Class ID mask to mark class IDs supported by the dataset the image
            # came from.
            active_class_ids = KL.Lambda(
                lambda x: parse_image_meta_graph(x)["active_class_ids"]
                )(input_image_meta)

            if not config.USE_RPN_ROIS:
                # Ignore predicted ROIs and use ROIs provided as an input.
                input_rois = KL.Input(shape=[config.POST_NMS_ROIS_TRAINING, 4],
                                      name="input_roi", dtype=np.int32)
                # Normalize coordinates
                target_rois = KL.Lambda(lambda x: norm_boxes_graph(
                    x, K.shape(input_image)[1:3]))(input_rois)
            else:
                target_rois = rpn_rois

            # Generate detection targets
            # Subsamples proposals and generates target outputs for training
            # Note that proposal class IDs, gt_boxes, and gt_masks are zero
            # padded. Equally, returned rois and targets are zero padded.
            rois, target_class_ids, target_bbox, target_mask =\
                DetectionTargetLayer(config, name="proposal_targets")([
                    target_rois, input_gt_class_ids, gt_boxes, input_gt_masks])

            # Network Heads
            # TODO: verify that this handles zero padded ROIs
            mrcnn_class_logits, mrcnn_class, mrcnn_bbox =\
                fpn_classifier_graph(rois, mrcnn_feature_maps, input_image_meta,
                                     config.POOL_SIZE, config.NUM_CLASSES,
                                     train_bn=config.TRAIN_BN,
                                     fc_layers_size=config.FPN_CLASSIF_FC_LAYERS_SIZE)

            if callable(config.FPN_MASK):
                mrcnn_mask = config.FPN_MASK(rois, mrcnn_feature_maps,
                                              input_image_meta,
                                              config.MASK_POOL_SIZE,
                                              config.NUM_CLASSES,
                                              train_bn=config.TRAIN_BN)
            else:
                mrcnn_mask = build_fpn_mask_graph(rois, mrcnn_feature_maps,
                                              input_image_meta,
                                              config.MASK_POOL_SIZE,
                                              config.NUM_CLASSES,
                                              train_bn=config.TRAIN_BN)

            # TODO: clean up (use tf.identify if necessary)
            output_rois = KL.Lambda(lambda x: x * 1, name="output_rois")(rois)

            # Losses
            rpn_class_loss = KL.Lambda(lambda x: rpn_class_loss_graph(*x), name="rpn_class_loss")(
                [input_rpn_match, rpn_class_logits])
            rpn_bbox_loss = KL.Lambda(lambda x: rpn_bbox_loss_graph(config, *x), name="rpn_bbox_loss")(
                [input_rpn_bbox, input_rpn_match, rpn_bbox])
            class_loss = KL.Lambda(lambda x: mrcnn_class_loss_graph(*x), name="mrcnn_class_loss")(
                [target_class_ids, mrcnn_class_logits, active_class_ids])
            bbox_loss = KL.Lambda(lambda x: mrcnn_bbox_loss_graph(*x), name="mrcnn_bbox_loss")(
                [target_bbox, target_class_ids, mrcnn_bbox])
            mask_loss = KL.Lambda(lambda x: mrcnn_mask_loss_graph(*x), name="mrcnn_mask_loss")(
                [target_mask, target_class_ids, mrcnn_mask])

            # Model
            inputs = [input_image, input_image_meta,
                      input_rpn_match, input_rpn_bbox, input_gt_class_ids, input_gt_boxes, input_gt_masks]
            if not config.USE_RPN_ROIS:
                inputs.append(input_rois)
            outputs = [rpn_class_logits, rpn_class, rpn_bbox,
                       mrcnn_class_logits, mrcnn_class, mrcnn_bbox, mrcnn_mask,
                       rpn_rois, output_rois,
                       rpn_class_loss, rpn_bbox_loss, class_loss, bbox_loss, mask_loss]
            model = KM.Model(inputs, outputs, name='mask_rcnn')
        else:
            # Network Heads
            # Proposal classifier and BBox regressor heads
            mrcnn_class_logits, mrcnn_class, mrcnn_bbox =\
                fpn_classifier_graph(rpn_rois, mrcnn_feature_maps, input_image_meta,
                                     config.POOL_SIZE, config.NUM_CLASSES,
                                     train_bn=config.TRAIN_BN,
                                     fc_layers_size=config.FPN_CLASSIF_FC_LAYERS_SIZE)

            # Detections
            # output is [batch, num_detections, (y1, x1, y2, x2, class_id, score)] in
            # normalized coordinates
            detections = DetectionLayer(config, name="mrcnn_detection")(
                [rpn_rois, mrcnn_class, mrcnn_bbox, input_image_meta])

            # Create masks for detections
            detection_boxes = KL.Lambda(lambda x: x[..., :4])(detections)
            if callable(config.FPN_MASK):
                mrcnn_mask = config.FPN_MASK(detection_boxes, mrcnn_feature_maps,
                                              input_image_meta,
                                              config.MASK_POOL_SIZE,
                                              config.NUM_CLASSES,
                                              train_bn=config.TRAIN_BN)
            else:
                mrcnn_mask = build_fpn_mask_graph(detection_boxes, mrcnn_feature_maps,
                                              input_image_meta,
                                              config.MASK_POOL_SIZE,
                                              config.NUM_CLASSES,
                                              train_bn=config.TRAIN_BN)

            model = KM.Model([input_image, input_image_meta, input_anchors],
                             [detections, mrcnn_class, mrcnn_bbox,
                                 mrcnn_mask, rpn_rois, rpn_class, rpn_bbox],
                             name='mask_rcnn')

        # Add multi-GPU support.
        if config.GPU_COUNT > 1:
            from mrcnn.parallel_model import ParallelModel
            model = ParallelModel(model, config.GPU_COUNT)

        return model
"""


# change PyramidROIAlign to dinamic
# fix mrcnn model.PyramidROIAlign.call 397
"""
    # Loop through levels and apply ROI pooling to each. P2 to Pn.
    pooled = []
    box_to_level = []
    for i, level in enumerate(range(2, len(feature_maps)+2)):
"""

# add comment
# fix mrcnn config.Config 36
"""
    # This should not be a controllable variable, the original comment is wrong
        # Number of training steps per epoch
        # This doesn't need to match the size of the training set. Tensorboard
        # updates are saved at the end of each epoch, so setting this to a
        # smaller number means getting more frequent TensorBoard updates.
        # Validation stats are also calculated at each epoch end and they
        # might take a while, so don't set this too small to avoid spending
        # a lot of time on validation stats.
    # STEPS_PER_EPOCH should be num_data / batch_size
    STEPS_PER_EPOCH = 1000

"""
# fix mrcnn config.Config 76
"""
    # The strides of each layer of the FPN Pyramid. These values
    # are based on a Resnet101 backbone.
    # Strides for P2 ~ P6, not for C1 ~ C5 
    BACKBONE_STRIDES = [4, 8, 16, 32, 64]
"""

############################################################
#  Configurations
############################################################

CLASS_NAMES = ['BG', 'pothole']
IMAGE_MIN_DIM = 512
IMAGE_MAX_DIM = 512
EPOCH = 40

"""
heads: The RPN, classifier and mask heads of the network
all: All the layers
3+: Train Resnet stage 3 and up
4+: Train Resnet stage 4 and up
5+: Train Resnet stage 5 and up
"""
TRAIN_AREA = "4+"

class PotholeConfig(Config):
    """Configuration for training on the toy  dataset.
    Derives from the base Config class and overrides some values.
    """
    # Give the configuration a recognizable name
    NAME = "pothole"

    # We use a GPU with 12GB memory, which can fit two images.
    # Adjust down if you use a smaller GPU.
    IMAGES_PER_GPU = 2

    # Train or freeze batch normalization layers
    #     None: Train BN layers. This is the normal mode
    #     False: Freeze BN layers. Good when using a small batch size
    #     True: (don't use). Set layer in training mode even when predicting
    # TRAIN_BN = None

    # Number of classes (including background)
    NUM_CLASSES = 1 + 1  # Background + pothole

    # Number of training steps per epoch
    # this value should = ceil(num_of_train_data / BATCH_SIZE)
    # or None, it will calculate by itself
    STEPS_PER_EPOCH = 2

    # Skip detections with < 90% confidence
    DETECTION_MIN_CONFIDENCE = 0.9

    # must equal to global variables
    IMAGE_MIN_DIM = 512
    IMAGE_MAX_DIM = 512
    
    # resnet50, resnet101
    # ResNet50, Xception, MobileNetV1, InceptionV3, InceptionResNetV2
    # DenseNet201
    BACKBONE_NAME = "InceptionV3"

    if BACKBONE_NAME == "resnet50" or BACKBONE_NAME == "resnet101":
        BACKBONE = BACKBONE_NAME
    else:
        BACKBONE, FPN_MASK, LAYER_REG, \
        COMPUTE_BACKBONE_SHAPE, BACKBONE_STRIDES \
             = bb.Package(BACKBONE_NAME)

############################################################
#  Dataset
############################################################

class PotholeDataset(utils.Dataset):

    def load_pothole(self, dataset_dir, subset="", jump_bg=True):
        global CLASS_NAMES, IMAGE_MIN_DIM, IMAGE_MAX_DIM
        """Load a subset of the Pothole dataset.
        dataset_dir: Root directory of the dataset.
        subset: Subset to load: train or val
        """
        # Add classes. We have only one class to add.
        self.add_class("pothole", 1, "pothole")

        # Train or validation dataset?
        # assert subset in ["train", "val", ""]
        dataset_dir = os.path.join(dataset_dir, subset)

        json_path = os.path.join(dataset_dir, "*.json")
        # print(glob.glob(json_path))

        for f in glob.glob(json_path) :

            annotations = json.load(open(f))
            annotations = list(annotations.values())  # don't need the dict keys
            if jump_bg: annotations = [a for a in annotations if a['mask']] # not empty mask

            for a in annotations:
                add = True

                # Read Image

                image = None
                window = (0, 0, 0, 0)
                scale = 1
                padding = [(0, 0), (0, 0), (0, 0)]
                crop = None
                height = 0
                width = 0

                image_path = os.path.join(dataset_dir, a['filename'])
                if not os.path.exists(image_path):
                    add = False
                    print(image_path, "not exist")
                else: 
                    # print(image_path, "success")
                    image = skimage.io.imread(image_path)
                    image, window, scale, padding, crop = utils.resize_image(
                        image,
                        min_dim=IMAGE_MIN_DIM,
                        min_scale=0,
                        max_dim=IMAGE_MAX_DIM,
                        mode="square")
                    height, width = image.shape[:2]

                # Read Mask

                mask = []
                class_ids = []

                for m in a['mask'] :
                    mask_path = os.path.join(dataset_dir, m['mask_path'])
                    if not os.path.exists(mask_path):
                        add = False
                        print(mask_path, "not exist")
                    else: 
                        # print(mask_path, "success")
                        mask_pic = skimage.io.imread(mask_path, as_gray=True)
                        mask_2D = mask_pic > 0.5
                        mask.append(mask_2D)
                        class_ids.append(CLASS_NAMES.index(m['label']))

                mask = np.stack(mask, axis=-1)
                mask = utils.resize_mask(mask, scale, padding, crop)

                if add : 
                    self.add_image(
                    "pothole",
                    image_id=a['filename'],  # use file name as a unique image id
                    image=image, path=image_path,
                    width=width, height=height,
                    mask=mask, class_ids=class_ids)

    def load_image(self, image_id):
        """Load the specified image and return a [H,W,3] Numpy array.
        """
        # If not a pothole dataset image, delegate to parent class.
        info = self.image_info[image_id]
        if info["source"] != "pothole":
            return super(self.__class__, self).load_image(image_id)

        # Load image
        image = info['image']

        # If grayscale. Convert to RGB for consistency.
        if image.ndim != 3:
            image = skimage.color.gray2rgb(image)
        # If has an alpha channel, remove it for consistency
        if image.shape[-1] == 4:
            image = image[..., :3]

        return image, (0, 0, info['height'], info['width']), 1

    def load_mask(self, image_id):
        """Generate instance masks for an image.
       Returns:
        masks: A bool array of shape [height, width, instance count] with
            one mask per instance.
        class_ids: a 1D array of class IDs of the instance masks.
        """
        # If not a pothole dataset image, delegate to parent class.
        info = self.image_info[image_id]
        if info["source"] != "pothole":
            return super(self.__class__, self).load_mask(image_id)

        # Convert polygons to a bitmap mask of shape
        # [height, width, instance_count]
        mask = info['mask']
        class_ids = info['class_ids']
       
        return mask, np.array(class_ids, dtype=np.int32)

    def image_reference(self, image_id):
        """Return the path of the image."""
        info = self.image_info[image_id]
        if info["source"] == "pothole":
            return info["path"]
        else:
            super(self.__class__, self).image_reference(image_id)

############################################################
#  Model
############################################################

from keras.engine import saving
from keras import backend as K

def load_backbone_weights_from_hdf5_group(f, layers, reshape=False):
    """Implements topological (order-based) weight loading.

    # Arguments
        f: A pointer to a HDF5 group.
        layers: a list of target layers.
        reshape: Reshape weights to fit the layer when the correct number
            of values are present but the shape does not match.

    # Raises
        ValueError: in case of mismatch between provided layers
            and weights file.
    """
    if 'keras_version' in f.attrs:
        original_keras_version = f.attrs['keras_version'].decode('utf8')
    else:
        original_keras_version = '1'
    if 'backend' in f.attrs:
        original_backend = f.attrs['backend'].decode('utf8')
    else:
        original_backend = None
    
    filtered_layers = []
    for layer in layers:
        weights = layer.weights
        if weights:
            filtered_layers.append(layer)

    print("Model fit layers:")
    print(*list(type(i) for i in filtered_layers), sep = "\n")

    layer_names = saving.load_attributes_from_hdf5_group(f, 'layer_names')
    filtered_layer_names = []
    for name in layer_names:
        g = f[name]
        weight_names = saving.load_attributes_from_hdf5_group(g, 'weight_names')
        if weight_names:
            filtered_layer_names.append(name)

    print("H5 fit layers:")
    print(*list(i for i in filtered_layer_names), sep = "\n")

    """
    if len(filtered_layer_names) != len(filtered_layers):
        raise ValueError('You are trying to load a weight file '
                         'containing ' + str(len(filtered_layer_names)) +
                         ' layers into a model with ' +
                         str(len(filtered_layers)) + ' layers.')
    """

    # We batch weight value assignments in a single backend call
    # which provides a speedup in TensorFlow.
    weight_value_tuples = []
    for k, name in enumerate(filtered_layer_names):
        g = f[name]
        weight_names = saving.load_attributes_from_hdf5_group(g, 'weight_names')
        weight_values = [np.asarray(g[weight_name]) for weight_name in weight_names]
        layer = filtered_layers[k]
        symbolic_weights = layer.weights
        weight_values = saving.preprocess_weights_for_loading(layer,
                                                       weight_values,
                                                       original_keras_version,
                                                       original_backend,
                                                       reshape=reshape)
        if len(weight_values) != len(symbolic_weights):
            raise ValueError('Layer #' + str(k) +
                             ' (named "' + layer.name +
                             '" in the current model) was found to '
                             'correspond to layer ' + name +
                             ' in the save file. '
                             'However the new layer ' + layer.name +
                             ' expects ' + str(len(symbolic_weights)) +
                             ' weights, but the saved weights have ' +
                             str(len(weight_values)) +
                             ' elements.')
        weight_value_tuples += zip(symbolic_weights, weight_values)

    K.batch_set_value(weight_value_tuples)
    

def load_weights_from_hdf5_group_by_name(f, layers, skip_mismatch=False,
                                         reshape=False):
    """Implements name-based weight loading.

    (instead of topological weight loading).

    Layers that have no matching name are skipped.

    # Arguments
        f: A pointer to a HDF5 group.
        layers: A list of target layers.
        skip_mismatch: Boolean, whether to skip loading of layers
            where there is a mismatch in the number of weights,
            or a mismatch in the shape of the weights.
        reshape: Reshape weights to fit the layer when the correct number
            of values are present but the shape does not match.

    # Raises
        ValueError: in case of mismatch between provided layers
            and weights file and skip_mismatch=False.
    """
    if 'keras_version' in f.attrs:
        original_keras_version = f.attrs['keras_version'].decode('utf8')
    else:
        original_keras_version = '1'
    if 'backend' in f.attrs:
        original_backend = f.attrs['backend'].decode('utf8')
    else:
        original_backend = None

    # New file format.
    layer_names = saving.load_attributes_from_hdf5_group(f, 'layer_names')

    # Reverse index of layer name to list of layers with name.
    index = {}
    for layer in layers:
        if layer.name:
            index.setdefault(layer.name, []).append(layer)

    # We batch weight value assignments in a single backend call
    # which provides a speedup in TensorFlow.
    weight_value_tuples = []
    for k, name in enumerate(layer_names):
        g = f[name]
        weight_names = saving.load_attributes_from_hdf5_group(g, 'weight_names')
        weight_values = [np.asarray(g[weight_name]) for weight_name in weight_names]

        for layer in index.get(name, []):
            symbolic_weights = layer.weights
            weight_values = saving.preprocess_weights_for_loading(
                layer,
                weight_values,
                original_keras_version,
                original_backend,
                reshape=reshape)
            if len(weight_values) != len(symbolic_weights):
                if skip_mismatch:
                    warnings.warn('Skipping loading of weights for '
                                  'layer {}'.format(layer.name) + ' due to mismatch '
                                  'in number of weights ({} vs {}).'.format(
                                      len(symbolic_weights), len(weight_values)))
                    continue
                else:
                    raise ValueError('Layer #' + str(k) +
                                     ' (named "' + layer.name +
                                     '") expects ' +
                                     str(len(symbolic_weights)) +
                                     ' weight(s), but the saved weights' +
                                     ' have ' + str(len(weight_values)) +
                                     ' element(s).')
            # Set values.
            for i in range(len(weight_values)):
                symbolic_shape = K.int_shape(symbolic_weights[i])
                if symbolic_shape != weight_values[i].shape:
                    if skip_mismatch:
                        warnings.warn('Skipping loading of weights for '
                                      'layer {}'.format(layer.name) + ' due to '
                                      'mismatch in shape ({} vs {}).'.format(
                                          symbolic_weights[i].shape,
                                          weight_values[i].shape))
                        continue
                    else:
                        raise ValueError('Layer #' + str(k) +
                                         ' (named "' + layer.name +
                                         '"), weight ' +
                                         str(symbolic_weights[i]) +
                                         ' has shape {}'.format(symbolic_shape) +
                                         ', but the saved weight has shape ' +
                                         str(weight_values[i].shape) + '.')
                else:
                    weight_value_tuples.append((symbolic_weights[i],
                                                weight_values[i]))

    K.batch_set_value(weight_value_tuples)

class My_MaskRCNN(MaskRCNN):

    def load_backbone_weights(self, filepath, exclude=None):
        """Modified version of the corresponding Keras function with
        the addition of multi-GPU support and the ability to exclude
        some layers from loading.
        exclude: list of layer names to exclude
        """

        import h5py

        if h5py is None:
            raise ImportError('`load_weights` requires h5py.')
        f = h5py.File(filepath, mode='r')
        if 'layer_names' not in f.attrs and 'model_weights' in f:
            f = f['model_weights']

        # In multi-GPU training, we wrap the model. Get layers
        # of the inner model because they have the weights.
        keras_model = self.keras_model
        layers = keras_model.inner_model.layers if hasattr(keras_model, "inner_model")\
            else keras_model.layers

        print("Model all layers:")
        print(*list(i.name for i in layers), sep = "\n")

        # Exclude some layers
        if exclude:
            layers = filter(lambda l: l.name not in exclude, layers)

        load_backbone_weights_from_hdf5_group(f, layers)
        # load_weights_from_hdf5_group_by_name(f, layers)

        if hasattr(f, 'close'):
            f.close()

        # Update the log directory
        self.set_log_dir(filepath)

############################################################
#  Fuctions
############################################################

def train(model):
    global EPOCH, TRAIN_AREA
    """Train the model."""
    # Training dataset.
    dataset_train = PotholeDataset()
    dataset_train.load_pothole(args.dataset, "train")
    dataset_train.prepare()

    if model.config.STEPS_PER_EPOCH is None :
        model.config.STEPS_PER_EPOCH = \
            int(len(dataset_train.image_info) / model.config.BATCH_SIZE)

    # Validation dataset
    dataset_val = PotholeDataset()
    dataset_val.load_pothole(args.dataset, "val")
    dataset_val.prepare()

    # *** This training schedule is an example. Update to your needs ***
    # Since we're using a very small dataset, and starting from
    # COCO trained weights, we don't need to train too long. Also,
    # no need to train all layers, just the heads should do it.
    print("Training network heads")
    model.train(dataset_train, dataset_val,
                learning_rate=config.LEARNING_RATE,
                epochs=EPOCH,
                layers=TRAIN_AREA)

def color_splash(image, mask):
    """Apply color splash effect.
    image: RGB image [height, width, 3]
    mask: instance segmentation mask [height, width, instance count]

    Returns result image.
    """
    # Make a grayscale copy of the image. The grayscale copy still
    # has 3 RGB channels, though.
    gray = skimage.color.gray2rgb(skimage.color.rgb2gray(image)) * 255
    # Copy color pixels from the original color image where mask is set
    if mask.shape[-1] > 0:
        # We're treating all instances as one, so collapse the mask into one layer
        mask = (np.sum(mask, -1, keepdims=True) >= 1)
        splash = np.where(mask, image, gray).astype(np.uint8)
    else:
        splash = gray.astype(np.uint8)
    return splash

def detect_and_color_splash(model, image_path=None, video_path=None):
    global CLASS_NAMES
    assert image_path or video_path

    # Image or video?
    if image_path:
        # Run model detection and generate the color splash effect
        print("Running on {}".format(args.image))
        for seq in glob.glob(args.image) :
            s = os.path.split(seq)
            file_name = s[1].split(".")[0]+'.png'
            
            image = skimage.io.imread(seq)
            r = model.detect([image], verbose=1)[0]
            # Color splash
            # splash = color_splash(image, r['masks'])
            # before
            # visualize.display_instances(image, r['rois'], r['masks'], r['class_ids'], 
            #                class_names, r['scores'])
            # after
            visualize.display_instances(image, r['rois'], r['masks'], r['class_ids'], 
                            CLASS_NAMES, r['scores'], save_name = 'splash_'+file_name)
            # Save output
            print("Saved", file_name)
            # file_name = 'color_' + file_name
            # skimage.io.imsave(file_name, splash)
            
    elif video_path:
        import cv2
        # Video capture
        vcapture = cv2.VideoCapture(video_path)
        width = int(vcapture.get(cv2.CAP_PROP_FRAME_WIDTH))
        height = int(vcapture.get(cv2.CAP_PROP_FRAME_HEIGHT))
        fps = vcapture.get(cv2.CAP_PROP_FPS)

        # Define codec and create video writer
        file_name = "splash_{:%Y%m%dT%H%M%S}.avi".format(datetime.datetime.now())
        vwriter = cv2.VideoWriter(file_name,
                                  cv2.VideoWriter_fourcc(*'MJPG'),
                                  fps, (width, height))

        count = 0
        success = True
        while success:
            print("frame: ", count)
            # Read next image
            success, image = vcapture.read()
            if success:
                # OpenCV returns images as BGR, convert to RGB
                image = image[..., ::-1]
                # Detect objects
                r = model.detect([image], verbose=0)[0]
                # Color splash
                splash = color_splash(image, r['masks'])
                # RGB -> BGR to save image to video
                splash = splash[..., ::-1]
                # Add image to video writer
                vwriter.write(splash)
                count += 1
        vwriter.release()
        print("Saved to ", file_name)

def evaluate_model(dataset, model, cfg):
    global CLASS_NAMES
    APs = list()
    for image_id in dataset.image_ids:
        # 載入指定 image id 的影象、邊框和掩膜
        image, image_meta, gt_class_id, gt_bbox, gt_mask = modellib.load_image_gt(dataset, cfg, image_id, use_mini_mask=False)
        # 轉換畫素值（例如居中）
        # scaled_image = mold_image(image, cfg)
        # 將影象轉換為樣本
        # sample = expand_dims(scaled_image, 0)
        # 作出預測
        r = model.detect([image], verbose=1)[0]
        # 
        visualize.display_instances(image, r['rois'], r['masks'], r['class_ids'], 
                            CLASS_NAMES, r['scores'], save_name = 'predict_'+str(image_id))
        # 統計計算，包括計算 AP
        AP = 0.0
        if len(gt_class_id) == 0 or len(r['class_ids']) == 0:
            if len(gt_class_id) == len(r['class_ids']): Ap = 1.0
            else: Ap = 0.0
        else: AP, _, _, _ = utils.compute_ap(gt_bbox, gt_class_id, gt_mask,
                                             r["rois"], r["class_ids"], r["scores"], r['masks'], iou_threshold=0.5)
        print("AP: %.3f" % AP)
        # 儲存
        APs.append(AP)
    # 計算所有圖片的平均 AP
    # print(APs)
    mAP = np.mean(APs)
    return mAP

############################################################
#  Main(Input/Init)
############################################################

if __name__ == '__main__':
    import argparse

    # Parse command line arguments
    parser = argparse.ArgumentParser(
        description='Train Mask R-CNN to detect pothole.')
    parser.add_argument("command",
                        metavar="<command>",
                        help="'train' or 'splash' or 'evaluate'")
    parser.add_argument('--dataset', required=False,
                        metavar="/path/to/pothole/dataset/",
                        help='Directory of the Pothole dataset')
    parser.add_argument('--weights', required=False,
                        metavar="/path/to/weights.h5",
                        help="Path to weights .h5 file or 'coco'")
    parser.add_argument('--backbone_weights', required=False,
                        metavar="/path/to/weights.h5",
                        help="Path to weights .h5")
    parser.add_argument('--logs', required=False,
                        default=DEFAULT_LOGS_DIR,
                        metavar="/path/to/logs/",
                        help='Logs and checkpoints directory (default=logs/)')
    parser.add_argument('--image', required=False,
                        metavar="path or URL to image",
                        help='Image to apply the color splash effect on')
    parser.add_argument('--video', required=False,
                        metavar="path or URL to video",
                        help='Video to apply the color splash effect on')
    args = parser.parse_args()
    
    # Validate arguments
    if args.command == "train":
        assert args.dataset, "Argument --dataset is required for training"
        assert (args.weights or args.backbone_weights), "Argument --weights is required for training"
    elif args.command == "splash":
        assert args.image or args.video, "Provide --image or --video to apply color splash"
        assert args.weights, "Argument --weights is required for training"
    elif args.command == "evaluate":
        assert args.dataset, "Argument --dataset is required for evaluating model"
        assert args.weights, "Argument --weights is required for training"

    print("Dataset: ", args.dataset)
    print("Weights: ", args.weights)
    print("Backbone_Weights: ", args.backbone_weights)
    print("Logs: ", args.logs)

    # Configurations
    if args.command == "train":
        config = PotholeConfig()
    else:
        class InferenceConfig(PotholeConfig):
            # Set batch size to 1 since we'll be running inference on
            # one image at a time. Batch size = GPU_COUNT * IMAGES_PER_GPU
            GPU_COUNT = 1
            IMAGES_PER_GPU = 1
        config = InferenceConfig()
    config.display()

    # Create model
    if args.command == "train":
        model = My_MaskRCNN(mode="training", config=config,
                                  model_dir=args.logs)
    else:
        model = My_MaskRCNN(mode="inference", config=config,
                                  model_dir=args.logs)

    # Select weights file to load
    if args.weights:
        print("load weights : ", args.weights)
        if args.weights.lower() == "coco":
            weights_path = COCO_WEIGHTS_PATH
            # Download weights file
            if not os.path.exists(weights_path):
                utils.download_trained_weights(weights_path)
            model.load_weights(weights_path, by_name=True, exclude=[
                "mrcnn_class_logits", "mrcnn_bbox_fc",
                "mrcnn_bbox", "mrcnn_mask"])
        elif args.weights.lower() == "last":
            # Find last trained weights
            weights_path = model.find_last()
            model.load_weights(weights_path, by_name=True)
        elif args.weights.lower() == "imagenet":
            # Start from ImageNet trained weights
            weights_path = model.get_imagenet_weights()
            model.load_weights(weights_path, by_name=True)
        else: 
            # Load custom weights
            weights_path = args.weights
            model.load_weights(weights_path, by_name=True)
    else: # args.backbone_weights
        print("load backbone weights : ", args.backbone_weights)
        weights_path = args.backbone_weights
        model.load_backbone_weights(weights_path)

    # Train or evaluate
    if args.command == "test":
        print("Test Finish."); sys.exit(0)
    elif args.command == "train":
        train(model)
    elif args.command == "splash":
        detect_and_color_splash(model, image_path=args.image,
                                video_path=args.video)
    elif args.command == "evaluate":
        evaluate_set = PotholeDataset()
        evaluate_set.load_pothole(args.dataset, jump_bg=False)
        evaluate_set.prepare()
        mAP = evaluate_model(evaluate_set, model, config)
        print("mAP: %.3f" % mAP)
    else:
        print("'{}' is not recognized. "
              "Use 'train', 'splash', 'evaluate'".format(args.command))
