"""
Mask R-CNN
Train on the toy Balloon dataset and implement color splash effect.

Copyright (c) 2018 Matterport, Inc.
Licensed under the MIT License (see LICENSE for details)
Written by Waleed Abdulla

------------------------------------------------------------

Usage: import the module (see Jupyter notebooks for examples), or run from
       the command line as such:

    # Train a new model starting from pre-trained COCO weights
    python3 balloon.py train --dataset=/path/to/balloon/dataset --weights=coco

    # Resume training a model that you had trained earlier
    python3 balloon.py train --dataset=/path/to/balloon/dataset --weights=last

    # Train a new model starting from ImageNet weights
    python3 balloon.py train --dataset=/path/to/balloon/dataset --weights=imagenet

    # Apply color splash to an image
    python3 balloon.py splash --weights=/path/to/weights/file.h5 --image=<URL or path to file>

    # Apply color splash to video using the last weights you trained
    python3 balloon.py splash --weights=last --video=<URL or path to file>
"""

import os
import glob
import sys
import json
import datetime
import skimage.io
import skimage.draw
import numpy as np

# Root directory of the project
ROOT_DIR = os.path.abspath("./")

# Import Mask RCNN
sys.path.append(ROOT_DIR)  # To find local version of the library

def load_pothole(dataset_dir, dataset_out_dir):

    json_path = os.path.join(dataset_dir, "*.json")
    for f in glob.glob(json_path) :
        # Load annotations
        # VGG Image Annotator (up to version 1.6) saves each image in the form:
        # { 'filename': '28503151_5b5b7ec140_b.jpg',
        #   'regions': {
        #       '0': {
        #           'region_attributes': {},
        #           'shape_attributes': {
        #               'all_points_x': [...],
        #               'all_points_y': [...],
        #               'name': 'polygon'}},
        #       ... more regions ...
        #   },
        #   'size': 100202
        # }
        # We mostly care about the x and y coordinates of each region
        # Note: In VIA 2.0, regions was changed from a dict to a list.
        annotations_ori = json.load(open(f))
        annotations_ori = list(annotations_ori.values())  # don't need the dict keys

        # The VIA tool saves images in the JSON even if they don't have any
        # annotations. Skip unannotated images.
        annotations = [a for a in annotations_ori if a['regions']]
        # Add images
        for a in annotations:
            # Get the x, y coordinaets of points of the polygons that make up
            # the outline of each object instance. These are stores in the
            # shape_attributes (see json format above)
            # The if condition is needed to support VIA versions 1.x and 2.x.
            if type(a['regions']) is dict:
                polygons = [r['shape_attributes'] for r in a['regions'].values()]
            else:
                polygons = [r['shape_attributes'] for r in a['regions']] 

            # load_mask() needs the image size to convert polygons to masks.
            # Unfortunately, VIA doesn't include it in JSON, so we must read
            # the image. This is only managable since the dataset is tiny.
            image_path = os.path.join(dataset_dir, a['filename']) 
            image_path_without_ex, image_ex = os.path.splitext(image_path)
            name = os.path.split(image_path_without_ex)[-1]

            image_out_path = os.path.join(dataset_out_dir, a['filename'])
            
            if not os.path.exists(image_path): 
                print(image_path, "not exist")
            else: 
                print(image_path, "success")
                image = skimage.io.imread(image_path)
                skimage.io.imsave( image_out_path, image )
            height, width = image.shape[:2]

            masks = []
            
            for i, p in enumerate(polygons):
                # Get indexes of pixels inside the polygon and set them to 1
                rr, cc = skimage.draw.polygon(p['all_points_y'], p['all_points_x'])
                
                mask = np.ones([height, width, 3], dtype=np.uint8)
                for color in range(3) :
                    mask[rr, cc, color] = 255

                mask_name = name + "_mask" + str(i) + ".jpg"
                mask_path = os.path.join(dataset_out_dir, mask_name)
                skimage.io.imsave( mask_path, mask )
                print("Mask: " + mask_path)
                
                mask_temp = {
                    "label": p['label'],
                    "mask_path": mask_name
                }
                masks.append(mask_temp)
                
            data = {
                a['filename']+str(a['size']) : {
                    'filename' : a['filename'],
                    'size' : a['size'],
                    'mask' : masks,
                    "file_attributes": {}
                }
            }
            
            out = os.path.join(dataset_out_dir, name+'.json')
            fp = open(out, 'w')
            fp.write(json.dumps(data))
            fp.close()

        annotations = [a for a in annotations_ori if not a['regions']]

        for a in annotations:
            image_path = os.path.join(dataset_dir, a['filename']) 
            image_path_without_ex, image_ex = os.path.splitext(image_path)
            image_name = os.path.split(image_path_without_ex)[-1]

            image_out_path = os.path.join(dataset_out_dir, a['filename'])
    
            if not os.path.exists(image_path): 
                print(image_path, "not exist")
            else: 
                print(image_path, "success")
                image = skimage.io.imread(image_path)
                skimage.io.imsave( image_out_path, image )
            height, width = image.shape[:2]

            masks = []
            data = {
                a['filename']+str(a['size']) : {
                    'filename' : a['filename'],
                    'size' : a['size'],
                    'mask' : masks,
                    "file_attributes": {}
                }
            }
            
            out = os.path.join(dataset_out_dir, image_name+'.json')
            fp = open(out, 'w')
            fp.write(json.dumps(data))
            fp.close()

if __name__ == '__main__':
    import argparse

    # Parse command line arguments
    parser = argparse.ArgumentParser(
        description='Train Mask R-CNN to detect pothole.')
    parser.add_argument("command",
                        metavar="<command>",
                        help="'train' or 'splash'")
    parser.add_argument('--dataset', required=False,
                        metavar="/path/to/pothole/dataset/",
                        help='Directory of the Pothole dataset')
    parser.add_argument('--dataset_out', required=False,
                        metavar="/path/to/pothole/dataset/",
                        help='Directory of the Pothole dataset')
    args = parser.parse_args()
    
    # Validate arguments
    if args.command == "convert":
        assert args.dataset, "Argument --dataset is required for training"
    elif args.command == "test": sys.exit(0)

    print("Dataset: ", args.dataset)

    # Train or evaluate
    if args.command == "convert":
        load_pothole(args.dataset, args.dataset_out)

